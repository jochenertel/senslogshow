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
  slg_dtemper tempi;

  int li;


  /* help menu ************************************************************************************/
  if (parArgTypExists (argc, argv, 'h')) {
    printf (VERSION "\n");
    printf ("  -> parameters (optional):\n");
    printf ("     -h  :  prints this help menu\n");

    return (0);
  }


  /***************************************************************************/


//  date.y = 0;
//  date.m = 0;
//  date.d = 0;
//
//  slg_date_set_int (&date, 14, 1, 2020);
//  slg_date_to_string (tempstr, &date);
//  printf ("%s\n", tempstr);
//  slg_date_to_fstring (tempstr, &date);
//  printf ("%s\n", tempstr);
//
//  slg_date_set_int (&date, 7, 4, 2014);
//  slg_date_to_string (tempstr, &date);
//  printf ("%s\n", tempstr);
//  slg_date_to_fstring (tempstr, &date);
//  printf ("%s\n", tempstr);
//
//  slg_date_set_int (&date, 23, 11, 2014);
//  slg_date_to_string (tempstr, &date);
//  printf ("%s\n", tempstr);
//  slg_date_to_fstring (tempstr, &date);
//  printf ("%s\n", tempstr);

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


//  slg_date_set_str (&date, "29.10.1985");
//
//  k = slg_date_to_string (tempstr, &date);
//  printf ("%s (%lu)\n", tempstr, (unsigned long) k);


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

//  t = -7;
//  slg_temper2str (tempstr, 1, t);
//  printf ("temperature: \"%s\"(%li)\n", tempstr, (long) t);
//
//  t = 239;
//  slg_temper2str (tempstr, 1, t);
//  printf ("temperature: \"%s\"(%li)\n", tempstr, (long) t);
//
//
//  k = 200000;
//  slg_rain2str (tempstr, 0, k);
//  printf ("rain: \"%s\" (%lu)\n", tempstr, (unsigned long) k);
//  slg_rain2str (tempstr, 1, k);
//  printf ("rain: \"%s\" (%lu)\n", tempstr, (unsigned long) k);
//
//  k = 10025;
//  slg_rain2str (tempstr, 0, k);
//  printf ("rain: \"%s\" (%lu)\n", tempstr, (unsigned long) k);
//  slg_rain2str (tempstr, 1, k);
//  printf ("rain: \"%s\" (%lu)\n", tempstr, (unsigned long) k);

  k = slg_readdayfile (&wurst, "2021-12-26w.txt", 2);
  printf ("slg_readdayfile: %lu\n", (unsigned long) k);

  k = slg_dtemper_read (&tempi, &wurst, 3);
  printf ("slg_dtemper_read: %lu\n", (unsigned long) k);

  k = slg_dtemper_indmax (&tempi);
  printf ("slg_dtemper_indmax: %lu\n", (unsigned long) k);

  slg_timeindex2str (tempstr, tempi.tmode, 0, k);
  printf ("slg_timeindex2str: %s\n", tempstr);

  slg_temper2str (tempstr, 1, tempi.val[k]);
  printf ("slg_temper2str: %s\n", tempstr);


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


//  printf ("read return: %lu\n", (unsigned long) k);
//  if (k == 0) {
//    printf ("empty lines: %lu\n", (unsigned long) slg_cntemptylines (&wurst));
//    printf ("empty secs : %lu\n", (unsigned long) slg_cntemptysecs (&wurst));
//
//    slg_date_to_string (tempstr, &wurst.date);
//    printf ("locid  = %lu\n", (unsigned long) wurst.locid);
//    printf ("locstr = %s\n", wurst.locstr);
//    printf ("tmode  = %lu\n", (unsigned long) wurst.tmode);
//    printf ("date   = %s\n", tempstr);
//    printf ("comm.  = %s\n", wurst.comment);
//
//    for (k=0; k < wurst.colnum; k++) {
//      printf ("column %lu ---------------\n", (unsigned long) (k+1));
//      printf ("typ  = %lu\n", (unsigned long) wurst.coltyp[k]);
//      printf ("id   = %lu\n", (unsigned long) wurst.colid[k]);
//      printf ("str  = %s\n", wurst.colstr[k]);
//    }
//  }
//
//  k = slg_cntinvalidvals (&wurst);
//  printf ("num invalid values: %lu\n", (unsigned long) k);
//
//  k = slg_writedayfile ("2021-12-26_neu.txt", &wurst, 1);
//  printf ("read return (write dayfile): %lu\n", (unsigned long) k);


  return (0);
}



