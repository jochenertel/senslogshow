/***************************************************************************************************
 *
 * file     : slg_temper.c
 *
 * function : senslog project c-library - temperature processing functions
 *
 * author   : Jochen Ertel
 *
 * created  : 09.01.2022
 * updated  : 23.01.2022
 *
 **************************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "slg_temper.h"
#include "slg_date.h"
#include "slg_values.h"
#include "slg_dayfile.h"



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
uint32_t slg_dtemper_read (slg_dtemper *dtemper, slg_daydata *daydata, uint32_t id)
{
  uint32_t c, i;

  c = slg_colexist (daydata, DF_TEMP, id);
  if (c == 0) return (1);

  dtemper->tmode = daydata->tmode;
  dtemper->tlen = slg_timeindexnum (daydata->tmode);
  dtemper->last = slg_lastmline (daydata);
  strcpy (dtemper->name, daydata->colstr[c-2]);

  for (i=0; i < dtemper->tlen; i++) {
    dtemper->val[i] = slg_gettemperval (daydata, c, i);
  }

  return (0);
}


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
uint32_t slg_dtemper_indmax (slg_dtemper *dtemper)
{
  uint32_t i, ind;
  int32_t  max;

  /* check if all values are invalid */
  i = 0;
  while ((dtemper->val[i] == CNERR) && (i != (dtemper->tlen - 1))) {
    i++;
  }
  if (dtemper->val[i] == CNERR) return (0);

  /* find maximum */
  max = - CNERR;

  for (i = 0; i < dtemper->tlen; i++) {
    if (dtemper->val[i] != CNERR) {
      if (dtemper->val[i] >= max) {
        ind = i;
        max = dtemper->val[i];
      }
    }
  }

  return (ind);
}


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
uint32_t slg_dtemper_indmin (slg_dtemper *dtemper)
{
  uint32_t i, ind;
  int32_t  min;

  /* check if all values are invalid */
  i = 0;
  while ((dtemper->val[i] == CNERR) && (i != (dtemper->tlen - 1))) {
    i++;
  }
  if (dtemper->val[i] == CNERR) return (0);

  /* find minimum */
  min = CNERR;

  for (i = 0; i < dtemper->tlen; i++) {
    if (dtemper->val[i] != CNERR) {
      if (dtemper->val[i] <= min) {
        ind = i;
        min = dtemper->val[i];
      }
    }
  }

  return (ind);
}


/* calculates average temperature of a day
 *
 * parameters:
 *   *dtemper:  day temperature object
 *
 * return value:
 *   atemper :  average temperature T*10
 *
 ****************************************************************************************/
int32_t slg_dtemper_average (slg_dtemper *dtemper)
{
  uint32_t i;
  int32_t  at, k;

  /* check if all values are invalid */
  i = 0;
  while ((dtemper->val[i] == CNERR) && (i != (dtemper->tlen - 1))) {
    i++;
  }
  if (dtemper->val[i] == CNERR) return (CNERR);

  /* calculate average */
  at = 0;
  k = 0;

  for (i = 0; i < dtemper->tlen; i++) {
    if (dtemper->val[i] != CNERR) {
      at += dtemper->val[i];
      k++;
    }
  }

  return (at / k);
}


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
int32_t slg_dtemper_maxindayout30 (slg_dtemper *dtemper, slg_date *date)
{
  int32_t  th[12] = {150, 150, 200, 250, 300, 350, 350, 350, 300, 250, 200, 150};
  int32_t  tmin, tmax, res;
  uint32_t k;

  k = slg_dtemper_indmin (dtemper);
  tmin = dtemper->val[k];

  k = slg_dtemper_indmax (dtemper);
  tmax = dtemper->val[k];

  res = th[date->m -1];
  if ((tmin != CNERR) && (tmax != CNERR)) {
    if (tmax > res) res += 50;
    if (tmin < (res-300)) res -= 50;
  }

  return (res);
}


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
                              slg_dtemper *dtemper2, uint32_t invwind2b, uint32_t invwind2e)
{
  uint32_t i;

  /* check input ****************************************************/
  if (dtemper1->tmode != dtemper2->tmode) return (1);
  if (dtemper1->tlen != dtemper2->tlen) return (1);
  if (dtemper1->last != dtemper2->last) return (1);

  /* set header values **********************************************/
  dtemper->tmode = dtemper1->tmode;
  dtemper->tlen = dtemper1->tlen;
  dtemper->last = dtemper1->last;
  strcpy (dtemper->name, name);

  /* merge temperature values ***************************************/
  for (i = 0; i < dtemper->tlen; i++) {

    if ((dtemper1->val[i] != CNERR) && (dtemper2->val[i] != CNERR)) {
      if (dtemper1->val[i] < dtemper2->val[i]) dtemper->val[i] = dtemper1->val[i];
      else dtemper->val[i] = dtemper2->val[i];
    }

    if ((dtemper1->val[i] == CNERR) && (dtemper2->val[i] == CNERR)) {
      dtemper->val[i] = CNERR;
    }

    if ((dtemper1->val[i] != CNERR) && (dtemper2->val[i] == CNERR)) {
      if ((i >= invwind1b) && (i <= invwind1e)) dtemper->val[i] = CNERR;
      else dtemper->val[i] = dtemper1->val[i];
    }

    if ((dtemper1->val[i] == CNERR) && (dtemper2->val[i] != CNERR)) {
      if ((i >= invwind2b) && (i <= invwind2e)) dtemper->val[i] = CNERR;
      else dtemper->val[i] = dtemper2->val[i];
    }
  }

  return (0);
}




