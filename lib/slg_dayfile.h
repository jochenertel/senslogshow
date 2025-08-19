/***************************************************************************************************
 *
 * file     : slg_dayfile.h
 *
 * function : senslog project c-library - dayfile parsing and modifying functions
 *
 * author   : Jochen Ertel
 *
 * created  : 26.06.2021
 * updated  : 21.08.2024
 *
 **************************************************************************************************/

#include <stdint.h>

#include "slg_date.h"


#ifndef _slg_dayfile_h
#define _slg_dayfile_h



/* defines and structures *************************************************************************/
/**************************************************************************************************/
/**************************************************************************************************/

# define MAX_MLN_NUM    96    /* max. number of measurement lines */
# define MAX_MLN_VALS   16    /* max. number of measurement lines */
# define MAX_MLN_LEN   150    /* max. line length of measurement lines +1 */

# define DF_TEMP 1
# define DF_RAIN 2
# define DF_EVNT 3


/* dayfile structure (is completely private) */
typedef struct {
  uint32_t  locid;                              /* location id */
  char      locstr[50];                         /* location string */
  uint32_t  tmode;                              /* time_mode */
  slg_date  date;                               /* date of day */
  char      comment[100];                       /* day comment, notes */

  uint32_t  colnum;                             /* number of columns (max. MAX_MLN_VALS) */
  uint32_t  coltyp[MAX_MLN_VALS];               /* list of column types */
  uint32_t  colid[MAX_MLN_VALS];                /* list of column ids */
  char      colstr[MAX_MLN_VALS][50];           /* list of column strings */

  char      msrline[MAX_MLN_NUM][MAX_MLN_LEN];  /* measure value lines */
} slg_daydata;


/* month structure (is completely private) */
typedef struct {
  uint32_t     locid;                           /* location id */
  char         locstr[50];                      /* location string */
  uint32_t     tmode;                           /* time_mode */
  slg_date     date;                            /* date of first day of month */

  uint32_t     dvalid[31];                      /* 0: day does not exist, 1: day exists */
  slg_daydata  daydata[31];                     /* array of day data */
} slg_monthdata;



/* read and write day files ***********************************************************************/
/**************************************************************************************************/
/**************************************************************************************************/


/* reads a dayfile into a daydata structure
 *
 * parameters:
 *   *daydata:   target daydata structure
 *   *filename:  path/filename of dayfile to read
 *   hmode:      0: normal header in file
 *               1: no header in file (assumed Bretnig: TEMP RAIN)
 *               2: no header in file (assumed Dresden Wittenberger: TEMP TEMP TEMP)
 *               3: no header in file (assumed Dresden Hofefeld: TEMP RAIN)
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
uint32_t slg_readdayfile (slg_daydata *daydata, char *filename, uint32_t hmode);


/* writes a daydata structure into a dayfile
 *
 * parameters:
 *   *filename:  path/filename of dayfile to write
 *   *daydata:   source daydata structure
 *   mode:       0: write meassurement lines directly from stored strings
 *               1: reassemble meassurement lines from single values
 *
 * return value:
 *    0 :  operation successfull
 *    1 :  error: opening file failed
 *    2 :  error: invalid daydata
 *
 ****************************************************************************************/
uint32_t slg_writedayfile (char *filename, slg_daydata *daydata, uint32_t mode);



/* dayfile checker functions **********************************************************************/
/**************************************************************************************************/
/**************************************************************************************************/


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


/* counts invalid values (error marker) in all columns of all measurement lines
 *
 * parameters:
 *   *daydata:   daydata object
 *
 * return value:
 *    <num> :  number of invalid values
 *
 ****************************************************************************************/
uint32_t slg_cntinvalidvals (slg_daydata *daydata);


/* checks if a column of a special typ and id exist and gives back column number
 *
 * parameters:
 *   *daydata:  daydata object
 *   typ     :  column typ
 *   id      :  column id
 *
 * return value:
 *         0 :  column does not exist
 *      >= 2 :  column number
 *
 ****************************************************************************************/
uint32_t slg_colexist (slg_daydata *daydata, uint32_t typ, uint32_t id);


/* gets last valid measurement line index of a dayfile
 *
 * parameters:
 *   *daydata:  daydata object
 *
 * return value:
 *   <line_ind> :  line index (0, 1, 2, ...)
 *
 ****************************************************************************************/
uint32_t slg_lastmline (slg_daydata *daydata);



/* dayfile get value functions ********************************************************************/
/**************************************************************************************************/
/**************************************************************************************************/


/* get a special temperature value from dayfile
 *
 * parameters:
 *   *daydata:  daydata object
 *   c       :  temperature column index (2, 3, 4, ...)
 *   k       :  time index (0, 1, 2, ...)
 *
 * return value:
 *   CNERR   :  error, invalid value or value does not exist
 *   temper  :  temperature T*10
 *
 ****************************************************************************************/
int32_t slg_gettemperval (slg_daydata *daydata, uint32_t c, uint32_t k);


/* get a special rain value from dayfile
 *
 * parameters:
 *   *daydata:  daydata object
 *   c       :  rain column index (2, 3, 4, ...)
 *   k       :  time index (0, 1, 2, ...)
 *
 * return value:
 *   CNERR   :  error, invalid value or value does not exist
 *   rain    :  rain*100
 *
 ****************************************************************************************/
uint32_t slg_getrainval (slg_daydata *daydata, uint32_t c, uint32_t k);


/* get a special boolean event value from dayfile
 *
 * parameters:
 *   *daydata:  daydata object
 *   c       :  event column index (2, 3, 4, ...)
 *   k       :  time index (0, 1, 2, ...)
 *
 * return value:
 *   CNERR   :  error, invalid value or value does not exist
 *   event   :  0 or 1
 *
 ****************************************************************************************/
uint32_t slg_geteventval (slg_daydata *daydata, uint32_t c, uint32_t k);



/* read month data ********************************************************************************/
/**************************************************************************************************/
/**************************************************************************************************/


/* reads a all dayfiles of a month into a monthdata structure
 * - dayfile names are assumed to be in format "yyyy-mm-dd.txt"
 *
 * parameters:
 *   *monthdata: target mothdata structure
 *   *pathname:  path name of dayfiles to read (incl. '/') or empty string
 *   hmode:      0: normal header in dayfiles
 *               1: no header in dayfiles (assumed Bretnig: TEMP RAIN)
 *               2: no header in dayfiles (assumed Dresden Wittenberger: TEMP TEMP TEMP)
 *               3: no header in dayfiles (assumed Dresden Hofefeld: TEMP RAIN)
 *
 * return value:
 *    0 :  operation successfull
 *    1 :  error: invalid parameter (month, year, hmode)
 *    2 :  error: not even one dayfile found
 *    3 :  error: dayfiles have not the same location id
 *    4 :  error: dayfiles have not the same time mode
 *    100*day+dayerror:
 *       -> errorcode / 100: error in dayfile of day (1...31)
 *       -> errorcode % 100: errorcode of dayfile (2...15)
 *
 ****************************************************************************************/
uint32_t slg_readmonth (slg_monthdata *monthdata, char *pathname,
                        uint32_t year, uint32_t month, uint32_t hmode);



/* month checker functions ************************************************************************/
/**************************************************************************************************/
/**************************************************************************************************/


/* checks if a column of a special typ and id exists in all existing dayfiles of month
 *
 * parameters:
 *   *monthdata : mothdata object
 *   typ        : column typ
 *   id         : column id
 *
 * return value:
 *         0 :  column with typ and id does not exist in all dayfiles of month
 *         1 :  column with typ and id exists in all existing dayfiles of month
 *
 ****************************************************************************************/
uint32_t slg_colexist_month (slg_monthdata *monthdata, uint32_t typ, uint32_t id);




#endif
