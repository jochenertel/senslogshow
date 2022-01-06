/***************************************************************************************************
 *
 * file     : slg_date.h
 *
 * function : senslog project c-library - date functions
 *            - all calculations base on unix-time in 32 bit unsigned integer format
 *            - so, these functions here support a date range from 1970 to 2105
 *            - date and time are related to MEZ only (unix-time 0 -> 01.01.1970 01:00 o'clock)
 *            - for output strings German language is supported only
 *
 * author   : Jochen Ertel
 *
 * created  : 07.01.2020
 * updated  : 06.01.2022
 *
 **************************************************************************************************/

#include <stdint.h>


#ifndef _slg_date_h
#define _slg_date_h


/* structures *************************************************************************************/
/**************************************************************************************************/
/**************************************************************************************************/

/* date structure (is empty if all values are zero) */
typedef struct {
  uint32_t y;    /* year */
  uint32_t m;    /* month */
  uint32_t d;    /* day */
} slg_date;





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
uint32_t slg_date_set_int (slg_date *date, uint32_t day,  uint32_t month,  uint32_t year);


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
uint32_t slg_date_set_str (slg_date *date, char *dstr);


/* set date to todays value (is always related to winter time = MEZ!)
 *
 * parameters:
 *   *date:      pointer to date to be set
 *
 * return value:
 *   -
 *
 ****************************************************************************************/
void slg_date_set_today (slg_date *date);


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
void slg_date_copy (slg_date *date_d, slg_date *date_s);


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
uint32_t slg_date_inc (slg_date *date);


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
uint32_t slg_date_dec (slg_date *date);


/* subtraction of two dates (difference in days)
 *
 * parameters:
 *   *date_a:  pointer to date a
 *   *date_b:  pointer to date b
 *
 * return value:
 *   <num_days> :  date_a - date_b (0: days are equal or, at least one date is invalid)
 *
 ****************************************************************************************/
int32_t slg_date_sub (slg_date *date_a, slg_date *date_b);


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
uint32_t slg_date_dow (char *dowstr, slg_date *date);


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
uint32_t slg_date_is_summertime (slg_date *date);


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
uint32_t slg_date_compare (slg_date *date_a, slg_date *date_b);


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
uint32_t slg_date_number_days_in_month (slg_date *date);


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
uint32_t slg_date_number_days_in_year (slg_date *date);


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
uint32_t slg_date_to_string (char *str, slg_date *date);


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
uint32_t slg_date_to_fstring (char *str, slg_date *date);



#endif

