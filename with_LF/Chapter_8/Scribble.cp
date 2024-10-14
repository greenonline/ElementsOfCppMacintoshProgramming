///////////////////////////////////////////////////////////////////
//
// This file: Scribble.cp
//
// This is the main application object for the Scribble program
//
// © 1990 Dan Weston, All Rights Reserved
// 
///////////////////////////////////////////////////////////////////

#include <Quickdraw.h> 
#include <Windows.h> 
#include <Memory.h> 
#include <Files.h> 
#include <Errors.h>

#include "TApp.h" 
#include "TDoc.h"

///////////////////////////////////////////////////////////////////
//
// constants
// 
///////////////////////////////////////////////////////////////////

const short rPenMenu = 131; 
const short i1X1 = 1;
const short i2X2 = 2;
const short i3X3 = 3;
const short iBlack = 5; 
const short iGray = 6; 
const short iWhite = 7;

static const short kEveryItem = 0;

///////////////////////////////////////////////////////////////////
//
// utility functions
// 
///////////////////////////////////////////////////////////////////

inline short min(short a, short b) {return (a< b ? a : b);}

///////////////////////////////////////////////////////////////////
//
// class declarations
// 
///////////////////////////////////////////////////////////////////
enum penPat{patBlack, patGray, patWhite};

class TScribbleDoc : public TDoc{





private:
  short fPenSize;
  penPat fPattern;
  PicHandle fPic;

public:
  TScribbleDoc(OSType theCreator = '????', SFReply *reply= (SFReply *)nil);


  // this method does the doodling
  virtual void DoContent(EventRecord *theEvent);

  // take care of our document-specific menus
  virtual void AdjustDocMenus(void);
  virtual Boolean DoDocMenuCommand(short menuID, short menuItem);

protected:
  // draw the picture during updates 
  virtual void Draw(Rect *r);

public:

  // make this do nothing so that grow box isn't drawn 
  Virtual void DoDrawGrowicon(void) {};

  // This is the file type of the document 
  virtual OSType GetDocType() {return 'SPCT';}

  // file related methods
  virtual Boolean ReadDocFile(short refNum);
  virtual Boolean WriteDocFile(short refNum);
  virtual Boolean CanSaveAs(void) {return true;}

  // override these methods to fiddle with pen menu
  // so that it is enabled when a scribble doc is active
  // and disabled when a scribble doc is disabled or close
  virtual void Activate(void);
  virtual void Deactivate(void);
  virtual Boolean DoClose(void);

protected:
  // new method to actually enable and disable pen menu
  virtual void TogglePenMenu(Boolean enable);

  // new methods related to pen menu
  void SetPenSize(short p) {fPenSize = p;}
  void SetPenPat(penPat p) {fPattern = p;} 
  short GetPenSize(void) {return fPenSize;} 
  penPat GetPenPat(void) {return fPattern;}

};


class TScribbleApp : public TApp{ 

protected:
  // make our kind of document
  virtual TDoc * MakeDoc(SFReply *reply= (SFReply *)nil);

  // yes, we can open old documents
  virtual Boolean CanOpen(void) {return true;}

  // file info for SFGetFile and CreateFile
  virtual OSType GetCreator(void){return 'SCBL';}
  virtual int GetNumFileTypes(void) {return 1;};
  virtual SFTypeList GetFileTypesList(void);
};
///////////////////////////////////////////////////////////////////
//
// Globals
// 
///////////////////////////////////////////////////////////////////


SFTypeList gtheTypes = {'SPCT'};

///////////////////////////////////////////////////////////////////
//
// main
// 
///////////////////////////////////////////////////////////////////
 

void main(void) {
  TScribbleApp theApp;

  // initialize the application 
  if(theApp.InitApp()) {
    // open one window to start with
    if(! theApp.OpenDocFromFinder())
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
// TScribbleApp::MakeDoc
// 
///////////////////////////////////////////////////////////////////
TDoc * TScribbleApp::MakeDoc(SFReply *reply) {
  return new TScribbleDoc(GetCreator(), reply);
}

///////////////////////////////////////////////////////////////////
//
// TScribbleApp::GetFileTypesList
// 
///////////////////////////////////////////////////////////////////
SFTypeList TScribbleApp::GetFileTypesList(void) {
  return gtheTypes;
}

///////////////////////////////////////////////////////////////////
//
// TScribbleDoc: :DoDocMenuCommand
// 
///////////////////////////////////////////////////////////////////
Boolean TScribbleDoc::DoDocMenuCommand(short menuID, short menuItem) {
  if( menuID == rPenMenu) { 
    switch ( menuItem ) {

      case i1X1:
        SetPenSize(1);
        break;
      case i2X2:
        SetPenSize(2);
        break;
      case i3X3:
        SetPenSize(3);
        break; 
      case iBlack:
        SetPenPat(patBlack)
        break; 
      case iGray:
        SetPenPat(patGray);
        break; 
      case iWhite:
        SetPenPat(patWhite)
        break;
      default:
        return false; // this should never happen
    }
    // tell the app that we handled this menu item 
    return true;
  } else {
    // its not one of our menus, give the parent class a chance 
    return TDoc::DoDocMenuCommand(menuID, menuItem);
  }
}

///////////////////////////////////////////////////////////////////
//
// TScribbleDoc::AdjustDocMenus
// 
///////////////////////////////////////////////////////////////////
void TScribbleDoc::AdjustDocMenus(void) {
  // Do the pen menu
  MenuHandle menu= GetMHandle(rPenMenu);

  Checkitem(menu,ilXl,GetPenSize() l); 
  Checkitem(menu,i2X2,GetPenSize() 2); 
  Checkitem(menu,i3X3,GetPenSize() 3); 
  Checkitem(menu,iBlack,GetPenPat() == patBlack); 
  Checkitem(menu,iGray,GetPenPat() == patGray); 
  Checkitem(menu,iWhite,GetPenPat() == patWhite);

  // now let the parent class have a shot at the menus 
  TDoc::AdjustDocMenus();
}

///////////////////////////////////////////////////////////////////
//
// TScribbleDoc::TogglePenMenu
//
///////////////////////////////////////////////////////////////////
void TScribbleDoc::TogglePenMenu(Boolean enable) {
  MenuHandle menu= GetMHandle(rPenMenu);

  SetMenuAbility(menu,kEveryitem,enable);
  DrawMenuBar();
}

///////////////////////////////////////////////////////////////////
//
// TScribbleDoc::Activate
// 
///////////////////////////////////////////////////////////////////
void TScribbleDoc::Activate(void) {
  TDoc::Activate();
  TogglePenMenu(true);
}

///////////////////////////////////////////////////////////////////
//
// TScribbleDoc::Deactivate
// 
///////////////////////////////////////////////////////////////////
void TScribbleDoc::Deactivate(void) {
  TDoc::Deactivate();
  TogglePenMenu(false);
}

///////////////////////////////////////////////////////////////////
//
// TScribbleDoc: :DoClose
// 
///////////////////////////////////////////////////////////////////
Boolean TScribbleDoc::DoClose(void) {
  if(TDoc: :DoClose()) {
    TogglePenMenu(false);
    return true;
  } else
    return false;
}

///////////////////////////////////////////////////////////////////
//
// TScribbleDoc::TScribbleDoc
// 
///////////////////////////////////////////////////////////////////
TScribbleDoc::TScribbleDoc(OSType theCreator, SFReply *reply):TDoc(theCreator, reply) {

  fPenSize = 2; 
  fPattern = patGray;
  fPic = nil;
}

///////////////////////////////////////////////////////////////////
//
// TScribbleDoc::Draw
// 
///////////////////////////////////////////////////////////////////
void TScribbleDoc::Draw(Rect * /*r*/) {
  if(fPic != nil)
    DrawPicture(fPic,&((**fPic) .picFrame));
}

///////////////////////////////////////////////////////////////////
//
// TScribbleDoc::DoContent
// 
///////////////////////////////////////////////////////////////////
void TScribbleDoc::DoContent(EventRecord* theEvent) {
  Point newPoint;

  if (fDocWindow) { 
    SetPort(fDocWindow);
    PenSize(fPenSize,fPenSize);

    if(fPattern == patBlack) PenPat(qd.black);
    if(fPattern == patGray) PenPat (qd.gray);
    if(fPattern == patWhite) PenPat(qd.white);

    GlobalToLocal(&theEvent->where);
    MoveTo(theEvent->where.h, theEvent->where.v);
    do {
      GetMouse(&newPoint);
      LineTo(newPoint.h, newPoint.v);
    } while(StillDown());

    fNeedtoSave true;

    // now take a snap shot of window
    if(fPic != nil)
      KillPicture(fPic);
    fPic = OpenPicture(&fDocWindow->portRect);
    CopyBits(&fDocWindow->portBits,
             &fDocWindow->portBits,
             &fDocWindow->portRect,
             &fDocWindow->portRect,
             srcCopy,
             (RgnHandle)nil);
    ClosePicture();
  }
}

///////////////////////////////////////////////////////////////////
//
// TScribbleDoc::ReadDocFile
// 
///////////////////////////////////////////////////////////////////
Boolean TScribbleDoc::ReadDocFile(short refNum) {
  const short kHAdjust 50;
  const short kWAdjust = 40;

  if(fDocWindow) {
    long !en;
    OSErr err= GetEOF(refNum,&len); Handle thePic = NewHandle(len); 
    if(thePic == nil) {
      ErrorAlert(rDocErrorStrings,sNoMem); return false;
    }

    HLock(thePic);
    err= SetFPos(refNum,fsFromStart,0);
    err= FSRead(refNum,&len, (Ptr)*thePic);
    HUnlock(thePic);
    if(err == noErr) {
      // now make the window the size of the picture 
      Rect r = (**{(PicHandle)thePic)) .picFrame;
      short height = r.bottom - r.top;
      short width = r.right - r.left;
      r = qd.screenBits.bounds;

      height= min(height ,r.bottom - r.top - kHAdjust);
      width= min(width, r.right - r.left - kWAdjust);
      SizeWindow(fDocWindow, width, height, true);
      // set the member to reference Picture
      fPic = (PicHandle)thePic;
      return true;
    } else {
      DisposHandle(thePic); return false;
    }
  }
  // if there ain't no window... return false;
}

///////////////////////////////////////////////////////////////////
//
// TScribbleDoc::WriteDocFile
// 
///////////////////////////////////////////////////////////////////

Boolean TScribbleDoc::WriteDocFile(short refNum) {
  if (fDocWindow) { 
    if(fPic != nil) {
      long len = GetHandleSize((Handle)fPic);
      HLock((Handle)fPic);
      OSErr err= SetFPos(refNum,fsFromStart,0);
      err = FSWrite (refNum, &len, (Ptr) *fPic);
      HUnlock((Handle)fPic);
      if(err == noErr)
        return true;
      else
        return false;
    }
  }
  // if there ain't no window... 
  return false;

}