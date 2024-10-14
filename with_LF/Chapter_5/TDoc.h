////////////////////////////////////////////////////////////////////////////////////////////// 
//
// This is the generic document object
//
// © 1990 Dan Weston, All Rights Reserved
// 
////////////////////////////////////////////////////////////////////////////////////////////// 

#ifndef TDoc_Defs 
#define TDoc_Defs

// Include necessary interface files 
#include <Types.h>
#include <Quickdraw.h>
#include <Windows.h>
#include <Packages.h> 
#include <Menus.h>

////////////////////////////////////////////////////////////////////////////////////////////// 
//
// constants
//
////////////////////////////////////////////////////////////////////////////////////////////// 

const short kScrollBarWidth = 16;
const short kScrollBarPos = kScrollBarWidth -1;

const short rErrorAlert = 255;
const short rDocErrorStrings = 255; 
const short sNoMem = 1;
const short sFileOpen = 2;
const short sUnknownErr = 3;

const short rGenericDoc = 1000;

const short rWantToSave = 500; 
const short iYes = 1;
const short iNo = 3;
const short iCancel = 2;

////////////////////////////////////////////////////////////////////////////////////////////// 
//
// utility routines
// 
////////////////////////////////////////////////////////////////////////////////////////////// 

// Define HiWrd and LoWrd function inline for efficiency
inline short HiWrd(long aLong) {return (short) (((aLong) >> 16) & 0xFFFF);} 
inline short LoWrd(long aLong) {return (short) ((aLong) & 0xFFFF);}
void ErrorAlert(short StringsID, short theErrorID);

inline void SetMenuAbility(MenuHandle menu, short item, Boolean enable) {enable? EnableItem(menu, item) : DisableItem(menu, item);}

////////////////////////////////////////////////////////////////////////////////////////////// 
//
// class declarations
// 
////////////////////////////////////////////////////////////////////////////////////////////// 

class TDoc {
protected: 
  OSType fCreator;
  SFReply fFileInfo;
  Boolean fFileOpen;
  short fRefNum;
  WindowPtr fDocWindow;
  Boolean fNeedtoSave;
  Boolean fNeedtoSaveAs;

public:

  // SFinfo will be non-nil when opening an existing document
  TDoc(OSType theCreator = '????', SFReply * SFInfo = (SFReply *) nil); 

  // virtual destructor so that derived destructors will be called
  virtual ~TDoc(void);

  // called by TApp when making a document,
  // you probably won't override this
  virtual Boolean MakeWindow(Boolean colorWindow );

  // override to get your own WIND resource read in
  virtual short GetWinID(void);

  // InitDoc is available for your initialization
  // routines that might fail,
  // It gets called after the window is created 
  virtual Boolean TDoc::InitDoc(void) {return true;};

  // utilities that manipulate the window 
  // can't be overridden
  WindowPtr GetDocWindow(void) { return fDocWindow; }
  void SetDocWindowTitle(Str255 title)
    {if(fDocWindow)SetWTitle(fDocWindow, title);} 
  void MoveDocWindow(short h, short v)
    {if(fDocWindow)MoveWindow(fDocWindow, h, v, true);}
  void ShowDocWindow(void)
    {if(fDocWindow)ShowWindow(fDocWindow);} 

// Event actions



  // this probably won't be overridden
  virtual void DoTheUpdate(EventRecord* theEvent);

protected:
  // override this to draw window contents virtual
  void Draw(Rect *r);

public:
  // override this if you don't want grow box
  virtual void DoDrawGrowIcon(void)
    {if(fDocWindow)DrawGrowIcon(fDocWindow);}

  virtual void DoActivate(EventRecord* theEvent);

  // override these to de/activate window (TEActivate, etc)
  virtual void Activate(void) {}
  virtual void Deactivate(void) {}

 // override these if you don't want default behavior
  virtual void DoZoom(short partCode);
  virtual void DoGrow(EventRecord* theEvent);
  virtual void DoDrag(EventRecord* theEvent);
  virtual short GetMinHeight(void) {return 75;}
  virtual short GetMinWidth(void) {return 75;}

  // override these to respond to clicks and keys
  virtual void DoContent(EventRecord* theEvent) {}
  virtual void DoKeyDown(EventRecord* theEvent) {}

  virtual void DoIdle(void) {}
  virtual void AdjustCursor(Point where) {} //where is in local coords

// Edit menu and clipboard actions
public: // public functions are called from App class
  virtual void DoUndo(void) {}
  virtual Boolean DoCut(Handle *theData,OSType *theType)
    {*theData = nil;*theType = '????';return false;}
  virtual Boolean DoCopy(Handle *theData,OSType *theType)
    {*theData = nil;*theType = '????';return false;}
  virtual void DoPaste(Handle theData,OSType theType) {}

protected:  // protected functions are only called from Doc class
  virtual void DoClear(void) {}
  virtual void DoSelectAll(void) {}

public:


  // called by app to handle doc specific menu items
  // return true if you handle the menu command
  virtual Boolean DoDocMenuCommand(short menuID, short menuItem);
  virtual void AdjustDocMenus(void);

  // query state of document - useful for adjusting menu state
  // override if you can do any of these operations
  virtual Boolean CanUndo(void) { return false; }
  virtual Boolean HaveSelection(void) { return false; }
  virtual Boolean CanPaste(OSType /*theType*/) { return false; }
  virtual Boolean CanSelectAll(void) {return false;}
  virtual Boolean CanClose(void) { return true; }
  virtual Boolean CanSaveAs(void) { return false; }
  virtual Boolean CanPageSetup(void) {return false;}
  virtual Boolean CanPrint(void) { return false; }

  // this probably won't be overridden
  virtual Boolean CanSave(void) { return fNeedtoSave;}

  // override these to read and write files
  virtual Boolean ReadDocFile(short /*refNum*/) {return true;}
  virtual Boolean WriteDocFile(short /*refNum*/) {return true;}

  // override this if you have your own file type, such as TEXT
  virtual OSType GetDocType(void) {return '????';}

  // these probably don't need to be changed
  virtual short OpenDocFile(SFReply *reply);
  virtual void CloseDocFile(short refNum);

  // these are called when user chooses associated menu item
  // these probably don't need to be changed
  virtual Boolean DoClose(void);
  virtual Boolean DoSave(void) ;
  virtual Boolean DoSaveAs(void);

  // this is called when user tries to close a doc
  // with unsaved changes
  // this probably doesn't need to be changed
  virtual short WantToSave(void);

  // override these for printing support
  virtual void DoPageSetup(void) {}
  virtual void DoPrint(void) {}
};
#endif TDoc_Defs

