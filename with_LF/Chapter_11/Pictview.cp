///////////////////////////////////////////////////////////////////
//
// This file: PictView.cp C++ Source
//
// This is the main application object for the PictView program
//
// © 1990 Dan Weston, All Rights Reserved
//
///////////////////////////////////////////////////////////////////

#include <Quickdraw.h> 
#include <Windows.h> 
#include <Memory.h> 
#include <Files.h> 
#include <Errors.h> 
#include <Printing.h>
#include "TApp.h" 
#include "TScrollDoc.h"

///////////////////////////////////////////////////////////////////
//
// class declarations
// 
///////////////////////////////////////////////////////////////////
const long kPictHeaderSize = 512;

class TPICTDoc : public TScrollDoc{
  protected:
    Handle fPict;
    Handle fHeader; THPrint fPrintRecord;
  public:
    TPICTDoc(OSType theCreator virtual ~TPICTDoc();
    virtual Boolean InitDoc(void);
  protected:
    // routines you must override
    virtual short GetVertSize(void);
    virtual short GetHorizSize(void);
    virtual short GetVertLineScrollAmount(void) {return 16;}
    virtual short GetHorizLineScrollAmount(void) {return 16;}





    // draw the picture
    void Draw(Rect *r);

  public:

    // This is the file type of the document
    virtual OSType GetDocType() {return 'PICT';}

    // this function reads in the file
    virtual Boolean ReadDocFile(short refNum);

    // disable the SaveAs menu
    virtual Boolean CanSaveAs(void) {return false;}

    virtual void DoPageSetup(void);
    virtual void DoPrint(void);

    virtual Boolean CanPrint(void) {return true;}
    virtual Boolean CanPageSetup(void) {return true;}
};

class TPICTApp : public TApp{
  protected:
    // make our kind of document
    virtual TDoc * MakeDoc(SFReply *reply = (SFReply *)nil);

    virtual Boolean CanOpen(void) {return true;}

    // configure SFGetFile
    virtual int GetNumFileTypes(void) {return l;}
    virtual SFTypeList GetFileTypesList(void);

    // disable the New menu
    virtual Boolean CanNew(void) {return false;}
};

///////////////////////////////////////////////////////////////////
//
// Globals
// 
///////////////////////////////////////////////////////////////////

// list of file types for SFGetFile 
SFTypeList gtheTypes = {'PICT');

///////////////////////////////////////////////////////////////////
//
// main
// 
///////////////////////////////////////////////////////////////////
void main(void)
{
  TPICTApp theApp;
  // initialize the application
  if(theApp.InitApp()) {

    // allow the user to open a PICT file first thing
    if(! theApp.OpenDocFromFinder())
      theApp.OpenOldDoc();

    // Start our main event loop running. 
    // This won't return until user quits
    theApp.EventLoop();

    //now clean up
    theApp.CleanUp();
}

///////////////////////////////////////////////////////////////////
//
// TPICTApp::MakeDoc
// 
///////////////////////////////////////////////////////////////////
TDoc * TPICTApp::MakeDoc(SFReply *reply) {
  return new TPICTDoc(GetCreator(), reply);
}

///////////////////////////////////////////////////////////////////
//
// TPICTApp::GetFileTypesList
// 
///////////////////////////////////////////////////////////////////
SFTypeList TPICTApp::GetFileTypesList(void) {
  return gtheTypes;
}

///////////////////////////////////////////////////////////////////
//
// TPICTDoc::TPICTDoc
// 
///////////////////////////////////////////////////////////////////
TPICTDoc::TPICTDoc(OSType theCreator, SFReply *reply):
  TScrollDoc(theCreator, reply) {

  fPict = nil; 
  fHeader = nil;
  fPrintRecord = nil;
}

///////////////////////////////////////////////////////////////////
//
// TPICTDoc::-TPICTDoc
// 
///////////////////////////////////////////////////////////////////
TPICTDoc::~TPICTDoc() {
  if(fHeader != nil) {
    DisposHandle(fHeader);
    fHeader = nil;
  }
  if(fPict != nil) {
    KillPicture((PicHandle)fPict); 
    fPict = nil;
  }
  if(fPrintRecord != nil) { 
    DisposHandle((Handle)fPrintRecord); 
    fPrintRecord = nil;
  }
}

///////////////////////////////////////////////////////////////////
//
// TPICTDoc::InitDoc
// 
///////////////////////////////////////////////////////////////////
Boolean TPICTDoc::InitDoc(void) {
  if(TScrollDoc::InitDoc()) {
    fPrintRecord = (THPrint)NewHandle(sizeof(TPrint)); 
    if(fPrintRecord l= nil) {
      PrOpen();
      PrintDefault(fPrintRecord); PrClose();
      return true;
    }
  }
  // or if something went wrong 
  return false;
}

///////////////////////////////////////////////////////////////////
//
// TPICTDoc::Draw
// 
///////////////////////////////////////////////////////////////////
void TPICTDoc::Draw(Rect *r) {
  if(fPict !=nil) {
    EraseRect(r);
    DrawPicture((PicHandle)fPict, &((**((PicHandle)fPict)) .picFrame));
  }
}

///////////////////////////////////////////////////////////////////
//
// TPICTDoc::ReadDocFile
// 
///////////////////////////////////////////////////////////////////
Boolean TPICTDoc::ReadDocFile(short refNum) {
  if (fDocWindow) {
    long pictLength;
    long headerLength = kPictHeaderSize;
    OSErr err= GetEOF(refNum, &pictLength);
    pictLength -= kPictHeaderSize;
    Handle thePic = NewHandle(pictLength);
    if(thePic == nil) {
      ErrorAlert(rDocErrorStrings, sNoMem); return false;
    }
    Handle theHeader = NewHandle(headerLength);
    if(theHeader ==nil) {
      ErrorAlert(rDocErrorStrings, sNoMem);
      DisposHandle(thePic);
      return false;
    }
    HLock(theHeader);
    HLock(thePic);
    err = SetFPos(refNum, fsFromStart, 0);
    err= FSRead(refNum, &headerLength, (Ptr)*theHeader);
    err= FSRead(refNum, &pictLength, (Ptr)*thePic);
    HUnlock(thePic);
    HUnlock(theHeader);
    if(err == noErr) {
      fPict = thePic;
      fHeader = theHeader;
      AdjustScrollBars();
      return true;
    } else { 
      DisposHandle(thePic);
      DisposHandle(theHeader);
      return false;
    }
  }
  // if there ain't no window...
  return false;
}

///////////////////////////////////////////////////////////////////
//
// TPICTDoc::GetVertSize
// 
///////////////////////////////////////////////////////////////////
short TPICTDoc::GetVertSize(void) {

  Rect r;
  if (fPict) {
    r = (**((PicHandle)fPict)) .picFrame;
    return r.bottom - r.top; 
  } else
    return 0;
}

///////////////////////////////////////////////////////////////////
//
// TPICTDoc: :GetHorizSize
// 
///////////////////////////////////////////////////////////////////
short TPICTDoc::GetHorizSize(void) {
  Rect r;
  if(fPict){
    r = (**((PicHandle)fPict)) .picFrame;
    return r.right - r.left;
  } else
    return 0;
}

///////////////////////////////////////////////////////////////////
//
// TPICTDoc::DoPageSetup
// 
///////////////////////////////////////////////////////////////////
void TPICTDoc: :DoPageSetup(void) {
  // open the print manager
  PrOpen ();

  // put up the style dialog
  (void)PrStlDialog(fPrintRecord);

  // and close the print manager
  PrClose();
}

///////////////////////////////////////////////////////////////////
//
// TPICTDoc::DoPrint
// 
///////////////////////////////////////////////////////////////////
void TPICTDoc::DoPrint(void) {
  TPPrPort printPort;

  // open the print manager PrOpen();
  // if print record doesn't match printer,
  // put up the style dialog
  if(PrV alidate(fPrintRecord))

    // if user cancels style dialog, cancel all printing
    if( ! PrStlDialog(fPrintRecord)) {
      PrClose(); return;
    }
    // Always put up the job dialog, 
    // check to see if user cancels 
    if(! PrJobDialog(fPrintRecord)) {
      PrClose ();
      return;
  }

  // now open the printing port
  printPort PrOpenDoc(fPrintRecord,nil,nil);

  // open a page
  PrOpenPage(printPort,nil);

  // draw the image
  // use an empty rect to avoid unnecessary EraseRect
  Rect r;
  SetRect(&r,0,0,0,0);
  Draw(&r);

  // close the page
  PrClosePage(printPort);

  // close the printing port
  PrCloseDoc(printPort);

  // call PrPicFile for spooled printing (imagewriter) 
  if((**fPrintRecord) .prJob.bJDocLoop != 0) {
    TPrStatus status; PrPicFile(fPrintRecord,nil,nil,nil,&status);
  }
  // close the print manager 
  PrClose();
}

