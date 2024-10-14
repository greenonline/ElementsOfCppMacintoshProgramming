/* PictView.rsrc.r 
*
* rez source for PictView resources 
*
* © 1990 Dan Weston All rights reserved 
*
* Build it with the following rez command 
*
* rez PictView.rsrc.r -o PictView.rsrc -t rsrc -c RSED *
*/

#include "types.r"

resource 'MENU' (128, preload){
  128,
  textMenuProc, 0x7FFFFFFD, enabled, apple,
  { /* array: 2 elements */
    /* [1] */
    "About PictView ... ", noIcon, noKey, noMark, plain,
    /* [2] */
    "-", noIcon, noKey, noMark, plain

resource 'DITL' (128, purgeable){
  {
    /* array DITLarray: 5 elements */
    /* [1l */
    {76, 195, 96, 275},
    Button {
      enabled,
      "OK"
    },
    /* [2] */
    {9, 10, 48, 292},
    StaticText {
      disabled,
      "PictView: a simple application to view "
      "PICT files."
    },
    /* [3] */
    {55, 199, 70, 245},
    StaticText {
      disabled, 
      ""
    },
    /* [4] */
    {56, 21, 72, 149},
    StaticText {
      disabled, 
      "version 1.0"
    },
    /* [5] */
    {75, 23, 99, 154},
    StaticText {
      disabled, 
      "March 15, 1990"
    }
  }
};


resource 'ALRT' (128, purgeable) {
  {68, 76, 172, 376},
  128,
  { /* array: 4 elements */
    /* [l] */
    OK, visible, silent,
    /* [2] */
    OK, visible, silent,
    /* [3] */
    OK, visible, silent,
    /* [4] */
    OK, visible, silent
};



