/***************************************************************************************************
 *
 * file     : slg_test.c (test command line tool for slgshow library code)
 *
 * author   : Jochen Ertel
 *
 * created  : 07.01.2020
 * updated  : 15.01.2022
 *
 **************************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../lib/options.h"
#include "../lib/slg_date.h"
#include "../lib/slg_values.h"
#include "../lib/slg_dayfile.h"
#include "../lib/slg_temper.h"
#include "../lib/slg_rain.h"


#define VERSION "test command line tool for slgshow library code"



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
  int32_t t, r;
  char tempstr[20];
  slg_daydata wurst;
  slg_drain raini;


  /* help menu ************************************************************************************/
  if (parArgTypExists (argc, argv, 'h')) {
    printf (VERSION "\n");
    printf ("  -> parameters (optional):\n");
    printf ("     -h  :  prints this help menu\n");

    return (0);
  }


  /***************************************************************************/




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



  k = slg_readdayfile (&wurst, "2020-02-10.txt", 1);
  printf ("slg_readdayfile: %lu\n", (unsigned long) k);

  k = slg_drain_read (&raini, &wurst, 2);
  printf ("slg_drain_read: %lu\n", (unsigned long) k);

  k = slg_drain_sum (&raini);
  printf ("slg_drain_sum: %lu\n", (unsigned long) k);

  slg_rain2str (tempstr, 0, k);
  printf ("slg_drain_sum: %s\n", tempstr);


//  printf ("========================\n");
//  for (t=-999; t < 1000; t++) {
//    slg_temper2str (tempstr, 0, t);
//    r = slg_str2temper (tempstr);
//    printf ("%i -> %s -> %i ", t, tempstr, r);
//    if (t == r) printf ("-> OK!\n");
//    else printf ("-> FEHLER!\n");
//  }
//  printf ("========================\n");



//  printf ("========================\n");
//  for (k=0; k < 2000; k++) {
//    slg_rain2str (tempstr, 0, k);
//    i = slg_str2rain (tempstr);
//    printf ("%u -> %s -> %u ", k, tempstr, i);
//    if (k == i) printf ("-> OK!\n");
//    else printf ("-> FEHLER!\n");
//  }
//  printf ("========================\n");




  return (0);
}



