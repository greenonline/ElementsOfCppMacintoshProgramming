///////////////////////////////////////////////////////////////////
//
//
// This file: DebugTEApp.cp
//
// This is the main application object for the simplest
// application program using TTEDoc and TDebugDoc
//
// © 1990 Dan Weston, All Rights Reserved
// 
///////////////////////////////////////////////////////////////////

#include "TApp.h" 
#include "TDoc.h" 
#include "TTEDoc.h" 
#include "TDebugDoc.h"

///////////////////////////////////////////////////////////////////
//
// class declarations
// 
///////////////////////////////////////////////////////////////////

class TTEApp : public TApp{ 
  protected:
    virtual TDoc * MakeDoc(SFReply *reply= (SFReply *) nil); 
    virtual int GetNumFileTypes(void) {return l;};
    virtual SFTypeList GetFileTypesList(void);
    virtual Boolean CanOpen(void) {return true;}
    virtual OSType CanAcceptClipType(void) {return 'TEXT';)
};

///////////////////////////////////////////////////////////////////
//
// Globals
// 
///////////////////////////////////////////////////////////////////

SFTypeList gtheTypes {'TEXT'}; 
TDebugDoc *gdebugDoc nil;

///////////////////////////////////////////////////////////////////
//
// main
//
///////////////////////////////////////////////////////////////////
void main(void)

 // create an instance of TTEApp 
  TTEApp theApp;

  // initialize the application 
  if(theApp.InitApp()) {
    gdebugDoc = MakeDebugDoc(&theApp);

    // open one window to start with,
    // unless we got files from the Finder 
    if(! theApp.OpenDocFromFinder())
      theApp.OpenNewDoc();

    // run the event loop until user quits theApp.EventLoop();
    //now clean up 
    theApp.CleanUp();
  }
}

///////////////////////////////////////////////////////////////////
//
// TTEApp::MakeDoc
// 
///////////////////////////////////////////////////////////////////
TDoc * TTEApp::MakeDoc(SFReply *reply) {
  TTEDoc *tem p= new TTEDoc(GetCreator(),reply);
  *gdebugDoc << "Making a new document, address = "
<< (int)temp << endl;
  return temp;
}

///////////////////////////////////////////////////////////////////
//
// TTEApp::GetFileTypesList
// 
///////////////////////////////////////////////////////////////////
SFTypeList TTEApp::GetFileTypesList(void) {
  return gtheTypes;
}






