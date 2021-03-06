/************************************************************************/
/**

   \file       WindIO.c
   
   \version    V1.5
   \date       07.07.14
   \brief      Windowing I/O for various systems
   
   \copyright  (c) UCL / Dr. Andrew C. R. Martin 1992-2014
   \author     Dr. Andrew C. R. Martin
   \par
               Institute of Structural & Molecular Biology,
               University College London,
               Gower Street,
               London.
               WC1E 6BT.
   \par
               andrew@bioinf.org.uk
               andrew.martin@ucl.ac.uk
               
**************************************************************************

   This code is NOT IN THE PUBLIC DOMAIN, but it may be copied
   according to the conditions laid out in the accompanying file
   COPYING.DOC.

   The code may be modified as required, but any modifications must be
   documented so that the person responsible can be identified.

   The code may not be sold commercially or included as part of a 
   commercial product except as described in the file COPYING.DOC.

**************************************************************************

   Description:
   ============


**************************************************************************

   Usage:
   ======

**************************************************************************

   Revision History:
   =================
-  V0.4  25.09.92 Original test version
-  V1.0  11.03.94 Various tidying up for release 
                  Added RePrompt() and WindowMode()
-  V1.1  15.03.94 Added sInteractive and modified logic with sWindowMode
-  V1.2  18.03.94 Bug fix in WindowInteractive(). Includes SysDefs.h
-  V1.3  18.10.95 Moved Yorn() here from general.c
-  V1.4  01.02.01 Changed gets() to fgets()
-  V1.5  07.07.14 Use bl prefix for functions By: CTP


*************************************************************************/
/* Doxygen
   -------
   #GROUP    General Programming
   #SUBGROUP Text window handling - experimental
   #FUNCTION  blScreen()
   Writes information to the screen. Handles any windows as appropriate.

   #FUNCTION  blPrompt()
   Sets a prompt for input. If windowing is on, this simply sets the 
   prompt variable (the actual prompt is issued by the GetKybdString()
   function). If no windowing is used, the actual string is printed.
   If the prompt ends with a . it is simply printed; if not, a > is 
   appended.

   #FUNCTION  blRePrompt()
   Reissue the current prompt. Only has any effect when windowing is not
   in use. Normally only used by ReadBufferedFile() and 
   ProbeBufferedFile() to re-issue prompts while eating blank lines.

   #FUNCTION  blGetKybdString()
   Reads a string from the keyboard

   #FUNCTION  blPagingOn()
   Switches on screen paging.

   #FUNCTION  blPagingOff()
   Switches off screen paging.

   #FUNCTION  blWindowMode()
   Switch window mode on or off.

   #FUNCTION  blWindowInteractive()
   Switch interactive mode on or off.
   If switched off, calls WindowMode(FALSE) to switch off windowing

   #FUNCTION blYorN()
   Get a yes or no response from the keyboard
*/
/************************************************************************/
/* Definition of windowing type. If nothing defined, simple screen I/O
   will be used.
*/
/* #define CURSES        */ /* Curses windowing                         */
/* #define AMIGA_WINDOWS */ /* Amiga windowing                          */

/************************************************************************/
/* Includes
*/
#include <stdio.h>
#include <string.h>

#include "SysDefs.h"
#include "WindIO.h"

/************************************************************************/
/* Defines and macros
*/
#if defined(CURSES) || defined(AMIGA_WINDOWS)
#  define WINDOWING
#endif

/************************************************************************/
/* Globals
*/
static char sPromptString[40];
static int  sLineCount   = 0;
static BOOL sDoPaging    = FALSE,
            sWindowMode  = FALSE,
            sInteractive = TRUE;

/************************************************************************/
/* Prototypes
*/

/************************************************************************/
/*>blScreen(char *string)
   ----------------------
*//**

   \param[in]     *string    String to write on window

   Writes information to the screen. Handles any windows as appropriate.

-  25.09.92 Original
-  02.10.92 Added CURSES support
-  05.10.92 Added AMIGA_WINDOWS support
-  07.10.92 Added paging support
-  11.03.94 Added check on sWindowMode
-  14.03.94 Changed this to check on sInteractive
            Changed check on WINDOWING to sWindowMode
-  01.02.01 Added maxlen parameter
-  15.02.01 oops! removed maxlen parameter, value of 80 should have gone
            into GetKybdString()
-  07.07.14 Use bl prefix for functions By: CTP
*/
void blScreen(char *string)
{
   if(sDoPaging && sInteractive)
   {
      if(strchr(string,'\n'))
      {
         if(sLineCount++ > 18)
         {
            char dummy[80];
            
            sLineCount = 0;

            if(!sWindowMode)
               printf("\n");

            blPrompt("More...");
            blGetKybdString(dummy, 80);
         }
      }
   }

#ifndef WINDOWING
   printf("%s",string);
#else
   if(sWindowMode)
   {
#  ifdef AMIGA_WINDOWS
      WriteMessageNR(string);
#  endif
#  ifdef CURSES
      outputstring(string);
#  endif
   }
   else
   {
      printf("%s",string);
   }
#endif

   return;
}

/************************************************************************/
/*>blPrompt(char *string)
   ----------------------
*//**

   \param[in]     *string   Prompt string

   Sets a prompt for input. If windowing is on, this simply sets the 
   prompt variable (the actual prompt is issued by the GetKybdString()
   function). If no windowing is used, the actual string is printed.
   If the prompt ends with a . it is simply printed; if not, a > is 
   appended.

-  25.09.92 Original
-  02.10.92 Added CURSES support
-  05.10.92 Added AMIGA_WINDOWS support
-  11.03.94 Modified to save prompt string even if not windowing
-  15.03.94 Now sets up string and just calls RePrompt()
-  07.07.14 Use bl prefix for functions By: CTP
*/
void blPrompt(char *string)
{
   if(string[strlen(string)-1] == '.')
      sprintf(sPromptString,"%s ",string);
   else
      sprintf(sPromptString,"%s> ",string);

   blRePrompt();
}

/************************************************************************/
/*>void blRePrompt(void)
   ---------------------
*//**

   Reissue the current prompt. Only has any effect when windowing is not
   in use. Normally only used by ReadBufferedFile() and 
   ProbeBufferedFile() to re-issue prompts while eating blank lines.

-  10.03.94 Original   By: ACRM
-  15.03.94 Changed to work whenever we're not windowing and are 
            interactive
-  07.07.14 Use bl prefix for functions By: CTP
*/
void blRePrompt(void)
{
   if(!sWindowMode && sInteractive)
   {
      printf("%s",sPromptString);
      fflush(stdout);
   }
}

/************************************************************************/
/*>void blGetKybdString(char *string, int maxlen)
   ----------------------------------------------
*//**

   Reads a string from the keyboard

-  02.10.92 Original
-  05.10.92 Added AMIGA_WINDOWS support
-  15.03.94 Added check on sWindowMode
-  01.02.01 Added maxlen parameter and changed gets() to fgets()
-  07.07.14 Use bl prefix for functions By: CTP
*/
void blGetKybdString(char *string, int maxlen)
{
   if(sWindowMode)
   {
#ifdef AMIGA_WINDOWS
      ReadKybd(sPromptString,string,79);
#endif

#ifdef CURSES
      GetCursesString(sPromptString, string);
#endif

#ifndef WINDOWING
      fgets(string,maxlen,stdin);
#endif
   }
   else
   {
      fgets(string,maxlen,stdin);
   }
}

/************************************************************************/
/*>void blPagingOn(void)
   ---------------------
*//**

   Switches on screen paging.

-  07.10.92 Original
-  07.07.14 Use bl prefix for functions By: CTP
*/
void blPagingOn(void)
{
   sLineCount = 0;
   sDoPaging  = TRUE;
}

/************************************************************************/
/*>void blPagingOff(void)
   ----------------------
*//**

   Switches off screen paging.

-  07.10.92 Original
-  07.07.14 Use bl prefix for functions By: CTP
*/
void blPagingOff(void)
{
   sDoPaging  = FALSE;
}

/************************************************************************/
/*>void blWindowMode(BOOL mode)
   ----------------------------
*//**

   \param[in]     mode    TRUE:  Use windowing
                          FALSE: Output normally (default)

   Switch window mode on or off.

-  11.03.94 Original    By: ACRM
-  15.03.94 Added check on WINDOWING
-  07.07.14 Use bl prefix for functions By: CTP
*/
void blWindowMode(BOOL mode)
{
#ifdef WINDOWING
   sWindowMode = mode;
#else
   sWindowMode = FALSE;
#endif
}

/************************************************************************/
/*>void blWindowInteractive(BOOL mode)
   -----------------------------------
*//**

   \param[in]     mode    TRUE:  Is interactive (default)
                          FALSE: Not interactive

   Switch interactive mode on or off.
   If switched off, calls WindowMode(FALSE) to switch off windowing

-  15.03.94 Original    By: ACRM
-  17.03.94 Set sInteractive not sWindowMode!
-  07.07.14 Use bl prefix for functions By: CTP
*/
void blWindowInteractive(BOOL mode)
{
   sInteractive = mode;

   if(!mode) blWindowMode(FALSE);
}


/************************************************************************/
/*>int blYorN(char deflt)
   ----------------------
*//**

   \param[in]     *deflt   Default response ('y' or 'n') if return is 
                           pressed without a letter or an invalid letter
                           is given
   \return                 0 if the user responds with N or n
                           1 if the user responds with Y or y 
                           2 if the user responds with A or a
                           3 if the user responds with Q or q

   Get a yes or no response from the keyboard

   A default ('y' or 'n') is supplied in the function call and hitting 
   <return> or supplying any invalid character will result in the 
   default being used.
   
   The routine will work correctly with any response which starts with 
   the right letter (e.g. Yes, Yeah, yellow(!), no, Never, etc.)

-  18.06.93 Original    By: ACRM
-  01.02.01 Added maxlen parameter to GetKybdString()
-  07.07.14 Use bl prefix for functions By: CTP
*/
int blYorN(char deflt)
{
   char  buffer[80],
         response;
   int   i;

   blGetKybdString(buffer, 20);
   response = buffer[0];

   if((response != 'Y') && (response != 'y') &&
      (response != 'N') && (response != 'n') &&
      (response != 'A') && (response != 'a') &&
      (response != 'Q') && (response != 'q'))
   {
      response = deflt;
   }

   switch(response)
   {
   case 'Y':
   case 'y':
      i = 1;
      break;
   case 'N':
   case 'n':
      i = 0;
      break;
   case 'A':
   case 'a':
      i = 2;
      break;
   case 'Q':
   case 'q':
      i = 3;
      break;
   default:    /* Should never occur                                    */
      i=0; 
      break;
   }

   return(i);
}


