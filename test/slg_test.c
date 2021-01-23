/***************************************************************************************************
 *
 * file     : slg_test.c (test command line tool for slgshow library code)
 *
 * author   : Jochen Ertel
 *
 * created  : 07.01.2020
 * updated  : 23.01.2021
 *
 **************************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../lib/slg_date.h"
#include "../lib/slg_values.h"


#define VERSION "test command line tool for slgshow library code"



/***************************************************************************************************
 * functions for reading command line options and printing error messages
 **************************************************************************************************/

/* function : parArgTypExists ()
 *            -> checks whether an argument typ exists or not
 *               (an argument typ of programm call consists of 1 character
 *               with a leading '-')
 *            -> returns 0 if argument does not exist or the index of argument
 *               type in the argv[] array
 ******************************************************************************/
int parArgTypExists (int argc, char *argv[], char argType)
{
  int  i;
  char tmp[3];

  tmp[0] = '-';
  tmp[1] = argType;
  tmp[2] = 0;

  if (argc > 1) {
    for (i = 1; i < argc; i++) {
      if (!strcmp (argv[i], tmp))
        return (i);
    }
  }
  return (0);
}


/* function : parGetString ()
 *            -> gets string argument value
 *            -> returns 0 in error case, returns 1 if OK
 *               (string is limited to max. 1024 characters)
 ******************************************************************************/
int parGetString (int argc, char *argv[], char argType, char *value)
{
  int a;

  a = parArgTypExists (argc, argv, argType);

  /* error checking */
  if (a == 0) return (0);
  if (a >= (argc -1)) return (0);
  if (strlen(argv[a+1]) > 1024) return (0);

  strcpy(value, argv[a+1]);
  return (1);
}




/***************************************************************************************************
 * refernce functions for testing
 **************************************************************************************************/


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
uint32_t slg_year_is_leap_t (uint32_t year)
{
  if (((year % 4) == 0) && (((year % 100) != 0) || ((year % 400) == 0))) {
    return (1);
  }
  else {
    return (0);
  }
}




/* reference: increases a date by one day
 *
 * parameters:
 *   *date:      pointer to date to be increased
 *
 * return value:
 *   -
 *
 ****************************************************************************************/
void ref_date_inc (slg_date *date)
{
  uint32_t dpm[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

  if (slg_year_is_leap_t(date->y)) dpm[1] = 29;

  date->d += 1;
  if (date->d > dpm[date->m -1]) {
    date->d = 1;
    date->m += 1;
    if (date->m > 12) {
      date->m = 1;
      date->y += 1;
    }
  }
}





/***************************************************************************************************
 * main function
 **************************************************************************************************/

int main (int argc, char *argv[])
{
  slg_date date, date2;
  uint32_t i, k;
  int32_t t;
  char tempstr[20];

  /* help menu ************************************************************************************/
  if (parArgTypExists (argc, argv, 'h')) {
    printf (VERSION "\n");
    printf ("  -> parameters (optional):\n");
    printf ("     -h  :  prints this help menu\n");

    return (0);
  }


  /***************************************************************************/


  date.y = 0;
  date.m = 0;
  date.d = 0;

//  k = slg_date_set_int (&date, 14,  1,  2104);
//  printf ("%lu.%lu.%lu\n", (unsigned long) date.d, (unsigned long) date.m, (unsigned long) date.y);
//  printf ("Errorcode: %lu\n", (unsigned long) k);
//
//  k = slg_date_set_str (&date, "31.12.1968");
//  printf ("%lu.%lu.%lu\n", (unsigned long) date.d, (unsigned long) date.m, (unsigned long) date.y);
//  printf ("Errorcode: %lu\n", (unsigned long) k);
//
//  slg_date_set_today (&date);
//  printf ("%lu.%lu.%lu\n", (unsigned long) date.d, (unsigned long) date.m, (unsigned long) date.y);


  slg_date_set_str (&date, "29.10.1985");

  k = slg_date_to_string (tempstr, &date);
  printf ("%s (%lu)\n", tempstr, (unsigned long) k);


//  /***************************************************************************/
//
//
//  /* whole date range check */
//  date.d = 1;
//  date.m = 1;
//  date.y = 1970;
//  printf ("run whole date range check from 1970 to 2105:\n");
//  i = 0;
//  while (!((date.y == 2105) && (date.m == 12) && (date.d == 31))) {
//    i++;
//    slg_date_copy (&date2, &date);
//    k = slg_date_inc (&date);
//    if (k == 0) {
//      printf ("\nerror 1 increasing date: %lu.%lu.%lu\n", (unsigned long) date2.d, (unsigned long) date2.m, (unsigned long) date2.y);
//      return (1);
//    }
//    if (date.y != date2.y) printf ("%lu ", (unsigned long) date2.y);
//    ref_date_inc (&date2);
//    if (!((date.y == date2.y) && (date.m == date2.m) && (date.d == date2.d))) {
//      printf ("\nerror 2 increasing date: %lu.%lu.%lu\n", (unsigned long) date2.d, (unsigned long) date2.m, (unsigned long) date2.y);
//      return (1);
//    }
//  }
//  printf ("\ncheck finnished (%lu dates tested)\n", (unsigned long) i);


  /***************************************************************************/


//  /* read current date and print day of week */
//  slg_date_set_today (&date);
//  printf ("today is: %lu.%lu.%lu\n", (unsigned long) date.d, (unsigned long) date.m, (unsigned long) date.y);
//  i = slg_date_dow (&date, tempstr);
//  printf ("today is: %s\n", tempstr);

//  /* check summertime of 2020 */
//  date.y = 2020;
//  date.m = 1;
//  date.d = 1;
//  i = slg_date_is_summertime (&date);
//  printf ("%lu.%lu.%lu: summer: %lu\n", (unsigned long) date.d, (unsigned long) date.m, (unsigned long) date.y, (unsigned long) i);
//  date.m = 3;
//  date.d = 24;
//  for (k=0; k < 8; k++) {
//    i = slg_date_is_summertime (&date);
//    printf ("%lu.%lu.%lu: summer: %lu\n", (unsigned long) date.d, (unsigned long) date.m, (unsigned long) date.y, (unsigned long) i);
//    slg_date_inc (&date);
//  }
//  date.m = 10;
//  date.d = 24;
//  for (k=0; k < 8; k++) {
//    i = slg_date_is_summertime (&date);
//    printf ("%lu.%lu.%lu: summer: %lu\n", (unsigned long) date.d, (unsigned long) date.m, (unsigned long) date.y, (unsigned long) i);
//    slg_date_inc (&date);
//  }



//  k = slg_str_to_time_ind (0, "00:00");
//  printf ("index: %lu\n", (unsigned long) k);
//
//
//  slg_time_ind_to_str (tempstr, 1, 1, 95);
//  printf ("time: %s\n", tempstr);

// printf ("integer temp: %li\n", (long) slg_str2temper("-13.1"));

  t = -7;
  slg_temper2str (tempstr, 1, t);
  printf ("temperature: \"%s\"(%li)\n", tempstr, (long) t);

  t = 239;
  slg_temper2str (tempstr, 1, t);
  printf ("temperature: \"%s\"(%li)\n", tempstr, (long) t);


  k = 200000;
  slg_rain2str (tempstr, 0, k);
  printf ("rain: \"%s\" (%lu)\n", tempstr, (unsigned long) k);
  slg_rain2str (tempstr, 1, k);
  printf ("rain: \"%s\" (%lu)\n", tempstr, (unsigned long) k);

  k = 10025;
  slg_rain2str (tempstr, 0, k);
  printf ("rain: \"%s\" (%lu)\n", tempstr, (unsigned long) k);
  slg_rain2str (tempstr, 1, k);
  printf ("rain: \"%s\" (%lu)\n", tempstr, (unsigned long) k);



  return (0);
}



