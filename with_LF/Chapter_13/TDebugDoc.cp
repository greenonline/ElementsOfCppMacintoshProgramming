///////////////////////////////////////////////////////////////////
//
// TDebugDoc.cp
//
// A document/window that draws stream text in the window
//
// © 1990 Dan Weston, All Rights Reserved
// 
///////////////////////////////////////////////////////////////////
#include <Types.h> 
#include <QuickDraw.h> 
#include <Events.h> 
#include <Windows.h> 
#include <OSUtils.h> 
#include <Strings.h>
#include "TApp.h" 

#include "TDebugDoc.h"

// define a segment for DebugDoc 
//#pragma segment DebugDocSeg

///////////////////////////////////////////////////////////////////
//
// TDebugDoc::TDebugDoc
// 
///////////////////////////////////////////////////////////////////
TDebugDoc::TDebugDoc(TWindowStreamBuff *buff, OSType theCreator, SFReply * SFinfo) : TTEDoc(theCreator,SFinfo),ostream(buff) {
    // save a reference to the streambuffer
    // so we can disable it when the window closes 
  fBuff = buff;
}

///////////////////////////////////////////////////////////////////
//
// TDebugDoc: :~TDebugDoc
// 
///////////////////////////////////////////////////////////////////
TDebugDoc::~TDebugDoc(void) (
  // disable the streambuff so it won't 
  // try to output to a deleted document
  fBuff->fTEDoc = nil;
}

///////////////////////////////////////////////////////////////////
//
// MakeDebugDoc: A utility routine to make a debugging document
// 
///////////////////////////////////////////////////////////////////
TDebugDoc * MakeDebugDoc(TApp * theApp) {
  // grab some memory for the stream buffer 
  char* theBuffer =new char[kBufferSize];
  if(!theBuffer) return nil;

  // create the streambuffer
  TWindowStreamBuff *buff= new TWindowStreamBuff(theBuffer,

  // and pass it to the new DebugDoc's constructor 
  TDebugDoc *tem p = new TDebugDoc(buff);

  if(! temp)
    return nil;

  // make the window
  if( temp->MakeWindow(theApp->fenvRec.hasColorQD) &&
temp->InitDoc()) {
      temp->ShowDocWindow();
      theApp->AddDocument(temp);
      // connect the streambuff to the TEDocument 
      buff->fTEDoc temp;
      return temp;
  } else
      return nil;
}

///////////////////////////////////////////////////////////////////
//
// TWindowStreamBuff::TWindowStreamBuff
// 
///////////////////////////////////////////////////////////////////
TWindowStreamBuff::TWindowStreamBuff(char *p, int len):

  streambuf(p,len) {
  fTEDoc = nil;
}

///////////////////////////////////////////////////////////////////
//
// TWindowStreamBuff::overflow
//
///////////////////////////////////////////////////////////////////

int TWindowStreamBuff::overflow(int c) {
  if (fTEDoc) {
    // how many chars?
    long len = pptr() - base();

    // add them to the text document 
    fTEDoc->AddText((Ptr)base(), len);

    // reset everything 
    setp(base(), epptr());
    if (c != EOF)
      sputc(c); 
    return 0;
  }
  return EOF;
}