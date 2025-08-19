/***************************************************************************************************
 *
 * file     : slg_values.h
 *
 * function : senslog project c-library - meassurment conversion functions
 *
 * author   : Jochen Ertel
 *
 * created  : 17.10.2020
 * updated  : 06.08.2025
 *
 **************************************************************************************************/

#include <stdint.h>


#ifndef _slg_values_h
#define _slg_values_h


/* definitions ************************************************************************************/
/**************************************************************************************************/
/**************************************************************************************************/

# define CNERR 2000000000



/* convert functions ******************************************************************************/
/**************************************************************************************************/
/**************************************************************************************************/

/* converts a string into an uint32_t less then CNERR
 *
 * parameters:
 *   *str:  pointer to a c-string
 *
 * return value:
 *   CNERR :  error, invalid string
 *   other :  0 <= number < CNERR
 *
 ****************************************************************************************/
uint32_t slg_str2uint32 (char *str);


/* converts a string into an int32_t between -CNERR and +CNERR
 *
 * parameters:
 *   *str:  pointer to a c-string
 *
 * return value:
 *   CNERR :  error, invalid string
 *   other :  -CNERR < number < CNERR
 *
 ****************************************************************************************/
int32_t slg_str2int32 (char *str);


/* get time index number per day dependent on mode
 *
 * parameters:
 *   mode:  0: 96 values per day (15 min steps) starting with 00:00 ending with 23:45
 *          1: 96 values per day (15 min steps) starting with 00:15 ending with 24:00
 *
 * return value:
 *   CNERR :  error, invalid mode
 *   other :  number of time index values per day
 *
 ****************************************************************************************/
uint32_t slg_timeindexnum (uint32_t mode);


/* converts a day time string into index value depending on mode
 *
 * parameters:
 *   mode:  0: 96 values per day (15 min steps) starting with 00:00 ending with 23:45
 *          1: 96 values per day (15 min steps) starting with 00:15 ending with 24:00
 *   *str:  pointer to string of fix size (5 chars, format e.g. "17:30")
 *
 * return value:
 *   CNERR :  error, invalid string or mode
 *   other :  index of day time (0, 1, 2, ...)
 *
 ****************************************************************************************/
uint32_t slg_str2timeindex (uint32_t mode, char *str);


/* converts a day time index value into a string depending on mode
 *
 * parameters:
 *   *str:   pointer to target string (at least 6 chars size)
 *   mode:   0: 96 values per day (15 min steps) starting with 00:00 ending with 23:45
 *           1: 96 values per day (15 min steps) starting with 00:15 ending with 24:00
 *   summer: summer time flag (0: winter time, 1: summer time)
 *   ind:    index value (0, 1, 2, ...)
 *
 ****************************************************************************************/
void slg_timeindex2str (char *str, uint32_t mode, uint32_t summer, uint32_t ind);


/* converts a temperature string into integer (-99.9 to 999.9 deg celsius)
 *
 * parameters:
 *   *str:  pointer to string (3..5 chars, format e.g. "-13.7")
 *
 * return value:
 *   CNERR :  error, invalid string
 *   temp  :  temperature T*10
 *
 ****************************************************************************************/
int32_t slg_str2temper (char *str);


/* converts an integer value into a temperature string depending on mode
 *
 * parameters:
 *   *str:   pointer to target string (at least 6 chars size)
 *   mode:   0: variable string size (3 to 5 chars)
 *           1: fix string size (5 chars with leading spaces)
 *   temper: temperature value in tenth degree (-999 ... 9999)
 *
 ****************************************************************************************/
void slg_temper2str (char *str, uint32_t mode, int32_t temper);


/* converts a rain string into integer (0.00 to 99.99 mm)
 *
 * parameters:
 *   *str:  pointer to string (4..5 chars, format e.g. "12.75")
 *
 * return value:
 *   CNERR :  error, invalid string
 *   rain  :  rain*100
 *
 ****************************************************************************************/
uint32_t slgtmp_str2rain (char *str);


/* converts a rain string (old) into integer (0.0 to 99.9 mm)
 *
 * parameters:
 *   *str:  pointer to string (3..4 chars, format e.g. "12.4")
 *
 * return value:
 *   CNERR :  error, invalid string
 *   rain  :  rain*100
 *
 ****************************************************************************************/
uint32_t slgtmp_str2rainold (char *str);


/* converts a rain string into integer (0.00 to 99.99 mm)
 * !!! -> temporary a wrapper function for:
 * !!!    - slgtmp_str2rain()
 * !!!    - slgtmp_str2rainold()
 * !!! -> automatic format detection and value correction in old case (* 25/20)
 *
 * parameters:
 *   *str:  pointer to string (4..5 chars, format e.g. "12.75")
 *
 * return value:
 *   CNERR :  error, invalid string
 *   rain  :  rain*100
 *
 ****************************************************************************************/
uint32_t slg_str2rain (char *str);


/* converts an integer value into a rain string depending on mode
 *
 * parameters:
 *   *str: pointer to target string (at least 8 chars size)
 *   mode: 0: variable string size (4 to 7 chars)
 *         1: fix string size (5 chars with leading spaces, up to 99.99)
 *   rain: rain value in 100th mm (0 ... 999999)
 *
 ****************************************************************************************/
void slg_rain2str (char *str, uint32_t mode, uint32_t rain);


/* converts a boolean event string into integer (0 or 1)
 *
 * parameters:
 *   *str:  pointer to string (1 char, format e.g. "0")
 *
 * return value:
 *   CNERR :  error, invalid string
 *   event :  0 or 1
 *
 ****************************************************************************************/
uint32_t slg_str2event (char *str);


/* converts an integer value into a boolean event string
 *
 * parameters:
 *   *str:   pointer to target string (at least 2 chars size)
 *   event:  boolean event value (0 or 1)
 *
 ****************************************************************************************/
void slg_event2str (char *str, uint32_t event);



#endif

