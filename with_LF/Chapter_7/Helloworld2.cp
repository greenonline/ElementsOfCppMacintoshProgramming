///////////////////////////////////////////////////////////////////
//
// This file: Helloworld2.cp
//
// This is the source for the
// Helloworld2 program
//
//© 1990 Dan Weston, All Rights Reserved
// 
///////////////////////////////////////////////////////////////////

#include <Windows.h> 
#include <Fonts.h>
#include "TApp.h" 
#include "TDoc.h"

///////////////////////////////////////////////////////////////////
//
// class declarations
// 
///////////////////////////////////////////////////////////////////
class THelloDoc : public TDoc{

  protected:

    // draw the window
    virtual void Draw(Rect *r);
};

class THelloApp : public TApp{


  protected: 
    // make our kind of document
    virtual TDoc * MakeDoc(SFReply *reply= (SFReply *)nil);
};

///////////////////////////////////////////////////////////////////
//
// main
// 
///////////////////////////////////////////////////////////////////
void main(void) {
  THelloApp theApp;

  // initialize the application
  if(theApp.InitApp()) {

    // open one window to start with
    theApp.OpenNewDoc();

    // Start our main event loop running.
    // This won't return until user quits
    theApp.EventLoop();
    //now clean up
    theApp.CleanUp();
  }
}

///////////////////////////////////////////////////////////////////
//
// THelloApp::MakeDoc
// 
///////////////////////////////////////////////////////////////////
TDoc * THelloApp::MakeDoc(SFReply */*reply*/) {
  return new THelloDoc();
}

///////////////////////////////////////////////////////////////////
//
// THelloDoc::Draw
// 
///////////////////////////////////////////////////////////////////
void THelloDoc::Draw(Rect * r) { ·
  EraseRect(r);
  TextSize(48);
  MoveTo(20,65);
  DrawString("\phello world");
}