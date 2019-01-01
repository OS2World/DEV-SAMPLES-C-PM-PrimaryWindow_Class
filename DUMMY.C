/*----------------------------------------------------------------------------*/
/*--                                                                       ---*/
/*--     Creation function and window procedure for Object DUMMY         ---*/
/*--                                                                       ---*/
/*----------------------------------------------------------------------------*/

/***********************************/
/* C and PM definitions            */
/***********************************/
#define INCL_DOSMODULEMGR
#define INCL_WINDDE
#define INCL_WINFRAMEMGR
#define INCL_WINHELP
#define INCL_WININPUT
#define INCL_WINSTDDRAG
#include <os2.h>

#include <stdlib.h>
#include <string.h>
/*-----------------------------*/
/* Interfaces to other objects */
/*-----------------------------*/
#include <seidlg2.h>

/*----------------------*/
/* External header file */
/*----------------------*/
#include <dummy.h>

/*----------------------*/
/* Internal header file */
/*----------------------*/
#include "dummyi.h"

/*-----------------------*/
/* Prototype definitions */
/*-----------------------*/

MRESULT EXPENTRY DlgWinProc( HWND hwnd, USHORT message,
                             MPARAM mp1, MPARAM mp2 );

MRESULT EXPENTRY DlgProc   ( HWND hwnd, USHORT message,
                             MPARAM mp1, MPARAM mp2 );

/*============================================================================*/
/*=========================     CreateDummy     ============================*/
/*============================================================================*/
/*                                                                            */
/*   PROCEDURE:        CreateDummy   (HWND, PVOID)                          */
/*                                                                            */
/*   INPUT PARAMETER:  (HWND) Window handle of the OWNER window               */
/*                     (PVOID) Pointer to parameter structure                 */
/*                                                                            */
/*   RETURN VALUE:     Window handle of the frame of the dialog window        */
/*                                                                            */
/*   DESCRIPTION:      This is the creation function for an object DUMMY    */
/*                                                                            */
/*   STARTED BY:       who knows !!!                                          */
/*                                                                            */
/*----------------------------------------------------------------------------*/
HWND EXPENTRY CreateDummy ( HWND hwnd, PVOID pParamDummy) {

  USHORT  usrc;
  HMODULE hModule;
  HWND    hwndFrame;

  usrc = DosGetModHandle (DLL_NAME, &hModule);


  hwndFrame = SeiCreateDialogWindow (HWND_DESKTOP,      /* Parent      */
                                     hwnd,              /* Owner       */
                                     WS_VISIBLE,        /* Frame style */
                                     NULL,              /* FCF         */
                                     DlgWinProc,        /* WinProc     */
                                     NULL,              /* Title       */
                                     hModule,           /* Resource    */
                                     DLG_MAIN,          /* ResourceID  */
                                     pParamDummy);

  return (hwndFrame);
  }


/*============================================================================*/
/*=========================     CreateSmallDummy     =======================*/
/*============================================================================*/
/*                                                                            */
/*   PROCEDURE:        CreateSmallDummy (HWND, PVOID))                      */
/*                                                                            */
/*   INPUT PARAMETER:  (HWND) Window handle of the OWNER window               */
/*                     (PVOID) Pointer to parameter structure                 */
/*                                                                            */
/*   RETURN VALUE:     Window handle of the frame of the dialog window        */
/*                                                                            */
/*   DESCRIPTION:      This is the creation function for an object DUMMY    */
/*                                                                            */
/*   STARTED BY:       who knows !!!                                          */
/*                                                                            */
/*----------------------------------------------------------------------------*/
HWND EXPENTRY CreateSmallDummy ( HWND hwnd, PVOID pParamDummy)
 {

  USHORT  usrc;
  HMODULE hModule;
  HWND    hwndFrame;

  usrc = DosGetModHandle (DLL_NAME, &hModule);


  hwndFrame = SeiCreateDialogWindow (HWND_DESKTOP,      /* Parent       */
                                     hwnd,              /* Owner        */
                                     NULL,              /* Frame style  */
                                     NULL,              /* FCF          */
                                     DlgWinProc,        /* WinProc      */
                                     NULL,              /* Title        */
                                     hModule,           /* Resource     */
                                     DLG_MAIN,          /* ResourceID   */
                                     pParamDummy);

  WinSetWindowPos (hwndFrame, HWND_TOP, 20, 20, 200, 200,
                   SWP_MOVE | SWP_SIZE | SWP_SHOW | SWP_ZORDER);

  return (hwndFrame);
  }


/*============================================================================*/
/*============================   DlgWinProc    ===============================*/
/*============================================================================*/
/*                                                                            */
/*   PROCEDURE:   DlgWinProc(HWND, USHORT, ULONG, ULONG)                      */
/*                                                                            */
/*   DESCRIPTION: Message handling procedure for ID_DIALOG                    */
/*                                                                            */
/*   STARTED BY:  main                                                        */
/*                                                                            */
/*----------------------------------------------------------------------------*/
MRESULT EXPENTRY DlgWinProc( HWND hwnd, USHORT message,
                             MPARAM mp1, MPARAM mp2 )  {

  switch (message) {

    case WM_INITDLG: {
         WinSetDlgItemText (hwnd, EF_NAME, PVOIDFROMMP (mp2));
         } break;

    case WM_CONTROL: {
         /*-----------------------------------------------------*/
         /* This peace of code makes sure, that the window with */
         /* the keyboard focus is always visible.               */
         /* This code is optional!                              */
         /*-----------------------------------------------------*/
         if (SeiIsSetFocusNotification (hwnd,
                                        SHORT1FROMMP (mp1),
                                        SHORT2FROMMP (mp1))) {
             SeiMoveToTop (hwnd,                         /* Window handle   */
                           SHORT1FROMMP (mp1),           /* Control ID      */
                           FALSE);                       /* Only is outside */

             SeiMoveToLeft (hwnd,                        /* Window handle   */
                            SHORT1FROMMP (mp1),          /* Control ID      */
                            FALSE);                      /* Only is outside */
         }
         } break;

    case WM_ACTIVATE: {
         SeiChangeHelpTable (SeiQueryDialogFrame (hwnd),
                             DLL_NAME, DUMMY_HELPTABLE, HELP_LIB,
                             SHORT1FROMMP (mp1));
         } break;

    case WM_CLOSE: {
         WinDestroyWindow (hwnd);
         } break;

    case WM_COMMAND: {
         switch (SHORT1FROMMP (mp1))
           {
           case MN_FILEOPEN:
                CreateSmallDummy (WinQueryWindow (SeiQueryDialogFrame (hwnd),
                                                  QW_OWNER, TRUE),
                                  "Little John Doe");

                break;

           case MN_FILEEXIT:
                /*--------------------*/
                /* Destroy the window */
                /*--------------------*/
                WinDestroyWindow (hwnd);
                break;

           case MN_FILENEW: {
                HMODULE     hmod;

                /*---------------------*/
                /* Open a modal dialog */
                /*---------------------*/
                DosGetModHandle (DLL_NAME, &hmod);
                WinDlgBox (HWND_DESKTOP, hwnd,
                           DlgProc, hmod, DLG_DIALOG, NULL);
                } break;

           default:
                DosBeep (500,100);
                break;
           } /* end WM_COMMAND switch */

           } return (0); /* !!! Very, very, very IMPORTANT !!!!!             */
                         /* Never use break at the end of WM_COMMAND !!!!!!  */

    /*********************************************************************/
    /**********************  DRAG DROP SUPPORT ***************************/
    /*********************************************************************/
    case WM_BUTTON2DOWN: {
         HPOINTER    hPtr;
         PSZ         pszSendableDatatypes[1];
         /*---------------------------------------------------------------*/
         /* Build list of data types sendable through direct manipulation */
         /*---------------------------------------------------------------*/
         pszSendableDatatypes[0] = DDECUSTOMERTYPE;

         /*-----------------------------------------*/
         /* Determine the shape of the drag pointer */
         /*-----------------------------------------*/
         hPtr = WinSendMsg (SeiQueryDialogFrame(hwnd), WM_QUERYICON,
                            NULL, NULL);
         SeiInitDrag (hwnd, pszSendableDatatypes, 1, hPtr);
         return ((MRESULT)FALSE);
         }

    case DM_DRAGOVER: {
         SHORT sGoodDragitemIndex;
         PSZ         pszReceivableDatatypes[1];
 
         /*-----------------------------------------------------------------*/
         /* Build list of data types receivable through direct manipulation */
         /*----------------------------------------------------------------*/
         pszReceivableDatatypes[0] = DDECUSTOMERTYPE;
         if (SeiDragOver ((PDRAGINFO) mp1, pszReceivableDatatypes, 1,
                          &sGoodDragitemIndex )) {
             /*-----------------*/
             /* Data type is OK */
             /*-----------------*/
             DrgFreeDraginfo((PDRAGINFO)mp1);
             return (MRFROM2SHORT (DOR_DROP, DO_COPY));
         } else {
             /*---------------------*/
             /* Data type is not OK */
             /*---------------------*/
             DrgFreeDraginfo((PDRAGINFO)mp1);
             return (MRFROM2SHORT (DOR_NEVERDROP, DO_COPY));
         }
         } break;

    case DM_DROP: {
         /*-------------------------------------------*/
         /* Request data with format 0                */
         /* This method creates the DM_RENDER message */
         /*-------------------------------------------*/
         return ((MRESULT)SeiDrop (hwnd, (PDRAGINFO) mp1, 0));
         }

    case DM_RENDER: {
         /*--------------------------*/
         /* Render customer DDE data */
         /*--------------------------*/
         DDECUSTOMER   ddeCust;
         ddeCust.cb = (ULONG)sizeof (DDECUSTOMER);
         WinQueryDlgItemText (hwnd, EF_NAME, sizeof (ddeCust.pszCustomer),
                              ddeCust.pszCustomer);
         WinQueryDlgItemText (hwnd, EF_CITY, sizeof (ddeCust.pszCity),
                              ddeCust.pszCity);
         WinQueryDlgItemText (hwnd, EF_STREET, sizeof (ddeCust.pszStreet),
                              ddeCust.pszStreet);
         WinQueryDlgItemText (hwnd, EF_ZIP, sizeof (ddeCust.pszZip),
                              ddeCust.pszZip);
         return ((MRESULT)SeiRender (hwnd, (PDRAGTRANSFER)mp1, (ULONG)sizeof(DDECUSTOMER),
                                     &ddeCust));
         }

    case WM_DDE_DATA: {
         /*-----------------------------------------------------*/
         /* This message is received by the TARGET container    */
         /* Parameter 2 contains a pointer to the DDESTRUCT     */
         /*-----------------------------------------------------*/
         ULONG          cb;
         CHAR           pszDDEType[40];
         DDECUSTOMER    ddeCust;
         /*----------------------------------*/
         /* Determine the received data type */
         /*----------------------------------*/
         SeiQueryDDEDataInfo ((PDDESTRUCT) mp2, &cb, pszDDEType,
                              (ULONG) sizeof(pszDDEType));

         /*-----------*/
         /* Read data */
         /*-----------*/
         SeiGetDDEData ((PDDESTRUCT)mp2, (PVOID)&ddeCust, cb);

         /*---------------------*/
         /* Update entry fields */
         /*---------------------*/
         WinSetDlgItemText (hwnd, EF_NAME,
                              ddeCust.pszCustomer);
         WinSetDlgItemText (hwnd, EF_CITY,
                              ddeCust.pszCity);
         WinSetDlgItemText (hwnd, EF_STREET,
                              ddeCust.pszStreet);
         WinSetDlgItemText (hwnd, EF_ZIP,
                              ddeCust.pszZip);

         /*-------------------------*/
         /* Change the window title */
         /*-------------------------*/
         WinSetWindowText (SeiQueryDialogFrame (hwnd),
                           ddeCust.pszCustomer);

         /*-------------------------------------------------------*/
         /* Be polite, and don't forget to acknowledge the        */
         /* reception of the data !!!!!!!!!!!!!!!!!!!!!!          */
         /*-------------------------------------------------------*/
         SeiDDEAck (hwnd, (PDDESTRUCT) mp2, (HWND) mp1,
                    (USHORT) TRUE);
         } break;

    case WM_DDE_ACK: {
         /*------------------------------------------------*/
         /* Data transfer was OK. Let's free DDE structure */
         /*------------------------------------------------*/
         DosFreeSeg (SELECTOROF(mp2));
         } break;
    default:
         break;
    } /* end switch */
  /*-------------------------------------------------------------------*/
  /* Don't call WinDefDlgProc BUT SeiDefDlgProc !!!!!!!!               */
  /*-------------------------------------------------------------------*/
  return((MRESULT)SeiDefDlgProc(hwnd,message,mp1,mp2));

}/* end of DlgWinProc */

/*============================================================================*/
/*============================   DlgProc       ===============================*/
/*============================================================================*/
/*                                                                            */
/*   PROCEDURE:   DlgProc(HWND, USHORT, ULONG, ULONG)                         */
/*                                                                            */
/*   DESCRIPTION:                                                             */
/*                                                                            */
/*   STARTED BY:                                                              */
/*                                                                            */
/*----------------------------------------------------------------------------*/
MRESULT EXPENTRY DlgProc( HWND hwnd, USHORT message,
                          MPARAM mp1, MPARAM mp2 )  {

  switch (message) {


    case WM_ACTIVATE: {
         SeiChangeHelpTable (hwnd,
                             DLL_NAME, DUMMY_HELPTABLE, HELP_LIB,
                             SHORT1FROMMP (mp1));
         } break;

    default:
         break;
    } /* end switch */
  return((MRESULT)WinDefDlgProc(hwnd,message,mp1,mp2));

}/* end of DlgProc */
