/***************************************************************************************************
 *
 * file     : slg_legacy_htmlgen_month.c (command line tool "legacy senslog html month page generation")
 *
 * author   : Jochen Ertel
 *
 * created  : 31.10.2023
 * updated  : 30.11.2023
 *
 **************************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../lib/options.h"
#include "../../lib/slg_date.h"
#include "../../lib/slg_values.h"
#include "../../lib/slg_dayfile.h"
#include "../../lib/slg_temper.h"
#include "../../lib/slg_rain.h"


#define VERSION "legacy senslog html month page generation tool (version 0.3.1)"


/***************************************************************************************************
 * functions
 **************************************************************************************************/


/* writes an error message into output html file
 *
 * parameters:
 *   *fname:  output html file name
 *   e     :  error code of function slg_readmonth()
 *
 ****************************************************************************************/
void gen_error (char *fname, uint32_t e)
{
  uint32_t d;
  FILE *fpw;
  char estr[100];

  /* error string ***********************************************************************/
  if (e < 100) {
    d = 0;
    if (e ==  1) strcpy (estr, "(invalid parameter)");
    if (e ==  2) strcpy (estr, "(no dayfiles found)");
    if (e ==  3) strcpy (estr, "(dayfiles have different location ids)");
    if (e ==  4) strcpy (estr, "(dayfiles have different time modes)");
    if (e == 20) strcpy (estr, "(expected column ids not found)");
  }
  else {
    d = e / 100;
    e = e % 100;
    if (e ==  1) strcpy (estr, "file not found");
    if (e ==  2) strcpy (estr, "file contains invalid chars");
    if (e ==  3) strcpy (estr, "file contains a line which is to long");
    if (e ==  4) strcpy (estr, "unexpected file end");
    if (e ==  5) strcpy (estr, "first header part: invalid structure");
    if (e ==  6) strcpy (estr, "first header part: invalid time mode");
    if (e ==  7) strcpy (estr, "second header part: invalid structure");
    if (e ==  8) strcpy (estr, "second header part: invalid column typ");
    if (e ==  9) strcpy (estr, "second header part: to many columns");
    if (e == 10) strcpy (estr, "second header part: column ids are not unique");
    if (e == 11) strcpy (estr, "measurement lines: invalid number of values in line");
    if (e == 12) strcpy (estr, "measurement lines: a value is to long");
    if (e == 13) strcpy (estr, "measurement lines: invalid date or time value");
    if (e == 14) strcpy (estr, "measurement lines: invalid line order");
    if (e == 15) strcpy (estr, "to many lines");
  }

  /* write output html file *************************************************************/
  fpw = fopen (fname, "w");
  if (fpw == NULL) return;

  fprintf (fpw, "<!DOCTYPE html>\n");
  fprintf (fpw, "<head>\n");
  fprintf (fpw, "  <meta http-equiv=\"content-type\" content=\"text/html; charset=UTF-8\">\n");
  fprintf (fpw, "  <meta http-equiv=\"content-language\" content=\"de\">\n");
  fprintf (fpw, "  <meta name=\"author\" content=\"Jochen Ertel\">\n");
  fprintf (fpw, "  <meta name=\"description\" content=\"Tages-Wetter\">\n");
  fprintf (fpw, "  <meta name=\"generator\" content=\"" VERSION "\">\n");
  fprintf (fpw, "\n");
  fprintf (fpw, "  <title>Tages-Wetter</title>\n");
  fprintf (fpw, "\n");
  fprintf (fpw, "  <style>\n");
  fprintf (fpw, "    body {\n");
  fprintf (fpw, "      background: #FFFFFF;\n");
  fprintf (fpw, "      padding: 0;\n");
  fprintf (fpw, "      margin: 0;\n");
  fprintf (fpw, "      text-align: center;\n");
  fprintf (fpw, "    }\n");
  fprintf (fpw, "\n");
  fprintf (fpw, "    #rahmen {\n");
  fprintf (fpw, "      position: relative;\n");
  fprintf (fpw, "      width: 640px;\n");
  fprintf (fpw, "      background: #CCCCCC;\n");
  fprintf (fpw, "      margin: 20px auto;\n");
  fprintf (fpw, "      padding: 20px;\n");
  fprintf (fpw, "      border: 0px;\n");
  fprintf (fpw, "      border-radius: 10px;\n");
  fprintf (fpw, "      box-shadow: 10px 10px 10px silver;\n");
  fprintf (fpw, "      text-align: center;\n");
  fprintf (fpw, "    }\n");
  fprintf (fpw, "\n");
  fprintf (fpw, "    h1 {\n");
  fprintf (fpw, "      margin: 10px;\n");
  fprintf (fpw, "      padding: 0px;\n");
  fprintf (fpw, "      font-family: Arial, Helvetica, sans-serif;\n");
  fprintf (fpw, "      font-size: 2em;\n");
  fprintf (fpw, "      font-style: normal;\n");
  fprintf (fpw, "      font-weight:normal;\n");
  fprintf (fpw, "      color: #777777;\n");
  fprintf (fpw, "      text-align: center;\n");
  fprintf (fpw, "    }\n");
  fprintf (fpw, "\n");
  fprintf (fpw, "    p.p1 {\n");
  fprintf (fpw, "      margin: 5px;\n");
  fprintf (fpw, "      padding: 0px;\n");
  fprintf (fpw, "      font-family: Arial, Helvetica, sans-serif;\n");
  fprintf (fpw, "      font-style: normal;\n");
  fprintf (fpw, "      font-size: 1.5em;\n");
  fprintf (fpw, "      font-weight:normal;\n");
  fprintf (fpw, "      color: #777777;\n");
  fprintf (fpw, "    }\n");
  fprintf (fpw, "\n");
  fprintf (fpw, "    p.p2 {\n");
  fprintf (fpw, "      margin: 5px;\n");
  fprintf (fpw, "      padding: 0px;\n");
  fprintf (fpw, "      font-family: Arial, Helvetica, sans-serif;\n");
  fprintf (fpw, "      font-style: normal;\n");
  fprintf (fpw, "      font-size: 1em;\n");
  fprintf (fpw, "      font-weight:normal;\n");
  fprintf (fpw, "      color: #777777;\n");
  fprintf (fpw, "    }\n");
  fprintf (fpw, "\n");
  fprintf (fpw, "  </style>\n");
  fprintf (fpw, "</head>\n");
  fprintf (fpw, "\n");
  fprintf (fpw, "\n");

  fprintf (fpw, "<body>\n");
  fprintf (fpw, "  <div id=\"rahmen\">\n");
  fprintf (fpw, "    <h1>ERROR</h1>\n");
  fprintf (fpw, "\n");
  fprintf (fpw, "    <p class=\"p1\">Reading Month of Measurement Values Failed!</p>\n");
  fprintf (fpw, "    <p class=\"p2\">&nbsp;</p>\n");
  if (d > 0) {
    fprintf (fpw, "    <p class=\"p2\">(dayfile %lu: %s)</p>\n", (unsigned long) d, estr);
  }
  else {
    fprintf (fpw, "    <p class=\"p2\">%s</p>\n", estr);
  }
  fprintf (fpw, "\n");
  fprintf (fpw, "  </div>\n");
  fprintf (fpw, "</body>\n");
  fprintf (fpw, "</html>\n");

  fclose(fpw);

  return;
}



/* writes output html file for month weather
 *
 * parameters:
 *   *fname:  output html file name
 *   *month:  month data
 *   z     :  0: current month
 *            1: older month
 *   t     :  0: no dayfile link
 *            1: include dayfile link
 *
 ****************************************************************************************/
void gen_bretnig (char *fname, slg_monthdata *month, uint32_t z, uint32_t t)
{
  FILE         *fpw;
  char         c, smonth[20], srmsum[20], smont[20], smonthdec[20], smonthinc[20], slastd[20], stavar[20], stmin[20], stmax[20], tstr[20];
  uint32_t     i, ind, day;
  slg_date     tdate;
  slg_mtemper  mtemper;
  slg_mrain    mrain;


  if (month->date.m == 1)  strcpy (smonth, "Januar");
  if (month->date.m == 2)  strcpy (smonth, "Februar");
  if (month->date.m == 3)  strcpy (smonth, "Maerz");
  if (month->date.m == 4)  strcpy (smonth, "April");
  if (month->date.m == 5)  strcpy (smonth, "Mai");
  if (month->date.m == 6)  strcpy (smonth, "Juni");
  if (month->date.m == 7)  strcpy (smonth, "Juli");
  if (month->date.m == 8)  strcpy (smonth, "August");
  if (month->date.m == 9)  strcpy (smonth, "September");
  if (month->date.m == 10) strcpy (smonth, "Oktober");
  if (month->date.m == 11) strcpy (smonth, "November");
  if (month->date.m == 12) strcpy (smonth, "Dezember");

  /* calculate month links ******************************************/
  slg_date_copy (&tdate, &month->date);
  slg_date_to_fstring (smont, &tdate);
  smont[7] = 0;  /* cut day */
  slg_date_dec (&tdate);
  slg_date_to_fstring (smonthdec, &tdate);
  smonthdec[7] = 0;  /* cut day */
  for (i=0; i < 35; i++) slg_date_inc (&tdate);
  slg_date_to_fstring (smonthinc, &tdate);
  smonthinc[7] = 0;  /* cut day */

  /* calculate average, min and max temperatures ********************/
  slg_date_copy (&tdate, &month->date);
  slg_mtemper_read (&mtemper, month, 1);

  slg_temper2str (stavar, 0, slg_mtemper_average (&mtemper));

  day = slg_mtemper_daymin (&mtemper);
  if (day == 0) {
    slg_temper2str (stmin, 0, CNERR);
  }
  else {
    ind = slg_dtemper_indmin (&mtemper.dtemper[day-1]);
    slg_temper2str (stmin, 0, mtemper.dtemper[day-1].val[ind]);
  }

  day = slg_mtemper_daymax (&mtemper);
  if (day == 0) {
    slg_temper2str (stmax, 0, CNERR);
  }
  else {
    ind = slg_dtemper_indmax (&mtemper.dtemper[day-1]);
    slg_temper2str (stmax, 0, mtemper.dtemper[day-1].val[ind]);
  }

  /* calculate rain sum *********************************************/
  slg_mrain_read (&mrain, month, 2);
  slg_rain2str (srmsum, 0, slg_mrain_sum (&mrain));

  /* search last valid day in month *********************************/
  slg_date_copy (&tdate, &month->date);
  for (i=0; i < 31; i++) {
    if (month->dvalid[i]) {
      tdate.d = i + 1;
    }
  }
  slg_date_to_fstring (slastd, &tdate);


  /* write output html file ***************************************************/
  fpw = fopen (fname, "w");
  if (fpw == NULL) return;

  fprintf (fpw, "<!DOCTYPE html>\n");
  fprintf (fpw, "<head>\n");
  fprintf (fpw, "  <meta http-equiv=\"content-type\" content=\"text/html; charset=UTF-8\">\n");
  fprintf (fpw, "  <meta http-equiv=\"content-language\" content=\"de\">\n");
  fprintf (fpw, "  <meta name=\"author\" content=\"Jochen Ertel\">\n");
  fprintf (fpw, "  <meta name=\"description\" content=\"Monats-Wetter\">\n");
  fprintf (fpw, "  <meta name=\"generator\" content=\"" VERSION "\">\n");
  fprintf (fpw, "\n");
  fprintf (fpw, "  <title>Wetter %s</title>\n", smont);
  fprintf (fpw, "\n");
  fprintf (fpw, "  <style>\n");
  fprintf (fpw, "    body {\n");
  fprintf (fpw, "      background: #FFFFFF;\n");
  fprintf (fpw, "      padding: 0;\n");
  fprintf (fpw, "      margin: 0;\n");
  fprintf (fpw, "      text-align: center;\n");
  fprintf (fpw, "    }\n");
  fprintf (fpw, "\n");
  fprintf (fpw, "    #rahmen {\n");
  fprintf (fpw, "      position: relative;\n");
  fprintf (fpw, "      width: 640px;\n");
  fprintf (fpw, "      background: #FFC78F;\n");
  fprintf (fpw, "      margin: 20px auto;\n");
  fprintf (fpw, "      padding: 10px 20px 10px 20px;\n");
  fprintf (fpw, "      border: 0px;\n");
  fprintf (fpw, "      border-radius: 10px;\n");
  fprintf (fpw, "      box-shadow: 10px 10px 10px silver;\n");
  fprintf (fpw, "      text-align: center;\n");
  fprintf (fpw, "    }\n");
  fprintf (fpw, "\n");
  fprintf (fpw, "    .widget {\n");
  fprintf (fpw, "      position: relative;\n");
  fprintf (fpw, "      width: 640px;\n");
  fprintf (fpw, "      background: #FFFFFF;\n");
  fprintf (fpw, "      margin: 20px auto;\n");
  fprintf (fpw, "      padding: 10px 0px 0px 0px;\n");
  fprintf (fpw, "      border: 0px;\n");
  fprintf (fpw, "      border-radius: 10px;\n");
  fprintf (fpw, "      text-align: center;\n");
  fprintf (fpw, "    }\n");
  fprintf (fpw, "\n");
  fprintf (fpw, "    table.wid {\n");
  fprintf (fpw, "      width: 640px;\n");
  fprintf (fpw, "      margin: 0px 0px 0px 0px;\n");
  fprintf (fpw, "      padding: 0px;\n");
  fprintf (fpw, "      border-collapse:collapse;\n");
  fprintf (fpw, "      border: 0px;\n");
  fprintf (fpw, "    }\n");
  fprintf (fpw, "\n");
  fprintf (fpw, "    td.wid {\n");
  fprintf (fpw, "      width: 160px;\n");
  fprintf (fpw, "      margin: 0px;\n");
  fprintf (fpw, "      padding: 0px;\n");
  fprintf (fpw, "      border: 0px;\n");
  fprintf (fpw, "      text-align: center;\n");
  fprintf (fpw, "      vertical-align: text-bottom;\n");
  fprintf (fpw, "    }\n");
  fprintf (fpw, "\n");
  fprintf (fpw, "    table.lnk {\n");
  fprintf (fpw, "      width: 639px;\n");
  fprintf (fpw, "      margin: 10px 0px 20px 0px;\n");
  fprintf (fpw, "      padding: 0px;\n");
  fprintf (fpw, "      border-collapse:collapse;\n");
  fprintf (fpw, "      border: 0px;\n");
  fprintf (fpw, "    }\n");
  fprintf (fpw, "\n");
  fprintf (fpw, "    td.lnkl {\n");
  fprintf (fpw, "      width: 213px;\n");
  fprintf (fpw, "      margin: 0px;\n");
  fprintf (fpw, "      padding: 0px;\n");
  fprintf (fpw, "      border: 0px;\n");
  fprintf (fpw, "      text-align: left;\n");
  fprintf (fpw, "    }\n");
  fprintf (fpw, "\n");
  fprintf (fpw, "    td.lnkm {\n");
  fprintf (fpw, "      width: 213px;\n");
  fprintf (fpw, "      margin: 0px;\n");
  fprintf (fpw, "      padding: 0px;\n");
  fprintf (fpw, "      border: 0px;\n");
  fprintf (fpw, "      text-align: center;\n");
  fprintf (fpw, "    }\n");
  fprintf (fpw, "\n");
  fprintf (fpw, "    td.lnkr {\n");
  fprintf (fpw, "      width: 213px;\n");
  fprintf (fpw, "      margin: 0px;\n");
  fprintf (fpw, "      padding: 0px;\n");
  fprintf (fpw, "      border: 0px solid black;\n");
  fprintf (fpw, "      text-align: right;\n");
  fprintf (fpw, "    }\n");
  fprintf (fpw, "\n");
  fprintf (fpw, "    h1 {\n");
  fprintf (fpw, "      margin: 10px;\n");
  fprintf (fpw, "      padding: 0px;\n");
  fprintf (fpw, "      font-family: Arial, Helvetica, sans-serif;\n");
  fprintf (fpw, "      font-size: 2em;\n");
  fprintf (fpw, "      font-style: normal;\n");
  fprintf (fpw, "      font-weight:normal;\n");
  fprintf (fpw, "      color: #777777;\n");
  fprintf (fpw, "      text-align: center;\n");
  fprintf (fpw, "    }\n");
  fprintf (fpw, "\n");
  fprintf (fpw, "    p.p0 {\n");
  fprintf (fpw, "      margin: 5px;\n");
  fprintf (fpw, "      padding: 0px;\n");
  fprintf (fpw, "      font-family: Arial, Helvetica, sans-serif;\n");
  fprintf (fpw, "      font-style: normal;\n");
  fprintf (fpw, "      font-size: 2em;\n");
  fprintf (fpw, "      font-weight:normal;\n");
  fprintf (fpw, "      color: #777777;\n");
  fprintf (fpw, "    }\n");
  fprintf (fpw, "\n");
  fprintf (fpw, "    p.p1 {\n");
  fprintf (fpw, "      margin: 5px;\n");
  fprintf (fpw, "      padding: 0px;\n");
  fprintf (fpw, "      font-family: Arial, Helvetica, sans-serif;\n");
  fprintf (fpw, "      font-style: normal;\n");
  fprintf (fpw, "      font-size: 1.5em;\n");
  fprintf (fpw, "      font-weight:normal;\n");
  fprintf (fpw, "      color: #777777;\n");
  fprintf (fpw, "    }\n");
  fprintf (fpw, "\n");
  fprintf (fpw, "    p.p2 {\n");
  fprintf (fpw, "      margin: 5px;\n");
  fprintf (fpw, "      padding: 0px;\n");
  fprintf (fpw, "      font-family: Arial, Helvetica, sans-serif;\n");
  fprintf (fpw, "      font-style: normal;\n");
  fprintf (fpw, "      font-size: 1em;\n");
  fprintf (fpw, "      font-weight:normal;\n");
  fprintf (fpw, "      color: #777777;\n");
  fprintf (fpw, "    }\n");
  fprintf (fpw, "\n");
  fprintf (fpw, "    a:link {color:#8f542c;text-decoration:none;}\n");
  fprintf (fpw, "    a:visited {color:#8f542c;text-decoration:none;}\n");
  fprintf (fpw, "    a:hover {color:#d98347;text-decoration:none;}\n");
  fprintf (fpw, "    a:active {color:#d98347;text-decoration:none;}\n");
  fprintf (fpw, "\n");
  fprintf (fpw, "  </style>\n");
  fprintf (fpw, "</head>\n");
  fprintf (fpw, "\n");
  fprintf (fpw, "\n");

  fprintf (fpw, "<body>\n");
  fprintf (fpw, "  <div id=\"rahmen\">\n");

  fprintf (fpw, "    <table class=\"lnk\">\n");
  fprintf (fpw, "      <tr>\n");

  if (z == 0) {
    fprintf (fpw, "        <td class=\"lnkl\"><p class=\"p2\"><a href=\"%s.html\">Monat zurück</a></p></td>\n", smonthdec);
    if (t == 1) fprintf (fpw, "        <td class=\"lnkm\"><p class=\"p2\"><a href=\"index.html\">Tagesansicht</a></p></td>\n");
    else fprintf (fpw, "        <td class=\"lnkm\"><p class=\"p2\">&nbsp;</p></td>\n");
    fprintf (fpw, "        <td class=\"lnkr\"><p class=\"p2\">&nbsp;</p></td>\n");
  }

  if (z == 1) {
    fprintf (fpw, "        <td class=\"lnkl\"><p class=\"p2\"><a href=\"%s.html\">Monat zurück</a></p></td>\n", smonthdec);
    if (t == 1) fprintf (fpw, "        <td class=\"lnkm\"><p class=\"p2\"><a href=\"%s.html\">Tagesansicht</a></p></td>\n", slastd);
    else fprintf (fpw, "        <td class=\"lnkm\"><p class=\"p2\">&nbsp;</p></td>\n");
    fprintf (fpw, "        <td class=\"lnkr\"><p class=\"p2\"><a href=\"%s.html\">Monat vor</a></p></td>\n", smonthinc);
  }

  fprintf (fpw, "      </tr>\n");
  fprintf (fpw, "    </table>\n");
  fprintf (fpw, "\n");

  fprintf (fpw, "    <h1>%s</h1>\n", month->locstr);
  fprintf (fpw, "\n");
  fprintf (fpw, "    <p class=\"p0\">%s %lu</p>\n", smonth, (unsigned long) month->date.y);
  fprintf (fpw, "\n");

  fprintf (fpw, "    <div class=\"widget\">\n");
  fprintf (fpw, "      <canvas id=\"diagrammi\" width=\"620\" height=\"560\"></canvas>\n");
  fprintf (fpw, "    </div>\n");
  fprintf (fpw, "\n");

  fprintf (fpw, "    <div class=\"widget\">\n");
  fprintf (fpw, "      <table class=\"wid\">\n");
  fprintf (fpw, "        <tr>\n");
  fprintf (fpw, "          <td class=\"wid\"><p class=\"p2\">Temp. Minimum</p></td>\n");
  fprintf (fpw, "          <td class=\"wid\"><p class=\"p2\">Temp. Maximum</p></td>\n");
  fprintf (fpw, "          <td class=\"wid\"><p class=\"p2\">Temp. Mittel</p></td>\n");
  fprintf (fpw, "          <td class=\"wid\"><p class=\"p2\">Regensumme</p></td>\n");
  fprintf (fpw, "        </tr>\n");
  fprintf (fpw, "        <tr>\n");
  fprintf (fpw, "          <td class=\"wid\"><p class=\"p1\">%s &deg;C</p></td>\n", stmin);
  fprintf (fpw, "          <td class=\"wid\"><p class=\"p1\">%s &deg;C</p></td>\n", stmax);
  fprintf (fpw, "          <td class=\"wid\"><p class=\"p1\">%s &deg;C</p></td>\n", stavar);
  fprintf (fpw, "          <td class=\"wid\"><p class=\"p1\">%s mm</p></td>\n", srmsum);
  fprintf (fpw, "        </tr>\n");
  fprintf (fpw, "      </table>\n");
  fprintf (fpw, "    </div>\n");
  fprintf (fpw, "\n");

  fprintf (fpw, "    <script>\n");
  fprintf (fpw, "\n");
  fprintf (fpw, "      // ---------------------------------------------------------------------------------------------------\n");
  fprintf (fpw, "      // Diagramm zeichnen in einem canvas-Element: <canvas id=\"...\" width=\"620\" height=\"360\"></canvas>\n");
  fprintf (fpw, "      //\n");
  fprintf (fpw, "      // canvas_id   :  String der canvas Elemente-ID\n");
  fprintf (fpw, "      // ns          :  0: keine Niederschlagsachse\n");
  fprintf (fpw, "      //                1: Niederschlagsachse beschriften\n");
  fprintf (fpw, "      // ---------------------------------------------------------------------------------------------------\n");
  fprintf (fpw, "      function draw_diagram (canvas_id, ns) {\n");
  fprintf (fpw, "\n");
  fprintf (fpw, "        // interne Linien-zeichnen Funktionen -------------------------------------------\n");
  fprintf (fpw, "        function dd_line_h (px, py, l) {\n");
  fprintf (fpw, "          ctx.moveTo(px, py);\n");
  fprintf (fpw, "          ctx.lineTo(px+l, py);\n");
  fprintf (fpw, "        }\n");
  fprintf (fpw, "\n");
  fprintf (fpw, "        function dd_line_hd (px, py, l) {\n");
  fprintf (fpw, "          num = Math.floor(l / 6);\n");
  fprintf (fpw, "          for (i = 0; i < num; i = i + 1) {\n");
  fprintf (fpw, "            ctx.moveTo(px+6*i, py);\n");
  fprintf (fpw, "            ctx.lineTo(px+6*i+2, py);\n");
  fprintf (fpw, "          }\n");
  fprintf (fpw, "        }\n");
  fprintf (fpw, "\n");
  fprintf (fpw, "        function dd_line_v (px, py, l) {\n");
  fprintf (fpw, "          ctx.moveTo(px, py);\n");
  fprintf (fpw, "          ctx.lineTo(px, py+l);\n");
  fprintf (fpw, "        }\n");
  fprintf (fpw, "\n");
  fprintf (fpw, "        function dd_line_vd (px, py, l) {\n");
  fprintf (fpw, "          num = Math.floor(l / 6);\n");
  fprintf (fpw, "          for (i = 0; i < num; i = i + 1) {\n");
  fprintf (fpw, "            ctx.moveTo(px, py+6*i);\n");
  fprintf (fpw, "            ctx.lineTo(px, py+6*i+2);\n");
  fprintf (fpw, "          }\n");
  fprintf (fpw, "        }\n");
  fprintf (fpw, "\n");
  fprintf (fpw, "        // vorbereitende Einstellungen --------------------------------------------------\n");
  fprintf (fpw, "        var c = document.getElementById(canvas_id);\n");
  fprintf (fpw, "        var ctx = c.getContext(\"2d\");\n");
  fprintf (fpw, "\n");
  fprintf (fpw, "        ctx.translate(0.5, 0.5);  // Hack damit die Linien wirklich nur 1 px dick sind\n");
  fprintf (fpw, "        ctx.fillStyle = \"#FFFFFF\";\n");
  fprintf (fpw, "        ctx.fillRect(0,0,620,560);\n");
  fprintf (fpw, "\n");
  fprintf (fpw, "        ctx.lineWidth = 1;\n");
  fprintf (fpw, "\n");
  fprintf (fpw, "\n");
  fprintf (fpw, "        // Diagramm zeichnen ------------------------------------------------------------\n");
  fprintf (fpw, "        ctx.beginPath();\n");
  fprintf (fpw, "        ctx.strokeStyle=\"#AAAAAA\";\n");
  fprintf (fpw, "\n");
  fprintf (fpw, "        for (j = 1; j < 10; j++) {\n");
  fprintf (fpw, "          dd_line_hd (70, 20+j*50, 480);\n");
  fprintf (fpw, "        }\n");
  fprintf (fpw, "\n");
  fprintf (fpw, "        for (j = 1; j < 7; j++) {\n");
  fprintf (fpw, "          dd_line_vd (70+j*75, 20, 500);\n");
  fprintf (fpw, "        }\n");
  fprintf (fpw, "        ctx.stroke();\n");
  fprintf (fpw, "\n");

  fprintf (fpw, "        ctx.beginPath();\n");
  fprintf (fpw, "        ctx.strokeStyle=\"#FF0000\";\n");
  fprintf (fpw, "        dd_line_h (70, 20+150, 480);\n");
  fprintf (fpw, "        ctx.stroke();\n");
  fprintf (fpw, "\n");
  fprintf (fpw, "        ctx.beginPath();\n");
  fprintf (fpw, "        ctx.strokeStyle=\"#FF8C00\";\n");
  fprintf (fpw, "        dd_line_h (70, 20+250, 480);\n");
  fprintf (fpw, "        ctx.stroke();\n");
  fprintf (fpw, "\n");
  fprintf (fpw, "        ctx.beginPath();\n");
  fprintf (fpw, "        ctx.strokeStyle=\"#0000FF\";\n");
  fprintf (fpw, "        dd_line_h (70, 20+350, 480);\n");
  fprintf (fpw, "        ctx.stroke();\n");
  fprintf (fpw, "\n");

  fprintf (fpw, "        ctx.beginPath();\n");
  fprintf (fpw, "        ctx.strokeStyle=\"#777777\";\n");
  fprintf (fpw, "        dd_line_h (70, 20, 480);\n");
  fprintf (fpw, "        dd_line_h (70, 520, 480);\n");
  fprintf (fpw, "        dd_line_v (70, 20, 500);\n");
  fprintf (fpw, "        dd_line_v (550, 20, 500);\n");
  fprintf (fpw, "\n");
  fprintf (fpw, "        for (k = 0; k < 49; k++) {\n");
  fprintf (fpw, "          ctx.moveTo(70, 30+k*10);\n");
  fprintf (fpw, "          ctx.lineTo(74, 30+k*10);\n");
  fprintf (fpw, "          ctx.moveTo(546, 30+k*10);\n");
  fprintf (fpw, "          ctx.lineTo(550, 30+k*10);\n");
  fprintf (fpw, "        }\n");
  fprintf (fpw, "\n");
  fprintf (fpw, "        for (k = 0; k < 32; k++) {\n");
  fprintf (fpw, "          ctx.moveTo(85+k*15, 516);\n");
  fprintf (fpw, "          ctx.lineTo(85+k*15, 520);\n");
  fprintf (fpw, "          ctx.moveTo(85+k*15, 20);\n");
  fprintf (fpw, "          ctx.lineTo(85+k*15, 24);\n");
  fprintf (fpw, "        }\n");
  fprintf (fpw, "\n");
  fprintf (fpw, "        ctx.stroke();\n");
  fprintf (fpw, "\n");
  fprintf (fpw, "\n");
  fprintf (fpw, "        // Achsen beschriften -----------------------------------------------------------\n");
  fprintf (fpw, "        var zeitachse = [\"\", \"5.\", \"10.\", \"15.\", \"20.\", \"25.\", \"30.\"];\n");
  fprintf (fpw, "\n");
  fprintf (fpw, "        ctx.fillStyle = \"#777777\";\n");
  fprintf (fpw, "        ctx.font = '16px Arial';\n");
  fprintf (fpw, "\n");
  fprintf (fpw, "        // Zeitachse\n");
  fprintf (fpw, "        ctx.textAlign=\"center\";\n");
  fprintf (fpw, "        ctx.textBaseline=\"top\";\n");
  fprintf (fpw, "        for (k = 0; k < 7; k++) {\n");
  fprintf (fpw, "          ctx.fillText(zeitachse[k], 70+k*75, 530);\n");
  fprintf (fpw, "        }\n");
  fprintf (fpw, "\n");
  fprintf (fpw, "        // Temperaturachse\n");
  fprintf (fpw, "        ctx.textAlign=\"right\";\n");
  fprintf (fpw, "        ctx.textBaseline=\"middle\";\n");
  fprintf (fpw, "        for (k = 0; k < 11; k++) {\n");
  fprintf (fpw, "          ctx.fillText((35 - 5 * k), 60, 20+k*50);\n");
  fprintf (fpw, "        }\n");
  fprintf (fpw, "\n");
  fprintf (fpw, "        // Niederschlagsachse\n");
  fprintf (fpw, "        if (ns == 1) {\n");
  fprintf (fpw, "          ctx.fillStyle = \"#88BBFF\";\n");
  fprintf (fpw, "          var nsachse = [\"25.0\", \"22.5\", \"20.0\", \"17.5\", \"15.0\", \"12.5\", \"10.0\", \"7.5\", \"5.0\", \"2.5\", \"0.0\"];\n");
  fprintf (fpw, "          ctx.textAlign=\"left\";\n");
  fprintf (fpw, "          ctx.textBaseline=\"middle\";\n");
  fprintf (fpw, "          for (k = 0; k < 11; k++) {\n");
  fprintf (fpw, "            ctx.fillText(nsachse[k], 560, 20+k*50);\n");
  fprintf (fpw, "          }\n");
  fprintf (fpw, "        }\n");
  fprintf (fpw, "      }\n");
  fprintf (fpw, "\n");
  fprintf (fpw, "\n");

  fprintf (fpw, "      // ---------------------------------------------------------------------------------------------------\n");
  fprintf (fpw, "      // Temperaturkurve in Diagramm eines canvas-Elements zeichnen\n");
  fprintf (fpw, "      //\n");
  fprintf (fpw, "      // canvas_id   :  String der canvas Elemente-ID\n");
  fprintf (fpw, "      // arzeit      :  Array mit Zeitindex-Werten (1 ... 31, entspr. Tag)\n");
  fprintf (fpw, "      // artmin      :  Array mit Minimum-Temperaturwerten\n");
  fprintf (fpw, "      // artmid      :  Array mit Mittel-Temperaturwerten\n");
  fprintf (fpw, "      // artmax      :  Array mit Maximum-Temperaturwerten\n");
  fprintf (fpw, "      // ---------------------------------------------------------------------------------------------------\n");
  fprintf (fpw, "      function draw_temp (canvas_id, arzeit, artmin, artmid, artmax, maxtemp) {\n");
  fprintf (fpw, "        var colmin = \"#0000FF\";\n");
  fprintf (fpw, "        var colmid = \"#777777\";\n");
  fprintf (fpw, "        var colmax = \"#FF0000\";\n");
  fprintf (fpw, "\n");
  fprintf (fpw, "        // interne Zeichnen Funktionen --------------------------------------------------\n");
  fprintf (fpw, "        function dd_point (px, py, m) {\n");
  fprintf (fpw, "          if (m == 1) {\n");
  fprintf (fpw, "            ctx.beginPath();\n");
  fprintf (fpw, "            ctx.arc(px, py, 1, 0, 2 * Math.PI);\n");
  fprintf (fpw, "            ctx.stroke();\n");
  fprintf (fpw, "            ctx.beginPath();\n");
  fprintf (fpw, "            ctx.arc(px, py, 2, 0, 2 * Math.PI);\n");
  fprintf (fpw, "            ctx.stroke();\n");
  fprintf (fpw, "            ctx.beginPath();\n");
  fprintf (fpw, "            ctx.arc(px, py, 3, 0, 2 * Math.PI);\n");
  fprintf (fpw, "            ctx.stroke();\n");
  fprintf (fpw, "          }\n");
  fprintf (fpw, "          ctx.beginPath();\n");
  fprintf (fpw, "          ctx.arc(px, py, 4, 0, 2 * Math.PI);\n");
  fprintf (fpw, "          ctx.stroke();\n");
  fprintf (fpw, "        }\n");
  fprintf (fpw, "\n");
  fprintf (fpw, "        function dd_line (px1, py1, px2, py2) {\n");
  fprintf (fpw, "          ctx.beginPath();\n");
  fprintf (fpw, "          ctx.moveTo(px1, py1);\n");
  fprintf (fpw, "          ctx.lineTo(px2, py2);\n");
  fprintf (fpw, "          ctx.stroke();\n");
  fprintf (fpw, "        }\n");
  fprintf (fpw, "\n");
  fprintf (fpw, "        // vorbereitende Einstellungen --------------------------------------------------\n");
  fprintf (fpw, "        var c = document.getElementById(canvas_id);\n");
  fprintf (fpw, "        var ctx = c.getContext(\"2d\");\n");
  fprintf (fpw, "\n");
  fprintf (fpw, "        ctx.translate(0.5, 0.5);  // Hack damit die Linien wirklich nur 1 px dick sind\n");
  fprintf (fpw, "        ctx.lineWidth = 1;\n");
  fprintf (fpw, "\n");
  fprintf (fpw, "        // Kurve zeichnen ---------------------------------------------------------------\n");
  fprintf (fpw, "        var num = arzeit.length;\n");
  fprintf (fpw, "        var px = 0;\n");
  fprintf (fpw, "        var py_min = 0;\n");
  fprintf (fpw, "        var py_mid = 0;\n");
  fprintf (fpw, "        var py_max = 0;\n");
  fprintf (fpw, "        var notoveri = 1;\n");
  fprintf (fpw, "        var notovera = 1;\n");
  fprintf (fpw, "\n");
  fprintf (fpw, "        for (k = 0; k < num; k++) {\n");
  fprintf (fpw, "          px = arzeit[k] * 15 + 69;\n");
  fprintf (fpw, "\n");
  fprintf (fpw, "          py_min = 520 - ((artmin[k] * 10) - (35 * 10 - 500));\n");
  fprintf (fpw, "          notoveri = 1;\n");
  fprintf (fpw, "          if (py_min > 520) {\n");
  fprintf (fpw, "            notoveri = 0;\n");
  fprintf (fpw, "            py_min = 523;\n");
  fprintf (fpw, "          }\n");
  fprintf (fpw, "\n");
  fprintf (fpw, "          py_max = 520 - ((artmax[k] * 10) - (35 * 10 - 500));\n");
  fprintf (fpw, "          notovera = 1;\n");
  fprintf (fpw, "          if (py_max < 20) {\n");
  fprintf (fpw, "            notovera = 0;\n");
  fprintf (fpw, "            py_max = 17;\n");
  fprintf (fpw, "          }\n");
  fprintf (fpw, "\n");
  fprintf (fpw, "          py_mid = 520 - ((artmid[k] * 10) - (35 * 10 - 500));\n");
  fprintf (fpw, "\n");
  fprintf (fpw, "          ctx.strokeStyle=colmid;\n");
  fprintf (fpw, "          dd_point (px,py_mid,1);\n");
  fprintf (fpw, "\n");
  fprintf (fpw, "          dd_line (px-1, py_min, px-1, py_max);\n");
  fprintf (fpw, "          dd_line (px, py_min, px, py_max);\n");
  fprintf (fpw, "          dd_line (px+1, py_min, px+1, py_max);\n");
  fprintf (fpw, "\n");
  fprintf (fpw, "          ctx.strokeStyle=colmin;\n");
  fprintf (fpw, "          dd_point (px,py_min,notoveri);\n");
  fprintf (fpw, "\n");
  fprintf (fpw, "          ctx.strokeStyle=colmax;\n");
  fprintf (fpw, "          dd_point (px,py_max,notovera);\n");
  fprintf (fpw, "        }\n");
  fprintf (fpw, "      }\n");
  fprintf (fpw, "\n");
  fprintf (fpw, "\n");

  fprintf (fpw, "      // ---------------------------------------------------------------------------------------------------\n");
  fprintf (fpw, "      // Niederschlagswerte in Diagramm eines canvas-Elements zeichnen\n");
  fprintf (fpw, "      //\n");
  fprintf (fpw, "      // canvas_id   :  String der canvas Elemente-ID\n");
  fprintf (fpw, "      // arzeit      :  Array mit Zeitindex-Werten (1 ... 31, entspr. Tag)\n");
  fprintf (fpw, "      // arns        :  Array mit Niederschlagswerten\n");
  fprintf (fpw, "      // ---------------------------------------------------------------------------------------------------\n");
  fprintf (fpw, "      function draw_ns (canvas_id, arzeit, arns) {\n");
  fprintf (fpw, "        var color = \"#88BBFF\";\n");
  fprintf (fpw, "\n");
  fprintf (fpw, "        // interne Zeichnen Funktionen --------------------------------------------------\n");
  fprintf (fpw, "        function dd_line (px1, py1, px2, py2) {\n");
  fprintf (fpw, "          ctx.beginPath();\n");
  fprintf (fpw, "          ctx.moveTo(px1, py1);\n");
  fprintf (fpw, "          ctx.lineTo(px2, py2);\n");
  fprintf (fpw, "          ctx.stroke();\n");
  fprintf (fpw, "        }\n");
  fprintf (fpw, "\n");
  fprintf (fpw, "        // vorbereitende Einstellungen --------------------------------------------------\n");
  fprintf (fpw, "        var c = document.getElementById(canvas_id);\n");
  fprintf (fpw, "        var ctx = c.getContext(\"2d\");\n");
  fprintf (fpw, "\n");
  fprintf (fpw, "        ctx.translate(0.5, 0.5);  // Hack damit die Linien wirklich nur 1 px dick sind\n");
  fprintf (fpw, "        ctx.lineWidth = 1;\n");
  fprintf (fpw, "\n");
  fprintf (fpw, "        ctx.strokeStyle=color;\n");
  fprintf (fpw, "\n");
  fprintf (fpw, "        // Kurve zeichnen ---------------------------------------------------------------\n");
  fprintf (fpw, "        var num = arzeit.length;\n");
  fprintf (fpw, "        var px = 0;\n");
  fprintf (fpw, "        var py = 0;\n");
  fprintf (fpw, "\n");
  fprintf (fpw, "        for (k = 0; k < num; k++) {\n");
  fprintf (fpw, "          px = arzeit[k] * 15 + 64;\n");
  fprintf (fpw, "          py = 519 - (arns[k] * 20);\n");
  fprintf (fpw, "          if (arns[k] > 25) {\n");
  fprintf (fpw, "            py = 10;\n");
  fprintf (fpw, "          }\n");
  fprintf (fpw, "\n");
  fprintf (fpw, "          if (arns[k] > 0) {\n");
  fprintf (fpw, "            for (i = 0; i < 12; i++) {\n");
  fprintf (fpw, "              dd_line ((px+i), 520, (px+i), py);\n");
  fprintf (fpw, "            }\n");
  fprintf (fpw, "          }\n");
  fprintf (fpw, "        }\n");
  fprintf (fpw, "      }\n");
  fprintf (fpw, "\n");
  fprintf (fpw, "\n");

  fprintf (fpw, "      draw_diagram (\"diagrammi\", 1);\n");
  fprintf (fpw, "\n");

  fprintf (fpw, "      var grafzeit = ");
  c = '[';
  for (i = 0; i < 31; i++) {
    if ((mtemper.dvalid[i] == 1) && (slg_dtemper_checkvalid(&mtemper.dtemper[i]) == 0)) {
      fprintf (fpw, "%c", c);
      fprintf (fpw, "%lu", (unsigned long) (i+1));
      c = ',';
    }
  }
  fprintf (fpw, "];\n");

  fprintf (fpw, "      var graftmin = ");
  c = '[';
  for (i = 0; i < 31; i++) {
    if ((mtemper.dvalid[i] == 1) && (slg_dtemper_checkvalid(&mtemper.dtemper[i]) == 0)) {
      ind = slg_dtemper_indmin (&mtemper.dtemper[i]);
      slg_temper2str (tstr, 0, mtemper.dtemper[i].val[ind]);
      fprintf (fpw, "%c", c);
      fprintf (fpw, "%s", tstr);
      c = ',';
    }
  }
  fprintf (fpw, "];\n");

  fprintf (fpw, "      var graftmax = ");
  c = '[';
  for (i = 0; i < 31; i++) {
    if ((mtemper.dvalid[i] == 1) && (slg_dtemper_checkvalid(&mtemper.dtemper[i]) == 0)) {
      ind = slg_dtemper_indmax (&mtemper.dtemper[i]);
      slg_temper2str (tstr, 0, mtemper.dtemper[i].val[ind]);
      fprintf (fpw, "%c", c);
      fprintf (fpw, "%s", tstr);
      c = ',';
    }
  }
  fprintf (fpw, "];\n");

  fprintf (fpw, "      var graftmid = ");
  c = '[';
  for (i = 0; i < 31; i++) {
    if ((mtemper.dvalid[i] == 1) && (slg_dtemper_checkvalid(&mtemper.dtemper[i]) == 0)) {
      slg_temper2str (tstr, 0, slg_dtemper_average(&mtemper.dtemper[i]));
      fprintf (fpw, "%c", c);
      fprintf (fpw, "%s", tstr);
      c = ',';
    }
  }
  fprintf (fpw, "];\n");

  fprintf (fpw, "      var grafns   = ");
  c = '[';
  for (i = 0; i < 31; i++) {
    if ((mtemper.dvalid[i] == 1) && (slg_dtemper_checkvalid(&mtemper.dtemper[i]) == 0)) {
      slg_temper2str (tstr, 0, slg_dtemper_average(&mtemper.dtemper[i]));
      slg_rain2str (tstr, 0, slg_drain_sum (&mrain.drain[i]));
      fprintf (fpw, "%c", c);
      fprintf (fpw, "%s", tstr);
      c = ',';
    }
  }
  fprintf (fpw, "];\n");

  fprintf (fpw, "\n");
  fprintf (fpw, "      draw_ns (\"diagrammi\", grafzeit, grafns);\n");
  fprintf (fpw, "      draw_temp (\"diagrammi\", grafzeit, graftmin, graftmid, graftmax);\n");
  fprintf (fpw, "\n");

  fprintf (fpw, "    </script>\n");

  fprintf (fpw, "  </div>\n");
  fprintf (fpw, "</body>\n");
  fprintf (fpw, "</html>\n");

  fclose(fpw);
}



/* writes output html file for month weather
 *
 * parameters:
 *   *fname:  output html file name
 *   *month:  month data
 *   z     :  0: current month
 *            1: older month
 *   t     :  0: no dayfile link
 *            1: include dayfile link
 *
 ****************************************************************************************/
void gen_dresden (char *fname, slg_monthdata *month, uint32_t z, uint32_t t)
{
  FILE         *fpw;
  char         c, smonth[20], smont[20], smonthdec[20], smonthinc[20], slastd[20], stavar[20], stmin[20], stmax[20], tstr[20];
  uint32_t     i, ind, day;
  slg_date     tdate;
  slg_mtemper  mtemper, mtemper1, mtemper2;


  if (month->date.m == 1)  strcpy (smonth, "Januar");
  if (month->date.m == 2)  strcpy (smonth, "Februar");
  if (month->date.m == 3)  strcpy (smonth, "Maerz");
  if (month->date.m == 4)  strcpy (smonth, "April");
  if (month->date.m == 5)  strcpy (smonth, "Mai");
  if (month->date.m == 6)  strcpy (smonth, "Juni");
  if (month->date.m == 7)  strcpy (smonth, "Juli");
  if (month->date.m == 8)  strcpy (smonth, "August");
  if (month->date.m == 9)  strcpy (smonth, "September");
  if (month->date.m == 10) strcpy (smonth, "Oktober");
  if (month->date.m == 11) strcpy (smonth, "November");
  if (month->date.m == 12) strcpy (smonth, "Dezember");

  /* calculate month links ******************************************/
  slg_date_copy (&tdate, &month->date);
  slg_date_to_fstring (smont, &tdate);
  smont[7] = 0;  /* cut day */
  slg_date_dec (&tdate);
  slg_date_to_fstring (smonthdec, &tdate);
  smonthdec[7] = 0;  /* cut day */
  for (i=0; i < 35; i++) slg_date_inc (&tdate);
  slg_date_to_fstring (smonthinc, &tdate);
  smonthinc[7] = 0;  /* cut day */

  /* calculate average, min and max temperatures ********************/
  slg_date_copy (&tdate, &month->date);
  slg_mtemper_read (&mtemper1, month, 1);
  slg_mtemper_read (&mtemper2, month, 2);
  slg_mtemper_merge_2 (&mtemper, "Aussen", &mtemper1, 20, 42, &mtemper2, 39, 79);

  slg_temper2str (stavar, 0, slg_mtemper_average (&mtemper));

  day = slg_mtemper_daymin (&mtemper);
  if (day == 0) {
    slg_temper2str (stmin, 0, CNERR);
  }
  else {
    ind = slg_dtemper_indmin (&mtemper.dtemper[day-1]);
    slg_temper2str (stmin, 0, mtemper.dtemper[day-1].val[ind]);
  }

  day = slg_mtemper_daymax (&mtemper);
  if (day == 0) {
    slg_temper2str (stmax, 0, CNERR);
  }
  else {
    ind = slg_dtemper_indmax (&mtemper.dtemper[day-1]);
    slg_temper2str (stmax, 0, mtemper.dtemper[day-1].val[ind]);
  }

  /* search last valid day in month *********************************/
  slg_date_copy (&tdate, &month->date);
  for (i=0; i < 31; i++) {
    if (month->dvalid[i]) {
      tdate.d = i + 1;
    }
  }
  slg_date_to_fstring (slastd, &tdate);


  /* write output html file ***************************************************/
  fpw = fopen (fname, "w");
  if (fpw == NULL) return;

  fprintf (fpw, "<!DOCTYPE html>\n");
  fprintf (fpw, "<head>\n");
  fprintf (fpw, "  <meta http-equiv=\"content-type\" content=\"text/html; charset=UTF-8\">\n");
  fprintf (fpw, "  <meta http-equiv=\"content-language\" content=\"de\">\n");
  fprintf (fpw, "  <meta name=\"author\" content=\"Jochen Ertel\">\n");
  fprintf (fpw, "  <meta name=\"description\" content=\"Monats-Wetter\">\n");
  fprintf (fpw, "  <meta name=\"generator\" content=\"" VERSION "\">\n");
  fprintf (fpw, "\n");
  fprintf (fpw, "  <title>Wetter %s</title>\n", smont);
  fprintf (fpw, "\n");
  fprintf (fpw, "  <style>\n");
  fprintf (fpw, "    body {\n");
  fprintf (fpw, "      background: #FFFFFF;\n");
  fprintf (fpw, "      padding: 0;\n");
  fprintf (fpw, "      margin: 0;\n");
  fprintf (fpw, "      text-align: center;\n");
  fprintf (fpw, "    }\n");
  fprintf (fpw, "\n");
  fprintf (fpw, "    #rahmen {\n");
  fprintf (fpw, "      position: relative;\n");
  fprintf (fpw, "      width: 640px;\n");
  fprintf (fpw, "      background: #AAEECC;\n");
  fprintf (fpw, "      margin: 20px auto;\n");
  fprintf (fpw, "      padding: 10px 20px 10px 20px;\n");
  fprintf (fpw, "      border: 0px;\n");
  fprintf (fpw, "      border-radius: 10px;\n");
  fprintf (fpw, "      box-shadow: 10px 10px 10px silver;\n");
  fprintf (fpw, "      text-align: center;\n");
  fprintf (fpw, "    }\n");
  fprintf (fpw, "\n");
  fprintf (fpw, "    .widget {\n");
  fprintf (fpw, "      position: relative;\n");
  fprintf (fpw, "      width: 640px;\n");
  fprintf (fpw, "      background: #FFFFFF;\n");
  fprintf (fpw, "      margin: 20px auto;\n");
  fprintf (fpw, "      padding: 10px 0px 0px 0px;\n");
  fprintf (fpw, "      border: 0px;\n");
  fprintf (fpw, "      border-radius: 10px;\n");
  fprintf (fpw, "      text-align: center;\n");
  fprintf (fpw, "    }\n");
  fprintf (fpw, "\n");
  fprintf (fpw, "    table.wid {\n");
  fprintf (fpw, "      width: 639px;\n");
  fprintf (fpw, "      margin: 0px 0px 0px 0px;\n");
  fprintf (fpw, "      padding: 0px;\n");
  fprintf (fpw, "      border-collapse:collapse;\n");
  fprintf (fpw, "      border: 0px;\n");
  fprintf (fpw, "    }\n");
  fprintf (fpw, "\n");
  fprintf (fpw, "    td.wid {\n");
  fprintf (fpw, "      width: 213px;\n");
  fprintf (fpw, "      margin: 0px;\n");
  fprintf (fpw, "      padding: 0px;\n");
  fprintf (fpw, "      border: 0px;\n");
  fprintf (fpw, "      text-align: center;\n");
  fprintf (fpw, "      vertical-align: text-bottom;\n");
  fprintf (fpw, "    }\n");
  fprintf (fpw, "\n");
  fprintf (fpw, "    table.lnk {\n");
  fprintf (fpw, "      width: 639px;\n");
  fprintf (fpw, "      margin: 10px 0px 20px 0px;\n");
  fprintf (fpw, "      padding: 0px;\n");
  fprintf (fpw, "      border-collapse:collapse;\n");
  fprintf (fpw, "      border: 0px;\n");
  fprintf (fpw, "    }\n");
  fprintf (fpw, "\n");
  fprintf (fpw, "    td.lnkl {\n");
  fprintf (fpw, "      width: 213px;\n");
  fprintf (fpw, "      margin: 0px;\n");
  fprintf (fpw, "      padding: 0px;\n");
  fprintf (fpw, "      border: 0px;\n");
  fprintf (fpw, "      text-align: left;\n");
  fprintf (fpw, "    }\n");
  fprintf (fpw, "\n");
  fprintf (fpw, "    td.lnkm {\n");
  fprintf (fpw, "      width: 213px;\n");
  fprintf (fpw, "      margin: 0px;\n");
  fprintf (fpw, "      padding: 0px;\n");
  fprintf (fpw, "      border: 0px;\n");
  fprintf (fpw, "      text-align: center;\n");
  fprintf (fpw, "    }\n");
  fprintf (fpw, "\n");
  fprintf (fpw, "    td.lnkr {\n");
  fprintf (fpw, "      width: 213px;\n");
  fprintf (fpw, "      margin: 0px;\n");
  fprintf (fpw, "      padding: 0px;\n");
  fprintf (fpw, "      border: 0px solid black;\n");
  fprintf (fpw, "      text-align: right;\n");
  fprintf (fpw, "    }\n");
  fprintf (fpw, "\n");
  fprintf (fpw, "    h1 {\n");
  fprintf (fpw, "      margin: 10px;\n");
  fprintf (fpw, "      padding: 0px;\n");
  fprintf (fpw, "      font-family: Arial, Helvetica, sans-serif;\n");
  fprintf (fpw, "      font-size: 2em;\n");
  fprintf (fpw, "      font-style: normal;\n");
  fprintf (fpw, "      font-weight:normal;\n");
  fprintf (fpw, "      color: #777777;\n");
  fprintf (fpw, "      text-align: center;\n");
  fprintf (fpw, "    }\n");
  fprintf (fpw, "\n");
  fprintf (fpw, "    p.p0 {\n");
  fprintf (fpw, "      margin: 5px;\n");
  fprintf (fpw, "      padding: 0px;\n");
  fprintf (fpw, "      font-family: Arial, Helvetica, sans-serif;\n");
  fprintf (fpw, "      font-style: normal;\n");
  fprintf (fpw, "      font-size: 2em;\n");
  fprintf (fpw, "      font-weight:normal;\n");
  fprintf (fpw, "      color: #777777;\n");
  fprintf (fpw, "    }\n");
  fprintf (fpw, "\n");
  fprintf (fpw, "    p.p1 {\n");
  fprintf (fpw, "      margin: 5px;\n");
  fprintf (fpw, "      padding: 0px;\n");
  fprintf (fpw, "      font-family: Arial, Helvetica, sans-serif;\n");
  fprintf (fpw, "      font-style: normal;\n");
  fprintf (fpw, "      font-size: 1.5em;\n");
  fprintf (fpw, "      font-weight:normal;\n");
  fprintf (fpw, "      color: #777777;\n");
  fprintf (fpw, "    }\n");
  fprintf (fpw, "\n");
  fprintf (fpw, "    p.p2 {\n");
  fprintf (fpw, "      margin: 5px;\n");
  fprintf (fpw, "      padding: 0px;\n");
  fprintf (fpw, "      font-family: Arial, Helvetica, sans-serif;\n");
  fprintf (fpw, "      font-style: normal;\n");
  fprintf (fpw, "      font-size: 1em;\n");
  fprintf (fpw, "      font-weight:normal;\n");
  fprintf (fpw, "      color: #777777;\n");
  fprintf (fpw, "    }\n");
  fprintf (fpw, "\n");
  fprintf (fpw, "    a:link {color:#8f542c;text-decoration:none;}\n");
  fprintf (fpw, "    a:visited {color:#8f542c;text-decoration:none;}\n");
  fprintf (fpw, "    a:hover {color:#d98347;text-decoration:none;}\n");
  fprintf (fpw, "    a:active {color:#d98347;text-decoration:none;}\n");
  fprintf (fpw, "\n");
  fprintf (fpw, "  </style>\n");
  fprintf (fpw, "</head>\n");
  fprintf (fpw, "\n");
  fprintf (fpw, "\n");

  fprintf (fpw, "<body>\n");
  fprintf (fpw, "  <div id=\"rahmen\">\n");

  fprintf (fpw, "    <table class=\"lnk\">\n");
  fprintf (fpw, "      <tr>\n");

  if (z == 0) {
    fprintf (fpw, "        <td class=\"lnkl\"><p class=\"p2\"><a href=\"%s.html\">Monat zurück</a></p></td>\n", smonthdec);
    if (t == 1) fprintf (fpw, "        <td class=\"lnkm\"><p class=\"p2\"><a href=\"index.html\">Tagesansicht</a></p></td>\n");
    else fprintf (fpw, "        <td class=\"lnkm\"><p class=\"p2\">&nbsp;</p></td>\n");
    fprintf (fpw, "        <td class=\"lnkr\"><p class=\"p2\">&nbsp;</p></td>\n");
  }

  if (z == 1) {
    fprintf (fpw, "        <td class=\"lnkl\"><p class=\"p2\"><a href=\"%s.html\">Monat zurück</a></p></td>\n", smonthdec);
    if (t == 1) fprintf (fpw, "        <td class=\"lnkm\"><p class=\"p2\"><a href=\"%s.html\">Tagesansicht</a></p></td>\n", slastd);
    else fprintf (fpw, "        <td class=\"lnkm\"><p class=\"p2\">&nbsp;</p></td>\n");
    fprintf (fpw, "        <td class=\"lnkr\"><p class=\"p2\"><a href=\"%s.html\">Monat vor</a></p></td>\n", smonthinc);
  }

  fprintf (fpw, "      </tr>\n");
  fprintf (fpw, "    </table>\n");
  fprintf (fpw, "\n");

  fprintf (fpw, "    <h1>%s</h1>\n", month->locstr);
  fprintf (fpw, "\n");
  fprintf (fpw, "    <p class=\"p0\">%s %lu</p>\n", smonth, (unsigned long) month->date.y);
  fprintf (fpw, "\n");

  fprintf (fpw, "    <div class=\"widget\">\n");
  fprintf (fpw, "      <canvas id=\"diagrammi\" width=\"620\" height=\"560\"></canvas>\n");
  fprintf (fpw, "    </div>\n");
  fprintf (fpw, "\n");

  fprintf (fpw, "    <div class=\"widget\">\n");
  fprintf (fpw, "      <table class=\"wid\">\n");
  fprintf (fpw, "        <tr>\n");
  fprintf (fpw, "          <td class=\"wid\"><p class=\"p2\">Temp. Minimum</p></td>\n");
  fprintf (fpw, "          <td class=\"wid\"><p class=\"p2\">Temp. Maximum</p></td>\n");
  fprintf (fpw, "          <td class=\"wid\"><p class=\"p2\">Temp. Mittel</p></td>\n");
  fprintf (fpw, "        </tr>\n");
  fprintf (fpw, "        <tr>\n");
  fprintf (fpw, "          <td class=\"wid\"><p class=\"p1\">%s &deg;C</p></td>\n", stmin);
  fprintf (fpw, "          <td class=\"wid\"><p class=\"p1\">%s &deg;C</p></td>\n", stmax);
  fprintf (fpw, "          <td class=\"wid\"><p class=\"p1\">%s &deg;C</p></td>\n", stavar);
  fprintf (fpw, "        </tr>\n");
  fprintf (fpw, "      </table>\n");
  fprintf (fpw, "    </div>\n");
  fprintf (fpw, "\n");

  fprintf (fpw, "    <script>\n");
  fprintf (fpw, "\n");
  fprintf (fpw, "      // ---------------------------------------------------------------------------------------------------\n");
  fprintf (fpw, "      // Diagramm zeichnen in einem canvas-Element: <canvas id=\"...\" width=\"620\" height=\"360\"></canvas>\n");
  fprintf (fpw, "      //\n");
  fprintf (fpw, "      // canvas_id   :  String der canvas Elemente-ID\n");
  fprintf (fpw, "      // ns          :  0: keine Niederschlagsachse\n");
  fprintf (fpw, "      //                1: Niederschlagsachse beschriften\n");
  fprintf (fpw, "      // ---------------------------------------------------------------------------------------------------\n");
  fprintf (fpw, "      function draw_diagram (canvas_id, ns) {\n");
  fprintf (fpw, "\n");
  fprintf (fpw, "        // interne Linien-zeichnen Funktionen -------------------------------------------\n");
  fprintf (fpw, "        function dd_line_h (px, py, l) {\n");
  fprintf (fpw, "          ctx.moveTo(px, py);\n");
  fprintf (fpw, "          ctx.lineTo(px+l, py);\n");
  fprintf (fpw, "        }\n");
  fprintf (fpw, "\n");
  fprintf (fpw, "        function dd_line_hd (px, py, l) {\n");
  fprintf (fpw, "          num = Math.floor(l / 6);\n");
  fprintf (fpw, "          for (i = 0; i < num; i = i + 1) {\n");
  fprintf (fpw, "            ctx.moveTo(px+6*i, py);\n");
  fprintf (fpw, "            ctx.lineTo(px+6*i+2, py);\n");
  fprintf (fpw, "          }\n");
  fprintf (fpw, "        }\n");
  fprintf (fpw, "\n");
  fprintf (fpw, "        function dd_line_v (px, py, l) {\n");
  fprintf (fpw, "          ctx.moveTo(px, py);\n");
  fprintf (fpw, "          ctx.lineTo(px, py+l);\n");
  fprintf (fpw, "        }\n");
  fprintf (fpw, "\n");
  fprintf (fpw, "        function dd_line_vd (px, py, l) {\n");
  fprintf (fpw, "          num = Math.floor(l / 6);\n");
  fprintf (fpw, "          for (i = 0; i < num; i = i + 1) {\n");
  fprintf (fpw, "            ctx.moveTo(px, py+6*i);\n");
  fprintf (fpw, "            ctx.lineTo(px, py+6*i+2);\n");
  fprintf (fpw, "          }\n");
  fprintf (fpw, "        }\n");
  fprintf (fpw, "\n");
  fprintf (fpw, "        // vorbereitende Einstellungen --------------------------------------------------\n");
  fprintf (fpw, "        var c = document.getElementById(canvas_id);\n");
  fprintf (fpw, "        var ctx = c.getContext(\"2d\");\n");
  fprintf (fpw, "\n");
  fprintf (fpw, "        ctx.translate(0.5, 0.5);  // Hack damit die Linien wirklich nur 1 px dick sind\n");
  fprintf (fpw, "        ctx.fillStyle = \"#FFFFFF\";\n");
  fprintf (fpw, "        ctx.fillRect(0,0,620,560);\n");
  fprintf (fpw, "\n");
  fprintf (fpw, "        ctx.lineWidth = 1;\n");
  fprintf (fpw, "\n");
  fprintf (fpw, "\n");
  fprintf (fpw, "        // Diagramm zeichnen ------------------------------------------------------------\n");
  fprintf (fpw, "        ctx.beginPath();\n");
  fprintf (fpw, "        ctx.strokeStyle=\"#AAAAAA\";\n");
  fprintf (fpw, "\n");
  fprintf (fpw, "        for (j = 1; j < 10; j++) {\n");
  fprintf (fpw, "          dd_line_hd (70, 20+j*50, 480);\n");
  fprintf (fpw, "        }\n");
  fprintf (fpw, "\n");
  fprintf (fpw, "        for (j = 1; j < 7; j++) {\n");
  fprintf (fpw, "          dd_line_vd (70+j*75, 20, 500);\n");
  fprintf (fpw, "        }\n");
  fprintf (fpw, "        ctx.stroke();\n");
  fprintf (fpw, "\n");

  fprintf (fpw, "        ctx.beginPath();\n");
  fprintf (fpw, "        ctx.strokeStyle=\"#FF0000\";\n");
  fprintf (fpw, "        dd_line_h (70, 20+150, 480);\n");
  fprintf (fpw, "        ctx.stroke();\n");
  fprintf (fpw, "\n");
  fprintf (fpw, "        ctx.beginPath();\n");
  fprintf (fpw, "        ctx.strokeStyle=\"#FF8C00\";\n");
  fprintf (fpw, "        dd_line_h (70, 20+250, 480);\n");
  fprintf (fpw, "        ctx.stroke();\n");
  fprintf (fpw, "\n");
  fprintf (fpw, "        ctx.beginPath();\n");
  fprintf (fpw, "        ctx.strokeStyle=\"#0000FF\";\n");
  fprintf (fpw, "        dd_line_h (70, 20+350, 480);\n");
  fprintf (fpw, "        ctx.stroke();\n");
  fprintf (fpw, "\n");

  fprintf (fpw, "        ctx.beginPath();\n");
  fprintf (fpw, "        ctx.strokeStyle=\"#777777\";\n");
  fprintf (fpw, "        dd_line_h (70, 20, 480);\n");
  fprintf (fpw, "        dd_line_h (70, 520, 480);\n");
  fprintf (fpw, "        dd_line_v (70, 20, 500);\n");
  fprintf (fpw, "        dd_line_v (550, 20, 500);\n");
  fprintf (fpw, "\n");
  fprintf (fpw, "        for (k = 0; k < 49; k++) {\n");
  fprintf (fpw, "          ctx.moveTo(70, 30+k*10);\n");
  fprintf (fpw, "          ctx.lineTo(74, 30+k*10);\n");
  fprintf (fpw, "          ctx.moveTo(546, 30+k*10);\n");
  fprintf (fpw, "          ctx.lineTo(550, 30+k*10);\n");
  fprintf (fpw, "        }\n");
  fprintf (fpw, "\n");
  fprintf (fpw, "        for (k = 0; k < 32; k++) {\n");
  fprintf (fpw, "          ctx.moveTo(85+k*15, 516);\n");
  fprintf (fpw, "          ctx.lineTo(85+k*15, 520);\n");
  fprintf (fpw, "          ctx.moveTo(85+k*15, 20);\n");
  fprintf (fpw, "          ctx.lineTo(85+k*15, 24);\n");
  fprintf (fpw, "        }\n");
  fprintf (fpw, "\n");
  fprintf (fpw, "        ctx.stroke();\n");
  fprintf (fpw, "\n");
  fprintf (fpw, "\n");
  fprintf (fpw, "        // Achsen beschriften -----------------------------------------------------------\n");
  fprintf (fpw, "        var zeitachse = [\"\", \"5.\", \"10.\", \"15.\", \"20.\", \"25.\", \"30.\"];\n");
  fprintf (fpw, "\n");
  fprintf (fpw, "        ctx.fillStyle = \"#777777\";\n");
  fprintf (fpw, "        ctx.font = '16px Arial';\n");
  fprintf (fpw, "\n");
  fprintf (fpw, "        // Zeitachse\n");
  fprintf (fpw, "        ctx.textAlign=\"center\";\n");
  fprintf (fpw, "        ctx.textBaseline=\"top\";\n");
  fprintf (fpw, "        for (k = 0; k < 7; k++) {\n");
  fprintf (fpw, "          ctx.fillText(zeitachse[k], 70+k*75, 530);\n");
  fprintf (fpw, "        }\n");
  fprintf (fpw, "\n");
  fprintf (fpw, "        // Temperaturachse\n");
  fprintf (fpw, "        ctx.textAlign=\"right\";\n");
  fprintf (fpw, "        ctx.textBaseline=\"middle\";\n");
  fprintf (fpw, "        for (k = 0; k < 11; k++) {\n");
  fprintf (fpw, "          ctx.fillText((35 - 5 * k), 60, 20+k*50);\n");
  fprintf (fpw, "        }\n");
  fprintf (fpw, "\n");
  fprintf (fpw, "        // Niederschlagsachse\n");
  fprintf (fpw, "        if (ns == 1) {\n");
  fprintf (fpw, "          ctx.fillStyle = \"#88BBFF\";\n");
  fprintf (fpw, "          var nsachse = [\"25.0\", \"22.5\", \"20.0\", \"17.5\", \"15.0\", \"12.5\", \"10.0\", \"7.5\", \"5.0\", \"2.5\", \"0.0\"];\n");
  fprintf (fpw, "          ctx.textAlign=\"left\";\n");
  fprintf (fpw, "          ctx.textBaseline=\"middle\";\n");
  fprintf (fpw, "          for (k = 0; k < 11; k++) {\n");
  fprintf (fpw, "            ctx.fillText(nsachse[k], 560, 20+k*50);\n");
  fprintf (fpw, "          }\n");
  fprintf (fpw, "        }\n");
  fprintf (fpw, "      }\n");
  fprintf (fpw, "\n");
  fprintf (fpw, "\n");

  fprintf (fpw, "      // ---------------------------------------------------------------------------------------------------\n");
  fprintf (fpw, "      // Temperaturkurve in Diagramm eines canvas-Elements zeichnen\n");
  fprintf (fpw, "      //\n");
  fprintf (fpw, "      // canvas_id   :  String der canvas Elemente-ID\n");
  fprintf (fpw, "      // arzeit      :  Array mit Zeitindex-Werten (1 ... 31, entspr. Tag)\n");
  fprintf (fpw, "      // artmin      :  Array mit Minimum-Temperaturwerten\n");
  fprintf (fpw, "      // artmid      :  Array mit Mittel-Temperaturwerten\n");
  fprintf (fpw, "      // artmax      :  Array mit Maximum-Temperaturwerten\n");
  fprintf (fpw, "      // ---------------------------------------------------------------------------------------------------\n");
  fprintf (fpw, "      function draw_temp (canvas_id, arzeit, artmin, artmid, artmax, maxtemp) {\n");
  fprintf (fpw, "        var colmin = \"#0000FF\";\n");
  fprintf (fpw, "        var colmid = \"#777777\";\n");
  fprintf (fpw, "        var colmax = \"#FF0000\";\n");
  fprintf (fpw, "\n");
  fprintf (fpw, "        // interne Zeichnen Funktionen --------------------------------------------------\n");
  fprintf (fpw, "        function dd_point (px, py, m) {\n");
  fprintf (fpw, "          if (m == 1) {\n");
  fprintf (fpw, "            ctx.beginPath();\n");
  fprintf (fpw, "            ctx.arc(px, py, 1, 0, 2 * Math.PI);\n");
  fprintf (fpw, "            ctx.stroke();\n");
  fprintf (fpw, "            ctx.beginPath();\n");
  fprintf (fpw, "            ctx.arc(px, py, 2, 0, 2 * Math.PI);\n");
  fprintf (fpw, "            ctx.stroke();\n");
  fprintf (fpw, "            ctx.beginPath();\n");
  fprintf (fpw, "            ctx.arc(px, py, 3, 0, 2 * Math.PI);\n");
  fprintf (fpw, "            ctx.stroke();\n");
  fprintf (fpw, "          }\n");
  fprintf (fpw, "          ctx.beginPath();\n");
  fprintf (fpw, "          ctx.arc(px, py, 4, 0, 2 * Math.PI);\n");
  fprintf (fpw, "          ctx.stroke();\n");
  fprintf (fpw, "        }\n");
  fprintf (fpw, "\n");
  fprintf (fpw, "        function dd_line (px1, py1, px2, py2) {\n");
  fprintf (fpw, "          ctx.beginPath();\n");
  fprintf (fpw, "          ctx.moveTo(px1, py1);\n");
  fprintf (fpw, "          ctx.lineTo(px2, py2);\n");
  fprintf (fpw, "          ctx.stroke();\n");
  fprintf (fpw, "        }\n");
  fprintf (fpw, "\n");
  fprintf (fpw, "        // vorbereitende Einstellungen --------------------------------------------------\n");
  fprintf (fpw, "        var c = document.getElementById(canvas_id);\n");
  fprintf (fpw, "        var ctx = c.getContext(\"2d\");\n");
  fprintf (fpw, "\n");
  fprintf (fpw, "        ctx.translate(0.5, 0.5);  // Hack damit die Linien wirklich nur 1 px dick sind\n");
  fprintf (fpw, "        ctx.lineWidth = 1;\n");
  fprintf (fpw, "\n");
  fprintf (fpw, "        // Kurve zeichnen ---------------------------------------------------------------\n");
  fprintf (fpw, "        var num = arzeit.length;\n");
  fprintf (fpw, "        var px = 0;\n");
  fprintf (fpw, "        var py_min = 0;\n");
  fprintf (fpw, "        var py_mid = 0;\n");
  fprintf (fpw, "        var py_max = 0;\n");
  fprintf (fpw, "        var notoveri = 1;\n");
  fprintf (fpw, "        var notovera = 1;\n");
  fprintf (fpw, "\n");
  fprintf (fpw, "        for (k = 0; k < num; k++) {\n");
  fprintf (fpw, "          px = arzeit[k] * 15 + 69;\n");
  fprintf (fpw, "\n");
  fprintf (fpw, "          py_min = 520 - ((artmin[k] * 10) - (35 * 10 - 500));\n");
  fprintf (fpw, "          notoveri = 1;\n");
  fprintf (fpw, "          if (py_min > 520) {\n");
  fprintf (fpw, "            notoveri = 0;\n");
  fprintf (fpw, "            py_min = 523;\n");
  fprintf (fpw, "          }\n");
  fprintf (fpw, "\n");
  fprintf (fpw, "          py_max = 520 - ((artmax[k] * 10) - (35 * 10 - 500));\n");
  fprintf (fpw, "          notovera = 1;\n");
  fprintf (fpw, "          if (py_max < 20) {\n");
  fprintf (fpw, "            notovera = 0;\n");
  fprintf (fpw, "            py_max = 17;\n");
  fprintf (fpw, "          }\n");
  fprintf (fpw, "\n");
  fprintf (fpw, "          py_mid = 520 - ((artmid[k] * 10) - (35 * 10 - 500));\n");
  fprintf (fpw, "\n");
  fprintf (fpw, "          ctx.strokeStyle=colmid;\n");
  fprintf (fpw, "          dd_point (px,py_mid,1);\n");
  fprintf (fpw, "\n");
  fprintf (fpw, "          dd_line (px-1, py_min, px-1, py_max);\n");
  fprintf (fpw, "          dd_line (px, py_min, px, py_max);\n");
  fprintf (fpw, "          dd_line (px+1, py_min, px+1, py_max);\n");
  fprintf (fpw, "\n");
  fprintf (fpw, "          ctx.strokeStyle=colmin;\n");
  fprintf (fpw, "          dd_point (px,py_min,notoveri);\n");
  fprintf (fpw, "\n");
  fprintf (fpw, "          ctx.strokeStyle=colmax;\n");
  fprintf (fpw, "          dd_point (px,py_max,notovera);\n");
  fprintf (fpw, "        }\n");
  fprintf (fpw, "      }\n");
  fprintf (fpw, "\n");
  fprintf (fpw, "\n");

  fprintf (fpw, "      draw_diagram (\"diagrammi\", 0);\n");
  fprintf (fpw, "\n");

  fprintf (fpw, "      var grafzeit = ");
  c = '[';
  for (i = 0; i < 31; i++) {
    if ((mtemper.dvalid[i] == 1) && (slg_dtemper_checkvalid(&mtemper.dtemper[i]) == 0)) {
      fprintf (fpw, "%c", c);
      fprintf (fpw, "%lu", (unsigned long) (i+1));
      c = ',';
    }
  }
  fprintf (fpw, "];\n");

  fprintf (fpw, "      var graftmin = ");
  c = '[';
  for (i = 0; i < 31; i++) {
    if ((mtemper.dvalid[i] == 1) && (slg_dtemper_checkvalid(&mtemper.dtemper[i]) == 0)) {
      ind = slg_dtemper_indmin (&mtemper.dtemper[i]);
      slg_temper2str (tstr, 0, mtemper.dtemper[i].val[ind]);
      fprintf (fpw, "%c", c);
      fprintf (fpw, "%s", tstr);
      c = ',';
    }
  }
  fprintf (fpw, "];\n");

  fprintf (fpw, "      var graftmax = ");
  c = '[';
  for (i = 0; i < 31; i++) {
    if ((mtemper.dvalid[i] == 1) && (slg_dtemper_checkvalid(&mtemper.dtemper[i]) == 0)) {
      ind = slg_dtemper_indmax (&mtemper.dtemper[i]);
      slg_temper2str (tstr, 0, mtemper.dtemper[i].val[ind]);
      fprintf (fpw, "%c", c);
      fprintf (fpw, "%s", tstr);
      c = ',';
    }
  }
  fprintf (fpw, "];\n");

  fprintf (fpw, "      var graftmid = ");
  c = '[';
  for (i = 0; i < 31; i++) {
    if ((mtemper.dvalid[i] == 1) && (slg_dtemper_checkvalid(&mtemper.dtemper[i]) == 0)) {
      slg_temper2str (tstr, 0, slg_dtemper_average(&mtemper.dtemper[i]));
      fprintf (fpw, "%c", c);
      fprintf (fpw, "%s", tstr);
      c = ',';
    }
  }
  fprintf (fpw, "];\n");

  fprintf (fpw, "\n");
  fprintf (fpw, "      draw_temp (\"diagrammi\", grafzeit, graftmin, graftmid, graftmax);\n");
  fprintf (fpw, "\n");

  fprintf (fpw, "    </script>\n");

  fprintf (fpw, "  </div>\n");
  fprintf (fpw, "</body>\n");
  fprintf (fpw, "</html>\n");

  fclose(fpw);
}






/***************************************************************************************************
 * main function
 **************************************************************************************************/

int main (int argc, char *argv[])
{
  uint32_t       res, l, m, y, z, n, t, hm;
  char           namer[256], namew[256];
  slg_monthdata  month;

  /* help menu ************************************************************************************/
  if ((parArgTypExists (argc, argv, 'h')) || (argc == 1)) {
    printf (VERSION "\n");
    printf ("  -> parameters:\n");
    printf ("     -h        :  prints this help menu\n");
    printf ("     -m <uint> :  month\n");
    printf ("     -y <uint> :  year\n");
    printf ("     -i <str>  :  path of input dayfiles (empty string or with '/' at end)\n");
    printf ("     -o <str>  :  output html file\n");
    printf ("     -l <uint> :  location: 0: Bretnig\n");
    printf ("                            1: Dresden\n");
    printf ("     -z <uint> :  mode:  0: current month\n");
    printf ("                         1: older month\n");
    printf ("     -t        :  include a dayfile link (optional)\n");
    printf ("     -n        :  dayfile does not have a header yet (optional)\n");

    return (0);
  }


  /* read parameters ******************************************************************************/
  if (!(parArgTypExists (argc, argv, 'm'))) {
    printf ("slg_legacy_htmlgen_month: error: missing parameter \'-m\'\n");
    return (1);
  }
  res = parGetUint32 (argc, argv, 'm', &m);
  if (res == 0) {
    printf ("slg_legacy_htmlgen_month: error: can not read value of parameter \'-m\'\n");
    return (1);
  }

  if (!(parArgTypExists (argc, argv, 'y'))) {
    printf ("slg_legacy_htmlgen_month: error: missing parameter \'-y\'\n");
    return (1);
  }
  res = parGetUint32 (argc, argv, 'y', &y);
  if (res == 0) {
    printf ("slg_legacy_htmlgen_month: error: can not read value of parameter \'-y\'\n");
    return (1);
  }

  if (!(parArgTypExists (argc, argv, 'i'))) {
    printf ("slg_legacy_htmlgen_month: error: missing parameter \'-i\'\n");
    return (1);
  }
  res = parGetString (argc, argv, 'i', namer);
  if (res == 0) {
    printf ("slg_legacy_htmlgen_month: error: can not read value of parameter \'-i\'\n");
    return (1);
  }

  if (!(parArgTypExists (argc, argv, 'o'))) {
    printf ("slg_legacy_htmlgen_month: error: missing parameter \'-o\'\n");
    return (1);
  }
  res = parGetString (argc, argv, 'o', namew);
  if (res == 0) {
    printf ("slg_legacy_htmlgen_month: error: can not read value of parameter \'-o\'\n");
    return (1);
  }

  if (!(parArgTypExists (argc, argv, 'l'))) {
    printf ("slg_legacy_htmlgen_month: error: missing parameter \'-l\'\n");
    return (1);
  }
  res = parGetUint32 (argc, argv, 'l', &l);
  if (res == 0) {
    printf ("slg_legacy_htmlgen_month: error: can not read value of parameter \'-l\'\n");
    return (1);
  }
  if (l > 1) {
    printf ("slg_legacy_htmlgen_month: error: invalid location\n");
    return (1);
  }

  if (!(parArgTypExists (argc, argv, 'z'))) {
    printf ("slg_legacy_htmlgen_month: error: missing parameter \'-z\'\n");
    return (1);
  }
  res = parGetUint32 (argc, argv, 'z', &z);
  if (res == 0) {
    printf ("slg_legacy_htmlgen_month: error: can not read value of parameter \'-z\'\n");
    return (1);
  }
  if (z > 1) {
    printf ("slg_legacy_htmlgen_month: error: invalid mode\n");
    return (1);
  }

  if (parArgTypExists (argc, argv, 't')) t = 1;
  else t = 0;

  if (parArgTypExists (argc, argv, 'n')) n = 1;
  else n = 0;


  /* read dayfiles of month ***********************************************************************/
  if (n == 0) hm = 0;
  else hm = l + 1;

  res = slg_readmonth (&month, namer, y, m, hm);

  if (res != 0) {
    gen_error (namew, res);
    return (1);
  }

  /* check in case of bretnig location */
  if (l == 0) {
    if ((slg_colexist_month(&month, DF_TEMP, 1) == 0) || (slg_colexist_month(&month, DF_RAIN, 2) == 0)) {
      gen_error (namew, 20);
      return (1);
    }
    else {
      gen_bretnig (namew, &month, z, t);
    }
  }

  /* check in case of dresden location */
  if (l == 1) {
    if ((slg_colexist_month(&month, DF_TEMP, 1) == 0) || (slg_colexist_month(&month, DF_TEMP, 2) == 0)) {
      gen_error (namew, 20);
    }
    else {
      gen_dresden (namew, &month, z, t);
    }
  }


  return (0);
}
