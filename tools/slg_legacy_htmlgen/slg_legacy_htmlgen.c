/***************************************************************************************************
 *
 * file     : slg_legacy_htmlgen.c (command line tool "legacy senslog html page generation")
 *
 * author   : Jochen Ertel
 *
 * created  : 15.01.2022
 * updated  : 14.10.2023
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


#define VERSION "legacy senslog html page generation tool (version 0.3.0)"


/***************************************************************************************************
 * functions
 **************************************************************************************************/

/* writes an error message into output html file
 *
 * parameters:
 *   *fname:  output html file name
 *   e     :  error code of function slg_readdayfile()
 *
 ****************************************************************************************/
void gen_error (char *fname, uint32_t e)
{
  FILE *fpw;
  char estr[100];

  /* error string ***********************************************************************/
  if (e ==  1) strcpy (estr, "(file not found)");
  if (e ==  2) strcpy (estr, "(file contains invalid chars)");
  if (e ==  3) strcpy (estr, "(file contains a line which is to long)");
  if (e ==  4) strcpy (estr, "(unexpected file end)");
  if (e ==  5) strcpy (estr, "(first header part: invalid structure)");
  if (e ==  6) strcpy (estr, "(first header part: invalid time mode)");
  if (e ==  7) strcpy (estr, "(second header part: invalid structure)");
  if (e ==  8) strcpy (estr, "(second header part: invalid column typ)");
  if (e ==  9) strcpy (estr, "(second header part: to many columns)");
  if (e == 10) strcpy (estr, "(second header part: column ids are not unique)");
  if (e == 11) strcpy (estr, "(measurement lines: invalid number of values in line)");
  if (e == 12) strcpy (estr, "(measurement lines: a value is to long)");
  if (e == 13) strcpy (estr, "(measurement lines: invalid date or time value)");
  if (e == 14) strcpy (estr, "(measurement lines: invalid line order)");
  if (e == 15) strcpy (estr, "(to many lines)");
  if (e == 20) strcpy (estr, "(expected column ids not found)");

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
  fprintf (fpw, "    <p class=\"p1\">Reading Dayfile of Measurement Values Failed!</p>\n");
  fprintf (fpw, "    <p class=\"p2\">&nbsp;</p>\n");
  fprintf (fpw, "    <p class=\"p2\">%s</p>\n", estr);
  fprintf (fpw, "\n");
  fprintf (fpw, "  </div>\n");
  fprintf (fpw, "</body>\n");
  fprintf (fpw, "</html>\n");

  fclose(fpw);

  return;
}



/* writes output html file for day weather
 *
 * parameters:
 *   *fname:  output html file name
 *   *df   :  day data
 *   m     :  0: todays data
 *            1: yesterdays data
 *            2: older data
 *
 ****************************************************************************************/
void gen_bretnig (char *fname, slg_daydata *df, uint32_t m)
{
  FILE         *fpw;
  char         c, sdate[20], sdow[20], stavar[20], stcur[20], stmax[20], stmin[20], srsum[20],
               stimcur[20], stimmax[20], stimmin[20], sdatedec[20], sdateinc[20], tstr[20];
  uint32_t     summer, ind, i;
  int32_t      diamax;
  slg_date     date;
  slg_dtemper  temper;
  slg_drain    rain;


  /* read relevant values from dayfile ****************************************/
  slg_date_copy (&date, &df->date);
  slg_date_to_string (sdate, &date);
  slg_date_dow (sdow, &date);
  summer = slg_date_is_summertime (&date);

  slg_dtemper_read (&temper, df, 1);
  slg_drain_read (&rain, df, 2);

  ind = slg_dtemper_indmax (&temper);
  slg_timeindex2str (stimmax, temper.tmode, summer, ind);
  slg_temper2str (stmax, 0, temper.val[ind]);

  ind = slg_dtemper_indmin (&temper);
  slg_timeindex2str (stimmin, temper.tmode, summer, ind);
  slg_temper2str (stmin, 0, temper.val[ind]);

  ind = temper.last;
  slg_timeindex2str (stimcur, temper.tmode, summer, ind);
  slg_temper2str (stcur, 0, temper.val[ind]);

  slg_temper2str (stavar, 0, slg_dtemper_average (&temper));

  slg_rain2str (srsum, 0, slg_drain_sum (&rain));

  diamax = slg_dtemper_maxindayout30 (&temper, &date) / 10;

  slg_date_dec (&date);
  slg_date_to_fstring (sdatedec, &date);
  slg_date_inc (&date);
  slg_date_inc (&date);
  slg_date_to_fstring (sdateinc, &date);


  /* write output html file ***************************************************/
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
  fprintf (fpw, "      background: #FFC78F;\n");
  fprintf (fpw, "      margin: 20px auto;\n");
  fprintf (fpw, "      padding: 20px;\n");
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
  fprintf (fpw, "      padding: 10px 0px 10px 0px;\n");
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
  fprintf (fpw, "      margin: 20px 0px 0px 0px;\n");
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
  fprintf (fpw, "    <h1>%s</h1>\n", df->locstr);
  fprintf (fpw, "\n");
  fprintf (fpw, "    <p class=\"p1\">%s, %s</p>\n", sdow, sdate);
  fprintf (fpw, "    <p class=\"p2\">&nbsp;</p>\n");
  fprintf (fpw, "\n");
  fprintf (fpw, "    <div class=\"widget\">\n");
  fprintf (fpw, "      <table class=\"wid\">\n");
  fprintf (fpw, "        <tr>\n");
  fprintf (fpw, "          <td class=\"wid\"><p class=\"p2\">Temp. Minimum</p></td>\n");
  if (m == 0) {
    fprintf (fpw, "          <td class=\"wid\"><p class=\"p2\">Temp. Aktuell</p></td>\n");
  }
  else {
    fprintf (fpw, "          <td class=\"wid\"><p class=\"p2\">Temp. Mittel</p></td>\n");
  }
  fprintf (fpw, "          <td class=\"wid\"><p class=\"p2\">Temp. Maximum</p></td>\n");
  fprintf (fpw, "        </tr>\n");
  fprintf (fpw, "        <tr>\n");
  fprintf (fpw, "          <td class=\"wid\"><p class=\"p1\">%s &deg;C</p></td>\n", stmin);
  if (m == 0) {
    fprintf (fpw, "          <td class=\"wid\"><p class=\"p0\">%s &deg;C</p></td>\n", stcur);
  }
  else {
    fprintf (fpw, "          <td class=\"wid\"><p class=\"p0\">%s &deg;C</p></td>\n", stavar);
  }
  fprintf (fpw, "          <td class=\"wid\"><p class=\"p1\">%s &deg;C</p></td>\n", stmax);
  fprintf (fpw, "        </tr>\n");
  fprintf (fpw, "        <tr>\n");
  fprintf (fpw, "          <td class=\"wid\"><p class=\"p2\">%s Uhr</p></td>\n", stimmin);
  if (m == 0) {
    fprintf (fpw, "          <td class=\"wid\"><p class=\"p2\">%s Uhr</p></td>\n", stimcur);
  }
  else {
    fprintf (fpw, "          <td class=\"wid\"><p class=\"p2\">&nbsp;</p></td>\n");
  }
  fprintf (fpw, "          <td class=\"wid\"><p class=\"p2\">%s Uhr</p></td>\n", stimmax);
  fprintf (fpw, "        </tr>\n");
  fprintf (fpw, "      </table>\n");
  fprintf (fpw, "    </div>\n");
  fprintf (fpw, "\n");

  fprintf (fpw, "    <div class=\"widget\">\n");
  fprintf (fpw, "      <canvas id=\"diagrammi\" width=\"620\" height=\"360\"></canvas>\n");
  fprintf (fpw, "    </div>\n");
  fprintf (fpw, "\n");

  fprintf (fpw, "    <div class=\"widget\">\n");
  fprintf (fpw, "      <table class=\"wid\">\n");
  fprintf (fpw, "        <tr>\n");
  fprintf (fpw, "          <td class=\"wid\"><p class=\"p2\">&nbsp;</p></td>\n");
  fprintf (fpw, "          <td class=\"wid\"><p class=\"p2\">Regen Tagessumme</p></td>\n");
  fprintf (fpw, "          <td class=\"wid\"><p class=\"p2\">&nbsp;</p></td>\n");
  fprintf (fpw, "        </tr>\n");
  fprintf (fpw, "        <tr>\n");
  fprintf (fpw, "          <td class=\"wid\"><p class=\"p1\">&nbsp;</p></td>\n");
  fprintf (fpw, "          <td class=\"wid\"><p class=\"p0\">%s mm</p></td>\n", srsum);
  fprintf (fpw, "          <td class=\"wid\"><p class=\"p1\">&nbsp;</p></td>\n");
  fprintf (fpw, "        </tr>\n");
  fprintf (fpw, "      </table>\n");
  fprintf (fpw, "    </div>\n");
  fprintf (fpw, "\n");

  fprintf (fpw, "    <table class=\"lnk\">\n");
  fprintf (fpw, "      <tr>\n");

  if (m == 0) {
    fprintf (fpw, "        <td class=\"lnkl\"><p class=\"p2\"><a href=\"%s.html\">Tag zurück</a></p></td>\n", sdatedec);
    fprintf (fpw, "        <td class=\"lnkm\"><p class=\"p2\">&nbsp;</p></td>\n");
    fprintf (fpw, "        <td class=\"lnkr\"><p class=\"p2\">&nbsp;</p></td>\n");
  }

  if (m == 1) {
    fprintf (fpw, "        <td class=\"lnkl\"><p class=\"p2\"><a href=\"%s.html\">Tag zurück</a></p></td>\n", sdatedec);
    fprintf (fpw, "        <td class=\"lnkm\"><p class=\"p2\">&nbsp;</p></td>\n");
    fprintf (fpw, "        <td class=\"lnkr\"><p class=\"p2\"><a href=\"index.html\">Tag vor</a></p></td>\n");
  }

  if (m == 2) {
    fprintf (fpw, "        <td class=\"lnkl\"><p class=\"p2\"><a href=\"%s.html\">Tag zurück</a></p></td>\n", sdatedec);
    fprintf (fpw, "        <td class=\"lnkm\"><p class=\"p2\">&nbsp;</p></td>\n");
    fprintf (fpw, "        <td class=\"lnkr\"><p class=\"p2\"><a href=\"%s.html\">Tag vor</a></p></td>\n", sdateinc);
  }

  fprintf (fpw, "      </tr>\n");
  fprintf (fpw, "    </table>\n");
  fprintf (fpw, "\n");

  fprintf (fpw, "    <script>\n");
  fprintf (fpw, "\n");
  fprintf (fpw, "      // ---------------------------------------------------------------------------------------------------\n");
  fprintf (fpw, "      // Diagramm zeichnen in einem canvas-Element: <canvas id=\"...\" width=\"620\" height=\"360\"></canvas>\n");
  fprintf (fpw, "      //\n");
  fprintf (fpw, "      // canvas_id   :  String der canvas Elemente-ID\n");
  fprintf (fpw, "      // zeit        :  \"winter\": Zeitachse nach Winterzeit beschriften\n");
  fprintf (fpw, "      //                \"sommer\": Zeitachse nach Sommerzeit beschriften\n");
  fprintf (fpw, "      // maxtemp     :  oberer Temperaturwert an Temperaturachse\n");
  fprintf (fpw, "      // ns          :  0: keine Niederschlagsachse\n");
  fprintf (fpw, "      //                1: Niederschlagsachse beschriften\n");
  fprintf (fpw, "      // ---------------------------------------------------------------------------------------------------\n");
  fprintf (fpw, "      function draw_diagram (canvas_id, zeit, maxtemp, ns) {\n");
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
  fprintf (fpw, "        ctx.fillRect(0,0,620,360);\n");
  fprintf (fpw, "\n");
  fprintf (fpw, "        ctx.lineWidth = 1;\n");
  fprintf (fpw, "\n");
  fprintf (fpw, "\n");
  fprintf (fpw, "        // Diagramm zeichnen ------------------------------------------------------------\n");
  fprintf (fpw, "        ctx.beginPath();\n");
  fprintf (fpw, "        ctx.strokeStyle=\"#AAAAAA\";\n");
  fprintf (fpw, "\n");
  fprintf (fpw, "        for (j = 1; j < 6; j++) {\n");
  fprintf (fpw, "          dd_line_hd (70, 20+j*50, 480);\n");
  fprintf (fpw, "        }\n");
  fprintf (fpw, "\n");
  fprintf (fpw, "        for (j = 1; j < 8; j++) {\n");
  fprintf (fpw, "          dd_line_vd (70+j*60, 20, 300);\n");
  fprintf (fpw, "        }\n");
  fprintf (fpw, "        ctx.stroke();\n");
  fprintf (fpw, "\n");
  fprintf (fpw, "        ctx.beginPath();\n");
  fprintf (fpw, "        ctx.strokeStyle=\"#777777\";\n");
  fprintf (fpw, "        dd_line_h (70,20, 480);\n");
  fprintf (fpw, "        dd_line_h (70,320, 480);\n");
  fprintf (fpw, "        dd_line_v (70,20, 300);\n");
  fprintf (fpw, "        dd_line_v (550,20, 300);\n");
  fprintf (fpw, "\n");
  fprintf (fpw, "        for (k = 0; k < 29; k++) {\n");
  fprintf (fpw, "          ctx.moveTo(70, 30+k*10);\n");
  fprintf (fpw, "          ctx.lineTo(74, 30+k*10);\n");
  fprintf (fpw, "          ctx.moveTo(546, 30+k*10);\n");
  fprintf (fpw, "          ctx.lineTo(550, 30+k*10);\n");
  fprintf (fpw, "        }\n");
  fprintf (fpw, "\n");
  fprintf (fpw, "        for (k = 0; k < 23; k++) {\n");
  fprintf (fpw, "          ctx.moveTo(90+k*20, 316);\n");
  fprintf (fpw, "          ctx.lineTo(90+k*20, 320);\n");
  fprintf (fpw, "          ctx.moveTo(90+k*20, 20);\n");
  fprintf (fpw, "          ctx.lineTo(90+k*20, 24);\n");
  fprintf (fpw, "        }\n");
  fprintf (fpw, "\n");
  fprintf (fpw, "        ctx.stroke();\n");
  fprintf (fpw, "\n");
  fprintf (fpw, "\n");
  fprintf (fpw, "        // Achsen beschriften -----------------------------------------------------------\n");
  fprintf (fpw, "        if (zeit == \"winter\") {\n");
  fprintf (fpw, "          var zeitachse = [\"00:00\", \"03:00\", \"06:00\", \"09:00\", \"12:00\", \"15:00\", \"18:00\", \"21:00\", \"24:00\"];\n");
  fprintf (fpw, "        }\n");
  fprintf (fpw, "        if (zeit == \"sommer\") {\n");
  fprintf (fpw, "          var zeitachse = [\"01:00\", \"04:00\", \"07:00\", \"10:00\", \"13:00\", \"16:00\", \"19:00\", \"22:00\", \"25:00\"];\n");
  fprintf (fpw, "        }\n");
  fprintf (fpw, "\n");
  fprintf (fpw, "        ctx.fillStyle = \"#777777\";\n");
  fprintf (fpw, "        ctx.font = '16px Arial';\n");
  fprintf (fpw, "\n");
  fprintf (fpw, "        // Zeitachse\n");
  fprintf (fpw, "        ctx.textAlign=\"center\";\n");
  fprintf (fpw, "        ctx.textBaseline=\"top\";\n");
  fprintf (fpw, "        for (k = 0; k < 9; k++) {\n");
  fprintf (fpw, "          ctx.fillText(zeitachse[k], 70+k*60, 330);\n");
  fprintf (fpw, "        }\n");
  fprintf (fpw, "\n");
  fprintf (fpw, "        // Temperaturachse\n");
  fprintf (fpw, "        ctx.textAlign=\"right\";\n");
  fprintf (fpw, "        ctx.textBaseline=\"middle\";\n");
  fprintf (fpw, "        for (k = 0; k < 7; k++) {\n");
  fprintf (fpw, "          ctx.fillText((maxtemp - 5 * k), 60, 20+k*50);\n");
  fprintf (fpw, "        }\n");
  fprintf (fpw, "\n");
  fprintf (fpw, "        // Niederschlagsachse\n");
  fprintf (fpw, "        if (ns == 1) {\n");
  fprintf (fpw, "          ctx.fillStyle = \"#88BBFF\";\n");
  fprintf (fpw, "          var nsachse = [\"3.0\", \"2.5\", \"2.0\", \"1.5\", \"1.0\", \"0.5\", \"0.0\"];\n");
  fprintf (fpw, "          ctx.textAlign=\"left\";\n");
  fprintf (fpw, "          ctx.textBaseline=\"middle\";\n");
  fprintf (fpw, "          for (k = 0; k < 7; k++) {\n");
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
  fprintf (fpw, "      // arzeit      :  Array mit Zeitindex-Werten (0 ... 95, 0 entspr. 0:15 Uhr)\n");
  fprintf (fpw, "      // artemp      :  Array mit Temperaturwerten\n");
  fprintf (fpw, "      // maxtemp     :  oberer Temperaturwert an Temperaturachse\n");
  fprintf (fpw, "      // color       :  Farbe als RGB-String (z.B. \"#FF4444\")\n");
  fprintf (fpw, "      // ---------------------------------------------------------------------------------------------------\n");
  fprintf (fpw, "      function draw_temp (canvas_id, arzeit, artemp, maxtemp, color) {\n");
  fprintf (fpw, "\n");
  fprintf (fpw, "        // interne Zeichnen Funktionen --------------------------------------------------\n");
  fprintf (fpw, "        function dd_point (px, py, m) {\n");
  fprintf (fpw, "          if (m == 1) {\n");
  fprintf (fpw, "            ctx.beginPath();\n");
  fprintf (fpw, "            ctx.arc(px, py, 1, 0, 2 * Math.PI);\n");
  fprintf (fpw, "            ctx.stroke();\n");
  fprintf (fpw, "          }\n");
  fprintf (fpw, "          ctx.beginPath();\n");
  fprintf (fpw, "          ctx.arc(px, py, 2, 0, 2 * Math.PI);\n");
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
  fprintf (fpw, "        ctx.strokeStyle=color;\n");
  fprintf (fpw, "\n");
  fprintf (fpw, "        // Kurve zeichnen ---------------------------------------------------------------\n");
  fprintf (fpw, "        var num = arzeit.length;\n");
  fprintf (fpw, "        var px_old = 0;\n");
  fprintf (fpw, "        var py_old = 0;\n");
  fprintf (fpw, "        var px = 0;\n");
  fprintf (fpw, "        var py = 0;\n");
  fprintf (fpw, "        var notover = 1;\n");
  fprintf (fpw, "\n");
  fprintf (fpw, "        for (k = 0; k < num; k++) {\n");
  fprintf (fpw, "          px = arzeit[k] * 5 + 75;\n");
  fprintf (fpw, "          py = 320 - ((artemp[k] * 10) - (maxtemp * 10 - 300));\n");
  fprintf (fpw, "          notover = 1;\n");
  fprintf (fpw, "          if (py < 20) {\n");
  fprintf (fpw, "            notover = 0;\n");
  fprintf (fpw, "            py = 17;\n");
  fprintf (fpw, "          }\n");
  fprintf (fpw, "          if (py > 320) {\n");
  fprintf (fpw, "            notover = 0;\n");
  fprintf (fpw, "            py = 323;\n");
  fprintf (fpw, "          }\n");
  fprintf (fpw, "\n");
  fprintf (fpw, "          dd_point (px,py,notover);\n");
  fprintf (fpw, "\n");
  fprintf (fpw, "          if (k > 0) {\n");
  fprintf (fpw, "            if (arzeit[k] == (arzeit[k-1] + 1)) {\n");
  fprintf (fpw, "              dd_line (px_old, py_old, px, py);\n");
  fprintf (fpw, "            }\n");
  fprintf (fpw, "          }\n");
  fprintf (fpw, "\n");
  fprintf (fpw, "          px_old = px;\n");
  fprintf (fpw, "          py_old = py;\n");
  fprintf (fpw, "        }\n");
  fprintf (fpw, "      }\n");
  fprintf (fpw, "\n");
  fprintf (fpw, "\n");
  fprintf (fpw, "      // ---------------------------------------------------------------------------------------------------\n");
  fprintf (fpw, "      // Niederschlagswerte in Diagramm eines canvas-Elements zeichnen\n");
  fprintf (fpw, "      //\n");
  fprintf (fpw, "      // canvas_id   :  String der canvas Elemente-ID\n");
  fprintf (fpw, "      // arzeit      :  Array mit Zeitindex-Werten (0 ... 95, 0 entspr. 0:15 Uhr)\n");
  fprintf (fpw, "      // arns        :  Array mit Niederschlagswerten\n");
  fprintf (fpw, "      // color       :  Farbe als RGB-String (z.B. \"#FF4444\")\n");
  fprintf (fpw, "      // ---------------------------------------------------------------------------------------------------\n");
  fprintf (fpw, "      function draw_ns (canvas_id, arzeit, arns, color) {\n");
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
  fprintf (fpw, "          px = arzeit[k] * 5 + 70;\n");
  fprintf (fpw, "          py = 319 - (arns[k] * 100);\n");
  fprintf (fpw, "\n");
  fprintf (fpw, "          if (arns[k] > 0) {\n");
  fprintf (fpw, "            for (i = 0; i < 5; i++) {\n");
  fprintf (fpw, "              dd_line ((px+i), 320, (px+i), py);\n");
  fprintf (fpw, "            }\n");
  fprintf (fpw, "          }\n");
  fprintf (fpw, "        }\n");
  fprintf (fpw, "      }\n");
  fprintf (fpw, "\n");
  fprintf (fpw, "\n");

  if (summer) {
    fprintf (fpw, "      draw_diagram (\"diagrammi\", \"sommer\", %d, 1);\n", (int) diamax);
  }
  else {
    fprintf (fpw, "      draw_diagram (\"diagrammi\", \"winter\", %d, 1);\n", (int) diamax);
  }

  fprintf (fpw, "      var grafzeit = ");
  c = '[';
  for (i = 0; i < 96; i++) {
    if (temper.val[i] != CNERR) {
      fprintf (fpw, "%c", c);
      fprintf (fpw, "%lu", (unsigned long) i);
      c = ',';
    }
  }
  fprintf (fpw, "];\n");

  fprintf (fpw, "      var graftemp = ");
  c = '[';
  for (i = 0; i < 96; i++) {
    if (temper.val[i] != CNERR) {
      slg_temper2str (tstr, 0, temper.val[i]);
      fprintf (fpw, "%c", c);
      fprintf (fpw, "%s", tstr);
      c = ',';
    }
  }
  fprintf (fpw, "];\n");

  fprintf (fpw, "      var grafns = ");
  c = '[';
  for (i = 0; i < 96; i++) {
    if (temper.val[i] != CNERR) {
      slg_rain2str (tstr, 0, rain.val[i]);
      fprintf (fpw, "%c", c);
      fprintf (fpw, "%s", tstr);
      c = ',';
    }
  }
  fprintf (fpw, "];\n");

  fprintf (fpw, "      draw_ns (\"diagrammi\", grafzeit, grafns, \"#88BBFF\");\n");
  fprintf (fpw, "      draw_temp (\"diagrammi\", grafzeit, graftemp, %d, \"#FF3030\");\n", (int) diamax);

  fprintf (fpw, "    </script>\n");

  fprintf (fpw, "  </div>\n");
  fprintf (fpw, "</body>\n");
  fprintf (fpw, "</html>\n");

  fclose(fpw);
}



/* writes output html file for day weather
 *
 * parameters:
 *   *fname:  output html file name
 *   *df   :  day data
 *   m     :  0: todays data
 *            1: yesterdays data
 *            2: older data
 *
 ****************************************************************************************/
void gen_dresden (char *fname, slg_daydata *df, uint32_t m)
{
  FILE         *fpw;
  char         c, sdate[20], sdow[20], stavar[20], stcur[20], stmax[20], stmin[20],
               stimcur[20], stimmax[20], stimmin[20], sdatedec[20], sdateinc[20], tstr[20];
  uint32_t     summer, ind, i;
  int32_t      diamax;
  slg_date     date;
  slg_dtemper  temper1, temper2, temper;


  /* read relevant values from dayfile ****************************************/
  slg_date_copy (&date, &df->date);
  slg_date_to_string (sdate, &date);
  slg_date_dow (sdow, &date);
  summer = slg_date_is_summertime (&date);

  /* calculate real temperature by merging 2 sensors */
  slg_dtemper_read (&temper1, df, 1);
  slg_dtemper_read (&temper2, df, 2);
  slg_dtemper_merge_2 (&temper, "Aussen", &temper1, 20, 42, &temper2, 39, 79);

  ind = slg_dtemper_indmax (&temper);
  slg_timeindex2str (stimmax, temper.tmode, summer, ind);
  slg_temper2str (stmax, 0, temper.val[ind]);

  ind = slg_dtemper_indmin (&temper);
  slg_timeindex2str (stimmin, temper.tmode, summer, ind);
  slg_temper2str (stmin, 0, temper.val[ind]);

  ind = temper.last;
  slg_timeindex2str (stimcur, temper.tmode, summer, ind);
  slg_temper2str (stcur, 0, temper.val[ind]);

  slg_temper2str (stavar, 0, slg_dtemper_average (&temper));

  diamax = slg_dtemper_maxindayout30 (&temper, &date) / 10;

  slg_date_dec (&date);
  slg_date_to_fstring (sdatedec, &date);
  slg_date_inc (&date);
  slg_date_inc (&date);
  slg_date_to_fstring (sdateinc, &date);


  /* write output html file ***************************************************/
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
  fprintf (fpw, "      background: #AAEECC;\n");
  fprintf (fpw, "      margin: 20px auto;\n");
  fprintf (fpw, "      padding: 20px;\n");
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
  fprintf (fpw, "      padding: 10px 0px 10px 0px;\n");
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
  fprintf (fpw, "      margin: 20px 0px 0px 0px;\n");
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
  fprintf (fpw, "    a:link {color:#22aa66;text-decoration:none;}\n");
  fprintf (fpw, "    a:visited {color:#22aa66;text-decoration:none;}\n");
  fprintf (fpw, "    a:hover {color:#00aa00;text-decoration:none;}\n");
  fprintf (fpw, "    a:active {color:#00aa00;text-decoration:none;}\n");
  fprintf (fpw, "\n");
  fprintf (fpw, "  </style>\n");
  fprintf (fpw, "</head>\n");
  fprintf (fpw, "\n");
  fprintf (fpw, "\n");
  fprintf (fpw, "<body>\n");
  fprintf (fpw, "  <div id=\"rahmen\">\n");
  fprintf (fpw, "    <h1>%s</h1>\n", df->locstr);
  fprintf (fpw, "\n");
  fprintf (fpw, "    <p class=\"p1\">%s, %s</p>\n", sdow, sdate);
  fprintf (fpw, "    <p class=\"p2\">&nbsp;</p>\n");
  fprintf (fpw, "\n");
  fprintf (fpw, "    <div class=\"widget\">\n");
  fprintf (fpw, "      <table class=\"wid\">\n");
  fprintf (fpw, "        <tr>\n");
  fprintf (fpw, "          <td class=\"wid\"><p class=\"p2\">Temp. Minimum</p></td>\n");
  if (m == 0) {
    fprintf (fpw, "          <td class=\"wid\"><p class=\"p2\">Temp. Aktuell</p></td>\n");
  }
  else {
    fprintf (fpw, "          <td class=\"wid\"><p class=\"p2\">Temp. Mittel</p></td>\n");
  }
  fprintf (fpw, "          <td class=\"wid\"><p class=\"p2\">Temp. Maximum</p></td>\n");
  fprintf (fpw, "        </tr>\n");
  fprintf (fpw, "        <tr>\n");
  fprintf (fpw, "          <td class=\"wid\"><p class=\"p1\">%s &deg;C</p></td>\n", stmin);
  if (m == 0) {
    fprintf (fpw, "          <td class=\"wid\"><p class=\"p0\">%s &deg;C</p></td>\n", stcur);
  }
  else {
    fprintf (fpw, "          <td class=\"wid\"><p class=\"p0\">%s &deg;C</p></td>\n", stavar);
  }
  fprintf (fpw, "          <td class=\"wid\"><p class=\"p1\">%s &deg;C</p></td>\n", stmax);
  fprintf (fpw, "        </tr>\n");
  fprintf (fpw, "        <tr>\n");
  fprintf (fpw, "          <td class=\"wid\"><p class=\"p2\">%s Uhr</p></td>\n", stimmin);
  if (m == 0) {
    fprintf (fpw, "          <td class=\"wid\"><p class=\"p2\">%s Uhr</p></td>\n", stimcur);
  }
  else {
    fprintf (fpw, "          <td class=\"wid\"><p class=\"p2\">&nbsp;</p></td>\n");
  }
  fprintf (fpw, "          <td class=\"wid\"><p class=\"p2\">%s Uhr</p></td>\n", stimmax);
  fprintf (fpw, "        </tr>\n");
  fprintf (fpw, "      </table>\n");
  fprintf (fpw, "    </div>\n");
  fprintf (fpw, "\n");

  fprintf (fpw, "    <div class=\"widget\">\n");
  fprintf (fpw, "      <canvas id=\"diagrammi\" width=\"620\" height=\"360\"></canvas>\n");
  fprintf (fpw, "    </div>\n");
  fprintf (fpw, "\n");

  fprintf (fpw, "    <table class=\"lnk\">\n");
  fprintf (fpw, "      <tr>\n");

  if (m == 0) {
    fprintf (fpw, "        <td class=\"lnkl\"><p class=\"p2\"><a href=\"%s.html\">Tag zurück</a></p></td>\n", sdatedec);
    fprintf (fpw, "        <td class=\"lnkm\"><p class=\"p2\">&nbsp;</p></td>\n");
    fprintf (fpw, "        <td class=\"lnkr\"><p class=\"p2\">&nbsp;</p></td>\n");
  }

  if (m == 1) {
    fprintf (fpw, "        <td class=\"lnkl\"><p class=\"p2\"><a href=\"%s.html\">Tag zurück</a></p></td>\n", sdatedec);
    fprintf (fpw, "        <td class=\"lnkm\"><p class=\"p2\">&nbsp;</p></td>\n");
    fprintf (fpw, "        <td class=\"lnkr\"><p class=\"p2\"><a href=\"index.html\">Tag vor</a></p></td>\n");
  }

  if (m == 2) {
    fprintf (fpw, "        <td class=\"lnkl\"><p class=\"p2\"><a href=\"%s.html\">Tag zurück</a></p></td>\n", sdatedec);
    fprintf (fpw, "        <td class=\"lnkm\"><p class=\"p2\">&nbsp;</p></td>\n");
    fprintf (fpw, "        <td class=\"lnkr\"><p class=\"p2\"><a href=\"%s.html\">Tag vor</a></p></td>\n", sdateinc);
  }

  fprintf (fpw, "      </tr>\n");
  fprintf (fpw, "    </table>\n");
  fprintf (fpw, "\n");

  fprintf (fpw, "    <script>\n");
  fprintf (fpw, "\n");
  fprintf (fpw, "      // ---------------------------------------------------------------------------------------------------\n");
  fprintf (fpw, "      // Diagramm zeichnen in einem canvas-Element: <canvas id=\"...\" width=\"620\" height=\"360\"></canvas>\n");
  fprintf (fpw, "      //\n");
  fprintf (fpw, "      // canvas_id   :  String der canvas Elemente-ID\n");
  fprintf (fpw, "      // zeit        :  \"winter\": Zeitachse nach Winterzeit beschriften\n");
  fprintf (fpw, "      //                \"sommer\": Zeitachse nach Sommerzeit beschriften\n");
  fprintf (fpw, "      // maxtemp     :  oberer Temperaturwert an Temperaturachse\n");
  fprintf (fpw, "      // ns          :  0: keine Niederschlagsachse\n");
  fprintf (fpw, "      //                1: Niederschlagsachse beschriften\n");
  fprintf (fpw, "      // ---------------------------------------------------------------------------------------------------\n");
  fprintf (fpw, "      function draw_diagram (canvas_id, zeit, maxtemp, ns) {\n");
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
  fprintf (fpw, "        ctx.fillRect(0,0,620,360);\n");
  fprintf (fpw, "\n");
  fprintf (fpw, "        ctx.lineWidth = 1;\n");
  fprintf (fpw, "\n");
  fprintf (fpw, "\n");
  fprintf (fpw, "        // Diagramm zeichnen ------------------------------------------------------------\n");
  fprintf (fpw, "        ctx.beginPath();\n");
  fprintf (fpw, "        ctx.strokeStyle=\"#AAAAAA\";\n");
  fprintf (fpw, "\n");
  fprintf (fpw, "        for (j = 1; j < 6; j++) {\n");
  fprintf (fpw, "          dd_line_hd (70, 20+j*50, 480);\n");
  fprintf (fpw, "        }\n");
  fprintf (fpw, "\n");
  fprintf (fpw, "        for (j = 1; j < 8; j++) {\n");
  fprintf (fpw, "          dd_line_vd (70+j*60, 20, 300);\n");
  fprintf (fpw, "        }\n");
  fprintf (fpw, "        ctx.stroke();\n");
  fprintf (fpw, "\n");
  fprintf (fpw, "        ctx.beginPath();\n");
  fprintf (fpw, "        ctx.strokeStyle=\"#777777\";\n");
  fprintf (fpw, "        dd_line_h (70,20, 480);\n");
  fprintf (fpw, "        dd_line_h (70,320, 480);\n");
  fprintf (fpw, "        dd_line_v (70,20, 300);\n");
  fprintf (fpw, "        dd_line_v (550,20, 300);\n");
  fprintf (fpw, "\n");
  fprintf (fpw, "        for (k = 0; k < 29; k++) {\n");
  fprintf (fpw, "          ctx.moveTo(70, 30+k*10);\n");
  fprintf (fpw, "          ctx.lineTo(74, 30+k*10);\n");
  fprintf (fpw, "          ctx.moveTo(546, 30+k*10);\n");
  fprintf (fpw, "          ctx.lineTo(550, 30+k*10);\n");
  fprintf (fpw, "        }\n");
  fprintf (fpw, "\n");
  fprintf (fpw, "        for (k = 0; k < 23; k++) {\n");
  fprintf (fpw, "          ctx.moveTo(90+k*20, 316);\n");
  fprintf (fpw, "          ctx.lineTo(90+k*20, 320);\n");
  fprintf (fpw, "          ctx.moveTo(90+k*20, 20);\n");
  fprintf (fpw, "          ctx.lineTo(90+k*20, 24);\n");
  fprintf (fpw, "        }\n");
  fprintf (fpw, "\n");
  fprintf (fpw, "        ctx.stroke();\n");
  fprintf (fpw, "\n");
  fprintf (fpw, "\n");
  fprintf (fpw, "        // Achsen beschriften -----------------------------------------------------------\n");
  fprintf (fpw, "        if (zeit == \"winter\") {\n");
  fprintf (fpw, "          var zeitachse = [\"00:00\", \"03:00\", \"06:00\", \"09:00\", \"12:00\", \"15:00\", \"18:00\", \"21:00\", \"24:00\"];\n");
  fprintf (fpw, "        }\n");
  fprintf (fpw, "        if (zeit == \"sommer\") {\n");
  fprintf (fpw, "          var zeitachse = [\"01:00\", \"04:00\", \"07:00\", \"10:00\", \"13:00\", \"16:00\", \"19:00\", \"22:00\", \"25:00\"];\n");
  fprintf (fpw, "        }\n");
  fprintf (fpw, "\n");
  fprintf (fpw, "        ctx.fillStyle = \"#777777\";\n");
  fprintf (fpw, "        ctx.font = '16px Arial';\n");
  fprintf (fpw, "\n");
  fprintf (fpw, "        // Zeitachse\n");
  fprintf (fpw, "        ctx.textAlign=\"center\";\n");
  fprintf (fpw, "        ctx.textBaseline=\"top\";\n");
  fprintf (fpw, "        for (k = 0; k < 9; k++) {\n");
  fprintf (fpw, "          ctx.fillText(zeitachse[k], 70+k*60, 330);\n");
  fprintf (fpw, "        }\n");
  fprintf (fpw, "\n");
  fprintf (fpw, "        // Temperaturachse\n");
  fprintf (fpw, "        ctx.textAlign=\"right\";\n");
  fprintf (fpw, "        ctx.textBaseline=\"middle\";\n");
  fprintf (fpw, "        for (k = 0; k < 7; k++) {\n");
  fprintf (fpw, "          ctx.fillText((maxtemp - 5 * k), 60, 20+k*50);\n");
  fprintf (fpw, "        }\n");
  fprintf (fpw, "\n");
  fprintf (fpw, "        // Niederschlagsachse\n");
  fprintf (fpw, "        if (ns == 1) {\n");
  fprintf (fpw, "          ctx.fillStyle = \"#88BBFF\";\n");
  fprintf (fpw, "          var nsachse = [\"3.0\", \"2.5\", \"2.0\", \"1.5\", \"1.0\", \"0.5\", \"0.0\"];\n");
  fprintf (fpw, "          ctx.textAlign=\"left\";\n");
  fprintf (fpw, "          ctx.textBaseline=\"middle\";\n");
  fprintf (fpw, "          for (k = 0; k < 7; k++) {\n");
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
  fprintf (fpw, "      // arzeit      :  Array mit Zeitindex-Werten (0 ... 95, 0 entspr. 0:15 Uhr)\n");
  fprintf (fpw, "      // artemp      :  Array mit Temperaturwerten\n");
  fprintf (fpw, "      // maxtemp     :  oberer Temperaturwert an Temperaturachse\n");
  fprintf (fpw, "      // color       :  Farbe als RGB-String (z.B. \"#FF4444\")\n");
  fprintf (fpw, "      // ---------------------------------------------------------------------------------------------------\n");
  fprintf (fpw, "      function draw_temp (canvas_id, arzeit, artemp, maxtemp, color) {\n");
  fprintf (fpw, "\n");
  fprintf (fpw, "        // interne Zeichnen Funktionen --------------------------------------------------\n");
  fprintf (fpw, "        function dd_point (px, py, m) {\n");
  fprintf (fpw, "          if (m == 1) {\n");
  fprintf (fpw, "            ctx.beginPath();\n");
  fprintf (fpw, "            ctx.arc(px, py, 1, 0, 2 * Math.PI);\n");
  fprintf (fpw, "            ctx.stroke();\n");
  fprintf (fpw, "          }\n");
  fprintf (fpw, "          ctx.beginPath();\n");
  fprintf (fpw, "          ctx.arc(px, py, 2, 0, 2 * Math.PI);\n");
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
  fprintf (fpw, "        ctx.strokeStyle=color;\n");
  fprintf (fpw, "\n");
  fprintf (fpw, "        // Kurve zeichnen ---------------------------------------------------------------\n");
  fprintf (fpw, "        var num = arzeit.length;\n");
  fprintf (fpw, "        var px_old = 0;\n");
  fprintf (fpw, "        var py_old = 0;\n");
  fprintf (fpw, "        var px = 0;\n");
  fprintf (fpw, "        var py = 0;\n");
  fprintf (fpw, "        var notover = 1;\n");
  fprintf (fpw, "\n");
  fprintf (fpw, "        for (k = 0; k < num; k++) {\n");
  fprintf (fpw, "          px = arzeit[k] * 5 + 70;\n");
  fprintf (fpw, "          py = 320 - ((artemp[k] * 10) - (maxtemp * 10 - 300));\n");
  fprintf (fpw, "          notover = 1;\n");
  fprintf (fpw, "          if (py < 20) {\n");
  fprintf (fpw, "            notover = 0;\n");
  fprintf (fpw, "            py = 17;\n");
  fprintf (fpw, "          }\n");
  fprintf (fpw, "          if (py > 320) {\n");
  fprintf (fpw, "            notover = 0;\n");
  fprintf (fpw, "            py = 323;\n");
  fprintf (fpw, "          }\n");
  fprintf (fpw, "\n");
  fprintf (fpw, "          dd_point (px,py,notover);\n");
  fprintf (fpw, "\n");
  fprintf (fpw, "          if (k > 0) {\n");
  fprintf (fpw, "            if (arzeit[k] == (arzeit[k-1] + 1)) {\n");
  fprintf (fpw, "              dd_line (px_old, py_old, px, py);\n");
  fprintf (fpw, "            }\n");
  fprintf (fpw, "          }\n");
  fprintf (fpw, "\n");
  fprintf (fpw, "          px_old = px;\n");
  fprintf (fpw, "          py_old = py;\n");
  fprintf (fpw, "        }\n");
  fprintf (fpw, "      }\n");
  fprintf (fpw, "\n");
  fprintf (fpw, "\n");

  if (summer) {
    fprintf (fpw, "      draw_diagram (\"diagrammi\", \"sommer\", %d, 0);\n", (int) diamax);
  }
  else {
    fprintf (fpw, "      draw_diagram (\"diagrammi\", \"winter\", %d, 0);\n", (int) diamax);
  }

  fprintf (fpw, "      var grafzeit = ");
  c = '[';
  for (i = 0; i < 96; i++) {
    if (temper.val[i] != CNERR) {
      fprintf (fpw, "%c", c);
      fprintf (fpw, "%lu", (unsigned long) i);
      c = ',';
    }
  }
  fprintf (fpw, "];\n");

  fprintf (fpw, "      var graftemp = ");
  c = '[';
  for (i = 0; i < 96; i++) {
    if (temper.val[i] != CNERR) {
      slg_temper2str (tstr, 0, temper.val[i]);
      fprintf (fpw, "%c", c);
      fprintf (fpw, "%s", tstr);
      c = ',';
    }
  }
  fprintf (fpw, "];\n");

  fprintf (fpw, "      draw_temp (\"diagrammi\", grafzeit, graftemp, %d, \"#FF3030\");\n", (int) diamax);

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
  uint32_t     res, l, m, n, hm;
  char         namer[256], namew[256];
  slg_daydata  dayf;

  /* help menu ************************************************************************************/
  if ((parArgTypExists (argc, argv, 'h')) || (argc == 1)) {
    printf (VERSION "\n");
    printf ("  -> parameters:\n");
    printf ("     -h        :  prints this help menu\n");
    printf ("     -i <str>  :  input dayfile\n");
    printf ("     -o <str>  :  output html file\n");
    printf ("     -l <uint> :  location: 0: Bretnig\n");
    printf ("                            1: Dresden\n");
    printf ("     -m <uint> :  mode:  0: today\n");
    printf ("                         1: yesterday\n");
    printf ("                         2: older day\n");
    printf ("     -n        :  dayfile does not have a header yet (optional)\n");

    return (0);
  }


  /* read parameters ******************************************************************************/
  if (!(parArgTypExists (argc, argv, 'i'))) {
    printf ("slg_legacy_htmlgen: error: missing parameter \'-i\'\n");
    return (1);
  }
  res = parGetString (argc, argv, 'i', namer);
  if (res == 0) {
    printf ("slg_legacy_htmlgen: error: can not read value of parameter \'-i\'\n");
    return (1);
  }

  if (!(parArgTypExists (argc, argv, 'o'))) {
    printf ("slg_legacy_htmlgen: error: missing parameter \'-o\'\n");
    return (1);
  }
  res = parGetString (argc, argv, 'o', namew);
  if (res == 0) {
    printf ("slg_legacy_htmlgen: error: can not read value of parameter \'-o\'\n");
    return (1);
  }

  if (!(parArgTypExists (argc, argv, 'l'))) {
    printf ("slg_legacy_htmlgen: error: missing parameter \'-l\'\n");
    return (1);
  }
  res = parGetUint32 (argc, argv, 'l', &l);
  if (res == 0) {
    printf ("slg_legacy_htmlgen: error: can not read value of parameter \'-l\'\n");
    return (1);
  }
  if (l > 1) {
    printf ("slg_legacy_htmlgen: error: invalid location\n");
    return (1);
  }

  if (!(parArgTypExists (argc, argv, 'm'))) {
    printf ("slg_legacy_htmlgen: error: missing parameter \'-m\'\n");
    return (1);
  }
  res = parGetUint32 (argc, argv, 'm', &m);
  if (res == 0) {
    printf ("slg_legacy_htmlgen: error: can not read value of parameter \'-m\'\n");
    return (1);
  }
  if (m > 2) {
    printf ("slg_legacy_htmlgen: error: invalid mode\n");
    return (1);
  }

  if (parArgTypExists (argc, argv, 'n')) n = 1;
  else n = 0;


  /* read dayfile *********************************************************************************/
  if (n == 0) hm = 0;
  else hm = l + 1;

  res = slg_readdayfile (&dayf, namer, hm);

  if (res != 0) {
    gen_error (namew, res);
    return (1);
  }

  if (l == 0) {
    if ((slg_colexist(&dayf, DF_TEMP, 1) == 0) || (slg_colexist(&dayf, DF_RAIN, 2) == 0)) {
      gen_error (namew, 20);
    }
    else {
      gen_bretnig (namew, &dayf, m);
    }
  }

  if (l == 1) {
    if ((slg_colexist(&dayf, DF_TEMP, 1) == 0) || (slg_colexist(&dayf, DF_TEMP, 2) == 0)) {
      gen_error (namew, 20);
    }
    else {
      gen_dresden (namew, &dayf, m);
    }
  }


  return (0);
}
