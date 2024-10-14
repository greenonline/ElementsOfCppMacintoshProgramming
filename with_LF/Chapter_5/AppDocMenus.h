#define __USE_DEFINE__

////////////////////////////////////////////////////////////////////////////////////////////// 
//
// Menu ID constants shared by TApp and TDoc
// 
////////////////////////////////////////////////////////////////////////////////////////////// 

#ifdef __USE_CONST__
const short rMenuBarID = 128;         /* application's menu bar */

const short rAppleMenu = 128;    /* Apple menu */
const short iAbout = 1;

const short rFileMenu = 129;   /* File menu */
const short iNew = 1; 
const short iOpen = 2; 
// -----
const short iClose = 4; 
const short iSave = 5; 
const short iSaveAs = 6; 
//----
const short iPageSetup = 8;
const short iPrint = 9; 
//----
const short iQuit = 11;

const short rEdit = 130;       /* Edit menu */
const short iUndo = 1;
//----
const short iCut = 3;
const short iCopy = 4; 
const short iPaste = 5; 
const short iClear = 6;
//----
const short iSelectAll = 8;
#endif
 
 
#ifdef __USE_DEFINE__ 
#define rMenuBarID 128         /* application's menu bar */

#define rAppleMenu 128    /* Apple menu */
#define iAbout  1

#define rFileMenu 129   /* File menu */
#define iNew 1 
#define iOpen 2 
// -----
#define iClose 4
#define iSave 5 
#define iSaveAs 6
//----
#define iPageSetup 8
#define iPrint 9 
//----
#define iQuit 11

#define rEdit 130       /* Edit menu */
#define iUndo 1
//----
#define iCut 3
#define iCopy 4 
#define iPaste 5 
#define iClear 6
//----
#define iSelectAll  8
#endif
 
