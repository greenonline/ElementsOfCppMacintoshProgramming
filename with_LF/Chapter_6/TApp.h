//////////////////////////////////////////////////////////////////////////////////////////
//
// This is the generic application object
//
// © 1990 Dan Weston, All Rights Reserved
// 
//////////////////////////////////////////////////////////////////////////////////////////

#ifndef TApp_H 
#define TApp_H

#include <Types.h> 
//#include <Desk.h>  // #Error: include <Menus.h>, <Devices.h>, and <Events.h> instead of <Desk.h>
#include <Menus.h>
#include <Devices.h>
#include <Events.h>
#include <Events.h> 
#include <Menus.h> 
#include <OSUtils.h>

// we need definitions of Document class 
#include "TDoc.h"

////////////////////////////////////////////////////////////////////////////////////////////
// class TLink
// TLink is a utility class that is used by
// the TList class below.
// 
//////////////////////////////////////////////////////////////////////////////////////////
class TLink {
  TLink* fNext; //the link to the next item 
  void* fItem; //the item this link refers to

public:

  TLink(TLink *n = nil, void *item = nil) 
    {fNext = n; fItem = item;}
  TLink* GetNext ()
    { return fNext; }
  void* GetItem()
    { return fItem; }
  void SetNext(TLink* aLink) 
    { fNext = aLink; }
  void SetItem(void* anitem)
    { fItem = anitem; }
};

////////////////////////////////////////////////////////////////////////////////////////////
// class TList
//
//////////////////////////////////////////////////////////////////////////////////////////
class TList {
protected:
  friend class TIterator;
  TLink* fLink ;  // the first link in our list
  int fNumItems; // the number of elements in the list
public:
  TList (void) ; // constructor

  virtual void AddItem(void* item);
  virtual void RemoveItem(void* item);
  int NumItems () { return fNumItems; }
};

//////////////////////////////////////////////////////////////////////////////////////////
//
// class TIterator
// 
//////////////////////////////////////////////////////////////////////////////////////////
 
class TIterator {
  TLink* fCurLink;

public:
  TIterator(TList* list) { fCurLink = list->fLink; }
  void* Next(void);
};







//////////////////////////////////////////////////////////////////////////////////////////
 
//
// class TDocList
// 
//////////////////////////////////////////////////////////////////////////////////////////
 
class TDocList : public TList {
public:

  // add one new member function
  // find the TDocument associated with the window
  TDoc* FindDoc(WindowPtr window);
};

//////////////////////////////////////////////////////////////////////////////////////////
//
// class TApp
// 
//////////////////////////////////////////////////////////////////////////////////////////



class TApp {
public:

  // other classes might like to see this
  SysEnvRec fenvRec;

protected:

  // members just for TApp and derived classes
  TDocList* fDocList;
  TDoc* fCurDoc;
  Boolean fHaveWaitNextEvent;
  Boolean fDone;
  Boolean finBackground;
  Handle fClipData;
  OSType fClipType;
  Boolean fDAonTop;
  short fLastScrapCount;

public:

  // constructor needs to be public
  TApp (void) ;

  virtual ~TApp(void) {delete fDocList;}

  // These three member functions get called from main()
  virtual Boolean InitApp(void);
  virtual void EventLoop(void);
  virtual void CleanUp(void);

  // these functions create documents, and manipulate
  // the document list
  // probably won't be overridden
  virtual Boolean OpenDocFromFinder(void);
  virtual void OpenNewDoc(void);
  virtual void OpenOldDoc(void);

  // add a document to app's document list
  virtual void AddDocument(TDoc * theDoc);

protected:
  // override this function to make the kind of document
  // supported by your application
  virtual TDoc * MakeDoc(SFReply *reply= (SFReply *)nil);

  // override this if your app has a unique creator signature 
  virtual OSType GetCreator(void) {return '????';}

  // routines to override to configure SFGetFile
  // override these to specify the type of files you can open
  virtual int GetNumFileTypes(void) {return 0;};
  virtual SFTypeList GetFileTypesList(void) {return (SFTypeList)nil;}
  virtual Boolean AcceptableFileType(OSType theType);

  // common code for OpenDocFromFinder and OpenOldDoc
  virtual Boolean InitOldDoc(SFReply *reply);

// event handlers you shouldn't need to override in a typical application 
// these call event handlers for documents, where real functionality is
  virtual void OSEvent(EventRecord * theEvent);
  virtual void MouseDown(EventRecord * theEvent);
  virtual void KeyDown(EventRecord * theEvent);
  virtual void ActivateEvt(EventRecord * theEvent);
  virtual void UpdateEvt(EventRecord * theEvent);
  virtual void ExitLoop(void) {fDone =true;}
  virtual void AppIdle(void);

  // override these if you need to respond to
  // mouseups or disk inserted events
  virtual void MouseUp(EventRecord * theEvent) {}
  virtual void DiskEvt(EventRecord * theEvent) {}

  // MultiFinder friendly functions
  virtual void DoSuspend(EventRecord * theEvent,Boolean convertClip);
  virtual void DoResume(EventRecord * theEvent,Boolean convertClip);

  // how long to sleep in WaitNextEvent
  virtual unsigned long SleepVal(void) { return 0; }

// menu functions

  // handles standard DA, File, and Edit menus
  // override to handle additional menus
  // but call TApp::DoMenuCommand if your derived function 
  // doesn't handle menu choice
  virtual void DoMenuCommand(short menuID, short menuItem);

  // functions that control state of menu items
  virtual void AdjustMenus(void);
  virtual Boolean CanNew(void) {return true;}
  virtual Boolean CanOpen(void) (return false;}
  virtual Boolean CanQuit(void) {return true;}

  // responses for File Menu items
  virtual Boolean CloseADoc(TDoc * theDoc);
  virtual void Quit(void);

// clipboard support




  virtual void GetClipFromSystem(void);
  virtual void GiveClipToSystem(void);
  virtual OSType CanAcceptClipType(void);
  virtual Boolean ClipHasChanged(void);
  virtual void CheckForDASwitch(WindowPtr theFrontWindow);

  // responses to Edit menu items
  virtual void DoUndoCmd(TDoc * theDoc);
  virtual void DoCutCmd(TDoc * theDoc);
  virtual void DoCopyCmd(TDoc * theDoc);
  virtual void DoPasteCmd(TDoc * theDoc);

  // Utility routine
  Boolean TrapAvailable(short tNumber,TrapType tType);

};
#endif TApp_H

