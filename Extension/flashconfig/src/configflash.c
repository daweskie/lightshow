/*
 * Copyright (C) 2011 Zoltan Zidarics (Zamek)
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/** file
    brief
*/

#include <ch.h>
#include <hal.h>
#include <chprintf.h>
#include <stdlib.h>
#include <chtypes.h>
#include <chconf.h>
#include <halconf.h>
#include <mcuconf.h>
#include <stm32f4xx.h>
#include <sys/queue.h>
#include <string.h>
#include <misc.h>
#include <cmdutil.h>

#include <Flash.h>
#include <macros.h>

#define FLASH_EMPTY_CHAR (uint8_t)0xff
#define DELIMITER "="

struct CfgItem {
    char *key;
    uint32_t len;
    uint8_t *data;
    SLIST_ENTRY(CfgItem) next;
};

typedef struct {
    uint16_t sector;
    uint8_t *flashBase;
    int modified;
    SLIST_HEAD(cfgList, CfgItem) cfgHead;
} FlashConfig;

static FlashConfig flashConfig = {
    .sector=0,
    .flashBase=0,
    .modified=0,
    .cfgHead=NULL
};

static const uint32_t flashTable[] = {
    FLASH_Sector_0,  0x08000000,
    FLASH_Sector_1,  0x08004000,
    FLASH_Sector_2,  0x08008000,
    FLASH_Sector_3,  0x0800c000,
    FLASH_Sector_4,  0x08010000,
    FLASH_Sector_5,  0x08020000,
    FLASH_Sector_6,  0x08040000,
    FLASH_Sector_7,  0x08060000,
    FLASH_Sector_8,  0x08080000,
    FLASH_Sector_9,  0x080a0000,
    FLASH_Sector_10, 0x080c0000,
    FLASH_Sector_11, 0x080e0000
};

uint32_t cfgGetFlashBase(uint16_t sector) {
    uint16_t i=0;
    for (;i<sizeof(flashTable)/sizeof(uint32_t);i++)
        if (flashTable[i]==sector)
            return flashTable[i+1];

    return 0;
}

static struct CfgItem *cfgFind(char *key) {
    if (! (key && *key))
        return NULL;
    struct CfgItem *item;
    SLIST_FOREACH(item, &flashConfig.cfgHead, next)
        if (item && strcmp(item->key,key)==0)
            return item;

    return NULL;
}

int cfgRemove(char *key) {
    if (!(key && *key))
        return EXIT_FAILURE;
    struct CfgItem *item = cfgFind(key);
    if (!item)
        return EXIT_FAILURE;

    SLIST_REMOVE(&flashConfig.cfgHead, item, CfgItem, next);
    ++flashConfig.modified;
    return EXIT_SUCCESS;
}

int cfgAddItem(char *name, void *data, int len) {
    if (! (name && *name && data && len>0))
        return EXIT_FAILURE;

    struct CfgItem *item = cfgFind(name);
    if (item)
        return EXIT_FAILURE;

    MALLOC(item,sizeof(struct CfgItem));
    if (!item)
        return EXIT_FAILURE;

    MALLOC(item->key, strlen(name)+1);
    if (!item->key)
        return EXIT_FAILURE;

    strcpy(item->key, name);

    item->len = len;

    MALLOC(item->data, len);
    if (!item->data)
        return EXIT_FAILURE;
    memcpy(item->data, data, len);

    SLIST_INSERT_HEAD(&flashConfig.cfgHead,item,next);
    ++flashConfig.modified;
    return EXIT_SUCCESS;
}

int cfgGetItem(const char *name, uint8_t *data, uint16_t maxLen) {
    if (! (name && *name))
        return EXIT_FAILURE;

    struct CfgItem *item = cfgFind(name);
    if (!item || item->len > maxLen)
        return EXIT_FAILURE;

    memcpy(data, item->data, item->len);
    return EXIT_SUCCESS;
}

int cfgGetShort(const char *name, short *target) {
    if (! (name&&*name&&target) )
        return EXIT_FAILURE;

    struct CfgItem *item=cfgFind(name);
    if (! (item&&item->data) )
        return EXIT_FAILURE;

    *target = atoi(item->data);
    return EXIT_SUCCESS;
}

int cfgGetShortDef(const char *name, short def) {
    short result;
    return cfgGetShort(name, &result) == EXIT_SUCCESS ? result : def;
}

int cfgGetInt(const char *name, int *target) {
    if (! (name&&*name&&target) )
        return EXIT_FAILURE;

    struct CfgItem *item=cfgFind(name);
    if (! (item&&item->data) )
        return EXIT_FAILURE;

    *target = atoi(item->data);
    return EXIT_SUCCESS;
}

int cfgGetIntDef(const char *name, int def) {
    int result;
    return cfgGetInt(name, &result) == EXIT_SUCCESS ? result : def;
}

int cfgGetLong(const char *name, long *target) {
    if (! (name&&*name&&target) )
        return EXIT_FAILURE;

    struct CfgItem *item=cfgFind(name);
    if (! (item&&item->data) )
        return EXIT_FAILURE;

    *target = atol(item->data);
    return EXIT_SUCCESS;
}

int cfgGetLongDef(const char *name, long def) {
    long result;
    return cfgGetLong(name, &result) == EXIT_SUCCESS ? result : def;
}

int cfgGetLLong(const char *name, long long *target) {
    if (! (name&&*name&&target) )
        return EXIT_FAILURE;

    struct CfgItem *item=cfgFind(name);
    if (! (item&&item->data) )
        return EXIT_FAILURE;

    *target = atoll(item->data);
    return EXIT_SUCCESS;
}

int cfgGetLLongDef(const char *name, long long def) {
    long long result;
    return cfgGetLLong(name, &result) == EXIT_SUCCESS ? result : def;
}

int cfgGetFloat(const char *name, float *target) {
    if (! (name&&*name&&target) )
        return EXIT_FAILURE;

    struct CfgItem *item=cfgFind(name);
    if (! (item&&item->data) )
        return EXIT_FAILURE;

    *target = atof(item->data);
    return EXIT_SUCCESS;
}

int cfgGetFloatDef(const char *name, float def) {
    float result;
    return cfgGetFloat(name, &result) == EXIT_SUCCESS ? result : def;
}

static uint8_t *cfgParseConfigFlash(uint8_t *address) {
    if (!address)
        return address;
    uint8_t *ptr = address;
    while(*ptr != FLASH_EMPTY_CHAR && *ptr++)
        ;

    if (*ptr==FLASH_EMPTY_CHAR)
        return ptr;

    int len = *ptr;
    ptr += sizeof(uint32_t);

    cfgAddItem((char *)address, ptr, len);
    return ptr+len;
}

static int cfgLoad(void) {
    uint8_t *ptr = flashConfig.flashBase, *next;
    do {
        next = cfgParseConfigFlash(ptr);
        if (next<=ptr)
            break;
        ptr = next;
    }while(*next!=FLASH_EMPTY_CHAR);
    flashConfig.modified = 0;
    return EXIT_SUCCESS;
}

int cfgInit(int sector) {
    flashConfig.sector = sector;
    flashConfig.flashBase = (uint8_t *)cfgGetFlashBase(flashConfig.sector);
    if (!flashConfig.flashBase)
        return EXIT_FAILURE;
    SLIST_INIT(&flashConfig.cfgHead);
    return cfgLoad();
}

int cfgEraseFlash(void) {
    FLASH_Unlock();
    FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR | FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR | FLASH_FLAG_PGSERR);
    FLASH_Status status = FLASH_EraseSector(flashConfig.sector, VoltageRange_3); /* settings at 3*16kB */
    return status == FLASH_COMPLETE ? EXIT_SUCCESS : EXIT_FAILURE;
}

void cmd_saveConfig(BaseSequentialStream *chp, int argc, char *argv[]) {
#define chk_error(address,status) \
    do { \
        if ((status) != FLASH_COMPLETE) { \
            chprintf(chp,"Write error at:%X, status:%d\r\n", (address), (status)); \
            return; \
        } \
    } while(0)


    int forced = argc > 1 && strcmp(argv[0], "force") == 0;

    if(!flashConfig.modified && !forced) {
        chprintf(chp, "Config isn't modified, try save with force\r\n");
        return;
    }

    struct CfgItem *item;
    uint8_t *mem = flashConfig.flashBase;

    chprintf(chp, "Starting erase, mem:%X\r\n",mem);

    if (cfgEraseFlash() != EXIT_SUCCESS) {
        chprintf(chp, "Erase error\r\n");
        return ;
    }

    chprintf(chp, "Starting write mem:%x\r\n",mem);
    FLASH_Status status=FLASH_COMPLETE;

    SLIST_FOREACH(item, &flashConfig.cfgHead, next) {
        char *name = item->key;
        do {
            status = FLASH_ProgramByte((uint32_t)mem, *name);
            chk_error(mem, status);
            ++mem;
        } while(*name++);

        status = FLASH_ProgramWord((uint32_t)mem, item->len);
        chk_error(mem,status);
        mem += sizeof(uint32_t);
        uint32_t i;
        for(i=0;i<item->len;i++,mem++) {
            status = FLASH_ProgramByte((uint32_t)mem, *item->data++);
            chk_error(mem,status);
        }
    }

    FLASH_Lock();
    chprintf(chp, "Config saved successfully\r\n");
}

void cmd_showConfig(BaseSequentialStream *chp, int argc, char *argv[]) {
    (void) argc;
    (void) argv;
    struct CfgItem *item;
    int i=0;
    chprintf(chp, "Config sector is:%d, base address:%X, modified:%d\r\n",
                  flashConfig.sector,flashConfig.flashBase,flashConfig.modified);
    SLIST_FOREACH(item, &flashConfig.cfgHead, next)  {
        chprintf(chp, "%d. name:%s, length:%d, data:\r\n   ", i++, item->key, item->len);
        hexDump(chp, item->data, item->len);
    }
    chprintf(chp, "number of elements:%d\r\n", i);
}

void cmd_eraseConfig_flash(BaseSequentialStream *chp, int argc, char *argv[]) {
    (void) argc;
    (void) argv;
    chprintf(chp, "Erase config flash...");
    cfgEraseFlash();
    chprintf(chp, "finished\r\n");
}
