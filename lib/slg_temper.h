/***************************************************************************************************
 *
 * file     : slg_temper.h
 *
 * function : senslog project c-library - temperature processing functions
 *
 * author   : Jochen Ertel
 *
 * created  : 09.01.2022
 * updated  : 30.11.2023
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


/* month temperature array */
typedef struct {
  uint32_t     dvalid[31];     /* 0: day does not exist, 1: day exists */
  slg_dtemper  dtemper[31];    /* array of day temper objects */
} slg_mtemper;



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


/* check if at least one valid temperature value exist
 *
 * parameters:
 *   *dtemper:  day temperature object
 *
 * return value:
 *         0 :  valid temperature values exist
 *         1 :  error: no valid temperature values found
 *
 ****************************************************************************************/
int32_t slg_dtemper_checkvalid (slg_dtemper *dtemper);


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


/* merges two temperature objects to a new one in the following way:
 * - for each index the lowest temperature value is taken
 * - if one value is invalid the result is invalid only if the other is in invalid window
 *
 * parameters:
 *   *dtemper :  output day temperature object
 *   *name    :  name of output object
 *   *dtemper1:  input day temperature object 1
 *   invwind1b:  invalid temperature window begin of object 1
 *   invwind1e:  invalid temperature window end of object 1
 *   *dtemper2:  input day temperature object 2
 *   invwind2b:  invalid temperature window begin of object 2
 *   invwind2e:  invalid temperature window end of object 2
 *
 * return value:
 *          0 :  successfull
 *          1 :  input objects are not of same format
 *
 ****************************************************************************************/
uint32_t slg_dtemper_merge_2 (slg_dtemper *dtemper, char *name,
                              slg_dtemper *dtemper1, uint32_t invwind1b, uint32_t invwind1e,
                              slg_dtemper *dtemper2, uint32_t invwind2b, uint32_t invwind2e);



/* month related functions ************************************************************************/
/**************************************************************************************************/
/**************************************************************************************************/

/* read all temperature values of an id from all valid days of a month
 *
 * parameters:
 *   *mtemper  :  month temperature object
 *   *monthdata:  monthdata object
 *   id        :  temperature column id in daydata files
 *
 * return value:
 *         0 :  successfull
 *         1 :  error, invalid id or id is not temperature
 *
 ****************************************************************************************/
uint32_t slg_mtemper_read (slg_mtemper *mtemper, slg_monthdata *monthdata, uint32_t id);


/* check if at least one valid temperature value exist in month
 *
 * parameters:
 *   *mtemper:  month temperature object
 *
 * return value:
 *         0 :  valid temperature values in month exist
 *         1 :  error: no valid temperature values found
 *
 ****************************************************************************************/
int32_t slg_mtemper_checkvalid (slg_mtemper *mtemper);


/* finds day of min. temperature value in month
 * -> finds the newest one if there are more than one minimums
 * -> if all temperature values of month are invalid day 0 is returned
 *
 * parameters:
 *   *mtemper:  month temperature object
 *
 * return value:
 *       day :  day number (1..31) or invalid (0)
 *
 ****************************************************************************************/
uint32_t slg_mtemper_daymin (slg_mtemper *mtemper);


/* finds day of max. temperature value in month
 * -> finds the newest one if there are more than one maximums
 * -> if all temperature values of month are invalid day 0 is returned
 *
 * parameters:
 *   *mtemper:  month temperature object
 *
 * return value:
 *       day :  day number (1..31) or invalid (0)
 *
 ****************************************************************************************/
uint32_t slg_mtemper_daymax (slg_mtemper *mtemper);


/* calculates average temperature of a month
 *
 * parameters:
 *   *mtemper:  month temperature object
 *
 * return value:
 *   atemper :  average temperature T*10
 *
 ****************************************************************************************/
int32_t slg_mtemper_average (slg_mtemper *mtemper);


/* merges two month temperature objects to a new one in the following way:
 * - for each index the lowest temperature value is taken
 * - if one value is invalid the result is invalid only if the other is in invalid window
 *
 * parameters:
 *   *mtemper :  output month temperature object
 *   *name    :  name of output object
 *   *mtemper1:  input month temperature object 1
 *   invwind1b:  invalid temperature day window begin of object 1
 *   invwind1e:  invalid temperature day window end of object 1
 *   *mtemper2:  input month temperature object 2
 *   invwind2b:  invalid temperature day window begin of object 2
 *   invwind2e:  invalid temperature day window end of object 2
 *
 * return value:
 *          0 :  successfull
 *          1 :  input objects are not of same format
 *
 ****************************************************************************************/
uint32_t slg_mtemper_merge_2 (slg_mtemper *mtemper, char *name,
                              slg_mtemper *mtemper1, uint32_t invwind1b, uint32_t invwind1e,
                              slg_mtemper *mtemper2, uint32_t invwind2b, uint32_t invwind2e);


#endif

