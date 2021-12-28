/***************************************************************************************************
 *
 * file     : slg_date.c
 *
 * function : senslog project c-library - date functions
 *
 * author   : Jochen Ertel
 *
 * created  : 07.01.2020
 * updated  : 27.12.2021
 *
 **************************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>

#include "slg_date.h"



/* privat functions *********************************************************************************/
/**************************************************************************************************/
/**************************************************************************************************/

/* checks whether a year is a leap-year or not
 *
 * parameters:
 *   year:  year to be checked
 *
 * return value:
 *   0 :  year is not a leap-year
 *   1 :  year is a leap-year
 *
 ****************************************************************************************/
uint32_t slg_year_is_leap (uint32_t year)
{
  if (((year % 4) == 0) && (((year % 100) != 0) || ((year % 400) == 0))) {
    return (1);
  }
  else {
    return (0);
  }
}



/* checks whether a date is a valid one or not
 *
 * parameters:
 *   *date:  pointer to date to be checked
 *
 * return value:
 *   0 :  date is invalid
 *   1 :  date does exist
 *
 ****************************************************************************************/
uint32_t slg_date_is_valid (slg_date *date)
{
  uint32_t dpm[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

  if (slg_year_is_leap(date->y)) dpm[1] = 29;

  if ((date->y < 1970) || (date->y > 2105)) return (0);
  if ((date->m < 1) || (date->m > 12)) return (0);
  if ((date->d == 0) || (date->d > dpm[date->m-1])) return (0);

  return (1);
}



/* calculates unix-time of a date and checks correct date value:
 * - calculates unix-time-value of 12 o'clock (noon) MEZ of the date
 * - example: 01.01.1970 -> 11 * 3600 seconds = 39.600
 * - if unix-time output is zero, an error occured (invalid date)
 *
 * parameters:
 *   *date:  pointer to date to be converted
 *
 * return value:
 *   0   :  in error case (date is invalid)
 *   > 0 :  unix-time as explained above
 *
 ****************************************************************************************/
uint32_t slg_date_to_unix (slg_date *date)
{
  uint32_t days_acc[12] = {0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334};
  uint32_t num_leap_years, num_days, num_seconds;

  /* at first check if date is a valid one */
  if (! slg_date_is_valid (date)) return (0);

  /* number of leap years since 1970 */
  num_leap_years = ((date->y-1)-1968)/4 - ((date->y-1)-1900)/100 + ((date->y-1)-1600)/400;

  /* number of days since 01.01.1970 */
  num_days = (date->y-1970)*365 + num_leap_years + days_acc[date->m-1] + date->d-1;
  if ((date->m > 2) && (slg_year_is_leap(date->y))) num_days += 1;

  /* unix-time of date at 12:00 o'clock MEZ (11:00 o'clock UTC) */
  num_seconds = 86400 * num_days + 39600;

  return (num_seconds);
}



/* calculates date of a given unix-time:
 * - calculates MEZ date of a unix-time-value
 * - any unix-time value (32 bit value) can be converted
 *
 * parameters:
 *   *date:      pointer to date variable for result
 *   unix_time:  unix time
 *
 * return value:
 *   -
 *
 ****************************************************************************************/
void slg_unix_to_date (slg_date *date, uint32_t unix_time)
{
  /* this array contains day-numbers of 31.12. of all leap years */
  uint32_t leap_days[33] = {1095, 2556, 4017, 5478, 6939, 8400, 9861, 11322, 12783, 14244,
                            15705, 17166, 18627, 20088, 21549, 23010, 24471, 25932, 27393,
                            28854, 30315, 31776, 33237, 34698, 36159, 37620, 39081, 40542,
                            42003, 43464, 44925, 46386, 49307};
  uint32_t dpm[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
  uint32_t mez, num_days, num_leap_years, num_days_year, i;

  /* add time shift between MEZ and UTC */
  mez = unix_time + 3600;
  if (mez < 3600) mez = 0xffffffff;   /* handle overflow */

  /* number of days since 01.01.1970 */
  num_days = mez / 86400;

  /* look for number of leap years occured before, except current year */
  i = 0;
  while ((leap_days[i] < num_days) && (i < 33)) i++;
  num_leap_years = i;

  /* calculate year */
  date->y = ((num_days - num_leap_years) / 365) + 1970;
  if (num_leap_years < 33) {
    if (leap_days[num_leap_years] == num_days) date->y -= 1;
  }

  /* calculate month and day */
  num_days_year = num_days - num_leap_years - (date->y - 1970) * 365;
  if (slg_year_is_leap(date->y)) dpm[1] = 29;

  i = 0;
  while (dpm[i] <= num_days_year) {
    num_days_year -= dpm[i];
    i++;
  }
  date->m = i + 1;
  date->d = num_days_year + 1;
}



/* date functions *********************************************************************************/
/**************************************************************************************************/
/**************************************************************************************************/

/* set date from integers
 *
 * parameters:
 *   *date:   pointer to result date structure
 *   day:     day
 *   month:   month
 *   year:    year
 *
 * return value:
 *   0 : in error case (date is invalid)
 *   1 : correct date was read
 *
 ****************************************************************************************/
uint32_t slg_date_set_int (slg_date *date, uint32_t day,  uint32_t month,  uint32_t year)
{
  /* set date */
  date->d = day;
  date->m = month;
  date->y = year;

  /* check if date is a valid one */
  if (! slg_date_is_valid (date)) return (0);

  return (1);
}



/* set date from string
 *
 * parameters:
 *   *date:   pointer to result date structure
 *   *dstr:   pointer to string which includes a date
 *            (allowed formats: d.m.yyyy or dd.m.yyyy or d.mm.yyyy or dd.mm.yyyy)
 *
 * return value:
 *   0 : in error case (date is invalid)
 *   1 : correct date was read
 *
 ****************************************************************************************/
uint32_t slg_date_set_str (slg_date *date, char *dstr)
{
  char *d, *m, *y, temp[11];
  int r;
  unsigned long v;

  /* check and copy dstr */
  if ((strlen(dstr) < 8) || (strlen(dstr) > 10)) return (0);
  if (!((dstr[1] == '.') || (dstr[2] == '.'))) return (0);
  if (!((dstr[3] == '.') || (dstr[4] == '.') || (dstr[5] == '.'))) return (0);
  strcpy (temp, dstr);

  /* set positions of day, month and year sub values */
  d = &temp[0];
  if (temp[1] == '.') {temp[1] = 0; m = &temp[2];}
  if (temp[2] == '.') {temp[2] = 0; m = &temp[3];}
  if (temp[3] == '.') {temp[3] = 0; y = &temp[4];}
  if (temp[4] == '.') {temp[4] = 0; y = &temp[5];}
  if (temp[5] == '.') {temp[5] = 0; y = &temp[6];}

  /* read day, month and year values */
  r = sscanf (d, "%lu", &v);
  if (r == 0) return (0);
  date->d = (uint32_t) v;
  r = sscanf (m, "%lu", &v);
  if (r == 0) return (0);
  date->m = (uint32_t) v;
  r = sscanf (y, "%lu", &v);
  if (r == 0) return (0);
  date->y = (uint32_t) v;

  /* check if date is a valid one */
  if (! slg_date_is_valid (date)) return (0);

  return (1);
}



/* set date to todays value (is always related to winter time = MEZ!)
 *
 * parameters:
 *   *date:      pointer to date to be set
 *
 * return value:
 *   -
 *
 ****************************************************************************************/
void slg_date_set_today (slg_date *date)
{
  uint32_t udate;

  /* get current unix-time */
  udate = (uint32_t) time (NULL);

  /* convert to date */
  slg_unix_to_date (date, udate);
}



/* copies a date
 * - date_d = date_s
 *
 * parameters:
 *   *date_d:  pointer to date which will be set to date_s value
 *   *date_s:  pointer to source date
 *
 * return value:
 *   -
 *
 ****************************************************************************************/
void slg_date_copy (slg_date *date_d, slg_date *date_s)
{
  date_d->d = date_s->d;
  date_d->m = date_s->m;
  date_d->y = date_s->y;
}



/* increases a date by one day
 * - maximum date value supported is 31.12.2105
 *
 * parameters:
 *   *date:      pointer to date to be increased
 *
 * return value:
 *   0 :  in error case (date is invalid or date overflow)
 *   1 :  ok
 *
 ****************************************************************************************/
uint32_t slg_date_inc (slg_date *date)
{
  uint32_t udate;

  /* convert date to unix time and check it inherently */
  udate = slg_date_to_unix (date);
  if (udate == 0) return (0);

  /* check for date overflow */
  if ((date->y == 2105) && (date->m == 12) && (date->d == 31)) return (0);

  /* increase by one day */
  udate += 86400;

  /* reconvert to date */
  slg_unix_to_date (date, udate);

  return (1);
}



/* decreases a date by one day
 * - minimum date value supported is 01.01.1970
 *
 * parameters:
 *   *date:      pointer to date to be decreased
 *
 * return value:
 *   0 :  in error case (date is invalid or date underflow)
 *   1 :  ok
 *
 ****************************************************************************************/
uint32_t slg_date_dec (slg_date *date)
{
  uint32_t udate;

  /* convert date to unix time and check it inherently */
  udate = slg_date_to_unix (date);
  if (udate == 0) return (0);

  /* check for date underflow */
  if ((date->y == 1970) && (date->m == 1) && (date->d == 1)) return (0);

  /* decrease by one day */
  udate -= 86400;

  /* reconvert to date */
  slg_unix_to_date (date, udate);

  return (1);
}



/* calculate day of week in numeric and string format
 *
 * parameters:
 *   *dowstr:    pointer to char array of min. 11 chars or NULL if not needed
 *   *date:      pointer to date
 *
 * return value:
 *   0 :       in error case (date is invalid)
 *   1 .. 7 :  day of week (1: Montag, ..., 7: Sonntag)
 *
 ****************************************************************************************/
uint32_t slg_date_dow (char *dowstr, slg_date *date)
{
  uint32_t udate, dow;

  /* convert date to unix time and check it inherently */
  udate = slg_date_to_unix (date);
  if (udate == 0) return (0);

  /* calculate day number */
  udate = udate / 86400;

  /* calculate day of week (1.1.1970 was Donnerstag) */
  dow = ((udate + 3) % 7) + 1;

  /* print string */
  if (dowstr != NULL) {
    if (dow == 1) strcpy (dowstr, "Montag");
    if (dow == 2) strcpy (dowstr, "Dienstag");
    if (dow == 3) strcpy (dowstr, "Mittwoch");
    if (dow == 4) strcpy (dowstr, "Donnerstag");
    if (dow == 5) strcpy (dowstr, "Freitag");
    if (dow == 6) strcpy (dowstr, "Samstag");
    if (dow == 7) strcpy (dowstr, "Sonntag");
  }

  return (dow);
}



/* checks whether a date is summertime or not
 *
 * parameters:
 *   *date:  pointer to date to be checked
 *
 * return value:
 *   0 :  date is invalid or date in normal time
 *   1 :  date is summertime
 *
 ****************************************************************************************/
uint32_t slg_date_is_summertime (slg_date *date)
{
  slg_date date_first, date_last;
  uint32_t udate, udate_first, udate_last;

  /* convert date to unix time and check it inherently */
  udate = slg_date_to_unix (date);
  if (udate == 0) return (0);

  /* get first date of summertime in target year (last Sonntag in March) */
  date_first.y = date->y;
  date_first.m = 3;
  date_first.d = 25; /* the earliest summertime day of year possible */
  while ((slg_date_dow (NULL, &date_first)) != 7) {
    slg_date_inc (&date_first);
  }
  udate_first = slg_date_to_unix (&date_first);

  /* get last date of summertime in target year (last Sonntag in October) */
  date_last.y = date->y;
  date_last.m = 10;
  date_last.d = 25; /* the earliest normaltime day in autumn possible */
  while ((slg_date_dow (NULL, &date_last)) != 7) {
    slg_date_inc (&date_last);
  }
  slg_date_dec (&date_last);
  udate_last = slg_date_to_unix (&date_last);

  /* check for summertime */
  if ((udate >= udate_first) && (udate <= udate_last)) {
    return (1);
  }

  return (0);
}



/* compares two dates
 *
 * parameters:
 *   *date_a:  pointer to date a
 *   *date_b:  pointer to date b
 *
 * return value:
 *   0 :  at least one of the dates is invalid
 *   1 :  date_a == date_b
 *   2 :  date_a < date_b
 *   3 :  date_a > date_b
 *
 ****************************************************************************************/
uint32_t slg_date_compare (slg_date *date_a, slg_date *date_b)
{
  uint32_t udate_a, udate_b;

  /* convert dates to unix time and check them inherently */
  udate_a = slg_date_to_unix (date_a);
  if (udate_a == 0) return (0);
  udate_b = slg_date_to_unix (date_b);
  if (udate_b == 0) return (0);

  /* compare the dates */
  if (udate_a == udate_b) return (1);
  if (udate_a < udate_b)  return (2);
  if (udate_a > udate_b)  return (3);

  return (0);
}



/* get back number of days of addressed month
 *
 * parameters:
 *   *date:  pointer to date
 *
 * return value:
 *    0 :  date is invalid
 *   >0 :  day number of month
 *
 ****************************************************************************************/
uint32_t slg_date_number_days_in_month (slg_date *date)
{
  uint32_t dpm[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
  uint32_t result;

  /* check date */
  if (slg_date_to_unix (date) == 0) return (0);

  /* calculate number of days */
  result = dpm[date->m -1];
  if ((date->m == 2) && (slg_year_is_leap(date->y))) result += 1;

  return (result);
}



/* get back number of days of addressed year
 *
 * parameters:
 *   *date:  pointer to date
 *
 * return value:
 *    0 :  date is invalid
 *   >0 :  day number of year
 *
 ****************************************************************************************/
uint32_t slg_date_number_days_in_year (slg_date *date)
{
  uint32_t result;

  /* check date */
  if (slg_date_to_unix (date) == 0) return (0);

  /* calculate number of days */
  result = 365;
  if (slg_year_is_leap(date->y)) result += 1;

  return (result);
}



/* convertion of date to string (e.g. "27.12.2021")
 *
 * parameters:
 *   *str:    pointer to char array of min. 11 chars
 *   *date:   pointer to date
 *
 * return value:
 *   0 :  in error case (date is invalid)
 *   1 :  ok
 *
 ****************************************************************************************/
uint32_t slg_date_to_string (char *str, slg_date *date)
{
  /* check date */
  if (slg_date_to_unix (date) == 0) return (0);

  /* make string */
  if (date->d < 10) {
    str[0] = '0';
    sprintf (&str[1], "%lu", (unsigned long) date->d);
  }
  else {
    sprintf (&str[0], "%lu", (unsigned long) date->d);
  }

  str[2] = '.';

  if (date->m < 10) {
    str[3] = '0';
    sprintf (&str[4], "%lu", (unsigned long) date->m);
  }
  else {
    sprintf (&str[3], "%lu", (unsigned long) date->m);
  }

  str[5] = '.';

  sprintf (&str[6], "%lu", (unsigned long) date->y);

  return (1);
}



/* convertion of date to file name string (e.g. "2021-12-27")
 *
 * parameters:
 *   *str:    pointer to char array of min. 11 chars
 *   *date:   pointer to date
 *
 * return value:
 *   0 :  in error case (date is invalid)
 *   1 :  ok
 *
 ****************************************************************************************/
uint32_t slg_date_to_fstring (char *str, slg_date *date)
{
  /* check date */
  if (slg_date_to_unix (date) == 0) return (0);

  /* make string */
  sprintf (&str[0], "%lu", (unsigned long) date->y);

  str[4] = '-';

  if (date->m < 10) {
    str[5] = '0';
    sprintf (&str[6], "%lu", (unsigned long) date->m);
  }
  else {
    sprintf (&str[5], "%lu", (unsigned long) date->m);
  }

  str[7] = '-';

  if (date->d < 10) {
    str[8] = '0';
    sprintf (&str[9], "%lu", (unsigned long) date->d);
  }
  else {
    sprintf (&str[8], "%lu", (unsigned long) date->d);
  }

  return (1);
}




