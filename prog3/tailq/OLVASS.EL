Queue használata

Többszálon futó programok esetén a sorok (queue) használata megkerülhetetlen.
Például, ha az a feladat, hogy soros porton érkező adatot kell feldolgozni, akkor
célszerű egy szálat a vételre és egyet az feldolgozásra beállítani.
Ezzel szét tudjuk választani termelő és fogyasztó folyamatokat. Sokan úgy gondolják, hogy
erre nincs szükség, hanem a karakter vételekor rögtön el lehet intézni a vételt.
Nézzük, milyen hátrányai vannak ennek a megközelítésnek.
A karakter vétele aszinkron történik, nagyon gyakran interrupt-ban kell kezelni. A legtöbben úgy kezdik
az interrupt kezelő függvényt, hogy letiltják a további interrupt-ot amíg a feldolgozás tart. Ha egy keret
vételekor az utolsó karaktert is beolvastuk, akkor általában crc ellenőrzés után a kész keret feldolgozása következik,
ami sokáig tarthat. Ebben az esetben indokolatlanul hosszú ideig tarthat az interrupt tiltás, a rendszer "lekéshet" a
feldolgozás ideje alatt történt eseményekről. Ha nem tiltjuk le a további interrupt-ot, akkor a feldolgozás alatt újra
el fog indulni az új karakterrel a feldolgozás, holott az előzőt még be sem fejeztük.
Nehezíti a helyzetet, hogy aszinkron folyamatokat nem lehet debug-olni, mivel egy törésponttal befagyasztjuk az aktuális állapotot,
az aszinkron eseményeket nem fogja kezelni és egy szép tiszta labor állapotban fogunk debug-olni, amiben minden tökéletesen működik.
Sikeresen belemanőverztük magunkat egy ellenőrizhetetlen szituációba, amelyben csak remélni tudjuk, hogy úgy működik, ahogy szeretnénk.

A fentiek értelmében jó lenne szétválasztani a karaterek vételét végző termelő és a feldolgozást végző folyamatra a működést.
Ha a két folyamat közti adatátadást csak egy karakterrel valósítjuk meg, akkor mindkettő az ideje nagy részét azzal tölti, hogy a
másikra vár. A vevő addíg nem tud újabb karaktert venni, amíg az előzőt fel nem dolgozták. Megint eljutottunk oda, hogy az interrupt
túl sokáig tart. Célszerű az ilyen esetekben az eszkimó számlálási módszert alkalmazni: egy, kettő, sok. Vagyis, vagy egy dolgot kezelünk, vagy sokat.
Erre a célra fifo-kat szoktunk használni (first in first out). Szokták még queue-nak (sor) is nevezni. Így az igényeknek megfelelően lehet a fifo tárolási kapacitásával
finomhangolni a rendszert.

Vegyünk egy egyszerű példát a probléma demonstrálására:

Pizzéria

Adott egy pizzéria a következő feltételekkel:

A kemencében egyszerre 10 pizza készülhet (energiatakarékosság).
A pizzériában egyszerre adott számú vendég tartózkodhat.
A zárás előtt megadott idővel a konyha leáll, a maradék pizzát olcsóbban eladják.

Megvalósítás

Két folyamat van, az egyik a pizzák elkészítése, a másik a felszolgálás.
Mivel ismertek a vevői szokások, előre legyártanak a kemence kapacitásának megfelelő mennyiségű különböző pizzát.
A konyha folyamatosan gyárt, a felszolgálók az elkészült pizzákat sorban kiadják.
Célszerű két szálat indítani, az egyik a producer (termelő), a másik a consumer (fogyasztó).
Az elkészült pizzákat egy fifo-ba teszi a producer, innen a consumer fogyasztja, ha az nem üres.

A különböző programnyelvekhez az egyes framework-ok kínálnak fifo megvalósításokat, pl. a Javaban és C++-ban Collection néven.
A glibc-hez is létezik egy hasonló megvalósítás a BSD jóvoltából. A szükséges header file neve sys/queue.h. Ebben többféle célra
létezik megoldás és ami a legszebb, hogy minden makrókkal megvalósított.
A listák esetében a head nevű pont ismert, általában ez a lista első elemét tartalmazza.
Az elemeket egy struct-ban kell elhelyezni, amelynek lesz egy tagja, amit a lista kezel, ez mutat a következő, vagy ha
többszörösen láncolt listát használunk az előző elemre is.

A demóban a TAILQ nevű fifo-t használjuk. Ehhez az alábbi műveletek definiáltak:
TAILQ_ENTRY(TYPE);
Az adattároló struct listkezelő által használt mező típusa. A TYPE paraméter a struct típusneve. Esetünkben ez ilyen lesz:

struct pizza_t {
    int order;
    int pizza;
    TAILQ_ENTRY(pizza_t) entries;
};


TAILQ_HEAD(HEADNAME, TYPE);

A listakezelő változója, ezen keresztül érhetjük el a lista elemeit, ill. ezt kell átadni a listakezelő makróknak.
A HEADNAME egy struct típusa lesz, a TYPE a listakezelő által kezelt struktúra tag típusa.

A mi esetünkben a pizzeria adatstruktúra ilyen lesz:

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


TAILQ_INIT(TAILQ_HEAD *head);
A lista kezdeti inicializálását vágző makró. A head a listakezelő változója.


TAILQ_INSERT_AFTER(TAILQ_HEAD *head, TYPE *listelm,
                TYPE *elm, TAILQ_ENTRY NAME);

Egy elem beszúrása a megadott elem után.

TAILQ_INSERT_HEAD(TAILQ_HEAD *head,
                TYPE *elm, TAILQ_ENTRY NAME);
Egy elem beszúrása a lista elejére.

TAILQ_INSERT_TAIL(TAILQ_HEAD *head,
                TYPE *elm, TAILQ_ENTRY NAME);
Egy elem beszúrása a lista végére.

TAILQ_REMOVE(TAILQ_HEAD *head, TYPE *elm, TAILQ_ENTRY NAME);
Egy elem törlése a listáról.

A producer szál az alábbi módon működik:

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

A szál addíg fut, amíg a pizzéria konyhája nyitva van ( while(pizzeria.kitchen_opened) ).
A kitchen_opened változó volatile, ami azt jelenti, hogy minden olvasás esetén kötelező a memóriából beolvasni,
nem lehet regiszterben tárolni a gyorsabb végrehajtás érdekében. Így a szál észre fogja venni, ha egy másik szál le akarja
állítani.
A mutex-re azért van szükség, hogy a fifo adminisztrációját egyszerre csak egy szál végezhesse. Ha a mutex foglalt, akkor más szálak
várakozni kényszerülnek, amíg az fel nem szabadul. Populáris megközelítése a mutexnek a wc kulcs analógia. Ha valaki birtokolja a kulcsot,
addíg mindenkinek várakoznia kell, amíg az illető be nem fejezi és vissza nem teszi a kulcsot.

Látható, hogy a szál egyszerre BAKE_CAPACITY mennyiségű pizzát gyárt le, majd előkészíti a következő sorozatot és újrakezdi a sütést.
Az előkészítést a sleep(1) jelképezi.

A fogyasztó szál működése kettős, amíg a konyha működik, addíg folyamatosan kiszolgálja a pizzákat, de csak az asztalok számának megfeleően.
Amint a konyha leállt a sütéssel, a maradék pizzát árleszállítással kihordja, hogy ne romoljon meg.

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

A vendégek lassabban fogyasztják a pizzát mint ahogy sül, ezt a sleep(2) jelképezi. A GUESTS_MAX konstans a maximális
vendégszámot jelképezi.

Már csak a vezérlés marad, ezt az open(), close_kitchen() és close_pizzeria() függvények valósítják meg.
Az open() felinicializálja a változókat (lista, véletlen generátor, mutex) és elindítja a szálakat (pthread_create()).
A close függvényeknek meg kell várniuk, amíg a célzott szál valóban leáll. Ne felejtsük el, hogy a
kitchen_opened és pizzeria_opened változók, csak szándékot jelölnek, a szálak csak egy ponton vizsgálják meg az értéküket.
A begyújtott kemencét nem lehet azonnal kikapcsolni, a bent lévő pizzákat meg kell sütni, ill. a vendégeket sem lehet a záróra
pillanatában kidobni, meg kel várni amíg elfogyasztják a pizzájukat.
Ezért a pthread_join() függvénnyel megvárjuk, amíg a szál leáll.

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

A kész program kódja:

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
