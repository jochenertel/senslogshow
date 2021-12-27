/***************************************************************************************************
 *
 * file     : options.h
 * function : functions to read simple options from command line tool call
 * author   : Jochen Ertel
 * created  : 31.08.2016
 * updated  : 27.12.2021
 *
 **************************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>



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
int parArgTypExists (int argc, char *argv[], char argType)
{
  int  i;
  char tmp[3];

  tmp[0] = '-';
  tmp[1] = argType;
  tmp[2] = 0;

  if (argc > 1) {
    for (i = 1; i < argc; i++) {
      if (!strcmp (argv[i], tmp))
        return (i);
    }
  }
  return (0);
}


/* function : parGetString ()
 *            -> gets string argument value
 *            -> returns 0 in error case, returns 1 if OK
 *               (string is limited to max. 4096 characters)
 ******************************************************************************/
int parGetString (int argc, char *argv[], char argType, char *value)
{
  int a;

  a = parArgTypExists (argc, argv, argType);

  /* error checking */
  if (a == 0) return (0);
  if (a >= (argc -1)) return (0);
  if (strlen(argv[a+1]) > 4096) return (0);

  strcpy(value, argv[a+1]);
  return (1);
}


/* function : parGetUint32 ()
 *            -> gets uint32 argument value
 *            -> returns 0 in error case, returns 1 if OK
 ******************************************************************************/
int parGetUint32 (int argc, char *argv[], char argType, uint32_t *value)
{
  int a;
  unsigned long temp;

  a = parArgTypExists (argc, argv, argType);

  /* error checking */
  if (a == 0) return (0);
  if (a >= (argc -1)) return (0);

  a = sscanf(argv[a+1], "%lu", &temp);
  *value = (uint32_t) temp;

  return (a);
}




