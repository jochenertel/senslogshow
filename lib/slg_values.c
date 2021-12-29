/***************************************************************************************************
 *
 * file     : slg_values.c
 *
 * function : senslog project c-library - meassurment conversion functions
 *
 * author   : Jochen Ertel
 *
 * created  : 17.10.2020
 * updated  : 29.12.2021
 *
 **************************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "slg_values.h"



/* convert functions ******************************************************************************/
/**************************************************************************************************/
/**************************************************************************************************/

/* converts a string into an uint32_t less then CNERR
 *
 * parameters:
 *   *str:  pointer to a c-string
 *
 * return value:
 *   CNERR :  error, invalid string
 *   other :  0 <= number < CNERR
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



/* converts a string into an int32_t between -CNERR and +CNERR
 *
 * parameters:
 *   *str:  pointer to a c-string
 *
 * return value:
 *   CNERR :  error, invalid string
 *   other :  -CNERR < number < CNERR
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



/* get time index number per day dependent on mode
 *
 * parameters:
 *   mode:  0: 96 values per day (15 min steps) starting with 00:00 ending with 23:45
 *          1: 96 values per day (15 min steps) starting with 00:15 ending with 24:00
 *
 * return value:
 *   CNERR :  error, invalid mode
 *   other :  number of time index values per day
 *
 ****************************************************************************************/
uint32_t slg_timeindexnum (uint32_t mode)
{
  uint32_t num;

  num = CNERR;
  if (mode == 0) num = 96;
  if (mode == 1) num = 96;

  return (num);
}



/* converts a day time string into index value depending on mode
 *
 * parameters:
 *   mode:  0: 96 values per day (15 min steps) starting with 00:00 ending with 23:45
 *          1: 96 values per day (15 min steps) starting with 00:15 ending with 24:00
 *   *str:  pointer to string of fix size (5 chars, format e.g. "17:30")
 *
 * return value:
 *   CNERR :  error, invalid string or mode
 *   other :  index of day time (0, 1, 2, ...)
 *
 ****************************************************************************************/
uint32_t slg_str2timeindex (uint32_t mode, char *str)
{
  uint32_t h, m, ind;
  char tmp[3];

  if (mode > 1) return (CNERR);

  if (strlen(str) != 5) return (CNERR);
  if (str[2] != ':') return (CNERR);

  tmp[0] = str[0];
  tmp[1] = str[1];
  tmp[2] = 0;
  h = slg_str2uint32 (tmp);
  if (h == CNERR) return (CNERR);

  tmp[0] = str[3];
  tmp[1] = str[4];
  m = slg_str2uint32 (tmp);
  if (m == CNERR) return (CNERR);

  if (mode == 0) {
    if (h > 23) return (CNERR);
    if (m > 59) return (CNERR);
    if (m % 15) return (CNERR);

    ind = (h * 4) + (m / 15);
  }

  if (mode == 1) {
    if (h > 24) return (CNERR);
    if (m > 59) return (CNERR);
    if (m % 15) return (CNERR);
    if ((h == 0) && (m == 0)) return (CNERR);
    if ((h == 24) && (m != 0)) return (CNERR);

    ind = (h * 4) + (m / 15) - 1;
  }

  return (ind);
}



/* converts a day time index value into a string depending on mode
 *
 * parameters:
 *   *str:   pointer to target string (at least 6 chars size)
 *   mode:   0: 96 values per day (15 min steps) starting with 00:00 ending with 23:45
 *           1: 96 values per day (15 min steps) starting with 00:15 ending with 24:00
 *   summer: summer time flag (0: winter time, 1: summer time)
 *   ind:    index value (0, 1, 2, ...)
 *
 ****************************************************************************************/
void slg_timeindex2str (char *str, uint32_t mode, uint32_t summer, uint32_t ind)
{
  uint32_t i, h, m;

  /* handle error case of input parameters by setting string to "EE:EE" */
  if ((mode > 1) || (summer > 1) || (ind > 95)) {
    strcpy (str, "EE:EE");
    return;
  }

  if (mode == 0) {
    i = ind;
    h = i / 4;
    m = (i % 4) * 15;
  }

  if (mode == 1) {
    i = ind + 1;
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



/* converts a temperature string into integer (-99.9 to 999.9 deg celsius)
 *
 * parameters:
 *   *str:  pointer to string (3..5 chars, format e.g. "-13.7")
 *
 * return value:
 *   CNERR :  error, invalid string
 *   temp  :  temperature T*10
 *
 ****************************************************************************************/
int32_t slg_str2temper (char *str)
{
  size_t i, s;
  char tmp[5];

  s = strlen(str);

  if ((s < 3) || (s > 5)) return (CNERR);
  if (str[s-2] != '.') return (CNERR);

  for (i=0; i < (s-2); i++) tmp[i] = str[i];
  tmp[s-2] = str[s-1];
  tmp[s-1] = 0;

  return (slg_str2int32(tmp));
}



/* converts an integer value into a temperature string depending on mode
 *
 * parameters:
 *   *str:   pointer to target string (at least 6 chars size)
 *   mode:   0: variable string size (3 to 5 chars)
 *           1: fix string size (5 chars with leading spaces)
 *   temper: temperature value in tenth degree (-999 ... 9999)
 *
 ****************************************************************************************/
void slg_temper2str (char *str, uint32_t mode, int32_t temper)
{
  size_t i;
  int32_t t, z;

  /* handle error case of input parameters by setting string to "EEE.E" */
  if ((mode > 1) || (temper < -999) || (temper > 9999)) {
    strcpy (str, "EEE.E");
    return;
  }

  i = 0;
  t = temper;

  if (mode == 1) {
    if ((t > -100) && (t < 0)) {
      str[i] = ' '; i++;
    }
    if ((t > -1) && (t < 100)) {
      str[i] = ' '; i++;
      str[i] = ' '; i++;
    }
    if ((t > 99) && (t < 1000)) {
      str[i] = ' '; i++;
    }
  }

  if (t < 0) {
    str[i] = '-'; i++;
    t *= -1;
  }

  if (t / 1000) {
    z = t / 1000;
    str[i] = 0x30 + (char) z;
    i++;
  }

  if (t / 100) {
    z = (t / 100) % 10;
    str[i] = 0x30 + (char) z;
    i++;
  }

  if (t / 10) {
    z = (t / 10) % 10;
    str[i] = 0x30 + (char) z;
    i++;
  }

  if (t < 10) {
    str[i] = 0x30;
    i++;
  }

  str[i] = '.';
  i++;

  z = t % 10;
  str[i] = 0x30 + (char) z;
  i++;

  str[i] = 0;
}



/* converts a rain string into integer (0.00 to 99.99 mm)
 *
 * parameters:
 *   *str:  pointer to string (4..5 chars, format e.g. "12.75")
 *
 * return value:
 *   CNERR :  error, invalid string
 *   rain  :  rain*100
 *
 ****************************************************************************************/
uint32_t slgtmp_str2rain (char *str)
{
  size_t i, s;
  char tmp[5];

  s = strlen(str);

  if ((s < 4) || (s > 5)) return (CNERR);
  if (str[s-3] != '.') return (CNERR);

  for (i=0; i < (s-3); i++) tmp[i] = str[i];
  tmp[s-3] = str[s-2];
  tmp[s-2] = str[s-1];
  tmp[s-1] = 0;

  return (slg_str2uint32(tmp));
}



/* converts a rain string (old) into integer (0.0 to 99.9 mm)
 *
 * parameters:
 *   *str:  pointer to string (3..4 chars, format e.g. "12.4")
 *
 * return value:
 *   CNERR :  error, invalid string
 *   rain  :  rain*100
 *
 ****************************************************************************************/
uint32_t slgtmp_str2rainold (char *str)
{
  size_t i, s;
  char tmp[5];

  s = strlen(str);

  if ((s < 3) || (s > 4)) return (CNERR);
  if (str[s-2] != '.') return (CNERR);

  for (i=0; i < (s-2); i++) tmp[i] = str[i];
  tmp[s-2] = str[s-1];
  tmp[s-1] = '0';
  tmp[s] = 0;

  return (slg_str2uint32(tmp));
}



/* converts a rain string into integer (0.00 to 99.99 mm)
 * !!! -> temporary a wrapper function for:
 * !!!    - slgtmp_str2rain()
 * !!!    - slgtmp_str2rainold()
 * !!! -> automatic format detection and value correction in old case (* 25/20)
 *
 * parameters:
 *   *str:  pointer to string (4..5 chars, format e.g. "12.75")
 *
 * return value:
 *   CNERR :  error, invalid string
 *   rain  :  rain*100
 *
 ****************************************************************************************/
uint32_t slg_str2rain (char *str)
{
  uint32_t tmp;

  tmp = slgtmp_str2rain (str);
  if (tmp != CNERR) return (tmp);

  tmp = slgtmp_str2rainold (str);
  if (tmp != CNERR) {
    tmp = (tmp * 5) / 4;
    return (tmp);
  }

  return (CNERR);
}



/* converts an integer value into a rain string depending on mode
 *
 * parameters:
 *   *str: pointer to target string (at least 8 chars size)
 *   mode: 0: variable string size (4 to 7 chars)
 *         1: fix string size (5 chars with leading spaces, up to 99.99)
 *   rain: rain value in 100th mm (0 ... 999999)
 *
 ****************************************************************************************/
void slg_rain2str (char *str, uint32_t mode, uint32_t rain)
{
  size_t i;
  uint32_t r, z;

  /* handle error cases of input parameters by setting string to "EE.EE" */
  if ((mode > 1) || ((mode == 0) && (rain > 999999)) || ((mode == 1) && (rain > 9999))) {
    strcpy (str, "EE.EE");
    return;
  }

  i = 0;
  r = rain;

  if (mode == 1) {
    if (r < 1000) {
      str[i] = ' '; i++;
    }
  }

  if (r / 100000) {
    z = r / 100000;
    str[i] = 0x30 + (char) z;
    i++;
  }

  if (r / 10000) {
    z = (r / 10000) % 10;
    str[i] = 0x30 + (char) z;
    i++;
  }

  if (r / 1000) {
    z = (r / 1000) % 10;
    str[i] = 0x30 + (char) z;
    i++;
  }

  if (r / 100) {
    z = (r / 100) % 10;
    str[i] = 0x30 + (char) z;
    i++;
  }

  if (r < 100) {
    str[i] = 0x30;
    i++;
  }

  str[i] = '.';
  i++;

  z = (r / 10) % 10;
  str[i] = 0x30 + (char) z;
  i++;

  z = r % 10;
  str[i] = 0x30 + (char) z;
  i++;

  str[i] = 0;
}



