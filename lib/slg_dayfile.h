/***************************************************************************************************
 *
 * file     : slg_dayfile.h
 *
 * function : senslog project c-library - dayfile parsing and modifying functions
 *
 * author   : Jochen Ertel
 *
 * created  : 26.06.2021
 * updated  : 10.07.2021
 *
 **************************************************************************************************/

#include <stdint.h>

#include "slg_date.h"


#ifndef _slg_dayfile_h
#define _slg_dayfile_h


/* defines and structures *************************************************************************/
/**************************************************************************************************/
/**************************************************************************************************/

# define MXMLN   210    /* max. line length of measurement lines +1 */

# define DF_TEMP 1
# define DF_RAIN 2


/* dayfile structure (is completely private) */
typedef struct {
  uint32_t  locid;              /* location id */
  char      locstr[50];         /* location string */
  uint32_t  tmode;              /* time_mode */
  slg_date  date;               /* date of day */
  char      comment[100];       /* day comment, notes */

  uint32_t  colnum;             /* number of columns (max. 32) */
  uint32_t  coltyp[32];         /* list of column types */
  uint32_t  colid[32];          /* list of column ids */
  char      colstr[32][50];     /* list of column strings */

  char      msrline[96][MXMLN]; /* measure value lines */
} slg_daydata;



/* read and write day files ***********************************************************************/
/**************************************************************************************************/
/**************************************************************************************************/


/* reads a dayfile into a daydata structure
 *
 * parameters:
 *   *daydata:   target daydata structure
 *   *filename:  path/filename of dayfile to read
 *
 * return value:
 *    0 :  operation successfull
 *    1 :  error: file not found
 *    2 :  error: file contains invalid chars
 *    3 :  error: file contains a line which is to long
 *    4 :  error: unexpected file end
 *    5 :  error: first header part: invalid structure
 *    6 :  error: first header part: invalid time mode
 *    7 :  error: second header part: invalid structure
 *    8 :  error: second header part: invalid column typ
 *    9 :  error: second header part: to many columns
 *   10 :  error: second header part: column ids are not unique
 *   11 :  error: measurement lines: invalid number of values in line
 *   12 :  error: measurement lines: a value is to long
 *   13 :  error: measurement lines: invalid date or time value
 *   14 :  error: measurement lines: invalid line order
 *   15 :  error: to many lines
 *
 ****************************************************************************************/
uint32_t slg_readdayfile (slg_daydata *daydata, char *filename);


/* counts number of empty measurement lines in dayfile
 *
 * parameters:
 *   *daydata:   daydata object
 *
 * return value:
 *    <num> :  number of empty lines
 *
 ****************************************************************************************/
uint32_t slg_cntemptylines (slg_daydata *daydata);


/* counts number of empty measurement sections in dayfile
 *
 * parameters:
 *   *daydata:   daydata object
 *
 * return value:
 *    <num> :  number of empty sections
 *
 ****************************************************************************************/
uint32_t slg_cntemptysecs (slg_daydata *daydata);





#endif
