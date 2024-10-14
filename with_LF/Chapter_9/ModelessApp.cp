///////////////////////////////////////////////////////////////////
//
//
// This file: ModelessApp.cp C++ Source
//
// This is the main application object a simple
// application program using TModelessDoc
//
// © 1990 Dan Weston, All Rights Reserved
// 
///////////////////////////////////////////////////////////////////

#include "TApp.h"
#include "TDoc.h"
#include "TModelessDoc.h"

///////////////////////////////////////////////////////////////////
//
// constants
// 
///////////////////////////////////////////////////////////////////

const short iOK = 1;
const short iUserItem = 2;
///////////////////////////////////////////////////////////////////
//
// class declarations
// 
///////////////////////////////////////////////////////////////////

class TModelessApp : public TApp{
  virtual TDoc * MakeDoc(SFReply * reply (SFReply *) nil);
};

class TSampDlg : public TModelessDoc{
  public:
    TSampDlg(OSType creator,SFReply * theReply);

    virtual Boolean InitDoc(void);
  protected:
    virtual void DoItemHit(DialogPtr theDialog, short theItem);
};

///////////////////////////////////////////////////////////////////
//
// main
//
///////////////////////////////////////////////////////////////////
void main(void) (
  // create an instance of TModelessApp
  TModelessApp theApp ;

  // initialize the application
  if(theApp.InitApp()) (
    // open one window to start with,
    // unless we got files from the Finder 
    if(! theApp.OpenDocFromFinder())
      theApp.OpenNewDoc();

    // run the event loop until user quits 
    theApp.EventLoop();

    //now clean up 
    theApp.CleanUp();
}

///////////////////////////////////////////////////////////////////
//
// TModelessApp::MakeDoc
// 
///////////////////////////////////////////////////////////////////
TDoc * TModelessApp::MakeDoc(SFReply *reply) (
  return new TSarnpDlg(GetCreator(),reply);
}

///////////////////////////////////////////////////////////////////
//
// TSarnpDlg: :TSarnpDlg
// 
///////////////////////////////////////////////////////////////////
TSarnpDlg::TSarnpDlg(OSType creator,SFReply * theReply):
  TModelessDoc(creator, theReply) (
}

///////////////////////////////////////////////////////////////////
//
// TSarnpDlg::InitDoc
// 
///////////////////////////////////////////////////////////////////
Boolean TSarnpDlg::InitDoc(void) (
  // install user item proc
  void pascal UserItemProc(WindowPtr theWindow, short theItem);
  Rect theRect; short theType; Handle theItem;

  if(TModelessDoc::InitDoc()) {
    GetDItem((DialogPtr)fDocWindow, iUserItem, &theType, &theItem, &theRect);
    SetDItem((DialogPtr)fDocWindow, iUserItem, theType, (Handle)UserItemProc,
&theRect); 
    return true;
  } else
    return false;
}

///////////////////////////////////////////////////////////////////
//
// TSampDlg::DoItemHit
// 
///////////////////////////////////////////////////////////////////
void TSampDlg::DoItemHit(DialogPtr l*theDialog*l,short theItem) {
  if(theItem == iOK)
    SysBeep(l);
}

///////////////////////////////////////////////////////////////////
//
// UserItemProc
// 
///////////////////////////////////////////////////////////////////

void pascal UserItemProc(WindowPtr theWindow, short theItem) {
  Rect r;
  short theType; Handle theItemH; short width;

  GetDItem((DialogPtr)theWindow, theItem, &theType, &theItemH, &r);
  width = (r.right - r.left)
  EraseRect(&r);
  FrameRect(&r);
  for (short i = width I 2; i > 0; i -= 2){
    InsetRect(&r,2,2); FrameRect(&r);
  }
}
