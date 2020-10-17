/***************************************************************************************************
 *
 * file     : slg_values.h
 *
 * function : senslog project c-library - meassurment conversion functions
 *
 * author   : Jochen Ertel
 *
 * created  : 17.10.2020
 * updated  : 17.10.2020
 *
 **************************************************************************************************/

#include <stdint.h>


#ifndef _slg_values_h
#define _slg_values_h


/* convert functions ******************************************************************************/
/**************************************************************************************************/
/**************************************************************************************************/


/* converts a day time string into index value depending on mode
 *
 * parameters:
 *   mode:  0: 96 values per day (15 min steps) starting with 00:00 ending with 23:45
 *          1: 96 values per day (15 min steps) starting with 00:15 ending with 24:00
 *   *str:  pointer to string of fix size (5 chars, format e.g. "17:30")
 *
 * return value:
 *     0 :  error, invalid string or mode
 *   > 0 :  index of day time (1, 2, ...)
 *
 ****************************************************************************************/
uint32_t slg_str_to_time_ind (uint32_t mode, char *str);


/* converts a day time index value into a string depending on mode
 *
 * parameters:
 *   *str:   pointer to target string (at least 6 chars size)
 *   mode:   0: 96 values per day (15 min steps) starting with 00:00 ending with 23:45
 *           1: 96 values per day (15 min steps) starting with 00:15 ending with 24:00
 *   summer: summer time flag (0: winter time, >0: summer time)
 *   ind:    index value (1, 2, ...)
 *
 ****************************************************************************************/
void slg_time_ind_to_str (char *str, uint32_t mode, uint32_t summer, uint32_t ind);



#endif

