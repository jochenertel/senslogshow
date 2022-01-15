/***************************************************************************************************
 *
 * file     : slg_temper.h
 *
 * function : senslog project c-library - temperature processing functions
 *
 * author   : Jochen Ertel
 *
 * created  : 09.01.2022
 * updated  : 15.01.2022
 *
 **************************************************************************************************/

#include <stdint.h>

#include "slg_date.h"
#include "slg_dayfile.h"


#ifndef _slg_temper_h
#define _slg_temper_h


/* defines and structures *************************************************************************/
/**************************************************************************************************/
/**************************************************************************************************/


/* day temperature array */
typedef struct {
  uint32_t  tmode;             /* time_mode */
  uint32_t  tlen;              /* array length, dependent from tmode only */
  uint32_t  last;              /* last index (last mline in related dayfile) */
  char      name[50];          /* name of temperature */
  int32_t   val[MAX_MLN_NUM];  /* temperature values */
} slg_dtemper;



/* day related functions **************************************************************************/
/**************************************************************************************************/
/**************************************************************************************************/

/* read all temperature values of a column from dayfile
 *
 * parameters:
 *   *dtemper:  day temperature object
 *   *daydata:  daydata object
 *   id      :  temperature column id in daydata
 *
 * return value:
 *         0 :  successfull
 *         1 :  error, invalid id or id is not temperature
 *
 ****************************************************************************************/
uint32_t slg_dtemper_read (slg_dtemper *dtemper, slg_daydata *daydata, uint32_t id);


/* finds index of max. temperature value
 * -> finds the newest one if there are more than one maximums
 * -> if all temperature values are invalid index 0 is returned
 *
 * parameters:
 *   *dtemper:  day temperature object
 *
 * return value:
 *       ind :  index of max. temperature value
 *
 ****************************************************************************************/
uint32_t slg_dtemper_indmax (slg_dtemper *dtemper);


/* finds index of min. temperature value
 * -> finds the newest one if there are more than one minimums
 * -> if all temperature values are invalid index 0 is returned
 *
 * parameters:
 *   *dtemper:  day temperature object
 *
 * return value:
 *       ind :  index of min. temperature value
 *
 ****************************************************************************************/
uint32_t slg_dtemper_indmin (slg_dtemper *dtemper);


/* calculates average temperature of a day
 *
 * parameters:
 *   *dtemper:  day temperature object
 *
 * return value:
 *   atemper :  average temperature T*10
 *
 ****************************************************************************************/
int32_t slg_dtemper_average (slg_dtemper *dtemper);


/* calculates max. temperature for 30 degree diagram
 * -> first rule: max. temp depends on date only
 * -> second rule: if a temperature value is out of range, max. temper. is adapted
 *
 * parameters:
 *   *dtemper:  day temperature object
 *   *date   :  date
 *
 * return value:
 *   atemper :  average temperature T*10
 *
 ****************************************************************************************/
int32_t slg_dtemper_maxindayout30 (slg_dtemper *dtemper, slg_date *date);





#endif

