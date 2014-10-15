#ifndef DEMOLIB_H_INCLUDED
#define DEMOLIB_H_INCLUDED

/*
 * Copyright (C)  2013 Zoltan Zidarics (Zamek)
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
/** \file
    \brief
*/

#include <pthread.h>
#include <sys/queue.h>

#define KEY_START_VALUE "demo.startvalue"
#ifndef DEF_START_VALUE
#define DEF_START_VALUE 42
#endif

#define KEY_BURST "demo.burst"
#ifndef DEF_BURST
#define DEF_BURST 10
#endif


struct DemoItem {
    int value;
    TAILQ_ENTRY(DemoItem) entries;
};

struct DemoFifo {
    struct TailHead *headp;
    TAILQ_HEAD(TailHead, DemoItem) head;
    pthread_mutex_t mutex;
};

struct DemoLib;

typedef void (*Callback)(struct DemoLib *demolib, char *buffer, int blen);

struct DemoLib {
    char *name;
    pthread_t thread;
    int startValue;
    int burst;
    struct DemoFifo *values;
    struct DemoFifo *pool;
    volatile int running;
    volatile int paused;
    Callback callback;
};



struct DemoLib *dlCreateCfg(const char *cfgPrefix);

struct DemoLib *dlCreate(const char *name, int startValue, int burst);

int dlDestroy(struct DemoLib *demolib);

int dlGetStartValue(struct DemoLib *demolib);

void dlSetStartValue(struct DemoLib *demolib, int value);

int dlStart(struct DemoLib *demolib);

int dlStop(struct DemoLib *demolib);

void dlPause(struct DemoLib *demolib);

void dlResume(struct DemoLib *demolib);

int dlIsPaused(struct DemoLib *demolib);

int dlIsStopped(struct DemoLib *demolib);

int dlGetValue(struct DemoLib *demolib);

int setCallback(struct DemoLib *demolib, Callback cb);

#endif // DEMOLIB_H_INCLUDED
