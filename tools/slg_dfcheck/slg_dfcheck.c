/***************************************************************************************************
 *
 * file     : slg_dfcheck.c (command line tool "senslog dayfile checking")
 *
 * author   : Jochen Ertel
 *
 * created  : 27.12.2021
 * updated  : 06.01.2022
 *
 **************************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../lib/options.h"
#include "../../lib/slg_date.h"
#include "../../lib/slg_values.h"
#include "../../lib/slg_dayfile.h"


#define VERSION "senslog dayfile checking tool (version 0.1.1)"




/***************************************************************************************************
 * main function
 **************************************************************************************************/

int main (int argc, char *argv[])
{
  uint32_t     res, hm, hide, es, el, iv;
  int32_t      diff;
  char         tstr[256], pstr[256], fname[300];
  slg_date     date, edate;
  slg_daydata  dayf;

  /* help menu ************************************************************************************/
  if ((parArgTypExists (argc, argv, 'h')) || (argc == 1)) {
    printf (VERSION "\n");
    printf ("  -> parameters:\n");
    printf ("     -h        :  prints this help menu\n");
    printf ("     -s <str>  :  start date\n");
    printf ("     -e <str>  :  optional end date\n");
    printf ("     -p <str>  :  optional file path\n");
    printf ("     -d <uint> :  optional no header mode (1: Bretnig, 2: Dresden)\n");
    printf ("     -z        :  optional hide all dayfiles wich are ok and complete\n");

    return (0);
  }


  /* read parameters ******************************************************************************/
  if (!(parArgTypExists (argc, argv, 's'))) {
    printf ("slg_dfcheck: error: missing parameter \'-s\'\n");
    return (1);
  }

  res = parGetString (argc, argv, 's', tstr);
  if (res == 0) {
    printf ("slg_dfcheck: error: can not read value of parameter \'-s\'\n");
    return (1);
  }
  res = slg_date_set_str (&date, tstr);
  if (res == 0) {
    printf ("slg_dfcheck: error: invalid start date\n");
    return (1);
  }

  if (parArgTypExists (argc, argv, 'e')) {
    res = parGetString (argc, argv, 'e', tstr);
    if (res == 0) {
      printf ("slg_dfcheck: error: can not read value of parameter \'-e\'\n");
      return (1);
    }
    res = slg_date_set_str (&edate, tstr);
    if (res == 0) {
      printf ("slg_dfcheck: error: invalid end date\n");
      return (1);
    }
  }
  else {
    slg_date_copy (&edate, &date);
  }

  if (parArgTypExists (argc, argv, 'p')) {
    res = parGetString (argc, argv, 'p', pstr);
    if (res == 0) {
      printf ("slg_dfcheck: error: can not read value of parameter \'-p\'\n");
      return (1);
    }
  }
  else {
    pstr[0] = 0;  /* set empty string */
  }

  if (parArgTypExists (argc, argv, 'd')) {
    res = parGetUint32 (argc, argv, 'd', &hm);
    if (res == 0) {
      printf ("slg_dfcheck: error: can not read value of parameter \'-d\'\n");
      return (1);
    }
    if ((hm < 1) || (hm > 2)) {
      printf ("slg_dfcheck: error: invalid no header mode\n");
      return (1);
    }
  }
  else {
    hm = 0;
  }

  diff = slg_date_sub (&edate, &date);
  if (diff < 0) {
      printf ("slg_dfcheck: error: end date is before start date\n");
      return (1);
  }
  printf ("-> checking %i day files:\n", (int) (diff + 1));

  if (parArgTypExists (argc, argv, 'z')) hide = 1;
  else hide = 0;


  /* check dayfiles *******************************************************************************/
  while (slg_date_compare (&date, &edate) < 3) {

    /* construct file name including path */
    slg_date_to_fstring (tstr, &date);
    strcpy (fname, pstr);
    if (strlen(fname) != 0) strcat (fname, "/");
    strcat (fname, tstr);
    strcat (fname, ".txt");

    slg_date_to_string (tstr, &date);

    /* read file and check it */
    res = slg_readdayfile (&dayf, fname, hm);

    if (res > 0) {
      printf ("%s:   ", tstr);
      if (res ==  1) printf ("error (file not found)\n");
      if (res ==  2) printf ("error (file contains invalid chars)\n");
      if (res ==  3) printf ("error (file contains a line which is to long)\n");
      if (res ==  4) printf ("error (unexpected file end)\n");
      if (res ==  5) printf ("error (first header part: invalid structure)\n");
      if (res ==  6) printf ("error (first header part: invalid time mode)\n");
      if (res ==  7) printf ("error (second header part: invalid structure)\n");
      if (res ==  8) printf ("error (second header part: invalid column typ)\n");
      if (res ==  9) printf ("error (second header part: to many columns)\n");
      if (res == 10) printf ("error (second header part: column ids are not unique)\n");
      if (res == 11) printf ("error (measurement lines: invalid number of values in line)\n");
      if (res == 12) printf ("error (measurement lines: a value is to long)\n");
      if (res == 13) printf ("error (measurement lines: invalid date or time value)\n");
      if (res == 14) printf ("error (measurement lines: invalid line order)\n");
      if (res == 15) printf ("error (to many lines)\n");
    }
    else {
      es = slg_cntemptysecs (&dayf);
      el = slg_cntemptylines (&dayf);
      iv = slg_cntinvalidvals (&dayf);

      if (!hide || (es != 0) || (el != 0) || (iv != 0)) {
        printf ("%s:   ", tstr);
        printf ("empsec = %lu   ", (unsigned long) es);
        printf ("emplin = %lu   ", (unsigned long) el);
        printf ("invval = %lu\n", (unsigned long) iv);
      }
    }

    slg_date_inc (&date);
  }

  return (0);
}
