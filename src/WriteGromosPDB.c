/************************************************************************/
/**

   \file       WriteGromosPDB.c
   
   \version    V1.8
   \date       07.07.14
   \brief      Write a PDB file from a linked list
   
   \copyright  (c) UCL / Dr. Andrew C. R. Martin 1993-2014
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

   This routine will write a .PDB file of any size from a linked list of 
   the protein structure. This list is contained in a linked set of 
   structures of type pdb_entry. The strucure is set up by including the 
   file "pdb.h". For details of the structure, see this file.

**************************************************************************

   Usage:
   ======

\code
   blWriteGromosPDB(fp, pdb)
\endcode

   \param[in]     *fp      A pointer to the file to write
   \param[in]     *pdb     The start of the PDB linked list.

**************************************************************************

   Revision History:
   =================
-  V1.0  08.03.89 Original
-  V1.2  28.03.90 Modified to match the correct column definition of 
                  ReadPDB V1.2   (N.B. There was no V1.1)
-  V1.3  01.06.92 Corrected header, to match standard. Autodoc'd, 
                  ANSIed. Added FPU check.
-  V1.4  10.06.93 Changed to use NEXT() macro. void types
-  V1.5  22.02.94 Added TER card at end of file
-  V1.6  30.05.02 Changed PDB field from 'junk' to 'record_type'
-  V1.7  04.02.14 Use CHAINMATCH macro. By: CTP
-  V1.8  07.07.14 Use bl prefix for functions By: CTP

*************************************************************************/
/* Doxygen
   -------
   #GROUP    Handling PDB Data
   #SUBGROUP File IO
   #FUNCTION  blWriteGromosPDB()
   Write a PDB linked list by calls to WriteGromosPDBRecord()

   #FUNCTION  blWriteGromosPDBRecord()
   Write a GROMOS PDB record
*/
/************************************************************************/
/* Includes
*/
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

#include "MathType.h"
#include "pdb.h"
#include "macros.h"

/************************************************************************/
/*>void blWriteGromosPDB(FILE *fp, PDB *pdb)
   -----------------------------------------
*//**

   \param[in]     *fp   PDB file pointer to be written
   \param[in]     *pdb  PDB linked list to write

   Write a PDB linked list by calls to WritePDBRecord()

-  08.03.89 Original
-  01.06.92 ANSIed and autodoc'd
-  10.06.93 Uses NEXT macro; void type
-  08.07.93 Added insertion of TER cards
-  22.02.94 And a TER card at the end of the file
-  15.02.01 This is the old WritePDB()
-  04.02.14 Use CHAINMATCH macro. By: CTP
-  07.07.14 Use bl prefix for functions By: CTP
*/
void blWriteGromosPDB(FILE *fp,
                      PDB  *pdb)
{
   PDB   *p;
   char  PrevChain[8];
   
   strcpy(PrevChain,pdb->chain);

   for(p = pdb ; p ; NEXT(p))
   {
      if(!CHAINMATCH(PrevChain,p->chain))
      {
         /* Chain change, insert TER card                               */
         fprintf(fp,"TER   \n");
         strcpy(PrevChain,p->chain);
      }
      blWriteGromosPDBRecord(fp,p);
   }
   fprintf(fp,"TER   \n");
}

/************************************************************************/
/*>void blWriteGromosPDBRecord(FILE *fp, PDB *pdb)
   -----------------------------------------------
*//**

   \param[in]     *fp     PDB file pointer to be written
   \param[in]     *pdb    PDB linked list record to write

   Write a PDB record

-  08.03.89 Original
-  28.03.90 Changed to match ReadPDB() V1.2 for column widths
-  01.06.92 ANSIed and autodoc'd
-  10.06.93 void type
-  22.06.93 Changed to %lf. Ljust strings
-  11.03.94 %lf back to %f (!)
-  12.02.01 This is the old WritePDBRecord()
-  07.07.14 Use bl prefix for functions By: CTP
*/
void blWriteGromosPDBRecord(FILE *fp,
                          PDB  *pdb)
{
   fprintf(fp,"%-6s%5d  %-4s%-4s%1s%4d%1s   %8.3f%8.3f%8.3f%6.2f%6.2f\n",
           pdb->record_type,
           pdb->atnum,
           pdb->atnam,
           pdb->resnam,
           pdb->chain,
           pdb->resnum,
           pdb->insert,
           pdb->x,
           pdb->y,
           pdb->z,
           pdb->occ,
           pdb->bval);
}


