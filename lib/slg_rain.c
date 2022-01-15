/***************************************************************************************************
 *
 * file     : slg_rain.c
 *
 * function : senslog project c-library - rain processing functions
 *
 * author   : Jochen Ertel
 *
 * created  : 15.01.2022
 * updated  : 15.01.2022
 *
 **************************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "slg_rain.h"
#include "slg_date.h"
#include "slg_values.h"
#include "slg_dayfile.h"



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
uint32_t slg_drain_read (slg_drain *drain, slg_daydata *daydata, uint32_t id)
{
  uint32_t c, i;

  c = slg_colexist (daydata, DF_RAIN, id);
  if (c == 0) return (1);

  drain->tmode = daydata->tmode;
  drain->tlen = slg_timeindexnum (daydata->tmode);
  drain->last = slg_lastmline (daydata);
  strcpy (drain->name, daydata->colstr[c-2]);

  for (i=0; i < drain->tlen; i++) {
    drain->val[i] = slg_getrainval (daydata, c, i);
  }

  return (0);
}


/* calculates rain sum of a day
 *
 * parameters:
 *   *drain:  day rain object
 *
 * return value:
 *   srain :  sum rain*100
 *
 ****************************************************************************************/
uint32_t slg_drain_sum (slg_drain *drain)
{
  uint32_t i, res;

  /* calculate sum */
  res = 0;

  for (i = 0; i < drain->tlen; i++) {
    if (drain->val[i] != CNERR) {
      res += drain->val[i];
    }
  }

  return (res);
}

