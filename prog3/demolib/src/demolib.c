/*
 * Copyright (C)  2013 Zoltan Zidarics (Zamek)
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY{} without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
/** \file
    \brief
*/
#include <stdlib.h>
#include <pthread.h>
#include <sys/queue.h>
#include <time.h>
#include <unistd.h>
#include <string.h>

#include <demolib.h>
#include <config.h>
#include <syslog.h>
#include <macros.h>
#include <strutil.h>
#include <config.h>

static inline struct DemoItem *pop(struct DemoFifo *fifo)
{
    pthread_mutex_lock(&fifo->mutex);
    struct DemoItem *item=fifo->head.tqh_first;
    TAILQ_REMOVE(&fifo->head, fifo->head.tqh_first, entries);
    pthread_mutex_unlock(&fifo->mutex);
    return item;
}

static inline void push(struct DemoFifo *fifo, struct DemoItem *item)
{
   pthread_mutex_lock(&fifo->mutex);
   TAILQ_INSERT_TAIL(&fifo->head, item, entries);
   pthread_mutex_unlock(&fifo->mutex);
}

static void *generate(void *param)
{
    if (!param)
        return NULL;

    struct DemoLib *demolib = (struct DemoLib *) param;
    demolib->running = 1;
    demolib->paused = 0;
    time_t t = time(NULL);
    srandom(t);

    while(demolib->running) {
        sleep(1);
        if (demolib->paused || TAILQ_EMPTY(&demolib->pool->head))
            continue;

        struct DemoItem *item=pop(demolib->pool);
        item->value=rand();
        push(demolib->values, item);
    }
    return NULL;
}

struct DemoLib *dlCreateCfg(const char *cfgPrefix)
{
    struct StringBuilder *builder = cfgCreateKeyPrefix(NULL,cfgPrefix,KEY_START_VALUE);
    if (!builder)
        goto errorExit;

    int startValue = cfgGetIntDef(builder->chars, DEF_START_VALUE);
    strClearBuilder(builder);

    builder = cfgCreateKeyPrefix(builder, cfgPrefix, KEY_BURST);
    int burst = cfgGetIntDef(builder->chars, DEF_BURST);

    strFreeBuilder(builder);

    return dlCreate(cfgPrefix, startValue, burst);

errorExit:
    if (builder)
        strFreeBuilder(builder);
    return NULL;
}

struct DemoLib *dlCreate(const char *name, int startValue, int burst)
{
    struct DemoLib *res;
    MALLOCZ(res, sizeof(struct DemoLib));
    res->name = strdup(name);
    res->startValue = startValue;
    res->burst = burst;
    MALLOCZ(res->values, sizeof(struct DemoFifo));
    MALLOCZ(res->pool, sizeof(struct DemoFifo));
    pthread_mutex_init(&res->values->mutex, NULL);
    pthread_mutex_init(&res->pool->mutex, NULL);
    TAILQ_INIT(&res->values->head);
    TAILQ_INIT(&res->pool->head);
    int i=0;
    for (;i<res->burst;++i) {
        struct DemoItem *di;
        MALLOCZ(di, sizeof(struct DemoItem));
        push(res->pool, di);
    }

    return res;
}

int dlDestroy(struct DemoLib *demolib)
{
    if (!demolib)
        return EXIT_FAILURE;

    if (demolib->running)
        dlStop(demolib);

    struct DemoItem *item;
    while( (item=pop(demolib->values)) )
        FREE(item);

    while( (item=pop(demolib->pool)) )
        FREE(item);

    pthread_mutex_destroy(&demolib->values->mutex);
    pthread_mutex_destroy(&demolib->pool->mutex);
    FREE(demolib->values);
    FREE(demolib->pool);
    FREE(demolib);
    return EXIT_SUCCESS;
}

int dlGetStartValue(struct DemoLib *demolib)
{
    return demolib ? demolib->startValue : 0;
}

void dlSetStartValue(struct DemoLib *demolib, int value)
{
    if (!demolib||demolib->startValue ==value)
        return;

    cfgSetInt(KEY_START_VALUE, value);
    demolib->startValue = value;
}

int dlStart(struct DemoLib *demolib)
{
    if (!demolib || demolib->running)
        return EXIT_FAILURE;

    demolib->running = 1;
    pthread_create(&demolib->thread, NULL, generate, demolib);
    return EXIT_SUCCESS;
}

int dlStop(struct DemoLib *demolib)
{
    if (!demolib)
        return EXIT_FAILURE;

    demolib->running = 0;
    pthread_join(demolib->thread, NULL);
    return EXIT_SUCCESS;
}

void dlPause(struct DemoLib *demolib)
{
    if (demolib)
        demolib->paused = 1;
}

void dlResume(struct DemoLib *demolib)
{
    if (demolib)
        demolib->paused =0;
}

int dlIsPaused(struct DemoLib *demolib)
{
    return demolib ? demolib->paused : 0;
}

int dlIsStopped(struct DemoLib *demolib)
{
    return demolib ? ! demolib->running : 0;
}

int dlGetValue(struct DemoLib *demolib)
{
    if (!demolib||TAILQ_EMPTY(&demolib->values->head))
        return 0;

    struct DemoItem *item=pop(demolib->values);

    if (!item)
        return 0;

    int res = item->value;
    push(demolib->pool, item);
    return res;
}

int setCallback(struct DemoLib *demolib, Callback cb)
{
    if (!demolib ) //NULL pointer vizsgálatot nyomatni!
        return 0;

    demolib->callback=cb;
    if(demolib->callback)
        demolib->callback(demolib, NULL, 0);
}
