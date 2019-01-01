
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*            Test Environment for the testing of Object X                    */
/* (C) Copyright SES GmbH Berlin, SE Int.,1990,1991. All rights reserved.     */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/***********************************/
/* C and PM definitions            */
/***********************************/
#define INCL_WINFRAMEMGR
#define INCL_WINWINDOWMGR
#define INCL_WINATOM
#define INCL_WINDDE 
#define INCL_WINSYS
#define INCL_WINSTDDRAG
#define INCL_DOSMODULEMGR
#define INCL_WINHELP
#include <os2.h>
#include <stdlib.h>
#include <string.h>

/*---------------------------------*/
/* Interface file for Object DUMMY */
/*---------------------------------*/
#include <dummy.h>

/*---------------------------------*/
/* Interface file to SEIDLG2       */
/*---------------------------------*/
#include <seidlg2.h>

/*----------------------*/
/* Internal header file */
/*----------------------*/
#include "testi.h"

/***************************/
/* PROTOTYPE DEFINITIONS   */
/***************************/

MRESULT EXPENTRY WorkplaceWinProc( HWND hwnd, USHORT usMsgID,
                                   MPARAM mp1, MPARAM mp2 );


HWND cdecl  CreateWorkplace(HAB hAB);

MRESULT EXPENTRY DlgWinProc( HWND hwnd, USHORT usMsgID,
                             MPARAM mp1, MPARAM mp2 );

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*   PROCEDURE:   main()                                                      */
/*                                                                            */
/*   DESCRIPTION: The main procedure is initiated by the operating            */
/*                system and just calls the initialization routine            */
/*                SEIAPP with the name of the start function and              */
/*                argc and argv (optional).                                   */
/*                                                                            */
/*----------------------------------------------------------------------------*/
void cdecl main(USHORT argc, PSZ argv[])
{
   SeiApp(CreateWorkplace, argc, argv);
   DosExit (EXIT_THREAD, 0);
}



/*============================================================================*/
/*=======================     CreateWorkplace    =============================*/
/*============================================================================*/
HWND cdecl  CreateWorkplace (HAB hAB) {

/*--------------------*/
/* Internal variables */
/*--------------------*/
HWND        hFrame, hClient;                       /* Frame and client handle */
BOOL        bResult;
FRAMECDATA  fcdata;
HWND        hwndHelpInstance;

   /*----------------------------------------------*/
   /* Now we want to create the frame control data */
   /* to create the frame window                   */
   /*----------------------------------------------*/
   fcdata.cb            = sizeof(FRAMECDATA);
   fcdata.flCreateFlags =  FCF_STANDARD;
   fcdata.hmodResources = NULL;
   fcdata.idResources   = WIN_WORKPLACE;

   /*--------------------------*/
   /* Create the frame window  */
   /*--------------------------*/
   hFrame = WinCreateWindow(HWND_DESKTOP,     /* Parent                */
           WC_FRAME,                          /* Window class          */
           "",                                /* No window title       */
           NULL,                              /* No window style       */
           0,0,0,0,                           /* position/size         */
           HWND_DESKTOP,                      /* Owner                 */
           HWND_TOP,                          /* behind                */
           WIN_WORKPLACE,                     /* resource              */
           &fcdata,                           /* frame ctldata         */
           NULL);                             /* No presentation parms */
   if (hFrame == NULL) {
      DosBeep (1000,1000);
      return (NULL);
   }

   /*-----------------------------------------------------*/
   /* After the successful creation of the frame window   */
   /* we create the control data structure for the client */
   /* window and register the client window class         */
   /*-----------------------------------------------------*/
   bResult = WinRegisterClass(hAB,
                              "WORKPLACE",
                              WorkplaceWinProc,
                              CS_SIZEREDRAW,
                              sizeof (PVOID));
   if (!bResult){
      DosBeep (1000,500);
      return(0);
   }

  /* -------------------------------------*/
  /* Now the client window can be created */
  /* -------------------------------------*/
   hClient = WinCreateWindow(hFrame,        /* Parent window            */
             "WORKPLACE",              /* Window class name        */
             NULL,                          /* Window title             */
             NULL,                          /* window style             */
             0,0,0,0,                       /* position and size        */
             hFrame,                        /* Owner                    */
             HWND_TOP,                      /* Behind                   */
             FID_CLIENT,                    /* window id                */
             NULL,                          /* client control data      */
             NULL);                         /* Presentation parameter   */
  if (hClient == NULL) {
      DosBeep (1000, 100);
      return (0);
  }

  /*--------------------------*/
  /* Create the help instance */
  /*--------------------------*/
  hwndHelpInstance = SeiCreateHelpInstance (hFrame,
                                            NULL,
                                            TEST_HELPTABLE,
                                            "Application's Help Title",
                                            "TEST.HLP");

  /*---------------------------------------------*/
  /* It is now about time to position and size   */
  /* the frame window. For this we have to       */
  /* calculate the size of the frame based on    */
  /* the size of the client area in the template */
  /*---------------------------------------------*/
  WinSetWindowPos(hFrame,
                  HWND_TOP,
                  20,
                  20,
                  400,
                  400,
                  SWP_SIZE | SWP_MOVE | SWP_SHOW | SWP_ZORDER);
 return (hFrame);

}/* end of CreateWorkplace */




/*============================================================================*/
/*============================     WorkplaceWinProc    =======================*/
/*============================================================================*/
/*                                                                            */
/*   PROCEDURE:   WorkplaceWinProc (HWND, USHORT, ULONG, ULONG)               */
/*                                                                            */
/*   DESCRIPTION: Window handling procedure for main client window            */
/*                                                                            */
/*   STARTED BY:  main                                                        */
/*                                                                            */
/*----------------------------------------------------------------------------*/
MRESULT EXPENTRY WorkplaceWinProc( HWND hwnd, USHORT usMsgID,
                                   MPARAM mp1, MPARAM mp2 )
{
  /*--------------------*/
  /* Internal variables */
  /*--------------------*/

  switch (usMsgID) {

    case WM_CREATE: {
    } break;

    case WM_ACTIVATE: {
         /*-----------------------------------------------*/
         /* Wheever the window will be activated, it must */
         /* reset or set its help table                   */
         /*-----------------------------------------------*/
         SeiChangeHelpTable (WinQueryWindow (hwnd, QW_PARENT, TRUE),
                             NULL, TEST_HELPTABLE, HELP_LIB,
                             SHORT1FROMMP (mp1));
         } break;

    case WM_COMMAND: {
      switch (SHORT1FROMMP(mp1)) {

        case MN_FILEOPEN: {
             HWND      hwndDummy;
             /*--------------------------------------*/
             /* Create a primary or secondary window */
             /*--------------------------------------*/
             hwndDummy = CreateDummy (hwnd, "Charly Brown");
        } break;

        case MN_FILEEXIT: {
             WinPostMsg (hwnd, WM_QUIT, 0L, 0L);
        } break;

        case MN_ABOUT: {
             WinDlgBox (HWND_DESKTOP, hwnd, WinDefDlgProc, NULL,
                        DLG_ABOUT, NULL);
        } break;


        default: {
        } break;

      } /* end WM_COMMAND switch */

    } break; /* end of WM_COMMAND */

    case WM_PAINT: {
         HPS    hPS;
         RECTL  rect;
      /*------------------------------------------------------------*/
      /* Get a presentation space handle for the window's rectangle */
      /*------------------------------------------------------------*/
      hPS=WinBeginPaint(hwnd,
                        NULL,
                        &rect);
      /*---------------------------------*/
      /* Fill the rectangle with a color */
      /*---------------------------------*/
      WinFillRect(hPS,
                  &rect,
                  SYSCLR_WINDOW);

      /*----------------------------------------*/
      /* End painting in the presentation space */
      /*----------------------------------------*/
      WinEndPaint(hPS);
      } break;

    default: {
      break;
    }
  } /* end switch */

  return((MRESULT)WinDefWindowProc(hwnd, usMsgID, mp1, mp2));

}/* end of WinProc */
