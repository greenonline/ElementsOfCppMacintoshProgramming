///////////////////////////////////////////////////////////////////
//
// This is the generic scrolling document object
//
//© 1990 Dan Weston, All Rights Reserved
// 
///////////////////////////////////////////////////////////////////

#include <Types.h> 
#include <Windows.h> 
#include <OSUtils.h> 
#include <Files.h> 
#include <Errors.h> 
#include <Memory.h> 
#include <SysEqu.h>
#include "TScrollDoc.h"

// define a segment for the ScrollDoc code 
//#pragma segment ScrollSeg

///////////////////////////////////////////////////////////////////
//
// constants
// 
///////////////////////////////////////////////////////////////////
// how much of the screen to leave during page scroll
const short kScrollOverlap = 16;
// resource ID's for scroll bars
const rVScroll 128;
const rHScroll = 129;

///////////////////////////////////////////////////////////////////
//
// static members are defined like globals
// 
///////////////////////////////////////////////////////////////////
TScrollDoc *TScrollDoc::fCurrScrollDoc = nil;

///////////////////////////////////////////////////////////////////
//
// TScrollDoc::TScrollDoc
// 
///////////////////////////////////////////////////////////////////
TScrollDoc::TScrollDoc(OSType theCreator,SFReply * SFinfo):
TDoc(theCreator,SFinfo) {
  fHorizScrollBar = nil;
  fVertScrollBar = nil;

  fVOffset 0;
  fHOffset = 0;
}

///////////////////////////////////////////////////////////////////
//
// TScrollDoc::InitDoc
// 
///////////////////////////////////////////////////////////////////
Boolean TScrollDoc::InitDoc(void) {

  if( ! TDoc::InitDoc()) return false;
  if(fDocWindow != nil) {
    SetPort(fDocWindow);
    fHorizScrollBar = GetNewControl(rHScroll, fDocWindow);
    fVertScrollBar = GetNewControl(rVScroll, fDocWindow); 
    SizeScrollBars();
    SynchScrollBars();
    return ((fHorizScrollBar !=nil) && (fVertScrollBar !=nil));
}

///////////////////////////////////////////////////////////////////
//
// TScrollDoc::SizeScrollBars
// 
///////////////////////////////////////////////////////////////////
void TScrollDoc::SizeScrollBars(void) {

  if(fDocWindow !=nil) {
    FocusOnWindow();
    Rect r = fDocWindow->portRect;

    if(fVertScrollBar != nil) {
      HideControl(fVertScrollBar);
      SizeControl(fVertScrollBar, kScrollBarWidth, (r.bottom - r.top - kScrollBarPos) + 2);
      MoveControl(fVertScrollBar, r.right - kScrollBarPos, -1) ;
      ShowControl(fVertScrollBar); 
      ValidRect(&(**fVertScrollBar) .contrlRect);
      if(fHorizScrollBar != nil) {
        HideControl(fHorizScrollBar); 
        SizeControl(fHorizScrollBar,
(r.right - r.left - kScrollBarPos) + 2, kScrollBarWidth);
        MoveControl(fHorizScrollBar, -1, r.bottom - r.top - kScrollBarPos);
        ShowControl(fHorizScrollBar);
        ValidRect(&(**fHorizScrollBar) .contrlRect);
    }
  }
}

///////////////////////////////////////////////////////////////////
//
// TScrollDoc::AdjustScrollBars
// 
///////////////////////////////////////////////////////////////////
void TScrollDoc::AdjustScrollBars(void) {
  // don't activate the scroll bars until
  // the data extends beyond the window boundaries 
  // If currentCtlValue is greater than new ctlmax ,
  // scroll image to bring it in line
  Rect r ;
  GetContentRect(r);
  short
  dh=
  short
  short

  // now ask the document how big its image is
  // first for the vertical dimension
  if(fVertScrollBar != nil) {
    currentValue = GetCtlValue(fVertScrollBar);
    newMax = GetVertSize() - (r.bottom - r.top);
    if(newMax < 0)
      newMax = 0 ;
    if(currentValue > newMax)
      dv = currentValue - newMax;
    SetCtlMax(fVertScrollBar, newMax);
  }

  if(fHorizScrollBar != nil) {
    currentValue = GetCtlValue(fHorizScrollBar);
    newMax = GetHorizSize() - (r.right - r.left); if(newMax < 0)
newMax = 0 ;
    if(currentValue > newMax)
      dh = currentValue - newMax; 
    SetCtlMax(fHorizScrollBar, newMax);
  }

  // adjust the position of the image if the window 
  // has gotten bigger than the image.
  if(dh | dv){
    FocusOnContent();
    // invalidate the whole content area
    GetContentRect(r);
    InvalRect(&r);

    // shut the clip region down to zero
    // so that the scrolling won't actually
    // draw in the window, wait for update instead
    RgnHandle oldClip = NewRgn();
    GetClip(oldClip);
    SetRect(&r, 0, 0, 0, 0);
    ClipRect(&r);
    ScrollContents(dh, dv);

    // now reset the clip region
    SetClip(oldClip);
    DisposeRgn(oldClip);
  }
}

///////////////////////////////////////////////////////////////////
//
// TScrollDoc::SynchScrollBars
// 
///////////////////////////////////////////////////////////////////
void TScrollDoc::SynchScrollBars(void) {
  AdjustScrollBars();
  SetScrollBarValues();
}

///////////////////////////////////////////////////////////////////
//
// TScrollDoc::SetScrollBarValues
// 
///////////////////////////////////////////////////////////////////
void TScrollDoc: :SetScrollBarValues(void) {

  FocusOnWindow();
  if(fHorizScrollBar != nil)
    SetCtlValue(fHorizScrollBar, fHOffset);
  if(fVertScrollBar != nil)
    SetCtlValue(fVertScrollBar, fVOffset);
}

///////////////////////////////////////////////////////////////////
//
// TScrollDoc::FocusOnWindow
// 
///////////////////////////////////////////////////////////////////

void TScrollDoc::FocusOnWindow() {
  SetPort(fDocWindow);
  SetOrigin(0,0);
  Rect r = fDocWindow->portRect;
  ClipRect(&r);
}

///////////////////////////////////////////////////////////////////
//
// TScrollDoc::FocusOnContent
// 
///////////////////////////////////////////////////////////////////

void TScrollDoc::FocusOnContent() {
  SetPort(fDocWindow);
  SetOrigin(fHOffset,fVOffset);
  Rect r;
  GetContentRect(r);
  ClipRect(&r);
}

///////////////////////////////////////////////////////////////////
//
// TScrollDoc::GetVertPageScrollAmount
// 
///////////////////////////////////////////////////////////////////
short TScrollDoc::GetVertPageScrollAmount(void) {
  Rect r;
  GetContentRect(r);
  return r.bottom - r.top - kScrollOverlap;
}

///////////////////////////////////////////////////////////////////
//
// TScrollDoc::GetHorizPageScrollAmount
// 
///////////////////////////////////////////////////////////////////
short TScrollDoc::GetHorizPageScrollAmount(void) {
  Rect r;
  GetContentRect(r);
  return r.right - r.left - kScrollOverlap;
}


///////////////////////////////////////////////////////////////////
//
// TScrollDoc::GetContentRect
// 
///////////////////////////////////////////////////////////////////
void TScrollDoc::GetContentRect(Rect& r) {
  // how big is the content area of the window, discounting the
  // scroll bars
  r = fDocWindow->portRect;
  if(fVertScrollBar != nil)
    r.right -= kScrollBarPos;
  if(fHorizScrollBar !=nil)
    r.bottom -= kScrollBarPos;
}

///////////////////////////////////////////////////////////////////
//
// TScrollDoc::ScrollClick
// 
///////////////////////////////////////////////////////////////////
void TScrollDoc::ScrollClick(EventRecord *theEvent) {
  ControlHandle whichControl;
  short part;

  FocusOnWindow();
  if(part = FindControl(theEvent->where,fDocWindow,&whichControl))
  {
    switch (part) {
      case inThumb:
        DoThumbScroll(whichControl,theEvent->where);
        break;
      case inUpButton:
      case inDownButton:
        DoButtonScroll(whichControl,theEvent->where);
        break;
      case inPageUp:
      case inPageDown:
        DoPageScroll(whichControl,part);
        break;
    }
  }
}

///////////////////////////////////////////////////////////////////
//
// TScrollDoc::DoButtonScroll
// 
///////////////////////////////////////////////////////////////////
void TScrollDoc::DoButtonScroll(ControlHandle theControl,Point localPt) {

  // declare the action procedure
  pascal void ActionProc(ControlHandle theControl, short partCode);
  short result= TrackControl(theControl, localPt, (ProcPtr)ActionProc);
}

///////////////////////////////////////////////////////////////////
//
// TScrollDoc::DoPageScroll
// 
///////////////////////////////////////////////////////////////////
void TScrollDoc::DoPageScroll(ControlHandle theControl, short part) {
  short scrollAmount; Point thePt;
  short currentPart;

  if((theControl == fVertScrollBar))
    scrollAmount = GetVertPageScrollAmount();
  else
    scrollAmount = GetHorizPageScrollAmount();

  // repeat as long as user holds down mouse button
  do {
    GetMouse(&thePt);
    currentPart = TestControl(theControl, thePt);

    if(currentPart ==part) {
    if(currentPart == inPageUp) Scroll(theControl, -scrollAmount);
    if(currentPart == inPageDown) Scroll(theControl, scrollAmount);
    } 
  }while(Button());
}

///////////////////////////////////////////////////////////////////
//
// TScrollDoc::Scroll
// 
///////////////////////////////////////////////////////////////////
void TScrollDoc::Scroll(ControlHandle theControl, short change) (

  // this routine changes the value of the scroll bar
  // and scrolls the contents,
  // it can be used for arbitrary scrolling,
  // either from scroll bar action procs
  // or while dragging mouse outside window

  // save current clip region
  RgnHandle oldClip = NewRgn();
  GetClip(oldClip);

  short diff = 0;
  short oldValue GetCtlValue(theControl);
  short newValue = oldValue + change;

  // check for endpoint
  if (change < 0) (
    short minValue = GetCtlMin(theControl);
    if(newValue < minValue)
      newValue minValue;
  } else {
    short maxValue = GetCtlMax(theControl);
    // figure the new value and check for endpoint
    if(newValue > maxValue)
      newValue = maxValue;
  }
  diff = oldValue - newValue;

  // do the scrolling and set the new scroll bar values 
  FocusOnContent();
  if(theControl == fHorizScrollBar)
    ScrollContents(diff, 0);
  if(theControl == fVertScrollBar)
    ScrollContents(0, diff);

  FocusOnWindow();
  SetScrollBarValues();

  // restore old clip region
  SetClip(oldClip);
  DisposeRgn(oldClip);
}

///////////////////////////////////////////////////////////////////// 
//
// TScrollDoc::DoThumbScroll
// 
///////////////////////////////////////////////////////////////////
void TScrollDoc: :DoThumbScroll(ControlHandle theControl, Point localPt) {
  short oldValue = GetCtlValue(theControl);
  short trackResult = TrackControl(theControl,localPt,nil); 

  if(trackResult != 0) {
    short newValue = GetCtlValue(theControl);
    short diff = oldValue - newValue;
    FocusOnContent();
    if(theControl == fHorizScrollBar)
      ScrollContents(diff,0);
    if(theControl == fVertScrollBar)
      ScrollContents(O,diff); FocusOnWindow();
  }
}

///////////////////////////////////////////////////////////////////
//
// ActionProc
// 
///////////////////////////////////////////////////////////////////
pascal void ActionProc(ControlHandle theControl,short partCode) {
  // use static member function to get static member
  TScrollDoc * theCurrScrollDoc = TScrollDoc::GetCurrScrollDoc();

  short scrollAmount = 0;
  if(theControl == theCurrScrollDoc->GetVScroll())
    scrollAmount = theCurrScrollDoc->GetVertLineScrollAmount();  
  if(theControl == theCurrScrollDoc->GetHScroll())
    scrollAmount = theCurrScrollDoc->GetHorizLineScrollAmount();

  if(partCode == inUpButton)
    theCurrScrollDoc->Scroll(theControl,-scrollA mount);
  if(partCode == inDownButton)
    theCurrScrollDoc->Scroll(theControl,scrollA mount);
}

/////////////////////////////////////////////////////////////////// 
//
// TScrollDoc::ScrollContents
// 
///////////////////////////////////////////////////////////////////
void TScrollDoc::ScrollContents(short dh,short dv) {

  // determine the area to scroll
  Rect r;
  GetContentRect(r);

  // now scroll the image
  RgnHandle updateRgn = NewRgn(); 
  ScrollRect(&r,dh,dv,updateRgn);

  // keep track of how far off the origin we are
  fVOffset dv;
  fHOffset -= dh;

  // tell window to redraw uncovered content
  InvalRgn(updateRgn);

  // now force the update area to be drawn
  DoTheUpdate((EventRecord *)nil);

  // dispose of the region
  DisposeRgn(updateRgn);
}

///////////////////////////////////////////////////////////////////
//
// TScrollDoc::Activate
// 
///////////////////////////////////////////////////////////////////
void TScrollDoc::Activate(void) {

  FocusOnWindow();

  if(fVertScrollBar !=nil) ShowControl(fVertScrollBar);
  if(fHorizScrollBar !=nil) ShowControl(fHorizScrollBar);
  // set up static member so that scroll action proc can access
  // member functions
  fCurrScrollDoc = this;
}

///////////////////////////////////////////////////////////////////
//
// TScrollDoc::Deactivate
// 
///////////////////////////////////////////////////////////////////
void TScrollDoc::Deactivate(void) {

  FocusOnWindow();
  if(fVertScrollBar !=nil)
    HideControl(fVertScrollBar);
  if(fHorizScrollBar !=nil)
    HideControl(fHorizScrollBar);
}

///////////////////////////////////////////////////////////////////
//
// TScrollDoc::DoTheUpdate
// 
///////////////////////////////////////////////////////////////////
void TScrollDoc::DoTheUpdate(EventRecord * l*theEvent*I) {
  if(fDocWindow !=nil) {
    FocusOnContent();
    BeginUpdate(fDocWindow);
    Rect r = (**(fDocWindow->visRgn)) .rgnBBox;
    Draw(&r);
    FocusOnWindow();
    DrawControls(fDocWindow); 
    DoDrawGrowIcon(); 
    EndUpdate(fDocWindow);
  }
}

///////////////////////////////////////////////////////////////////
//
// TScrollDoc: :DoContent
// 
///////////////////////////////////////////////////////////////////
void TScrollDoc::DoContent(EventRecord* theEvent) {

  FocusOnWindow();
  GlobalToLocal(&theEvent->where);
  Rect contents;

  GetContentRect(contents);
  if(PtInRect(theEvent->where,&contents)) {
    FocusOnContent();
    ContentClick(theEvent);
  } else
    ScrollClick(theEvent);
}

///////////////////////////////////////////////////////////////////
//
// TScrollDoc::DoZoom
// 
///////////////////////////////////////////////////////////////////

void TScrollDoc::DoZoom(short partCode) {

  FocusOnWindow();
  // call the parent class
  TDoc::DoZoom(partCode);
  SizeScrollBars();
  SynchScrollBars();
}

///////////////////////////////////////////////////////////////////
//
// TScrollDoc::DoGrow
// 
///////////////////////////////////////////////////////////////////
void TScrollDoc::DoGrow(EventRecord* theEvent) {

  FocusOnWindow();
  // call the parent class 
  TDoc::DoGrow(theEvent);

  SizeScrollBars();
  SynchScrollBars();
}
