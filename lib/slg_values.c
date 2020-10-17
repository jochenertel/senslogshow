/***************************************************************************************************
 *
 * file     : slg_values.c
 *
 * function : senslog project c-library - meassurment conversion functions
 *
 * author   : Jochen Ertel
 *
 * created  : 17.10.2020
 * updated  : 17.10.2020
 *
 **************************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "slg_values.h"



/* private functions ******************************************************************************/
/**************************************************************************************************/
/**************************************************************************************************/


# define CNERR 2000000000


/* converts a string into an uint32_t less then CNERR
 *
 * parameters:
 *   *str:  pointer to a c-string
 *
 * return value:
 *   CNERR :  error, invalid string
 *   other :  number
 *
 ****************************************************************************************/
uint32_t slg_str2uint32 (char *str)
{
  unsigned long k;
  uint32_t w;
  size_t i, s;
  int r;

  s = strlen(str);

  if ((s < 1) || (s > 11)) return (CNERR);

  /* check that first char is a number or '+' and all other chars are numbers */
  if (!(((str[0] >= 0x30) && (str[0] <= 0x39)) || (str[0] == 0x2B))) return (CNERR);
  for (i=1; i < s ; i++) {
    if ((str[i] < 0x30) || (str[i] > 0x39)) return (CNERR);
  }

  r = sscanf(str, "%lu", &k);

  if (r != 1) return (CNERR);

  w = (uint32_t) k;

  if (w >= CNERR) return (CNERR);

  return (w);
}



/* converts a string into an int32_t less between -CNERR and +CNERR
 *
 * parameters:
 *   *str:  pointer to a c-string
 *
 * return value:
 *   CNERR :  error, invalid string
 *   other :  number
 *
 ****************************************************************************************/
int32_t slg_str2int32 (char *str)
{
  long k;
  int32_t w;
  size_t i, s;
  int r;

  s = strlen(str);

  if ((s < 1) || (s > 11)) return (CNERR);

  /* check that first char is a number or '+' or '-' and all other chars are numbers */
  if (!(((str[0] >= 0x30) && (str[0] <= 0x39)) || (str[0] == 0x2B) || (str[0] == 0x2D))) return (CNERR);
  for (i=1; i < s ; i++) {
    if ((str[i] < 0x30) || (str[i] > 0x39)) return (CNERR);
  }

  r = sscanf(str, "%li", &k);

  if (r != 1) return (CNERR);

  w = (int32_t) k;

  if ((w <= -CNERR) || (w >= CNERR)) return (CNERR);

  return (w);
}







/* convert functions ******************************************************************************/
/**************************************************************************************************/
/**************************************************************************************************/

/* converts a day time string into index value depending on mode
 *
 * parameters:
 *   mode:  0: 96 values per day (15 min steps) starting with 00:00 ending with 23:45
 *          1: 96 values per day (15 min steps) starting with 00:15 ending with 24:00
 *   *str:  pointer to string of fix size (5 chars, format e.g. "17:30")
 *
 * return value:
 *     0 :  error, invalid string or mode
 *   > 0 :  index of day time (1, 2, ...)
 *
 ****************************************************************************************/
uint32_t slg_str_to_time_ind (uint32_t mode, char *str)
{
  uint32_t h, m, ind;
  char tmp[3];

  if (mode > 1) return (0);

  if (strlen(str) != 5) return (0);
  if (str[2] != ':') return (0);

  tmp[0] = str[0];
  tmp[1] = str[1];
  tmp[2] = 0;
  h = slg_str2uint32 (tmp);
  if (h == CNERR) return (0);

  tmp[0] = str[3];
  tmp[1] = str[4];
  m = slg_str2uint32 (tmp);
  if (m == CNERR) return (0);

  if (mode == 0) {
    if (h > 23) return (0);
    if (m > 59) return (0);
    if (m % 15) return (0);

    ind = (h * 4) + (m / 15) + 1;
  }

  if (mode == 1) {
    if (h > 24) return (0);
    if (m > 59) return (0);
    if (m % 15) return (0);
    if ((h == 0) && (m == 0)) return (0);
    if ((h == 24) && (m != 0)) return (0);

    ind = (h * 4) + (m / 15);
  }

  return (ind);
}



/* converts a day time index value into a string depending on mode
 *
 * parameters:
 *   *str:   pointer to target string (at least 6 chars size)
 *   mode:   0: 96 values per day (15 min steps) starting with 00:00 ending with 23:45
 *           1: 96 values per day (15 min steps) starting with 00:15 ending with 24:00
 *   summer: summer time flag (0: winter time, >0: summer time)
 *   ind:    index value (1, 2, ...)
 *
 ****************************************************************************************/
void slg_time_ind_to_str (char *str, uint32_t mode, uint32_t summer, uint32_t ind)
{
  uint32_t i, h, m;

  /* handle error case of input parameters by setting string to "EE:EE" */
  if ((mode > 1) || (summer > 1) || (ind == 0) || (ind > 96)) {
    strcpy (str, "EE:EE");
    return;
  }

  if (mode == 0) {
    i = ind - 1;
    h = i / 4;
    m = (i % 4) * 15;
  }

  if (mode == 1) {
    i = ind;
    h = i / 4;
    m = (i % 4) * 15;
  }

  if (summer) h++;

  if (h < 10) {
    str[0] = '0';
    sprintf(&str[1], "%lu", (unsigned long) h);
  }
  else {
    sprintf(&str[0], "%lu", (unsigned long) h);
  }

  str[2] = ':';

  if (m < 10) {
    str[3] = '0';
    sprintf(&str[4], "%lu", (unsigned long) m);
  }
  else {
    sprintf(&str[3], "%lu", (unsigned long) m);
  }

  str[5] = 0;
}




