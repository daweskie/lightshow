#ifndef CONFIGFLASH_H_INCLUDED
#define CONFIGFLASH_H_INCLUDED

/*
 * Copyright (C)  2011 Zoltan Zidarics (Zamek)
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

int cfgInit(int sector);

int cfgRemove(char *key);

int cfgAddItem(char *name, void *data, int len);

int cfgGetItem(char *name, uint8_t *data, int maxLen);

int cfgGetShort(const char *name, short *target);

int cfgGetShortDef(const char *name, short def);

int cfgGetInt(const char *name, int *target);

int cfgGetIntDef(const char *name, int def);

int cfgGetLong(const char *name, long *target);

int cfgGetLongDef(const char *name, long def);

int cfgGetLLong(const char *name, long long *target);

int cfgGetLLongDef(const char *name, long long def);

int cfgGetFloat(const char *name, float *target);

int cfgGetFloatDef(const char *name, float def);

void cmd_showConfig(BaseSequentialStream *chp, int argc, char *argv[]);

void cmd_saveConfig(BaseSequentialStream *chp, int argc, char *argv[]);

void cmd_eraseConfigFlash(BaseSequentialStream *chp, int argc, char *argv[]);

#endif // CONFIGFLASH_H_INCLUDED
