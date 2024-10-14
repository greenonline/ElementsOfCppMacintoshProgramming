///////////////////////////////////////////////////////////////////
//
// This is the generic text edit object
//
// © 1990 Dan Weston, All Rights Reserved
// 
///////////////////////////////////////////////////////////////////

#ifndef TTEDoc_Defs 
#define TTEDoc_Defs

// Include necessary interface files 
#include <Types.h>
#include <Quickdraw.h>
#include <Windows.h>
#include <Packages.h> 

#include <TScrollDoc.h>

///////////////////////////////////////////////////////////////////
//
// constants
// 
///////////////////////////////////////////////////////////////////

// none

///////////////////////////////////////////////////////////////////
//
// class declarations
// 
///////////////////////////////////////////////////////////////////

class TTEDoc : public TTEDoc {

  protected: 
    TEHandle fTEHandle;

  public:
    // SFinfo will be non-nil when opening an existing document 
    TTEDoc(OSType theCreator = '????'
                 SFReply * SFinfo = (SFReply *)nil):

    // virtual destructor so that derived destructors will be called 
    virtual ~TTEDoc();

    vitual Boolean InitDoc(void);

// new functions to support text editing

    virtual void AddText(char *text, long len);

// Event actions
  protected:
    virtual void Draw(Rect *r);

  public:

    virtual void Activate(void);
    virtual void Deactivate(void);
    virtual void DoZoom(short partCode);
    virtual void DoGrow(EventRecord* theEvent);

    virtual void DoKeyDown(EventRecord* theEvent) 
    virtual void ContentClick(EventRecord* theEvent) 

    virtual void DoIdle(void);
    virtual void AdjustCursor(Point where);

// cliboard stuff

    // query state of document - useful for adjusting menu state
    // override if you can do any of these actions
    virtual Boolean HaveSelection(void)
    virtual Boolean CanSelectAll(void)
      {return true;}
    virtual Boolean CanPaste(OSType theType)
      {return (theType == 'TEXT');}


    virtual Boolean DoCut(Handle *theData, OSType *theType);
    virtual Boolean DoCopy(Handle *theData, OSType *theType);
    virtual void DPaste(Handle *theData, OSType *theType);
    virtual void DoClear(void);
    virtual void DoSelectAll(void);

// file related member functions
    virtual OSType GetDocType(){return 'TEXT');}
    virtual Boolean ReadDocFile(short refNum);
    virtual Boolean WriteDocFile(short refNum);
    virtual Boolean CanSaveAs(short refNum);

  protected:
    void SetTERect(void);

    // Scrolling stuff that we override
    virtual void GetContentRect(Rect &r);

    virtual void ScrollContents(short dh, short dv);
    virtual void SetScrollBarValues(void);

    virtual short GtVertSize(void);
    virtual short GetHorizSize(void);
    virtual short GetVertLineScrollAmount(void);
    virtual short GetHorizLineScrollAmount(void);
};



// click loop proc that isn't part of TEDoc class
pascal Boolean MyClickLoop(void);

#endif TTEDoc_Defs