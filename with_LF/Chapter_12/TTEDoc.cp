///////////////////////////////////////////////////////////////////
//
// This is the generic text edit object
//
// © 1990 Dan Weston, All Rights Reserved
// 
///////////////////////////////////////////////////////////////////

// Mac Includes
#include <Types.h> 
#include <Windows.h>
#include <OSUtils .h>
#include <Files.h>
#include <Errors.h>
#include <Memory.h>
#include <SysEqu.h>
#include <ToolUtils.h>

#include "TTEDoc.h"
      


// define the segment for the TEDoc classs 
//#pragma segment TEDocSeg

///////////////////////////////////////////////////////////////////
//
// constants
// 
///////////////////////////////////////////////////////////////////
const short kTEMargin = 4; 
const short kMaxShort = 32767;

///////////////////////////////////////////////////////////////////
//
// TTEDoc::TTEDoc
// 
///////////////////////////////////////////////////////////////////
TTEDoc::TTEDoc(OSType theCreator,SFReply * SFinfo):
  TScrollDoc(theCreator, SFinfo) { fTEHandle = nil;}


///////////////////////////////////////////////////////////////////
//
// TTEDoc::InitDoc
// 
///////////////////////////////////////////////////////////////////
Boolean TTEDoc::InitDoc(void) {
  Rect view, dest;
  if(TScrollDoc::InitDoc()) {
    SetPort(fDocWindow);
    view = dest = fDocWindow->portRect;
    dest.left += kTEMargin;
    dest.top += kTEMargin;
    dest.right = kMaxShort;
    dest.bottom = kMaxShort;
    fTEHandle = TENew(&dest, &view);
    SetTERect();
    TEAutoView(true, fTEHandle);

    // install the click loop procedure 
    SetClikLoop(MyClickLoop, fTEHandle);
  }
  return (fTEHandle !=nil);
}

///////////////////////////////////////////////////////////////////
//
// TTEDoc::~TTEDoc
//
///////////////////////////////////////////////////////////////////
TTEDoc::~TTEDoc(void) {
  if(fTEHandle != nil) {
    TEDispose(fTEHandle);
    fTEHandle = nil;
  }
}

///////////////////////////////////////////////////////////////////
//
// TTEDoc::ScrollContents
//
///////////////////////////////////////////////////////////////////
void TTEDoc::ScrollContents(short dh, short dv) {
  if(fTEHandle != nil)
    TEScroll(dh, dv, fTEHandle);
}

///////////////////////////////////////////////////////////////////
//
// TTEDoc::SetScrollBarValues
// 
///////////////////////////////////////////////////////////////////
void TTEDoc::SetScrollBarValues(void) {

  Rect visible = (**fTEHandle) .viewRect;
  Rect dest = (**fTEHandle) .destRect;

  short vPos = visible.top - dest.top;
  short hPos = visible.left - dest.left;

  FocusOnWindow();
  SetCtlValue(fHorizScrollBar, hPos); 
  SetCtlValue(fVertScrollBar, vPos);
}

///////////////////////////////////////////////////////////////////
//
// TTEDoc::GetContentRect
// 
///////////////////////////////////////////////////////////////////
void TTEDoc::GetContentRect(Rect& r) {

  // ask the base class how big the rect is 
  TScrollDoc::GetContentRect(r);
  // and now take away the TE margins
  r.left += kTEMargin;
  r.top += kTEMargin;
}

///////////////////////////////////////////////////////////////////
//
// TTEDoc::SetTERect
// 
///////////////////////////////////////////////////////////////////
void TTEDoc::SetTERect(void) {

  if(fTEHandle !=nil) {
    // set up the view rect 
    Rect r;
    GetContentRect(r);
    (**fTEHandle) .viewRect = r;
  }
}

///////////////////////////////////////////////////////////////////
//
// TTEDoc::GetVertSize
//
///////////////////////////////////////////////////////////////////
short TTEDoc::GetVertSize(void) {
  return ((**fTEHandle) .nLines * (**fTEHandle) .lineHeight) ;
}

///////////////////////////////////////////////////////////////////
//
// TTEDoc::GetHorizSize
// 
///////////////////////////////////////////////////////////////////
short TTEDoc: :GetHorizSize(void) {
  return (**fTEHandle) .destRect.right - (**fTEHandle) .destRect.left;
}

///////////////////////////////////////////////////////////////////
//
// TTEDoc::GetVertLineScrollAmount
// 
///////////////////////////////////////////////////////////////////
short TTEDoc::GetVertLineScrollAmount(void) {

  if(fTEHandle != nil)
    return (**fTEHandle) .lineHeight;
  else
    return 0;
}

///////////////////////////////////////////////////////////////////
//
// TTEDoc::GetHorizLineScrollAmount
// 
///////////////////////////////////////////////////////////////////
short TTEDoc::GetHorizLineScrollAmount(void) {
  if(fTEHandle != nil)
    return (**fTEHandle) .lineHeight;
  else
    return 0;
}

///////////////////////////////////////////////////////////////////
//
// TTEDoc::AddText
// 
///////////////////////////////////////////////////////////////////
void TTEDoc: :AddText(Ptr text, long len) {
  if(fTEHandle !=nil) {
    TEInsert(text, len, fTEHandle);
    fNeedtoSave = true;
    TESelView(fTEHandle);

    SynchScrollBars();
  }
}



///////////////////////////////////////////////////////////////////
//
// TTEDoc::HaveSelection
// 
///////////////////////////////////////////////////////////////////
Boolean TTEDoc::HaveSelection(void} {

  if(fTEHandle}
    return ((**fTEHandle} .selStart != (**fTEHandle} .selEnd};
  else
    return false;
}

///////////////////////////////////////////////////////////////////
//
// TTEDoc: :DoCut
// 
///////////////////////////////////////////////////////////////////
Boolean TTEDoc::DoCut(Handle *theData,OSType *theType) {

  Boolean result;
  if (result= DoCopy(theData, theType))
    DoClear(}; 
  return result;
}

///////////////////////////////////////////////////////////////////
//
// TTEDoc::DoCopy
//
///////////////////////////////////////////////////////////////////
Boolean TTEDoc::DoCopy(Handle *theData,OSType *theType} {
  if (fTEHandle) {
    // put data on TEScrap 
    TECopy(fTEHandle);

    // set theType 
    *theType = 'TEXT';

    // do this in case we fail 
    *theData = nil;

    // copy the handle to the data 
    Handle TEData = TEScrapHandle();
    OSErr err= HandToHand(&TEData);
    if(err != noErr)
      return false; 
    *theData = TEData;

    return true;
  }
}

///////////////////////////////////////////////////////////////////
//
// TTEDoc::DoPaste
// 
///////////////////////////////////////////////////////////////////
void TTEDoc::DoPaste(Handle theData,OSType theType) {
  if((fTEHandle) && (theType == 'TEXT')) {
    // put data in TEScrap
    long scrapLen = GetHandleSize(theData);
    TESetScrapLen(scrapLen);

    // set low memory TEScrap handle with our data handle 
    Handle * TEScrapHandle = (Handle *) TEScrpHandle; 
    *TEScrapHandle = theData;

    // now go ahead and paste 
    TEPaste(fTEHandle);

    fNeedtoSave = true;
    SynchScrollBars();
  }
}

///////////////////////////////////////////////////////////////////
//
// TTEDoc::DoClear
//
///////////////////////////////////////////////////////////////////
void TTEDoc::DoClear(void) {
  if(fTEHandle) {
    TEDelete(fTEHandle);
    fNeedtoSave = true; 
    SynchScrollBars();
  }
}

///////////////////////////////////////////////////////////////////
//
// TTEDoc::DoSelectAll
// 
///////////////////////////////////////////////////////////////////
void TTEDoc::DoSelectAll(void) {
  if(fTEHandle)
    TESetSelect(0, kMaxShort, fTEHandle);
}

///////////////////////////////////////////////////////////////////
//
// TTEDoc::Activate
// 
///////////////////////////////////////////////////////////////////

void TTEDoc::Activate(void) {
  TScrollDoc::Activate();
  if(fTEHandle)
    TEActivate(fTEHandle);
}

///////////////////////////////////////////////////////////////////
//
// TTEDoc::Deactivate
// 
///////////////////////////////////////////////////////////////////
void TTEDoc::Deactivate(void) {
  TScrollDoc::Deactivate();
  if(fTEHandle)
    TEDeactivate(fTEHandle);
}

///////////////////////////////////////////////////////////////////
//
// TTEDoc: :Draw
// 
///////////////////////////////////////////////////////////////////
void TTEDoc::Draw(Rect *r) {
  if(fTEHandle) {
    EraseRect(r);
    TEUpdate(r, fTEHandle);
  }
}

///////////////////////////////////////////////////////////////////
//
// TTEDoc: :ContentClick
// 
///////////////////////////////////////////////////////////////////
void TTEDoc::ContentClick(EventRecord *theEvent) {
  Boolean shiftKeyDown = ((theEvent->modifiers & shiftKey) != 0); 
  if(fTEHandle) {

    // turn off auto scrolling, we do it ourselves for clicking 
    TEAutoView(false,fTEHandle);

    TEClick(theEvent->where, shiftKeyDown, fTEHandle);
    TEAutoView(true, fTEHandle);
  }
}

///////////////////////////////////////////////////////////////////
//
// TTEDoc::DoKeyDown
// 
///////////////////////////////////////////////////////////////////
void TTEDoc::DoKeyDown(EventRecord* theEvent) {
  if(fTEHandle) {
    TEKey(LoWrd(theEvent->message), fTEHandle);
    fNeedtoSave = true;

    // reset the scroll bars since the key press
    // may have caused the text to scroll or added
    // text
    FocusOnWindow();
    SynchScrollBars();
  }
}

///////////////////////////////////////////////////////////////////
//
// TTEDoc::DoIdle
// 
///////////////////////////////////////////////////////////////////
void TTEDoc::DoIdle(void) {
  TScrollDoc::DoIdle();
  if(fTEHandle) {
    TEIdle(fTEHandle);
  }

  GrafPtr oldPort;
  GetPort(&oldPort);
  SetPort(fDocWindow);
  Point thePt;
  GetMouse(&thePt);
  AdjustCursor(thePt);
  SetPort(oldPort);
}

///////////////////////////////////////////////////////////////////
//
// TTEDoc::AdjustCursor
// 
///////////////////////////////////////////////////////////////////
void TTEDoc::AdjustCursor(Point where) {
  Rect r;
  // decide if it is in content or scroll bars
  GetContentRect(r);
  if(PtInRect(where, &r)) {
    CursHandle IBeam = GetCursor(iBeamCursor);
    if(IBeam !=nil) {
      SetCursor(*IBeam);
    }
  } else
    // it must be in the scroll bars or grow box 
    InitCursor();
}

///////////////////////////////////////////////////////////////////
//
// TTEDoc::DoZoom
// 
///////////////////////////////////////////////////////////////////
void TTEDoc::DoZoom(short partCode) {
  // call the parent class, this will adjust scroll bars 
  TScrollDoc::DoZoom(partCode);

  // adjust the TE rectangle
  SetTERect();
}

///////////////////////////////////////////////////////////////////
//
// TTEDoc::DoGrow
// 
///////////////////////////////////////////////////////////////////
void TTEDoc::DoGrow(EventRecord* theEvent) {

  // call the parent class, this will adjust scroll bars 
  TScrollDoc::DoGrow(theEvent);

  // adjust the TE rectangle 
  SetTERect();
}

///////////////////////////////////////////////////////////////////
//
// TTEDoc: :ReadDocFile
// 
///////////////////////////////////////////////////////////////////
Boolean TTEDoc::ReadDocFile(short refNum) {
  if((fDocWindow) && (fTEHandle !=nil)) {
    long len;
    OSErr err GetEOF(refNum,&len);





    // truncate to TE limits 
    if(len > kMaxShort) {
      len = kMaxShort;
    }
    Handle thetext = NewHandle(len);
    if(thetext == nil) {
      ErrorAlert(rDocErrorStrings, sNoMem);
      return false;
    }

    HLock(thetext);
    err = SetFPos(refNum, fsFromStart, 0);
    err = FSRead(refNum, &len, (Ptr)*thetext);
    HUnlock(thetext);
    if(err == noErr) {
      // add the text to the document
      HLock(thetext);
      TESetText(*thetext, len, fTEHandle);
      HUnlock(thetext);
      DisposHandle(thetext);

      // set the selection to the first char
      TESetSelect(0, 0, fTEHandle);

      // adjust scroll bars to new text
      SynchScrollBars();

      return true;

    } else {
      DisposHandle(thetext);
      return false;
    }
  }
  // if there ain't no window... 
  return false;
}

///////////////////////////////////////////////////////////////////
//
// TTEDoc::WriteDocFile
// 
///////////////////////////////////////////////////////////////////
Boolean TTEDoc::WriteDocFile(short refNum) {
  if((fDocWindow !=nil) && (fTEHandle != nil)) {
    long len = (long) (**fTEHandle) .teLength;
    CharsHandle thetext = TEGetText(fTEHandle); 
    HLock((Handle)thetext);
    OSErr err = SetFPos(refNum, fsFromStart, 0);
    err = FSWrite(refNum,&len, (Ptr)*thetext);
    HUnlock((Handle)thetext);
    if(err == noErr)
      return true;
    else
      return false;
  }
  // if there ain't no window...
  return false;
}

///////////////////////////////////////////////////////////////////
//
// MyClickLoop
// 
///////////////////////////////////////////////////////////////////
pascal Boolean MyClickLoop(void) {
  Point where;
  Rect view;

  // use static member function to get static member
  TScrollDoc * theCurrScrollDoc = TScrollDoc::GetCurrScrollDoc();

  theCurrScrollDoc->GetContentRect(view);

  GetMouse(&where);
  if( where.v > view.bottom) {
    theCurrScrollDoc->Scroll(theCurrScrollDoc->GetVScroll(), theCurrScrollDoc->GetVertLineScrollAmount());
  }
  if(where.h >view.right) {
    theCurrScrollDoc->Scroll(theCurrScrollDoc->GetHScroll(),
theCurrScrollDoc->GetHorizLineScrollAmount());
  }
  if(where.v <view.top) {
    theCurrScrollDoc->Scroll(theCurrScrollDoc->GetVScroll(),
-(theCurrScrollDoc->GetVertLineScrollAmount()));
  }
  if(where.h < view.left) {
    theCurrScrollDoc->Scroll(theCurrScrollDoc->GetHScroll(),
  }
  return true;
}





