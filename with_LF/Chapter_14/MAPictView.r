/* • Auto-Include the requirements for this source */ 

#ifndef __TYPES.R__
#include "Types.r"
#endif

#ifndef __MacAppTypes__
#include "MacAppTypes.r" 
#endif

#if qTemplateViews 
#ifndef ~ViewTypes 
#include "ViewTypes.r" 
#endif
#endif

#if qDebug
include "Debug.rsrc"; 
#endif
include "MacApp.rsrc"; 
include "Printing.rsrc";

include $$Shell("ObjApp")"MAPictView" 'CODE';

/* Resource ids */

/* The 'File is too large' alert */ 
#define kFileTooBig 1000
/* resource ID of window */
#define kWindowRsrcID 1004

resource 'view' (kWindowRsrcID, purgeable){
  {

  root, 'WIND', { 50, 40 }, { 250, 450 }, sizeVariable, sizeVariable, shown, enabled,
  Window {
    "",
      zoomDocProc, goAwayBox, resizable, modeless, ignoreFirstClick, freeOnClosing, disposeOnFree, closesDocument, openWithDocument, dontAdaptToScreen, stagger, forceOnScreen, dontCenter, noID, "" };

  'WIND', 'SCLR', {0,0}, {250-kSBarSizeMinus1, 450-kSBarSizeMinus1),
sizeRelSuperView, sizeRelSuperView, shown, enabled,
Scroller {
    "",
      vertScrollBar, horzScrollBar, 0, 0, 16, 16,
      vertConstrain, horzConstrain, { 0, 0, 0, 0 } };

  'SCLR', 'PicV', { 0, 0 }, { 116, 1020 },
          sizeVariable, sizeVariable, shown, enabled,
  View {"TPICTView"}
  }
};

resource 'SIZE' (-1) {
  saveScreen,
  acceptSuspendResumeEvents, enableOptionSwitch,
  canBackground, MultiFinderAware,
  backgroundAndForeground,
  dontGetFrontClicks,
  ignoreChildDiedEvents,
  is32BitCompatible,
  reserved,
  reserved,
  reserved,
  reserved,
  reserved,
  reserved,
  reserved,
#if qdebug
  1024 * 1024,
  760 * 1024 
#else
  1024 * 1024,
  760 * 1024 
#endif
}; 

/*
Printing to the LaserWriter is the time when the most temporary memory is in use. We need the segments in use at that time
*/ 
resource 'seg ! ' {256, purgeable){
  {
    "GWriteFile";
    "GClipboard";
    "GNonRes";
    "GFile";
    "GSelCommand";
    "GTerminate";
    "GClose";
    "GDoCommand";
  }
};

resource 'DITL' (phAboutApp, purgeable) {
  {
    /* [1] */
    {160, 182, 180, 262},
    Button {
      enabled,
      "OK" };
    /* [2] */
    {10, 75, 150, 320},
    StaticText {
      disabled,
      "This sample program views \nPICT files."
      "\n\nThis program was written "
      "with MacApp® © 1990 Dan Weston,"
      "\n© 1985-1989 Apple Computer, Inc."
    };
    /* [3] */
    {10, 20, 42, 52},
    Icon { 
      disabled,
      1
    }
  }
};

// Grab the default about box
include "Defaults.rsrc" 'ALRT' (phAboutApp) ;

/*
  Used when the user attempts to
  read a file larger than we can handle
*/

resource 'DITL' (kFileTooBig, purgeable) {
  {
    /* [1] */
    {82, 198, 100, 272},
    Button {
      enabled,
      "OK"
    };
    /* [2] */
    {10, 70, 77, 272},
    StaticText {
      disabled,
      "PICT View can't read the entire " "file because it is too long."
    };
    /* [3] */
    {10, 20, 42, 52},
    Icon {
      disabled,
      0
    }
  }
};

resource 'ALRT' (kFileTooBig, purgeable) {
  {100, llO, 210, 402},
  kFileTooBig,
  {
  /* [1] */ OK, visible, silent; 
  /* [2] */ OK, visible, silent; 
  /* [3] *I OK, visible, silent; 
  /* [4) */ OK, visible, silent
  } 
};

II Grab the default Apple/File menus 
include "Defaults.rsrc" 'cmnu' (mApple); 
include "Defaults.rsrc" 'cmnu' (mFile); 
include "Defaults.rsrc" 'cmnu' (mEdit);

/* Displayed menus on a non-hierarchical system */ 
resource 'MBAR' (kMBarDisplayed){
  {mApple; mFile; mEdit}
};

// Grab the default credits
include "Defaults.rsrc" 'STR#' (kDefaultCredits);

// Get the default MacApp® application icon 
// and necessary bundling rsrcs
include "Defaults.rsrc" 'MApp' (0);
include "Defaults.rsrc" 'FREF' (128);
include "Defaults.rsrc" 'BNDL' (128);
include "Defaults.rsrc" 'ICN#' (128);
   
// Get the default Version resources 
// Application or file specific 
include "Defaults.rsrc" 'vers' (1);

// Overall package
include "Defaults. rsrc" 'vers' (2);




