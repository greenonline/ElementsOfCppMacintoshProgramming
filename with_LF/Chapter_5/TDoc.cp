////////////////////////////////////////////////////////////////////////////////////////////// 
//
// This is the generic document object
//
// © 1990 Dan Weston, All Rights Reserved
// 
////////////////////////////////////////////////////////////////////////////////////////////// 

// Mac Includes 
#include <Types.h> 
#include <Windows.h> 
#include <OSUtils.h> 
#include <ToolUtils.h> 
#include <Dialogs.h> 
#include <Files.h> 
#include <Errors.h> 
#include <SysEqu.h>   // Only in MPW 3.2.3, need to move to MPW 3.5
//#include <Desk.h>  // #Error: include <Menus.h>, <Devices.h>, and <Events.h> instead of <Desk.h>
#include <Menus.h>
#include <Devices.h>
#include <Events.h>

#include "TDoc.h" 
#include "AppDocMenus.h"

// define the segment for the TDoc class 
//#pragma segment DocSeg

////////////////////////////////////////////////////////////////////////////////////////////// 
//
// ErrorAlert
// 
////////////////////////////////////////////////////////////////////////////////////////////// 

void ErrorAlert(short stringsID, short theError) {

  short result;
  Str255 theStr;
  Str255 nullStr;
  *nullStr = 0;

  GetIndString(theStr, stringsID, theError);
  ParamText(theStr, nullStr, nullStr, nullStr);
  result = CautionAlert(rErrorAlert, (ModalFilterProcPtr)nil);
}

////////////////////////////////////////////////////////////////////////////////////////////// 
//
// TDoc::TDoc
// 
////////////////////////////////////////////////////////////////////////////////////////////// 

TDoc::TDoc(OSType theCreator, SFReply * SFInfo) {

  fCreator = theCreator;
  fNeedtoSave = false;
  fDocWindow = nil;
  fRefNum = 0;
  fFileOpen = false;

  if(SFInfo !=nil) {
    fNeedtoSaveAs = false;
    fFileInfo = *SFInfo;
  } else {
    fNeedtoSaveAs = true; 
    fFileInfo.good = false;
  }
}

////////////////////////////////////////////////////////////////////////////////////////////// 
//
// TDoc::~TDoc
// 
////////////////////////////////////////////////////////////////////////////////////////////// 

TDoc::~TDoc(void) {
  if(fDocWindow != nil) {
    DisposeWindow(fDocWindow);
    fDocWindow = nil;
  }
}

////////////////////////////////////////////////////////////////////////////////////////////// 
//
// TDoc::GetWinID
// 
////////////////////////////////////////////////////////////////////////////////////////////// 

short TDoc::GetWinID(void) {
  return rGenericDoc;
}

////////////////////////////////////////////////////////////////////////////////////////////// 
//
// TDoc: :MakeWindow
// 
////////////////////////////////////////////////////////////////////////////////////////////// 

Boolean TDoc::MakeWindow(Boolean colorWindow) {
  if(colorWindow)
    fDocWindow = GetNewCWindow(GetWinID(),nil, (WindowPtr)-1);
  else
    fDocWindow = GetNewWindow(GetWinID(),nil, (WindowPtr)-1);
  return (fDocWindow != nil);
}

////////////////////////////////////////////////////////////////////////////////////////////// 
//
// TDoc::DoActivate
// 
////////////////////////////////////////////////////////////////////////////////////////////// 

void TDoc::DoActivate(EventRecord* theEvent) {
  Boolean activating = theEvent->modifiers & activeFlag;

  // no need to activate if no window
  if(fDocWindow ==nil)
    return;

  SetPort(fDocWindow); DoDrawGrowIcon();

  if(activating)
    Activate();
  else
    Deactivate();
}

////////////////////////////////////////////////////////////////////////////////////////////// 
//
// TDoc: :DoTheUpdate
// 
////////////////////////////////////////////////////////////////////////////////////////////// 

void TDoc::DoTheUpdate(EventRecord* /*theEvent*/) {
  if(fDocWindow != nil) {
    SetPort(fDocWindow); BeginUpdate(fDocWindow);
    Rect r = (**(fDocWindow->visRgn)).rgnBBox;
    Draw(&r);
    DoDrawGrowIcon();
    EndUpdate(fDocWindow);
  }
}

////////////////////////////////////////////////////////////////////////////////////////////// 
//
// TDoc::Draw
// 
////////////////////////////////////////////////////////////////////////////////////////////// 
void TDoc::Draw(Rect *r) {
  EraseRect(r);
}

////////////////////////////////////////////////////////////////////////////////////////////// 
//
// TDoc::DoGrow
// 
////////////////////////////////////////////////////////////////////////////////////////////// 
void TDoc::DoGrow(EventRecord* theEvent) {
  long result;

  // use desktop gray region as grow limits 
  RgnHandle theGrayRgn = GetGrayRgn();

  Rect r = (**theGrayRgn) .rgnBBox;
  r.top = GetMinHeight();
  r.left = GetMinWidth();

  SetPort(fDocWindow);
  result= GrowWindow(fDocWindow, theEvent->where, &r);
  if ( result != 0 ){
    // invalidate the old scroll bar areas r = fDocWindow->portRect;
    r.left = r.right - kScrollBarPos;
    InvalRect(&r);
    r = fDocWindow->portRect;
    r.top = r.bottom - kScrollBarPos;
    InvalRect(&r);

    // now make the window the new size
    SizeWindow(fDocWindow, LoWrd(result), HiWrd(result), true);

    // invalidate the new scroll bar areas r = fDocWindow->portRect;
    r.left = r.right - kScrollBarPos; InvalRect(&r);
    r = fDocWindow->portRect;
    r.top = r.bottom - kScrollBarPos; InvalRect(&r);
  }
}


////////////////////////////////////////////////////////////////////////////////////////////// 
//
// TDoc::DoZoom
// 
////////////////////////////////////////////////////////////////////////////////////////////// 
void TDoc::DoZoom(short partCode) {
  if(fDocWindow) {
    SetPort(fDocWindow);
    EraseRect(&fDocWindow->portRect);
    ZoomWindow(fDocWindow, partCode, fDocWindow == FrontWindow());

    // invalidate the whole content
    InvalRect(&fDocWindow->portRect);
  }
}



////////////////////////////////////////////////////////////////////////////////////////////// 
//
// TDoc::DoDrag
// 
////////////////////////////////////////////////////////////////////////////////////////////// 
void TDoc::DoDrag(EventRecord* theEvent) {
  // use desktop gray region as drag limits
  RgnHandle theGrayRgn = GetGrayRgn();
  Rect r = (**theGrayRgn) .rgnBBox;

  if(fDocWindow)
    DragWindow(fDocWindow, theEvent->where, &r);
}

////////////////////////////////////////////////////////////////////////////////////////////// 
//
// TDoc::DoDocMenuCommand
// 
////////////////////////////////////////////////////////////////////////////////////////////// 

Boolean TDoc::DoDocMenuCommand(short menuID, short menuItem) {

  switch ( menuID ) {
    case rFileMenu:
      switch ( menuItem ) {
        case iSave:
          DoSave();
          break;
        case iSaveAs:
          DoSaveAs();
          break;
        case iPageSetup:
          DoPageSetup();
          break;
        case iPrint:
          DoPrint();
          break;
        default:
          // we didn't handle command
          return false;
      } // end menuItem switch
      return true; // we handled this command
    case rEdit:
      if (!SystemEdit(menuItem-1)){
        switch ( menuItem ) {
          case iClear:
            DoClear();
            break;
          case iSelectAll:
            DoSelectAll () ;
            break;


          default:
            // we didn't handle command
            return false;
      } // end menuItem switch
      return true; // we handled this command
    } else
      return true; // SystemEdit handled command

  } // end menuID switch
  // we didn't handle command
  return false;
}

////////////////////////////////////////////////////////////////////////////////////////////// 
//
// TDoc::AdjustDocMenus
// 
////////////////////////////////////////////////////////////////////////////////////////////// 
void TDoc::AdjustDocMenus(void) {

  MenuHandle menu;

// Do the document's portion of the file menu 
  //menu = GetMHandle(rFileMenu);
  menu = GetMenuHandle(rFileMenu);
  SetMenuAbility(menu, iClose, CanClose());
  SetMenuAbility(menu, iSave, CanSave()); 
  SetMenuAbility(menu, iSaveAs, CanSaveAs());
  SetMenuAbility(menu, iPageSetup, CanPageSetup());
  SetMenuAbility(menu, iPrint, CanPrint());

// now the edit menu, App handles Paste Item
  //menu = GetMHandle(rEdit);
  menu = GetMenuHandle(rEdit);
  SetMenuAbility(menu, iUndo, CanUndo());
  SetMenuAbility(menu, iCut, HaveSelection() );
  SetMenuAbility(menu, iCopy, HaveSelection());
  SetMenuAbility(menu, iClear, HaveSelection());
  SetMenuAbility(menu, iSelectAll, CanSelectAll());
}

////////////////////////////////////////////////////////////////////////////////////////////// 
//
// TDoc: :OpenDocFile
// 
////////////////////////////////////////////////////////////////////////////////////////////// 
short TDoc::OpenDocFile(SFReply *reply) {

  short refnum;
  //OSErr err = FSOpen((Str255)reply->fName, reply->vRefNum, &refnum); 
  OSErr err = FSOpen(reply->fName, reply->vRefNum, &refnum); 
  switch(err) {
    case fnfErr: // file not found, create it
      //err = Create((Str255)reply->fName, reply->vRefNum, fCreator, GetDocType());
      err = Create(reply->fName, reply->vRefNum, fCreator, GetDocType());
      if(err == noErr) {
        //err = FSOpen( (Str255)reply->fName, reply->vRefNum, &refnum);
        err = FSOpen( reply->fName, reply->vRefNum, &refnum);

        if(err != noErr) 
          return 0;
      } else
        return 0;

      // if open was successful, fall through 
      // to next case

    case noErr: // file opened OK 
      fFileOpen = true;
      fRefNum = refnum;
      return refnum;
    case opWrErr:
      ErrorAlert(rDocErrorStrings, sFileOpen);
      return 0;
    default:
      ErrorAlert(rDocErrorStrings, sUnknownErr);
      return 0;
  }
}

////////////////////////////////////////////////////////////////////////////////////////////// 
//
// TDoc::CloseDocFile
// 
////////////////////////////////////////////////////////////////////////////////////////////// 
void TDoc::CloseDocFile(short refNum) {
  OSErr err= FSClose(refNum);
}

////////////////////////////////////////////////////////////////////////////////////////////// 
//
// TDoc: :DoClose
// 
////////////////////////////////////////////////////////////////////////////////////////////// 
Boolean TDoc::DoClose(void) {
  // you could give the user a chance to save if necessary
  // and possibly cancel the close operation

  if(fNeedtoSave) {
    // ask if they want to save it
    short saveit = WantToSave();
    if(saveit == iCancel)
      return false;
    if(saveit == iYes) {
      // User can cancel save at this point too
     if (! DoSave())
       return false;
    }
  }

  //close the file
  if(fFileOpen )
    CloseDocFile(fRefNum);

  // if all goes well, return true
  return true;
}

////////////////////////////////////////////////////////////////////////////////////////////// 
//
// TDoc::WantToSave
// 
////////////////////////////////////////////////////////////////////////////////////////////// 
short TDoc::WantToSave(void) {

  Str255 title;
  Str255 nullStr;
  *nullStr = 0;

  if (fDocWindow) {
    GetWTitle(fDocWindow ,title);
    ParamText(title, nullStr, nullStr, nullStr);
  } else
    ParamText(nullStr, nullStr, nullStr, nullStr);
  return Alert(rWantToSave, (ModalFilterProcPtr)nil);
}

////////////////////////////////////////////////////////////////////////////////////////////// 
//
// TDoc::DoSaveAs
// 
////////////////////////////////////////////////////////////////////////////////////////////// 
Boolean TDoc::DoSaveAs(void) {

  SFReply whereToSave;
  Point p;
  Str255 title;

  GetWTitle(fDocWindow , title);

  p.h = 100; 
  p.v = 100;
  SFPutFile (p,
            "\pSave file as ... ",
            title,
            (DlgHookProcPtr)nil,
            &whereToSave);

  if(! whereToSave.good) {
    // the user canceled the SaveAs return false;
  } else {
    fFileInfo = whereToSave;
    fRefNum = OpenDocFile(&whereToSave);
    if(fRefNum == 0) {
      // file didn't open
      return false;
    } else {
      fFileOpen = true;
      if(! WriteDocFile(fRefNum)) {
        // write was unsuccessful
        return false;
      } else {
        fNeedtoSave = false;
        fNeedtoSaveAs = false;
        SetDocWindowTitle(whereToSave.fName);
      }
    }
  }
  return true; // passed every test for success
}

////////////////////////////////////////////////////////////////////////////////////////////// 
//
// TDoc: : DoSave
// 
////////////////////////////////////////////////////////////////////////////////////////////// 
Boolean TDoc::DoSave(void) {

  if(fNeedtoSaveAs) return DoSaveAs();

  if(WriteDocFile(fRefNum)) {
    fNeedtoSave = false;
    fNeedtoSaveAs = false;
    return true;
  } else
    return false;
}
