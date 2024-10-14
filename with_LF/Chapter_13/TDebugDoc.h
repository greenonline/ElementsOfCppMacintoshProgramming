///////////////////////////////////////////////////////////////////
//
// TDebugDoc.h
//
// A document/window that draws stream text in the window
//
//© 1990 Dan Weston, All Rights Reserved
// 
///////////////////////////////////////////////////////////////////

#include <QuickDraw.h>
#include <Events.h> 
#include <Windows.h> 
#include <iostream.h>
#include "TTEDoc.h"

#ifndef TDebugDoc_Defs 
#define TDebugDoc_Defs

const short rDebugDoc = 5555; 
const int kBufferSize = 80;

///////////////////////////////////////////////////////////////////
//
// class TWindowStreamBuff
// 
///////////////////////////////////////////////////////////////////
class TWindowStreamBuff: public streambuf {
  public:
    TTEDoc * fTEDoc;
    int overflow(int c = EOF);
    TWindowStreamBuff(char *p, int len);
};

///////////////////////////////////////////////////////////////////
//
// class TDebugDoc
// 
///////////////////////////////////////////////////////////////////
class TDebugDoc : public TTEDoc,public ostream{
  protected:
    TWindowStreamBuff * fBuff;
  public:



    TDebugDoc::TDebugDoc(TWindowStreamBuff *buff, OSType theCreator = '????',
      SFReply * SFinfo = (SFReply *)nil); virtual -TDebugDoc(void);

    virtual short GetWinID(void) {return rDebugDoc;}

    // do this so Close menu isn't active
    // when debug window is on top
    virtual Boolean CanClose(void) { return false; };
};

// utility routine to make a DebugDoc and add it to doc list TDebugDoc * MakeDebugDoc(TApp * theApp);

#endif TDebugDoc_Defs