/**
    Demonstrating tailq as a normal queue;

    There are two process, a producer and a consumer. Producer produce some data asynchronous, and puts its products into the queue.
    There is a process as a consumer, which is gets products from the queue, if its not empty.

*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/queue.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>

#define MALLOC(ptr,size)  \
do { \
    ptr=malloc(size); \
    if (!ptr)  \
       abort(); \
} while(0) \

#define FREE(ptr) \
do { \
    free(ptr); \
    ptr=NULL; \
} while(0)


const char *menu[] = {
    {"Bacon" },
    {"Bolognese" },
    {"Broccoli" },
    {"Buddy" },
    {"California" },
    {"Cipolla" },
    {"Costolla" },
    {"Hawaii" },
    {"Margareta" },
    {"Mozzarella" },
    {"Palermo" },
    {"Popey" },
    {"Salami" },
    {"Sicilia" },
    {"Siena" },
    {"Vegas" },
    {"Vulcan" }
};

const int PIZZA_COUNTS = sizeof(menu)/sizeof(char *);

const int GUESTS_MAX = 7;
const int BAKE_CAPACITY = 10;
const int OPENING_TIME = 5; //sec
const int DISCOUNT_TIME = 2; // sev

struct {
    struct tailhead *headp;                 /* Tail queue head. */
    int product_order;
    volatile int pizzeria_opened;
    volatile int kitchen_opened;
    pthread_t producer;
    pthread_t consumer;
    pthread_mutex_t mutex;
    TAILQ_HEAD(tailhead, pizza_t) head;
} pizzeria;


struct pizza_t {
    int order;
    int pizza;
    TAILQ_ENTRY(pizza_t) entries;         /* Tail queue. */
};


void *bake(void *param)
{
    while(pizzeria.kitchen_opened) {
        int i;
        for(i=0;i<BAKE_CAPACITY;i++) {
            struct pizza_t *make;
            MALLOC(make, sizeof(struct pizza_t));
            make->order = ++pizzeria.product_order;
            make->pizza = rand() % PIZZA_COUNTS;

            pthread_mutex_lock(&pizzeria.mutex);
            TAILQ_INSERT_TAIL(&pizzeria.head, make, entries);
            pthread_mutex_unlock(&pizzeria.mutex);

            printf("Baking pizza, order:%d, value:%s\n", make->order, menu[make->pizza]);
        }
        sleep(1);
    }
    return NULL;
}

void sale_a_pizza() {
    struct pizza_t *sale;
    sale = pizzeria.head.tqh_first;
    printf("\t\t\tEat pizza, order:%d, name:%s\n", sale->order, menu[sale->pizza]);

    pthread_mutex_lock(&pizzeria.mutex);
    TAILQ_REMOVE(&pizzeria.head, pizzeria.head.tqh_first, entries);
    pthread_mutex_unlock(&pizzeria.mutex);

    FREE(sale);
}

void *consume(void *param)
{
    while(pizzeria.kitchen_opened) {
        sleep(2);
        int i=0;
        while (!TAILQ_EMPTY(&pizzeria.head) && i<GUESTS_MAX) {
            sale_a_pizza();
            i++;
        }
    }
    printf("\nPizzeria will be closed, let's sale the remaining pizzas\n");
    while (!TAILQ_EMPTY(&pizzeria.head))
       sale_a_pizza();

    return NULL;
}

void open() {
    TAILQ_INIT(&pizzeria.head);
    pizzeria.product_order=0;
    time_t t;
    time(&t);
    srand(t);
    pthread_mutex_init(&pizzeria.mutex, NULL);
    pizzeria.kitchen_opened=1;
    pizzeria.pizzeria_opened=1;
    pthread_create(&pizzeria.producer, NULL, bake, NULL);
    pthread_create(&pizzeria.consumer, NULL, consume, NULL);
    printf("\nNow the pizzeria is opening\n\n");
}

void close_kitchen() {
    pizzeria.kitchen_opened=0;
    pthread_join(pizzeria.producer,NULL);
    printf("\nNow the kitchen is closed, no more pizzas baken\n\n");
}

void close_pizzeria() {
    pizzeria.pizzeria_opened=0;
    pthread_join(pizzeria.consumer,NULL);
    printf("\nNow the pizzeria is closed, no more pizzas reamining\n");
    printf("Are remaining pizzas: %s\n", TAILQ_EMPTY(&pizzeria.head) ? "false" : "true");
}

int main(int argc, char **argv)
{
    open();
    sleep(OPENING_TIME);
    close_kitchen();
    sleep(DISCOUNT_TIME);
    close_pizzeria();
    return EXIT_SUCCESS;
}
