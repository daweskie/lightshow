/*
 * Copyright (C) 
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/*
#!#APP_NAME#!# application
*/

#define APP_NAME "#!#APP_NAME#!#"

#include <stdio.h>
#include <stdlib.h>


int main(int argc, char **argv) {
    printf("Starting %s\n",APP_NAME);
    return EXIT_SUCCESS;
}

