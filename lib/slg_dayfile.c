/***************************************************************************************************
 *
 * file     : slg_dayfile.c
 *
 * function : senslog project c-library - dayfile parsing and modifying functions
 *
 * author   : Jochen Ertel
 *
 * created  : 26.06.2021
 * updated  : 16.09.2025
 *
 **************************************************************************************************/


#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "slg_date.h"
#include "slg_values.h"
#include "slg_dayfile.h"



/* private helper functions ***********************************************************************/
/**************************************************************************************************/
/**************************************************************************************************/

/* reads a line from a text file:
 * - ignores empty lines (never returns it)
 * - replaces tabs by a space
 *
 * parameters:
 *   *line:  pointer to resulting line
 *   *mlen:  maximum line length (incl. 0x00 at end)
 *   *fpr:   pointer to file
 *
 * return value:
 *    0 :  operation successfull (file not end)
 *    1 :  operation successfull (end of file reached)
 *    2 :  operation successfull but end of file reached with no further line found
 *    3 :  error: file contains invalid chars
 *    4 :  error: file contains a line which is to long
 *
 ****************************************************************************************/
uint32_t slg_readtxtline (char *line, size_t mlen, FILE *fpr)
{
  int    chint;
  char   ch;
  size_t j;

  j = 0;
  chint = getc(fpr);
  ch = (char) (chint & 0xff);
  while (!((chint == EOF) || ((ch == 0x0a) && (j > 0)))) {
    if ((ch & 0x80) || (ch == 0x00)) {   /* filter for invalid chars */
      return (3);
    }
    if (ch == 0x09) ch = 0x20;  /* replace a tab by a space */
    if ((ch != 0x0d) && (ch != 0x0a)) {
      line[j] = ch;
      j++;
      if (j == mlen) {
        return (4);
      }
    }
    chint = getc(fpr);
    ch = (char) (chint & 0xff);
  }
  line[j] = 0x00;

  if ((chint == EOF) && (j > 0)) return (1);
  if ((chint == EOF) && (j == 0)) return (2);
  return (0);
}


/* counts number of values in a measurement value line (separated by spaces)
 *
 * parameters:
 *   *line:  pointer to line
 *
 * return value:
 *    <num> :  number of values
 *
 ****************************************************************************************/
uint32_t slg_mlnumval (char *line)
{
  uint32_t i, z, n;

  n = 0;
  i = 0;
  z = 0;
  while (line[i] != 0x00) {
    if (line[i] != ' ') {
      if (z == 0) n++;
      z = 1;
    }
    else z = 0;
    i++;
  }

  return (n);
}


/* cuts a value string from a measurement value line (separated by spaces)
 *
 * parameters:
 *   *value:  pointer for resulting value string (12 bytes must be allocated)
 *   *line :  pointer to line string
 *   k     :  value number (0, 1, 2, ...)
 *
 * return value:
 *    0 :  operation successfull
 *    1 :  error: value string to long (longer than 11 chars)
 *    2 :  error: line does contain lower than (k+1) values
 *
 ****************************************************************************************/
uint32_t slg_mlgetval (char *value, char *line, uint32_t k)
{
  uint32_t i, j, z, n;

  n = 0;
  i = 0;
  z = 0;
  while ((line[i] != 0x00) && (n != (k+1))) {
    if (line[i] != ' ') {
      if (z == 0) n++;
      z = 1;
    }
    else z = 0;
    i++;
  }

  if (n == (k+1)) {
    i--;
    j = 0;
    while ((line[i] != 0x00) && (line[i] != ' ') && (j < 11)) {
      value[j] = line[i];
      j++; i++;
    }
    if ((j == 11) && (line[i] != ' ') && (line[i] != 0x00)) return (1);
    value[j] = 0x00;
  }
  else {
    return (2);
  }

  return (0);
}



/* read and write day files ***********************************************************************/
/**************************************************************************************************/
/**************************************************************************************************/


/* reads a dayfile into a daydata structure
 *
 * parameters:
 *   *daydata:   target daydata structure
 *   *filename:  path/filename of dayfile to read
 *   hmode:      0: normal header in file
 *               1: no header in file (assumed Bretnig: TEMP RAIN)
 *               2: no header in file (assumed Dresden Wittenberger: TEMP TEMP TEMP)
 *               3: no header in file (assumed Dresden Hofefeld: TEMP RAIN)
 *
 * return value:
 *    0 :  operation successfull
 *    1 :  error: file not found
 *    2 :  error: file contains invalid chars
 *    3 :  error: file contains a line which is to long
 *    4 :  error: unexpected file end
 *    5 :  error: first header part: invalid structure
 *    6 :  error: first header part: invalid time mode
 *    7 :  error: second header part: invalid structure
 *    8 :  error: second header part: invalid column typ
 *    9 :  error: second header part: to many columns
 *   10 :  error: second header part: column ids are not unique
 *   11 :  error: measurement lines: invalid number of values in line
 *   12 :  error: measurement lines: a value is to long
 *   13 :  error: measurement lines: invalid date or time value
 *   14 :  error: measurement lines: invalid line order
 *   15 :  error: to many lines
 *
 ****************************************************************************************/
uint32_t slg_readdayfile (slg_daydata *daydata, char *filename, uint32_t hmode)
{
  FILE *fpr;
  uint32_t i, j, res, res2, ready, k, dmax;
  char     line[MAX_MLN_LEN], tmp[MAX_MLN_LEN], *ch;
  slg_date dtmp;

  /* open file */
  fpr = fopen(filename, "rb");
  if (fpr == NULL) return (1);


  /* header in file *************************************************/
  if (hmode == 0) {
    /* read first header part *******************************/
    ready = 0;
    i = 1;
    while (!(ready)) {
      res = slg_readtxtline (line, MAX_MLN_LEN, fpr);
      if (res == 3) {fclose(fpr); return (2);}
      if (res == 4) {fclose(fpr); return (3);}
      if ((res == 1) || (res == 2)) {fclose(fpr); return (4);}

      if (i == 1) {
        ch = strstr(line, "Location");
        if (ch != line) {fclose(fpr); return (5);}

        ch = strchr(line, '=');
        if (ch == NULL) {fclose(fpr); return (5);}
        ch++;
        j = 0;
        while ((ch[0] != ',') && (ch[0] != 0x00)) {
          if (ch[0] != ' ') {tmp[j] = ch[0]; j++;}
          ch++;
        }
        tmp[j] = 0x00;
        daydata->locid = slg_str2uint32 (tmp);
        if (daydata->locid == CNERR) {fclose(fpr); return (5);}
        if (ch[0] != ',') {fclose(fpr); return (5);}

        ch = strchr(line, '"');
        if (ch == NULL) {fclose(fpr); return (5);}
        ch++;
        j = 0;
        while ((ch[0] != '"') && (ch[0] != 0x00) && (j < 49)) {
          daydata->locstr[j] = ch[0];
          ch++; j++;
        }
        daydata->locstr[j] = 0x00;
        if (ch[0] != '"') {fclose(fpr); return (5);}
      }

      if (i == 2) {
        ch = strstr(line, "TimeMode");
        if (ch != line) {fclose(fpr); return (5);}

        ch = strchr(line, '=');
        if (ch == NULL) {fclose(fpr); return (5);}
        ch++;
        j = 0;
        while (ch[0] != 0x00) {
          if (ch[0] != ' ') {tmp[j] = ch[0]; j++;}
          ch++;
        }
        tmp[j] = 0x00;
        daydata->tmode = slg_str2uint32 (tmp);
        if (daydata->tmode == CNERR) {fclose(fpr); return (5);}
        if (daydata->tmode > 1) {fclose(fpr); return (6);}
      }

      if (i == 3) {
        ch = strstr(line, "Date");
        if (ch != line) {fclose(fpr); return (5);}

        ch = strchr(line, '=');
        if (ch == NULL) {fclose(fpr); return (5);}
        ch++;
        j = 0;
        while (ch[0] != 0x00) {
          if (ch[0] != ' ') {tmp[j] = ch[0]; j++;}
          ch++;
        }
        tmp[j] = 0x00;
        res2 = slg_date_set_str (&daydata->date, tmp);
        if (res2 == 0) {fclose(fpr); return (5);}
      }

      if (i == 4) {
        ch = strstr(line, "Comment");
        if (ch != line) {fclose(fpr); return (5);}

        ch = strchr(line, '=');
        if (ch == NULL) {fclose(fpr); return (5);}

        ch = strchr(line, '"');
        if (ch == NULL) {fclose(fpr); return (5);}
        ch++;
        j = 0;
        while ((ch[0] != '"') && (ch[0] != 0x00) && (j < 99)) {
          daydata->comment[j] = ch[0];
          ch++; j++;
        }
        daydata->comment[j] = 0x00;
        if (ch[0] != '"') {fclose(fpr); return (5);}
      }

      if (i == 5) {
        ch = strstr(line, "----------");
        if (ch != line) {fclose(fpr); return (5);}

        ready = 1;
      }

      i++;
    }


    /* read second header part ******************************/
    ready = 0;
    daydata->colnum = 0;
    while (!(ready)) {
      res = slg_readtxtline (line, MAX_MLN_LEN, fpr);
      if (res == 3) {fclose(fpr); return (2);}
      if (res == 4) {fclose(fpr); return (3);}
      if ((res == 1) || (res == 2)) {fclose(fpr); return (4);}

      ch = strstr(line, "----------");
      if (ch == line) {
        if (daydata->colnum == 0) {fclose(fpr); return (7);}
        ready = 1;
      }
      else {
        if (daydata->colnum == 32) {fclose(fpr); return (9);}

        ch = strstr(line, "Column");
        if (ch != line) {fclose(fpr); return (7);}
        tmp[0] = line[6];
        tmp[1] = line[7];
        tmp[2] = 0x00;
        res2 = slg_str2uint32 (tmp);
        if (res2 != (daydata->colnum + 1)) {fclose(fpr); return (7);}

        ch = strchr(line, '=');
        if (ch == NULL) {fclose(fpr); return (7);}
        ch++;
        j = 0;
        while ((ch[0] != ',') && (ch[0] != 0x00)) {
          if (ch[0] != ' ') {tmp[j] = ch[0]; j++;}
          ch++;
        }
        tmp[j] = 0x00;
        if (ch[0] != ',') {fclose(fpr); return (7);}
        daydata->colid[daydata->colnum] = slg_str2uint32 (tmp);
        if (daydata->colid[daydata->colnum] == CNERR) {fclose(fpr); return (7);}
        for (k=0; k < daydata->colnum; k++) {
          if (daydata->colid[k] == daydata->colid[daydata->colnum]) {fclose(fpr); return (10);}
        }

        ch++;
        j = 0;
        while ((ch[0] != ',') && (ch[0] != 0x00)) {
          if (ch[0] != ' ') {tmp[j] = ch[0]; j++;}
          ch++;
        }
        tmp[j] = 0x00;
        if (ch[0] != ',') {fclose(fpr); return (7);}
        daydata->coltyp[daydata->colnum] = 0;
        if (strcmp(tmp, "TEMP") == 0) daydata->coltyp[daydata->colnum] = DF_TEMP;
        if (strcmp(tmp, "RAIN") == 0) daydata->coltyp[daydata->colnum] = DF_RAIN;
        if (strcmp(tmp, "EVNT") == 0) daydata->coltyp[daydata->colnum] = DF_EVNT;
        if (daydata->coltyp[daydata->colnum] == 0) {fclose(fpr); return (8);}

        ch = strchr(line, '"');
        if (ch == NULL) {fclose(fpr); return (7);}
        ch++;
        j = 0;
        while ((ch[0] != '"') && (ch[0] != 0x00) && (j < 49)) {
          daydata->colstr[daydata->colnum][j] = ch[0];
          ch++; j++;
        }
        daydata->colstr[daydata->colnum][j] = 0x00;
        if (ch[0] != '"') {fclose(fpr); return (7);}

        daydata->colnum++;
      }
    }
  }


  /* no header in file, Bretnig header assumed (TEMP RAIN) **********/
  if (hmode == 1) {
    daydata->locid = 1;
    strcpy (daydata->locstr, "Bretnig, Charlottengrund 16");
    daydata->tmode = 1;

    res = slg_readtxtline (line, MAX_MLN_LEN, fpr);
    if (res == 3) {fclose(fpr); return (2);}
    if (res == 4) {fclose(fpr); return (3);}
    res = slg_mlgetval (tmp, line, 0);
    if (res > 0) {fclose(fpr); return (12);}
    res = slg_date_set_str (&dtmp, tmp);
    if (res == 0) {fclose(fpr); return (13);}
    slg_date_copy (&daydata->date, &dtmp);
    fclose(fpr);
    fpr = fopen(filename, "rb");
    if (fpr == NULL) return (1);

    strcpy (daydata->comment, "");
    daydata->colnum = 2;

    daydata->coltyp[0] = DF_TEMP;
    daydata->colid[0] = 1;
    strcpy (daydata->colstr[0], "Aussen Hauswand NW");

    daydata->coltyp[1] = DF_RAIN;
    daydata->colid[1] = 2;
    strcpy (daydata->colstr[1], "");
  }


  /* no header in file, Dresden Wittenberger header assumed (TEMP TEMP TEMP) *****/
  if (hmode == 2) {
    daydata->locid = 2;
    strcpy (daydata->locstr, "Dresden, Wittenberger Strasse 16");
    daydata->tmode = 0;

    res = slg_readtxtline (line, MAX_MLN_LEN, fpr);
    if (res == 3) {fclose(fpr); return (2);}
    if (res == 4) {fclose(fpr); return (3);}
    res = slg_mlgetval (tmp, line, 0);
    if (res > 0) {fclose(fpr); return (12);}
    res = slg_date_set_str (&dtmp, tmp);
    if (res == 0) {fclose(fpr); return (13);}
    slg_date_copy (&daydata->date, &dtmp);
    fclose(fpr);
    fpr = fopen(filename, "rb");
    if (fpr == NULL) return (1);

    strcpy (daydata->comment, "");
    daydata->colnum = 3;

    daydata->coltyp[0] = DF_TEMP;
    daydata->colid[0] = 1;
    strcpy (daydata->colstr[0], "Aussen NO-Seite");

    daydata->coltyp[1] = DF_TEMP;
    daydata->colid[1] = 2;
    strcpy (daydata->colstr[1], "Aussen SW-Seite");

    daydata->coltyp[2] = DF_TEMP;
    daydata->colid[2] = 3;
    strcpy (daydata->colstr[2], "Schlafzimmer");
  }


  /* no header in file, Dresden Hofefeld header assumed (TEMP RAIN) **********/
  if (hmode == 3) {
    daydata->locid = 1;
    strcpy (daydata->locstr, "Dresden, Am Hofefeld 2");
    daydata->tmode = 1;

    res = slg_readtxtline (line, MAX_MLN_LEN, fpr);
    if (res == 3) {fclose(fpr); return (2);}
    if (res == 4) {fclose(fpr); return (3);}
    res = slg_mlgetval (tmp, line, 0);
    if (res > 0) {fclose(fpr); return (12);}
    res = slg_date_set_str (&dtmp, tmp);
    if (res == 0) {fclose(fpr); return (13);}
    slg_date_copy (&daydata->date, &dtmp);
    fclose(fpr);
    fpr = fopen(filename, "rb");
    if (fpr == NULL) return (1);

    strcpy (daydata->comment, "");
    daydata->colnum = 2;

    daydata->coltyp[0] = DF_TEMP;
    daydata->colid[0] = 1;
    strcpy (daydata->colstr[0], "Aussen");

    daydata->coltyp[1] = DF_RAIN;
    daydata->colid[1] = 2;
    strcpy (daydata->colstr[1], "");
  }


  /* read measurement lines *****************************************/
  dmax = slg_timeindexnum (daydata->tmode);
  i = 0;
  ready = 0;
  while (!(ready)) {
    res = slg_readtxtline (line, MAX_MLN_LEN, fpr);
    if (res == 3) {fclose(fpr); return (2);}
    if (res == 4) {fclose(fpr); return (3);}
    if ((i == 0) && (res == 2)) {fclose(fpr); return (4);}
    if ((res == 1) || (res == 2)) ready = 1;

    if (res < 2) {
      /* check number of values in line against header */
      k = slg_mlnumval (line);
      if (k != (daydata->colnum + 2)) {fclose(fpr); return (11);}

      /* check date value of line against header */
      res2 = slg_mlgetval (tmp, line, 0);
      if (res2 > 0) {fclose(fpr); return (12);}  /* res2 can be 0 or 1 only, never 2 */
      res2 = slg_date_set_str (&dtmp, tmp);
      if (res2 == 0) {fclose(fpr); return (13);}
      res2 = slg_date_compare (&dtmp, &daydata->date);
      if (res2 != 1) {fclose(fpr); return (13);}

      /* check time value of line and find line position in array */
      res2 = slg_mlgetval (tmp, line, 1);
      if (res2 > 0) {fclose(fpr); return (12);}  /* res2 can be 0 or 1 only, never 2 */
      k = slg_str2timeindex (daydata->tmode, tmp);
      if (k == CNERR) {fclose(fpr); return (13);}
      if (k < i) {fclose(fpr); return (14);}
      while (k > i) {
        daydata->msrline[i][0] = 0x00;
        i++;
      }

      /* check all other values of line for valid string length */
      for (j=0; j < daydata->colnum; j++) {
        if (slg_mlgetval (tmp, line, (2+j)) != 0) {fclose(fpr); return (12);}
      }

      strcpy (daydata->msrline[i], line);
      i++;
    }

    if (res > 0) {
      while (dmax > i) {
        daydata->msrline[i][0] = 0x00;
        i++;
      }
    }

    if (i == dmax) ready = 1;
  }

  if (res == 0) {
    res = slg_readtxtline (line, MAX_MLN_LEN, fpr);
    if (res == 3) {fclose(fpr); return (2);}
    if (res == 4) {fclose(fpr); return (3);}
    if (res != 2) {fclose(fpr); return (15);}
  }

  fclose(fpr);
  return (0);
}


/* writes a daydata structure into a dayfile
 *
 * parameters:
 *   *filename:  path/filename of dayfile to write
 *   *daydata:   source daydata structure
 *   mode:       0: write meassurement lines directly from stored strings
 *               1: reassemble meassurement lines from single values
 *
 * return value:
 *    0 :  operation successfull
 *    1 :  error: opening file failed
 *    2 :  error: invalid daydata
 *
 ****************************************************************************************/
uint32_t slg_writedayfile (char *filename, slg_daydata *daydata, uint32_t mode)
{
  FILE *fpw;
  uint32_t i, c, res, dmax, v;
  char     stmp[20];

  /* open file */
  fpw = fopen(filename, "wb");
  if (fpw == NULL) return (1);

  /* write first header part ******************************/
  fprintf (fpw, "Location = %lu, \"%s\"\n", (unsigned long) daydata->locid, daydata->locstr);
  fprintf (fpw, "TimeMode = %lu\n", (unsigned long) daydata->tmode);
  res = slg_date_to_string (stmp, &daydata->date);
  if (res == 0) {fclose(fpw); return (2);}
  fprintf (fpw, "Date     = %s\n", stmp);
  fprintf (fpw, "Comment  = \"%s\"\n", daydata->comment);
  fprintf (fpw, "-------------------------------------------------\n");

  /* write second header part *****************************/
  if (daydata->colnum > 99) {fclose(fpw); return (2);}
  for (i=1; i <= daydata->colnum; i++) {
    if (i < 10) fprintf (fpw, "Column0%lu = ", (unsigned long) i);
    else fprintf (fpw, "Column%lu = ", (unsigned long) i);

    stmp[0] = 0;
    if (daydata->coltyp[i-1] == DF_TEMP) strcpy (stmp, "TEMP");
    if (daydata->coltyp[i-1] == DF_RAIN) strcpy (stmp, "RAIN");
    if (daydata->coltyp[i-1] == DF_EVNT) strcpy (stmp, "EVNT");
    if (stmp[0] == 0) {fclose(fpw); return (2);}

    fprintf (fpw, "%lu, %s, \"%s\"\n", (unsigned long) daydata->colid[i-1], stmp, daydata->colstr[i-1]);
  }
  fprintf (fpw, "-------------------------------------------------\n");

  /* write meassurement lines *****************************/
  if (mode > 1) {fclose(fpw); return (2);}
  dmax = slg_timeindexnum (daydata->tmode);

  if (mode == 0) {
    for (i=0; i < dmax; i++) {
      if (daydata->msrline[i][0] != 0) fprintf (fpw, "%s\n", daydata->msrline[i]);
    }
  }

  if (mode == 1) {
    for (i=0; i < dmax; i++) {
      if (daydata->msrline[i][0] != 0) {
        slg_date_to_string (stmp, &daydata->date);
        fprintf (fpw, "%s", stmp);
        slg_timeindex2str (stmp, daydata->tmode, 0, i);
        fprintf (fpw, " %s", stmp);

        for (c=0; c < daydata->colnum; c++) {
          slg_mlgetval (stmp, daydata->msrline[i], (2+c));
          if (daydata->coltyp[c] == DF_TEMP) {
            v = slg_str2temper (stmp);
            slg_temper2str (stmp, 1, v);
            fprintf (fpw, " %s", stmp);
          }
          if (daydata->coltyp[c] == DF_RAIN) {
            v = slg_str2rain (stmp);
            slg_rain2str (stmp, 1, v);
            fprintf (fpw, " %s", stmp);
          }
          if (daydata->coltyp[c] == DF_EVNT) {
            v = slg_str2event (stmp);
            slg_event2str (stmp, v);
            fprintf (fpw, " %s", stmp);
          }
        }
        fprintf (fpw, "\n");
      }
    }
  }

  fclose(fpw);
  return (0);
}



/* dayfile checker functions **********************************************************************/
/**************************************************************************************************/
/**************************************************************************************************/


/* counts number of empty measurement lines in dayfile
 *
 * parameters:
 *   *daydata:   daydata object
 *
 * return value:
 *    <num> :  number of empty lines
 *
 ****************************************************************************************/
uint32_t slg_cntemptylines (slg_daydata *daydata)
{
  uint32_t i, dmax, num;

  dmax = slg_timeindexnum (daydata->tmode);

  num = 0;
  for (i=0; i < dmax; i++) {
    if (daydata->msrline[i][0] == 0x00) num++;
  }

  return (num);
}


/* counts number of empty measurement sections in dayfile
 *
 * parameters:
 *   *daydata:   daydata object
 *
 * return value:
 *    <num> :  number of empty sections
 *
 ****************************************************************************************/
uint32_t slg_cntemptysecs (slg_daydata *daydata)
{
  uint32_t i, t, dmax, num;

  dmax = slg_timeindexnum (daydata->tmode);

  num = 0;
  t = 1;
  for (i=0; i < dmax; i++) {
    if (daydata->msrline[i][0] == 0x00) {
      if (t == 1) num++;
      t = 0;
    }
    else {
      t = 1;
    }
  }

  return (num);
}


/* counts invalid values (error marker) in all columns of all measurement lines
 *
 * parameters:
 *   *daydata:   daydata object
 *
 * return value:
 *    <num> :  number of invalid values
 *
 ****************************************************************************************/
uint32_t slg_cntinvalidvals (slg_daydata *daydata)
{
  uint32_t c, i, dmax, num;
  char     stmp[12];

  dmax = slg_timeindexnum (daydata->tmode);

  num = 0;
  for (c=0; c < daydata->colnum; c++) {
    for (i=0; i < dmax; i++) {
      if (daydata->msrline[i][0] != 0x00) {

        slg_mlgetval (stmp, daydata->msrline[i], (2+c));

        if (daydata->coltyp[c] == DF_TEMP) {
          if (slg_str2temper(stmp) == CNERR) num++;
        }

        if (daydata->coltyp[c] == DF_RAIN) {
          if (slg_str2rain(stmp) == CNERR) num++;
        }

        if (daydata->coltyp[c] == DF_EVNT) {
          if (slg_str2event(stmp) == CNERR) num++;
        }
      }
    }
  }

  return (num);
}


/* checks if a column of a special typ and id exist and gives back column number
 *
 * parameters:
 *   *daydata:  daydata object
 *   typ     :  column typ
 *   id      :  column id
 *
 * return value:
 *         0 :  column does not exist
 *      >= 2 :  column number
 *
 ****************************************************************************************/
uint32_t slg_colexist (slg_daydata *daydata, uint32_t typ, uint32_t id)
{
  uint32_t i;

  for (i=0; i < daydata->colnum; i++) {
    if ((daydata->coltyp[i] == typ) && (daydata->colid[i] == id)) return (i + 2);
  }

  return (0);
}


/* gets last valid measurement line index of a dayfile
 *
 * parameters:
 *   *daydata:  daydata object
 *
 * return value:
 *   <line_ind> :  line index (0, 1, 2, ...)
 *
 ****************************************************************************************/
uint32_t slg_lastmline (slg_daydata *daydata)
{
  uint32_t i, dmax;

  dmax = slg_timeindexnum (daydata->tmode);

  i = dmax - 1;
  while (daydata->msrline[i][0] == 0x00) {
    i--;
  }
  /* note: one line with values does always exist */

  return (i);
}



/* dayfile get value functions ********************************************************************/
/**************************************************************************************************/
/**************************************************************************************************/


/* get a special temperature value from dayfile
 *
 * parameters:
 *   *daydata:  daydata object
 *   c       :  temperature column index (2, 3, 4, ...)
 *   k       :  time index (0, 1, 2, ...)
 *
 * return value:
 *   CNERR   :  error, invalid value or value does not exist
 *   temper  :  temperature T*10
 *
 ****************************************************************************************/
int32_t slg_gettemperval (slg_daydata *daydata, uint32_t c, uint32_t k)
{
  char stmp[12];

  if (k >= slg_timeindexnum(daydata->tmode)) return (CNERR);

  if (daydata->msrline[k][0] != 0x00)
    slg_mlgetval (stmp, daydata->msrline[k], c);
  else
    return (CNERR);

  return (slg_str2temper(stmp));
}


/* get a special rain value from dayfile
 *
 * parameters:
 *   *daydata:  daydata object
 *   c       :  rain column index (2, 3, 4, ...)
 *   k       :  time index (0, 1, 2, ...)
 *
 * return value:
 *   CNERR   :  error, invalid value or value does not exist
 *   rain    :  rain*100
 *
 ****************************************************************************************/
uint32_t slg_getrainval (slg_daydata *daydata, uint32_t c, uint32_t k)
{
  char stmp[12];

  if (k >= slg_timeindexnum(daydata->tmode)) return (CNERR);

  if (daydata->msrline[k][0] != 0x00)
    slg_mlgetval (stmp, daydata->msrline[k], c);
  else
    return (CNERR);

  return (slg_str2rain(stmp));
}


/* get a special boolean event value from dayfile
 *
 * parameters:
 *   *daydata:  daydata object
 *   c       :  event column index (2, 3, 4, ...)
 *   k       :  time index (0, 1, 2, ...)
 *
 * return value:
 *   CNERR   :  error, invalid value or value does not exist
 *   event   :  0 or 1
 *
 ****************************************************************************************/
uint32_t slg_geteventval (slg_daydata *daydata, uint32_t c, uint32_t k)
{
  char stmp[12];

  if (k >= slg_timeindexnum(daydata->tmode)) return (CNERR);

  if (daydata->msrline[k][0] != 0x00)
    slg_mlgetval (stmp, daydata->msrline[k], c);
  else
    return (CNERR);

  return (slg_str2event(stmp));
}



/* read month data ********************************************************************************/
/**************************************************************************************************/
/**************************************************************************************************/


/* reads a all dayfiles of a month into a monthdata structure
 * - dayfile names are assumed to be in format "yyyy-mm-dd.txt"
 *
 * parameters:
 *   *monthdata: target mothdata structure
 *   *pathname:  path name of dayfiles to read (incl. '/') or empty string
 *   hmode:      0: normal header in dayfiles
 *               1: no header in dayfiles (assumed Bretnig: TEMP RAIN)
 *               2: no header in dayfiles (assumed Dresden Wittenberger: TEMP TEMP TEMP)
 *               3: no header in dayfiles (assumed Dresden Hofefeld: TEMP RAIN)
 *
 * return value:
 *    0 :  operation successfull
 *    1 :  error: invalid parameter (month, year, hmode)
 *    2 :  error: not even one dayfile found
 *    3 :  error: dayfiles have not the same location id
 *    4 :  error: dayfiles have not the same time mode
 *    100*day+dayerror:
 *       -> errorcode / 100: error in dayfile of day (1...31)
 *       -> errorcode % 100: errorcode of dayfile (2...15)
 *
 ****************************************************************************************/
uint32_t slg_readmonth (slg_monthdata *monthdata, char *pathname,
                        uint32_t year, uint32_t month, uint32_t hmode)
{
  uint32_t res, i, empty;
  slg_date date;
  char     fname[100], temp[20];

  /* set and check start date */
  res = slg_date_set_int (&date, 1, month, year);
  if (res == 0) return (1);
  slg_date_copy (&monthdata->date, &date);

  /* check other parameter */
  if (hmode > 3) return (1);
  if (strlen(pathname) > 85) return (1);

  /* initialise loop */
  for (i=0; i < 31; i++) monthdata->dvalid[i] = 0;
  empty = 1;

  /* read day by day from file */
  while (date.m == month) {
    i = date.d - 1;

    /* prepare day file name */
    strcpy (fname, pathname);
    slg_date_to_fstring (temp, &date);
    strcat (fname, temp);
    strcat (fname, ".txt");

    /* read dayfile (do nothing if file not found) */
    res = slg_readdayfile (&monthdata->daydata[i], fname, hmode);
    if (res > 1) return (100 * date.d + res);
    if (res == 0) {
      monthdata->dvalid[i] = 1;

      if (empty) {
        monthdata->locid = monthdata->daydata[i].locid;
        strcpy (monthdata->locstr, monthdata->daydata[i].locstr);
        monthdata->tmode = monthdata->daydata[i].tmode;
        empty = 0;
      }
      else {
        if (monthdata->locid != monthdata->daydata[i].locid) return (3);
        if (monthdata->tmode != monthdata->daydata[i].tmode) return (4);
      }
    }

    slg_date_inc (&date);
  }

  if (empty) return (2);

  return (0);
}



/* month checker functions ************************************************************************/
/**************************************************************************************************/
/**************************************************************************************************/


/* checks if a column of a special typ and id exists in all existing dayfiles of month
 *
 * parameters:
 *   *monthdata : mothdata object
 *   typ        : column typ
 *   id         : column id
 *
 * return value:
 *         0 :  column with typ and id does not exist in all dayfiles of month
 *         1 :  column with typ and id exists in all existing dayfiles of month
 *
 ****************************************************************************************/
uint32_t slg_colexist_month (slg_monthdata *monthdata, uint32_t typ, uint32_t id)
{
  uint32_t res, i;

  for (i=0; i < 31; i++) {
    if (monthdata->dvalid[i]) {
      res = slg_colexist (&monthdata->daydata[i], typ, id);
      if (res < 2) return (0);
    }
  }

  return (1);
}








