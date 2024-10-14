//////////////////////////////////////////////////////////////////
//
// MMAPICTView.cp
// This is the main program for MAPICTView
//
//© 1990 Dan Weston, All Rights Reserved
// 
//////////////////////////////////////////////////////////////////

#include <UMacApp.h> 
#include <UPrinting.h>
#include <UMAPictView.h>

//////////////////////////////////////////////////////////////////
//
// globals
// 
//////////////////////////////////////////////////////////////////
TPICTViewApp *gPICTViewApp;
#pragma segment Main

//////////////////////////////////////////////////////////////////
//
// main
// 
//////////////////////////////////////////////////////////////////
void main(void) (
  InitToolBox(); //Essential toolbox and utilities initialization

  if(ValidateConfiguration(&gConfiguration)) ( //Make sure we can run
    InitUMacApp(8);  //Initialize MacApp; 8 calls to MoreMasters
    InitUPrinting(); // Initialize the Printing unit
    gPICTViewApp = new TPICTViewApp;
    FailNIL(gPICTViewApp); 
    gPICTViewApp->IPICTViewApp();
    gPICTViewApp->Run();
  } else
    StdAlert(phUnsupportedConfiguration);
}
