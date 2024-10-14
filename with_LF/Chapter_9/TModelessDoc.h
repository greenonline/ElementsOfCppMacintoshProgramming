///////////////////////////////////////////////////////////////////
//
// This is the generic modeless dialog document object
//
// © 1990 Dan Weston, All Rights Reserved
// 
///////////////////////////////////////////////////////////////////

#ifndef TModelessDoc_Defs 
#define TModelessDoc_Defs

// Include necessary interface files 
#include <Types.h>
#include <Quickdraw.h>
#include <Windows.h>
#include <Packages.h> 
#include <TDoc.h>

///////////////////////////////////////////////////////////////////
//
// constants
// 
///////////////////////////////////////////////////////////////////

const short rGenericDialog = 1000;

///////////////////////////////////////////////////////////////////
//
// class declarations
// 
///////////////////////////////////////////////////////////////////

class TModelessDoc : public TDoc {
  public:
    // SFinfo will be non-nil when opening an existing document 
    TModelessDoc(OSType theCreator = '????'
                 SFReply * SFinfo = (SFReply *)nil):
    TDoc (theCreator, SFinfo) {};

    // virtual destructor so that derived destructors will be called 
    virtual ~TModelessDoc();

    // called by TApp when making a document,
    virtual Boolean MakeWindow(Boolean colorWindow );

    virtual short GetWinID(void);

// Event actions
    virtual void DoIdle(void);

    virtual void DoActivate(EventRecord* theEvent)
      {DoDialogEvent(theEvent);}
    virtual void DoTheUpdate(EventRecord* theEvent) 
      {DoDialogEvent(theEvent);}
    virtual void DoContent(EventRecord* theEvent) 
      {DoDialogEvent(theEvent);}
    virtual void DoKeyDown(EventRecord* theEvent) 
      {DoDialogEvent(theEvent);}

    // disable grow actions
    virtual void DoGrow(EventRecord* theEvent) {}
    virtual void DoDrawGrowIcon(void)
      {}
  protected:
    virtual void DoDialogEvent(EventRecord* theEvent);
    virtual void DoItemHit(DialogPtr theDialog, short theItem) {}
};

#endif TModelessDoc_Defs