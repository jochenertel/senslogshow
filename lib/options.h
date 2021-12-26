/***************************************************************************************************
 *
 * file     : options.h
 * function : functions to read simple options from command line tool call
 * author   : Jochen Ertel
 * created  : 31.08.2016
 * updated  : 26.12.2021
 *
 **************************************************************************************************/

#ifndef _options_h
#define _options_h


/* functions to read program call options *********************************************************/
/**************************************************************************************************/
/**************************************************************************************************/

/* function : parArgTypExists ()
 *            -> checks whether an argument typ exists or not
 *               (an argument typ of programm call consists of 1 character
 *               with a leading '-')
 *            -> returns 0 if argument does not exist or the index of argument
 *               type in the argv[] array
 ******************************************************************************/
int parArgTypExists (int argc, char *argv[], char argType);


/* function : parGetString ()
 *            -> gets string argument value
 *            -> returns 0 in error case, returns 1 if OK
 *               (string is limited to max. 4096 characters)
 ******************************************************************************/
int parGetString (int argc, char *argv[], char argType, char *value);


/* function : parGetUint32 ()
 *            -> gets uint32 argument value
 *            -> returns 0 in error case, returns 1 if OK
 ******************************************************************************/
int parGetUint32 (int argc, char *argv[], char argType, uint32_t *value);



#endif

