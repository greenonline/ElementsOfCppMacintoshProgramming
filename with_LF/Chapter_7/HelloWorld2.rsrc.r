/* HelloWorld2.rsrc.r
*
* rez source for HelloWorld2 resources
*
* © 1990 Dan Weston All rights reserved
* 
* Build it with the following rez command
*
* rez HelloWorld2.rsrc.r -o HelloWorld2.rsrc -t rsrc -c RSED
* 
*/

#include "types.r"

resource 'ALRT' (128, purgeable) {
  {68, 76, 172, 376),
  128,
  {
    /* array: 4 elements */ /* [1] */
    OK, visible, silent,
    /* [2] */
    OK, visible, silent,
    /* [3] */
    OK, visible, silent, 
    /* [4] */
    OK, visible, silent
};

resource 'DITL' (128, purgeable){
  {
    /* array DITLarray: 5 elements */ 
    /* [1] */
    {65, 199, 85, 279),
    Button {
      enabled,
      "OK" 
    },
    /* [2] */
    {8, 8, 24, 304),
    StaticText {
      disabled,
      "Helloworld2: a very simple application." 
    },
    /* [3] */
    {44, 199, 59, 245),
    StaticText {
      disabled,
      ""
    },
    /* [4] */
    {42, 34, 58, 162},
    StaticText {
      disabled, 
      "version 1.0"
    },
    /* [5] */
    (65, 34, 89, 165},
    StaticText {
      disabled,
      "January 15, 1990"
    }
  }
};

resource 'MENU' (128, preload) {
  128,
  textMenuProc, 
  0x7FFFFFFD, 
  enabled,
  apple,
  { /* array: 2 elements */
    /* [1] */
    "About Helloworld2", no!con, noKey, noMark, plain,
    /* [2] */
    "-", no!con, noKey, noMark, plain
  }
};

resource 'WIND' (1000) {
  (40, 40, 152, 368},
  zoomDocProc,
  invisible,
  goAway,
  OxO,
  Untitled
};