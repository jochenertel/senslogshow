/***************************************************************************************************
 *
 * file     : slg_rain.h
 *
 * function : senslog project c-library - rain processing functions
 *
 * author   : Jochen Ertel
 *
 * created  : 15.01.2022
 * updated  : 05.11.2023
 *
 **************************************************************************************************/

#include <stdint.h>

#include "slg_date.h"
#include "slg_dayfile.h"


#ifndef _slg_rain_h
#define _slg_rain_h


/* defines and structures *************************************************************************/
/**************************************************************************************************/
/**************************************************************************************************/


/* day rain array */
typedef struct {
  uint32_t  tmode;             /* time_mode */
  uint32_t  tlen;              /* array length, dependent from tmode only */
  uint32_t  last;              /* last index (last mline in related dayfile) */
  char      name[50];          /* name of rain */
  uint32_t  val[MAX_MLN_NUM];  /* rain values */
} slg_drain;


/* month rain array */
typedef struct {
  uint32_t   dvalid[31];   /* 0: day does not exist, 1: day exists */
  slg_drain  drain[31];    /* array of day rain objects */
} slg_mrain;



/* day related functions **************************************************************************/
/**************************************************************************************************/
/**************************************************************************************************/

/* read all rain values of a column from dayfile
 *
 * parameters:
 *   *drain  :  day rain object
 *   *daydata:  daydata object
 *   id      :  rain column id in daydata
 *
 * return value:
 *         0 :  successfull
 *         1 :  error, invalid id or id is not rain
 *
 ****************************************************************************************/
uint32_t slg_drain_read (slg_drain *drain, slg_daydata *daydata, uint32_t id);


/* calculates rain sum of a day
 *
 * parameters:
 *   *drain:  day rain object
 *
 * return value:
 *   srain :  sum rain*100
 *
 ****************************************************************************************/
uint32_t slg_drain_sum (slg_drain *drain);



/* month related functions ************************************************************************/
/**************************************************************************************************/
/**************************************************************************************************/

/* read all rain values of an id from all valid days of a month
 *
 * parameters:
 *   *mrain    :  month rain object
 *   *monthdata:  monthdata object
 *   id        :  rain column id in daydata files
 *
 * return value:
 *         0 :  successfull
 *         1 :  error, invalid id or id is not rain
 *
 ****************************************************************************************/
uint32_t slg_mrain_read (slg_mrain *mrain, slg_monthdata *monthdata, uint32_t id);


/* calculates rain sum of a month
 *
 * parameters:
 *   *mrain:  month rain object
 *
 * return value:
 *   srain :  sum rain*100
 *
 ****************************************************************************************/
uint32_t slg_mrain_sum (slg_mrain *mrain);




#endif

