//////////////////////////////////////////////////////////////////
//
// UMAPICTView.cp
// This is the implementation of the application objects for the
// MAPictView program
//
// The interface for these objects is in UMAPICTView.h
//
//© 1990 Dan Weston, All Rights Reserved
// 
//////////////////////////////////////////////////////////////////

#include <UMacApp.h> 
#include <UPrinting.h>

#include <ToolUtils.h> 

#include <UMAPictView.h>

//////////////////////////////////////////////////////////////////
//
// constants
// 
//////////////////////////////////////////////////////////////////

// application signature
const OSType kSignature = 'DANW';

// file-type code for saved disk files 
const OSType kFileType = 'PICT';

//'view' template for a PICTView window 
const short kWindowRsrcID = 1004;

// how much to stagger doc windows 
const short kStaggerAmount = 16;

// ALRT for file-too-big
const short kFileTooBig 1000;

// size of PICT file header
const long kPictHeaderSize = 512;

//////////////////////////////////////////////////////////////////
//
// globals
// 
//////////////////////////////////////////////////////////////////

short gStaggerCount;



#pragma segment Ainit
//////////////////////////////////////////////////////////////////
//
// TPICTViewApp::IPICTViewApp
// 
//////////////////////////////////////////////////////////////////

pascal void TPICTViewApp::IPICTViewApp(void){
  IApplication(kFileType);
  gStaggerCount = 0;

  // So the linker doesn't dead strip class info. 
  // (gDeadStripSuppression is never true, so the 
  // code never actually executes at run time.) 
  if( gDeadStripSuppression) {
    TPICTView *aPICTView;
    aPICTView = new TPICTView;
  }
}

#pragma segment AOpen
//////////////////////////////////////////////////////////////////
//
// TPICTViewApplication::DoMakeDocument
// 
//////////////////////////////////////////////////////////////////
pascal struct TDocument *TPICTViewApp::DoMakeDocument(CmdNumber /*itsCmdNumber*/){

  TPICTDocument *aPICTDocument;

  aPICTDocument = new TPICTDocument;
  FailNIL(aPICTDocument);
  aPICTDocument->IPICTDocument();
  return aPICTDocument;
}

//#pragma segment AOpen
//////////////////////////////////////////////////////////////////
//
// TPICTViewApplication::OpenNew
// 
//////////////////////////////////////////////////////////////////

pascal void TPICTViewApp::OpenNew( CmdNumber /*itsCmdNumber*/) {
  AppFile anAppFile;

  if (ChooseDocument( cFinderOpen, &anAppFile ))
   OpenOld( cFinderOpen, &anAppFile );
}

#pragma segment ARes
//////////////////////////////////////////////////////////////////
//
// TPICTViewApp::DoSetupMenus
// 
//////////////////////////////////////////////////////////////////
pascal void TPICTViewApp::DoSetupMenus(void) {
  inherited::DoSetupMenus();

  Enable(cNew,false);
}

#pragma segment AOpen
//////////////////////////////////////////////////////////////////
//
// TPICTDocument::IPICTDocument
// 
//////////////////////////////////////////////////////////////////
pascal void TPICTDocument::IPICTDocument(void) {

  // do the inherited stuff
  IDocument(kFileType,
            kSignature, kUsesDataFork,
            ! kUsesRsrcFork, kDataOpen,
            ! kRsrcOpen);

  // and now do our specific members
  fPICTData = nil;
  fPICTHeader = nil;
}

#pragma segment AClose
//////////////////////////////////////////////////////////////////
//
// TPICTDocument::Free
// 
//////////////////////////////////////////////////////////////////
pascal void TPICTDocument::Free(void) {
  if(fPICTData !=nil) {
    DisposHandle(fPICTData);
    fPICTData = nil;
  }
  if(fPICTHeader != nil) {
    DisposHandle(fPICTHeader);
    fPICTHeader = nil;
  inherited::Free();
}

#pragma segment AOpen
//////////////////////////////////////////////////////////////////
//
// TPICTDocument::DoMakeViews
// 
//////////////////////////////////////////////////////////////////

pascal void TPICTDocument::DoMakeViews(Boolean /*forPrinting*/) {

  TView *theWindow,*thePictView;
  TStdPrintHandler *aHandler;

  theWindow = NewTemplateWindow(kWindowRsrcID, this); 

  FailNIL(theWindow);
  thePictView = theWindow->FindSubView('PicV');

  aHandler = new TStdPrintHandler;
  FailNIL(aHandler);
  aHandler->IStdPrintHandler(this,
                             thePictView,
                             !kSquareDots,
                             kFixedSize,
                             !kFixedSize);
                             ShowReverted();

}

#pragma segment AReadFile
//////////////////////////////////////////////////////////////////
//
// TPICTDocument::DoRead
// 
//////////////////////////////////////////////////////////////////
pascal void TPICTDocument::DoRead(short aRefNum,
Boolean /*rsrcExists*/, Boolean /*forPrinting*/} {

  long pictSize;
  long headerSize = kPictHeaderSize;

  // calculate size of file, subtract header size 
  FailOSErr(GetEOF(aRefNum, &pictSize));
  pictSize = pictSize - kPictHeaderSize;




  // allocate memory for header and pict 
  fPICTHeader = NewPermHandle(kPictHeaderSize); 
  FailNIL(fPICTHeader);

  fPICTData = NewPermHandle(pictSize);
  FailNIL(fPICTData);

  // now read header and pict
  HLock(fPICTHeader);
  FailOSErr(FSRead(aRefNum, &headerSize, *fPICTHeader)); 
  HUnlock(fPICTHeader);

  HLock(fPICTData);
  FailOSErr(FSRead(aRefNum, &pictSize, *fPICTData) ); 
  HUnlock(fPICTData);
}

#pragma segment ARes
//////////////////////////////////////////////////////////////////
//
// TPICTDocument::DoSetupMenus
// 
//////////////////////////////////////////////////////////////////
pascal void TPICTDocument::DoSetupMenus(void) {
  inherited::DoSetupMenus();
  Enable(cSaveAs, false);
  Enable(cSaveCopy, false);
}

#pragma segment A Fields
//////////////////////////////////////////////////////////////////
//
// TPICTDocument: :Fields
// 
//////////////////////////////////////////////////////////////////
pascal void TPICTDocument::Fields(pascal void (*DoToField) (
StringPtr fieldName,
Ptr fieldAddr,
short fieldType,
void *DoToField_StaticLink), void *DoToField_StaticLink) {

  DoToField("\pTPICTDocument", nil, bClass,DoToField_StaticLink);
  DoToField ("\pfPICTData", (Ptr) &fPICTData, bHandle,DoToField_StaticLink);
  DoToField("\pfPICTHeader", (Ptr) &fPICTHeader, bHandle,DoToField_StaticLink);

  inherited::Fields(DoToField,DoToField_StaticLink);
}

#pragma segment ARes
//////////////////////////////////////////////////////////////////
//
// TPICTView::CalcMinSize
// 
//////////////////////////////////////////////////////////////////
pascal void TPICTView::CalcMinSize(VPoint *minSize) {

  short hSize, vSize;
  TPICTDocument * PICTDoc (TPICTDocument *)fDocument;

  if(PICTDoc->fPICTData) {
    hSize = (**((PicHandle)PICTDoc->fPICTData)) .picFrame.right
- (**((PicHandle)PICTDoc->fPICTData)) .picFrame.left;
    vSize = (**((PicHandle)PICTDoc->fPICTData)) .picFrame.bottom
- (**((PicHandle)PICTDoc->fPICTData)) .picFrame.top; 
    SetVPt(minSize, hSize, vSize);
  } else
    SetVPt(minSize,0,0);
}

#pragma segment ARes
//////////////////////////////////////////////////////////////////
//
// TPICTView::Draw
// 
//////////////////////////////////////////////////////////////////
pascal void TPICTView::Draw(Rect */*area*/) {

  TPICTDocument * PICTDoc = (TPICTDocument *)fDocument;

  if(PICTDoc->fPICTData)
    DrawPicture((PicHandle) (PICTDoc->fPICTData),
                &(**((PicHandle)PICTDoc->fPICTData)) .picFrame);
}