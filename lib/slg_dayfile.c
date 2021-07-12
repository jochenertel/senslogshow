/***************************************************************************************************
 *
 * file     : slg_dayfile.c
 *
 * function : senslog project c-library - dayfile parsing and modifying functions
 *
 * author   : Jochen Ertel
 *
 * created  : 26.06.2021
 * updated  : 12.07.2021
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

  if (line[i] != 0x00) {
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
uint32_t slg_readdayfile (slg_daydata *daydata, char *filename)
{
  FILE *fpr;
  uint32_t i, j, res, res2, ready, k, dmax;
  char     line[MXMLN], tmp[MXMLN], *ch;
  slg_date dtmp;

  /* open file */
  fpr = fopen(filename, "rb");
  if (fpr == NULL) return (1);


  /* read first header part *******************************/
  ready = 0;
  i = 1;
  while (!(ready)) {
    res = slg_readtxtline (line, MXMLN, fpr);
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
    res = slg_readtxtline (line, MXMLN, fpr);
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


  /* read measurement lines *******************************/
  if (daydata->tmode == 0) dmax = 96;
  if (daydata->tmode == 1) dmax = 96;
  i = 0;
  ready = 0;
  while (!(ready)) {
    res = slg_readtxtline (line, MXMLN, fpr);
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
    res = slg_readtxtline (line, MXMLN, fpr);
    if (res == 3) {fclose(fpr); return (2);}
    if (res == 4) {fclose(fpr); return (3);}
    if (res != 2) {fclose(fpr); return (15);}
  }

  fclose(fpr);
  return (0);
}


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

  if (daydata->tmode == 0) dmax = 96;
  if (daydata->tmode == 1) dmax = 96;

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

  if (daydata->tmode == 0) dmax = 96;
  if (daydata->tmode == 1) dmax = 96;

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

  if (daydata->tmode == 0) dmax = 96;
  if (daydata->tmode == 1) dmax = 96;

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
      }
    }
  }

  return (num);
}




