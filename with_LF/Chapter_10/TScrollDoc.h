///////////////////////////////////////////////////////////////////
//
// This is the generic scrolling document object
//
// © 1990 Dan Weston, All Rights Reserved
// 
///////////////////////////////////////////////////////////////////

#ifndef TScrollDoc_Defs 
#define TScrollDoc_Defs

// Include necessary interface files 
#include <Types.h>
#include <Quickdraw.h>
#include <Windows.h>
#include <Packages.h> 
#include <TDoc.h>

///////////////////////////////////////////////////////////////////
//
// class declarations
// 
///////////////////////////////////////////////////////////////////

class TScrollDoc : public TDoc { 
  protected:
    ControlHandle fHorizScrollBar;
    ControlHandle fVertScrollBar;

    short fVOffset;
    short fHOffset;

    static TScrollDoc *fCurrScrollDoc;

  public:
    static TScrollDoc *GetCurrScrollDoc(void) {return fCurrScrollDoc;)
    ControlHandle GetVScroll(void) {return fVertScrollBar;} 
    ControlHandle GetHScroll(void) {return fHorizScrollBar;}

    // SFinfo will be non-nil when opening an existing document 
    TScrollDoc(OSType theCreator '????',
               SFReply * SFinfo = (SFReply *)nil);

    // virtual destructor so that derived destructors will be called
    virtual -TScrollDoc() {}
    virtual Boolean InitDoc(void);

// Event actions that are different from TDoc
    // you probably won't need to override these
    virtual void DoContent(EventRecord* theEvent);
    virtual void DoTheUpdate(EventRecord *theEvent);
    virtual void DoZoom(short partCode);
    virtual void DoGrow(EventRecord *theEvent);

    // override these for activation deactivation stuff 
    // be sure and call TScrollDoc::Activate or
    // TScrollDoc::Deactivate in your override functions.
    virtual void Activate(void);
    virtual void Deactivate(void);

  protected:
  // new functions to support scrolling
    // you probably won't need to override these
    virtual void ScrollClick(EventRecord *theEvent);

    virtual void DoThumbScroll(ControlHandle theControl,Point localPt);
    virtual void DoPageScroll(ControlHandle theControl, short part);
    virtual void DoButtonScroll(ControlHandle theControl,Point localPt);

    virtual void SizeScrollBars(void);
    virtual void AdjustScrollBars(void);
    virtual void SetScrollBarValues(void);
    virtual void SynchScrollBars(void);

  public:
    virtual void FocusOnWindow();
    virtual void FocusOnContent();
    virtual void Scroll(ControlHandle theControl, short change);
    virtual void GetContentRect(Rect& r);

    // routines you must override
    virtual short GetVertSize(void) {return 0;}
    virtual short GetHorizSize(void) {return 0;}
    virtual short GetVertLineScrollAmount(void) {return 0;}
    virtual short GetHorizLineScrollAmount(void) {return 0;}

    // override these only if you don't want a page 
    // scroll to be one window full
    virtual short GetVertPageScrollAmount(void);
    virtual short GetHorizPageScrollAmount(void);

    // routines you might override
    virtual void ContentClick(EventRecord *theEvent) {}
    virtual void ScrollContents(short dh, short dv);
};
#endif TScrollDoc Defs

