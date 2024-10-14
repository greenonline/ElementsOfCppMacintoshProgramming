//////////////////////////////////////////////////////////////////////////////////////////
//
// This is the generic application object
//
//© 1990 Dan Weston, All Rights Reserved
// 
//////////////////////////////////////////////////////////////////////////////////////////

// Mac Includes 
#include <Types.h> 
#include <Events.h> 
#include <Windows.h> 
#include <Menus.h> 
#include <Dialogs.h> 
//#include <Desk.h>  // #Error: include <Menus.h>, <Devices.h>, and <Events.h> instead of <Desk.h>
#include <Menus.h>
#include <Devices.h>
#include <Events.h>

#include <ToolUtils.h> 
#include <Fonts.h> 
#include <Memory.h> 
#include <OSUtils.h> 
#include <Traps.h> 
#include <SegLoad.h> 
#include <Scrap.h>

// our includes
#include "TApp.h" 
#include "AppDocMenus.h"

// define the segment for the TApp class 
#pragma segmenTAppSeg

//////////////////////////////////////////////////////////////////////////////////////////
//
// constants
// 
//////////////////////////////////////////////////////////////////////////////////////////
const short rAboutID = 128;    /* about alert */

const short kOSEvent = app4Evt;
const short kSuspendResumeMessage = 0x01; 
const short kClipConvertMask = 0x02; 
const short kResumeMask = 0x01;
const short kMouseMovedMessage = 0xFA;

const short kStagger = 20; 
const short kHPos = 20; 
const short kVPos = 50;

//////////////////////////////////////////////////////////////////////////////////////////
//
// TList::TList
// 
//////////////////////////////////////////////////////////////////////////////////////////
TList::TList(void) {
  fLink = nil; fNumItems = 0;
}

//////////////////////////////////////////////////////////////////////////////////////////
//
// TList::AddItem
// 
//////////////////////////////////////////////////////////////////////////////////////////
void TList::AddItem(void* item) {
  TLink* temp;

  temp= new TLink(fLink,item);
  fLink = temp;
  fNumItems++;
}

//////////////////////////////////////////////////////////////////////////////////////////
//
// TList::RemoveItem
// 
//////////////////////////////////////////////////////////////////////////////////////////
void TList::RemoveItem(void* item) {
  TLink* temp; TLink* last;

  last = nil;
  for (temp = fLink; temp != nil; temp = temp->GetNext())
    if (temp->GetItem() == item) {
      // if first item in list, just set first
      if (last == nil)
        fLink = temp->GetNext();
      else
        last->SetNext(temp->GetNext());
      delete temp;
      fNumItems--;
      return;
    } else
      last = temp;
}

//////////////////////////////////////////////////////////////////////////////////////////
//
// TDocList::FindDoc
// find the TDoc associated with the window
// 
//////////////////////////////////////////////////////////////////////////////////////////

TDoc* TDocList::FindDoc(WindowPtr window) { 
  TLink* temp;
  TDoc* tDoc;

  for (temp= fLink; temp !=nil; temp= temp->GetNext()) {
    tDoc = (TDoc*)temp->GetItem();
    if (tDoc->GetDocWindow() == window)
      return tDoc; 
  }
  return nil;
}

//////////////////////////////////////////////////////////////////////////////////////////
//
// TIterator::Next
// 
//////////////////////////////////////////////////////////////////////////////////////////
void* TIterator::Next(void) {
  TLink* link = fCurLink;

  if (fCurLink) {
    fCurLink = fCurLink->GetNext();
    return (link->GetItem());
  } else
    return nil;
}

//////////////////////////////////////////////////////////////////////////////////////////
//
// TApp::TApp
// 
//////////////////////////////////////////////////////////////////////////////////////////
TApp::TApp(void) {
  // initialize our class variables fCurDoc = nil;
  fDone = false;
  finBackground = false;
  fClipData = nil; fClipType = '????'; fDAonTop = false; fLastScrapCount = 0;

  // initialize Mac Toolbox components
  InitGraf((Ptr) &qd.thePort);
  InitFonts () ;
  InitWindows () ;
  InitMenus();
  TEInit();
  InitDialogs((ResumeProcPtr) nil);
  InitCursor();
  (void) SysEnvirons(curSysEnvVers, &fenvRec);

  // expand the heap so new code segments load at the top
  MaxApplZone();

  // allocate an empty document list
  fDocList = new TDocList;

  // check to see if WaitNextEvent is implemented 
  fHaveWaitNextEvent = TrapAvailable(_WaitNextEvent, ToolTrap);

  // read menus into menu bar
  Handle menuBar = GetNewMBar(rMenuBarID); // install menus
  SetMenuBar(menuBar); 
  //DisposHandle(menuBar);
  DisposeHandle(menuBar);
  // add DA names to Apple menu AddResMenu(GetMHandle(rAppleMenu), 'DRVR');
  DrawMenuBar();
}

////////////////////////////////////////////////////////////////////////////////////////// 
//
// TApp::InitApp
// 
//////////////////////////////////////////////////////////////////////////////////////////
Boolean TApp::InitApp(void) {
  GetClipFromSystem();
  return true;
}

//////////////////////////////////////////////////////////////////////////////////////////
//
// TApp::CleanUp
// 
//////////////////////////////////////////////////////////////////////////////////////////
 
void TApp::CleanUp(void) {
  GiveClipToSystem();
}

//////////////////////////////////////////////////////////////////////////////////////////
//
// TApp::CheckForDASwitch
//
//////////////////////////////////////////////////////////////////////////////////////////

void TApp::CheckForDASwitch(WindowPtr theFrontWindow) {

  if(theFrontWindow == nil) return;

  Boolean DAWindowOnTop;
  DAWindowOnTop = ((WindowPeek)theFrontWindow)->windowKind < 0;
  // if the state has changed since we last checked it, then 
  // do clipboard conversion
  if(DAWindowOnTop != fDAonTop) {
    fDAonTop = DAWindowOnTop;
    if(DAWindowOnTop) {
      // DA is becoming active, give up the clipboard
      GiveClipToSystem();
    } else {
      // DA is becoming inactive, reclaim clip if necessary if(ClipHasChanged())
      GetClipFromSystem();
    }
  }
}

//////////////////////////////////////////////////////////////////////////////////////////
//
// TApp::ClipHasChanged
// 
//////////////////////////////////////////////////////////////////////////////////////////
Boolean TApp::ClipHasChanged(void) {
  PScrapStuff scrapinfo = InfoScrap();
  return (scrapinfo->scrapCount != fLastScrapCount);
}

//////////////////////////////////////////////////////////////////////////////////////////
//
// TApp::GiveClipToSystem
// 
//////////////////////////////////////////////////////////////////////////////////////////
void TApp::GiveClipToSystem(void) {
  if(fClipData !=nil) {
    long result= ZeroScrap(); 
    if(result != noErr)
      return;
    long size= GetHandleSize(fClipData);

    HLock(fClipData);
    result= PutScrap(size,fClipType,*fClipData);
    HUnlock(fClipData);
  }
  // update our scrapcount field so we can tell if scrap 
  // has changed later on
  PScrapStuff scrapinfo = InfoScrap();
  fLastScrapCount = scrapinfo->scrapCount;
}


//////////////////////////////////////////////////////////////////////////////////////////
//
// TApp::GetClipFromSystem
// 
//////////////////////////////////////////////////////////////////////////////////////////





void TApp::GetClipFromSystem(void){
  long offset;
  Handle newData = NewHandle(0);
  OSType newType = CanAcceptClipType();
  long result= GetScrap(newData, newType, &offset);
  if(result > 0) {


    if(fClipData != nil)
      //DisposHandle(fClipData);
      DisposeHandle(fClipData);
    fClipData = newData;
    fClipType = newType;
  }
}

//////////////////////////////////////////////////////////////////////////////////////////
//
// TApp::CanAcceptClipType
// 
//////////////////////////////////////////////////////////////////////////////////////////
OSType TApp::CanAcceptClipType(void) {
  return '????';
}

//////////////////////////////////////////////////////////////////////////////////////////
//
// TApp::EventLoop
// 
//////////////////////////////////////////////////////////////////////////////////////////
void TApp::EventLoop(void) {
  int gotEvent;
  EventRecord theEvent; WindowPtr theFrontWindow;



  while (fDone == false) {
    theFrontWindow = FrontWindow();

    // find out if a DA is becoming active or inactive
    CheckForDASwitch(theFrontWindow);

    // see if window belongs to a document,
    // FindDoc will return nil if not one of our windows 
    fCurDoc = fDocList->FindDoc(theFrontWindow);

    if (fHaveWaitNextEvent)
      gotEvent = WaitNextEvent(everyEvent,&theEvent, SleepVal(), (RgnHandle) nil);
    else {
      SystemTask();
      gotEvent = GetNextEvent(everyEvent, &theEvent);
    }

    // make sure we got a real event 
    if (gotEvent == false) {
      AppIdle();
      continue;
    }

    switch (theEvent.what) {
      case nullEvent:


        AppIdle(); break;
      case mouseDown: MouseDown(&theEvent); break;
      case mouseUp : MouseUp(&theEvent); break;
      case keyDown :
      case autoKey : KeyDown(&theEvent); break;
      case updateEvt : UpdateEvt(&theEvent); break;
      case diskEvt : DiskEvt(&theEvent); break;
      case activateEvt : ActivateEvt(&theEvent); break;
      case kOSEvent :
        OSEvent(&theEvent);
        break;
      default :
        break;
    } // end switch
  }   // end while
}

//////////////////////////////////////////////////////////////////////////////////////////
//
// TApp::AddDocument
// 
//////////////////////////////////////////////////////////////////////////////////////////
void TApp::AddDocument(TDoc *theDoc) {
  fDocList->AddItem(theDoc);
  fCurDoc = theDoc;
}

//////////////////////////////////////////////////////////////////////////////////////////
//
// TApp::OpenDocFromFinder
// 
//////////////////////////////////////////////////////////////////////////////////////////
Boolean TApp::OpenDocFromFinder(void) {
  short message;
  short count;
  AppFile theApp;
  SFReply reply;
  Boolean fileOpened = false;

  // see if there are any files to be opened or printed CountAppFiles(&message,&count};
  if(count == 0)
    return false;

  for(short i = count;i;i--) { 
    GetAppFiles(i,&theApp);
    // convert theApp to an SFReply
    reply.good = true;
    reply.fType = theApp.fType;
    reply.vRefNum = theApp.vRefNum;
    reply.version = theApp.versNum; 
    unsigned char strLen = theApp.fName[0]; 
    for(short j = 0; j <= strLen; j++)
      reply.fName[j] = theApp.fName[j];

    // check here to see if file is an acceptable type
    if(AcceptableFileType(reply.fType))
      // now create the document and open the file
      if(InitOldDoc(&reply))
        fileOpened = true;
  }
  return fileOpened;
}

//////////////////////////////////////////////////////////////////////////////////////////
//
// TApp::OpenNewDoc
// Creates a new document object, staggers it, and adds it to doclist
// 
//////////////////////////////////////////////////////////////////////////////////////////
void TApp::OpenNewDoc(void) {
  TDoc * newDoc = MakeDoc();

  if(newDoc) {
    if((newDoc->MakeWindow(fenvRec.hasColorQD)) && (newDoc->InitDoc())) {
      short numDocs = fDocList->NumItems() ;
      newDoc->MoveDocWindow(kHPos + (numDocs * kStagger), kVPos + (numDocs * kStagger));

      newDoc->ShowDocWindow();
      AddDocument(newDoc); 
    } else {
      // MakeWindow or InitDoc failed, but doc created
      delete(newDoc);} 
  }
}

//////////////////////////////////////////////////////////////////////////////////////////
//
// TApp::OpenOldDoc
// 
//////////////////////////////////////////////////////////////////////////////////////////
void TApp::OpenOldDoc(void) {

  SFReply reply;
  Point p;

  p.h = 100;
  p.v = 100;
  SFGetFile(p,
            //(Str255)"",
            "",
            (FileFilterProcPtr)nil, GetNumFileTypes(), GetFileTypesList(),
            (DlgHookProcPtr)nil, &reply);
  // don't go on if user cancels dialog 
  if(!reply.good)
    return;

  (void)InitOldDoc(&reply);
}

//////////////////////////////////////////////////////////////////////////////////////////
//
// TApp::InitOldDoc
//
// Creates a new document object, reads in data for it, 
// sets the window title to file name, staggers it,
// and adds it to doclist
// This routine is shared by both OpenDocFromFinder
// and OpenOldDoc
// 
//////////////////////////////////////////////////////////////////////////////////////////
Boolean TApp::InitOldDoc(SFReply *reply) {
  TDoc * newDoc = MakeDoc(reply); 

  if (newDoc) {
    if((newDoc->MakeWindow(fenvRec.hasColorQD)) && (newDoc->InitDoc())) {
      short numDocs = fDocList->NumItems() ;
      newDoc->MoveDocWindow(kHPos + (numDocs * kStagger), kVPos + (numDocs * kStagger));
      //newDoc->SetDocWindowTitle((Str255)reply->fName);
      newDoc->SetDocWindowTitle(reply->fName);
      short refNum = newDoc->OpenDocFile(reply);

      if(refNum != 0){
        if(newDoc->ReadDocFile(refNum)) {
          newDoc->ShowDocWindow();
          AddDocument(newDoc);
        } else {
          // open was successful, but read failed 
          newDoc->CloseDocFile(refNum);
          delete(newDoc);
          return false;
        }
      } else {
        // file not opened successfully, but doc created
        delete(newDoc);
        return false;
      }
    } else {
      // MakeWindow or InitDoc failed, but doc created delete(newDoc);
      return false;
    }
  } else {
    // document not created
    return false;
  }
  // if we get this far, all went well
  return true;
}


//////////////////////////////////////////////////////////////////////////////////////////
//
// TApp: :MakeDoc
// Override this function to make the type of document
// that your application uses
// 
//////////////////////////////////////////////////////////////////////////////////////////
TDoc * TApp::MakeDoc(SFReply *reply) {
  return new TDoc(GetCreator(),reply);
}

//////////////////////////////////////////////////////////////////////////////////////////
//
// TApp: :AcceptableFileType
// 
//////////////////////////////////////////////////////////////////////////////////////////
Boolean TApp::AcceptableFileType(OSType theType) {
  int numTypes = GetNumFileTypes();
  OSType *theTypeList = (OSType *)GetFileTypesList();
  if ( (numTypes == 0) || (theTypeList == nil))
    return true;

  for (int i = 0;i < numTypes; i++) { 
    if(theType == *theTypeList++)
      return true;
  }
 return false;
}

//////////////////////////////////////////////////////////////////////////////////////////
//
// TApp: :OSEvent
// 
//////////////////////////////////////////////////////////////////////////////////////////
void TApp::OSEvent(EventRecord * theEvent) {
  Boolean doConvert;
  unsigned char evType;

  // is it a multifinder event?



  evType = (unsigned char) (theEvent->message >> 24) & 0x00ff;
  switch (evType) { // high byte of message is type of event
    case kMouseMovedMessage:
      AppIdle(); // mouse-moved is also an idle event break;
    case kSuspendResumeMessage:
      doConvert = (theEvent->message & kClipConvertMask) != 0;
      finBackground = (theEvent->message & kResumeMask) == 0;
      if (finBackground)
        DoSuspend(theEvent,doConvert);
      else
        DoResume(theEvent,doConvert); break;
  }
}

//////////////////////////////////////////////////////////////////////////////////////////
//
// TApp::MouseDown
// 
//////////////////////////////////////////////////////////////////////////////////////////
void TApp::MouseDown(EventRecord * theEvent) {
  WindowPtr theWindow;
  short partCode = FindWindow(theEvent->where, &theWindow);
  TDoc * tempDoc = fDocList->FindDoc(theWindow);

  switch (partCode) {
    case inSysWindow:
      SystemClick(theEvent,theWindow);
      break;
    case inMenuBar :
      AdjustMenus();
      long mResult = MenuSelect(theEvent->where); if (HiWrd(mResult) != 0) {
      DoMenuCommand(HiWrd(mResult),LoWrd(mResult)); HiliteMenu(0);
      }
      break;
    case inGoAway:
      if (TrackGoAway(theWindow, theEvent->where)) CloseADoc(tempDoc);
      break;
    case inDrag :
      if(tempDoc !=nil) tempDoc->DoDrag(theEvent);
      break;
    case inGrow :
      if (tempDoc != nil)
        tempDoc->DoGrow(theEvent); break;
    case inZoomIn : case inZoomOut :
      if ((TrackBox(theWindow, theEvent->where, partCode)) && (tempDoc !=nil))
        tempDoc->DoZoom(partCode); break;
    case inContent :
      if(theWindow != FrontWindow())
        SelectWindow(theWindow);
      else
        if(tempDoc !=nil) tempDoc->DoContent(theEvent);
      break;
  }
}

//////////////////////////////////////////////////////////////////////////////////////////
//
// TApp::KeyDown
// 
//////////////////////////////////////////////////////////////////////////////////////////
void TApp::KeyDown(EventRecord * theEvent) {
  char key;
  long mResult;

  key= (char) (theEvent->message & charCodeMask);
  if ((theEvent->modifiers & cmdKey) && (theEvent->what == keyDown)) {
    // only do command keys if we are not autokeying
    AdjustMenus(); // make sure menus are up to date 
    mResult = MenuKey(key);

    // if it wasn't a menu key, pass it through 
    if (HiWrd(mResult) != 0) {
      DoMenuCommand(HiWrd(mResult), LoWrd(mResult)); HiliteMenu(0);
      return;
    } else {
      if (fCurDoc != nil)
        fCurDoc->DoKeyDown(theEvent);
    }
  }
}

//////////////////////////////////////////////////////////////////////////////////////////
//
// TApp::ActivateEvt
// 
//////////////////////////////////////////////////////////////////////////////////////////
void TApp::ActivateEvt(EventRecord * theEvent) {





  WindowPtr theWindow;

  // event record contains window ptr
  theWindow = (WindowPtr) theEvent->message;

  // see if window belongs to a document
  TDoc *tempDoc = fDocList->FindDoc(theWindow);
  if (tempDoc != nil)
    tempDoc->DoActivate(theEvent );
}

//////////////////////////////////////////////////////////////////////////////////////////
//
// TApp::UpdateEvt
// 
//////////////////////////////////////////////////////////////////////////////////////////
void TApp::UpdateEvt(EventRecord * theEvent) {
  WindowPtr theWindow;

  // event record contains window ptr theWindow = (WindowPtr) theEvent->message;
  // see if window belongs to a document
  TDoc *tempDoc = fDocList->FindDoc(theWindow);
  if (tempDoc != nil) tempDoc->DoTheUpdate(theEvent);
}

//////////////////////////////////////////////////////////////////////////////////////////
//
// TApp::AppIdle
// 
//////////////////////////////////////////////////////////////////////////////////////////
void TApp::AppIdle(void) {
  if (fCurDoc != nil) fCurDoc->DoIdle();
}

//////////////////////////////////////////////////////////////////////////////////////////
//
// TApp: :DoSuspend
// 
//////////////////////////////////////////////////////////////////////////////////////////
void TApp::DoSuspend(EventRecord * theEvent, Boolean convertClip) {
  if(convertClip)
    GiveClipToSystem();
  if (fCurDoc != nil) {
    // tell DoActivate to deactivate
    theEvent->modifiers &= (!activeFlag);
    fCurDoc->DoActivate(theEvent );
  }
}

//////////////////////////////////////////////////////////////////////////////////////////
//
// TApp::DoResume
// 
//////////////////////////////////////////////////////////////////////////////////////////
void TApp::DoResume(EventRecord * theEvent, Boolean convertClip) {
  if(convertClip)
    GetClipFromSystem();
  if (fCurDoc !=nil){
    // tell DoActivate to activate 
    theEvent->modifiers |= activeFlag;
    fCurDoc->DoActivate(theEvent);
  }
}

//////////////////////////////////////////////////////////////////////////////////////////
//
// TApp::AdjustMenus
// Enable and disable menus based on the current state.
// 
//////////////////////////////////////////////////////////////////////////////////////////
void TApp::AdjustMenus(void) {
  MenuHandle menu;

  // first give the current document a chance to adjust the menus
  if(fCurDoc !=nil)
    fCurDoc->AdjustDocMenus();

  // Now do the file menu
  //menu = GetMHandle(rFileMenu);
  menu = GetMenuHandle(rFileMenu);

  // the app controls whether we can open and new and quit 
  SetMenuAbility(menu, iNew, CanNew());
  SetMenuAbility(menu, iOpen, CanOpen());
  SetMenuAbility(menu, iQuit, CanQuit());
  if ( fCurDoc == nil ){
    // no current doc, disable File menu items 
    // usually handled by the document
    SetMenuAbility(menu, iClose, false);
    SetMenuAbility(menu, iSave, false);
    SetMenuAbility(menu, iSaveAs, false);
    SetMenuAbility(menu, iPageSetup, false);
    SetMenuAbility(menu, iPrint, false);
  }

  // now the edit menu 
  //menu = GetMHandle(rEdit);
  menu = GetMenuHandle(rEdit);
  // if no current doc, then enable edit menu depending 
  // on whether a DA is on top
  if ( fCurDoc == nil ){
    SetMenuAbility(menu, iUndo, fDAonTop);
    SetMenuAbility(menu, iCut, fDAonTop);
    SetMenuAbility(menu, iCopy, fDAonTop);
    SetMenuAbility(menu, iPaste, fDAonTop); 
    SetMenuAbility(menu, iClear, fDAonTop);
    SetMenuAbility(menu, iSelectAll, fDAonTop);
  } else {
    // Paste is the one Edit item that the doc can't
    // set by itself
    SetMenuAbility(menu, iPaste, (fClipData !=nil) && (fCurDoc->CanPaste(fClipType)));
  }
}

//////////////////////////////////////////////////////////////////////////////////////////
//
// TApp::DoMenuCommand
// 
//////////////////////////////////////////////////////////////////////////////////////////
void TApp::DoMenuCommand(short menuID, short menuItem) {
  short itemHit; Str255 daName; short daRefNum;

  // allow the current doc a chance to handle it first 
  if((fCurDoc !=nil) && (fCurDoc->DoDocMenuCommand(menuID, menuItem)))
    return;
  switch ( menuID ) { 
    case rAppleMenu:
      switch ( menuItem ) {
        case iAbout:
          itemHit = Alert(rAboutID, nil);
          break;
        default:
          //GetItem(GetMHandle(rAppleMenu), menuItem, daName);
          //GetItem(GetMenuHandle(rAppleMenu), menuItem, daName);
          GetMenuItemText(GetMenuHandle(rAppleMenu), menuItem, daName);
          daRefNum = OpenDeskAcc(daName);
          break;
      } // end menuItem switch 
      break;
    case rFileMenu:
      switch ( menuItem ) {
        case iNew:
          OpenNewDoc();
          break;
        case iOpen:
          OpenOldDoc();
          break;
       case iClose:
          CloseADoc(fCurDoc);
          break;
       case iQuit:
          Quit();
          break;
      } // end menuItem switch
      break;
    case rEdit:
      if (!SystemEdit(menuItem-1)) {
        switch ( menuItem ) {

          case iUndo: DoUndoCmd(fCurDoc); break;
          case iCut: DoCutCmd(fCurDoc); break;
          case iCopy: DoCopyCmd(fCurDoc); break;
          case iPaste: DoPasteCmd(fCurDoc); break;
        } // end menuItem switch 
      } // end if
      break;
  } // end menuID switch
}

//////////////////////////////////////////////////////////////////////////////////////////
//
// TApp::CloseADoc
// 
//////////////////////////////////////////////////////////////////////////////////////////

Boolean TApp::CloseADoc(TDoc * theDoc) {
  if(theDoc !=nil)
    if(theDoc->DoClose()) {
      fDocList->RemoveItem(theDoc);
      if(theDoc == fCurDoc)
        fCurDoc = nil;
        delete theDoc;
        return true;
    }
  // if we get here, the doc didn't close 
  return false;
}

//////////////////////////////////////////////////////////////////////////////////////////
//
// TApp::Quit
// 
//////////////////////////////////////////////////////////////////////////////////////////
void TApp::Quit(void) {
  TIterator iter(fDocList); TDoc * nextDoc;
  Boolean OKToQuit = true;

  // ask each doc if it is ready to Quit
  // It is possible that the user may cancel 
  // while saving one of these documents,
  // thus aborting the Quit process
  while (nextDoc = (TDoc *)iter.Next())
    if(! CloseADoc(nextDoc)) {
      OKToQuit = false;
      break; // don't continue iterating
    }
  if(OKToQuit)
    ExitLoop ();
}
//////////////////////////////////////////////////////////////////////////////////////////
//
// TApp: :DoUndoCmd
// 
//////////////////////////////////////////////////////////////////////////////////////////
void TApp::DoUndoCmd(TDoc * theDoc) {
  if(theDoc !=nil) theDoc->DoUndo();
}

//////////////////////////////////////////////////////////////////////////////////////////
//
// TApp::DoCutCmd
// 
//////////////////////////////////////////////////////////////////////////////////////////
void TApp::DoCutCmd(TDoc * theDoc) {

  Handle newData; OSType newType;

  if(theDoc != nil) 
    if(theDoc->DoCut(&newData,&newType)) {
      //get rid of old clip data if DoCut succeeds
      if(fClipData != nil)
        //DisposHandle(fClipData);
        DisposeHandle(fClipData);
     fClipData = newData;
     fClipType = newType;
    }
}

//////////////////////////////////////////////////////////////////////////////////////////
//
// TApp::DoCopyCmd
// 
//////////////////////////////////////////////////////////////////////////////////////////
void TApp::DoCopyCmd(TDoc * theDoc) {
  Handle newData; OSType newType;

  if(theDoc != nil)
    if(theDoc->DoCopy(&newData,&newType)) {
      //get rid of old clip data if DoCopy succeeds
      if(fClipData != nil)
        //DisposHandle(fClipData);
        DisposeHandle(fClipData);
      fClipData = newData;
      fClipType = newType;
    }
}

//////////////////////////////////////////////////////////////////////////////////////////
//
// TApp::DoPasteCmd
// 
//////////////////////////////////////////////////////////////////////////////////////////
void TApp::DoPasteCmd(TDoc * theDoc) {
  if(theDoc !=nil)
    theDoc->DoPaste(fClipData, fClipType);
}

//////////////////////////////////////////////////////////////////////////////////////////
//
// TApp::TrapAvailable
// 
//////////////////////////////////////////////////////////////////////////////////////////
Boolean TApp::TrapAvailable(short tNumber,TrapType tType) {
  return NGetTrapAddress(tNumber, tType) != GetTrapAddress(_Unimplemented);
}
