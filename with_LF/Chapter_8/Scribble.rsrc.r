/* Scribble.rsrc.r
*
* rez source for Scribble resources
*
* © 1990 Dan Weston All rights reserved
*
* Build it with the following rez command
*
* rez Scribble.rsrc.r -o Scribble.rsrc -t rsrc -c RSED
*
*/

#include "types.r"

resource 'MENU' (128, preload) {
  128,

  textMenuProc, 0x7FFFFFFD, enabled, apple,
  { /* array: 2 elements */
    /* [1] */
    "About Scribble ... ", noicon, noKey, noMark, plain,
    /* [2] */
    "-", noicon, noKey, noMark, plain

  }
};

resource 'MENU' (131) 131,
textMenuProc, allEnabled, enabled, "Pen",
  {
    /* array: 7 elements */ 
    /* [1] */
    "1 x 1", noIcon, noKey, check, plain,
    /* [2] */
    "2 x 2", noIcon, noKey, noMark, plain,
    /* [3] */
    "3 x 3"' noIcon, noKey, noMark, plain,
    /* [4] */
    "-", noicon, noKey, noMark, plain, 
    /* [5] */
    "Black", noicon, noKey, check, plain,
    /* [6] */
    "Gray", noicon, noKey, noMark, plain,
    /* [7] */
    "White", noicon, noKey, noMark, plain

  }
};



resource 'DITL' (128, purgeable) {
  {
    /* array DITLarray: 5 elements */
    /* [1] */
    {65, 199, 85, 279},
    Button { 
      enabled,
      "OK" 
    },
    /* [2] */
    {8, 8, 24, 304},
    StaticText { 
      disabled,
      "Scribble: a very simple application."
    },
    /* [3] */
    {44, 199, 59, 245}, 
    StaticText {
      disabled,
    },
    /* [4] */
    {42, 34, 58, 162},
    StaticText {
      disabled, "version 1.0"
    },
    /* [5] */
    {65, 34, 89, 165},
    StaticText {
      disabled,
      "February 15, 1990"
    }
  }
};

resource 'ALRT' (128, purgeable) {
  {68, 76, 172, 376},
  128,
  {
    /* array: 4 elements */ 
    /* [1] */
    OK, visible, silent,
    /* [2] */
    OK, visible, silent, 
    /* [3J */
    OK, visible, silent, 
    /* [4] */
    OK, visible, silent
  }
};




resource 'BNDL' (128) { 
  'SCBL'
  0,
  { /* array TypeArray: 2 elements */
    /* [1] */
    'ICN#',
    { /* array IDArray: 2 elements */
      /* [1] */
      0, 128,
      /* [2] */
      1, 129
    },
    /* [2] */
    'FREF'
    { /* array IDArray: 2 elements */
      /* [1] */ 
      0, 128,
      /* [2] */ 
      1, 129
    }
  }
};

resource 'FREF' (129) { 
  'SPCT',
  1,
  "",
};

resource 'FREF' (128) { 
  'APPL',
  0, 
  "",
};

resource 'ICN#' (129' preload) { 
  {
    /* array: 2 elements */
    /* [1] */
    $"0FFF FE00 0800 0300 0812 6280 089B 4E40"
    $"0889 4A20 08CB 4Al0 0ASF FBF8 OB70 1CE8"
    $"0920 0708 09C0 0228 05C0 03E8 0CC0 0118"
    $"0B0E 71F8 088E 7088 088E F088 0880 8088"
    $"0880 C088 0880 4088 0843 C088 0840 0108"
    $"0820 1108 082C 7108 0823 C308 0810 0608"
    $"0818 1C08 080F F005 0800 0008 0800 0008"
    $"0800 0008 0800 0008 0800 0008 0FFF FFF8", 
    /* [2] */
    $"0FFF FE00 0FFF FF00 0FFF FF80 0FFF FFC0"
    $"0FFF FFE0 0FFF FFF0 0FFF FFF8 0FFF FFF8"
    $"0FFF FFF8 0FFF FFF8 0FFF FFF8 0FFF FFF8"
    $"0FFF FFF8 0FFF FFF8 0FFF FFF8 0FFF FFF8"
    $"0FFF FFF8 0FFF FFF8 0FFF FFF8 0FFF FFF8"
    $"0FFF FFF8 0FFF FFF8 0FFF FFF8 0FFF FFF8"
    $"0FFF FFF8 0FFF FFF8 0FFF FFF8 0FFF FFF8"
    $"0FFF FFF8 0FFF FFF8 0FFF FFF8 0FFF FFF8"
  }
};

resource 'ICN#' (128, pre load) {
  {
    /* array: 2 elements */
    /* [1] */
    $"0001 0000 0002 8000 0004 4000 0008 2000"
    $"0010 1000 0025 4800 0055 5400 0097 0200"
    $"015C 7500 0270 3480 0540 1C40 09C0 0820"
    $"1680 0E10 2390 C808 4110 FF04 8090 C082"
    $"4086 8041 2085 3022 1087 C814 087E 7F8F"
    $"044A 3007 0227 0007 0111 8007 008F E007"
    $"0040 1FE7 0020 021F 0010 0407 0008 0800"
    $"0004 1000 0002 2000 0001 4000 0000 80",
    /* [2] */
    $"0001 0000 0003 8000 0007 C000 000F E000"
    $"001F F000 003F F800 007F FC00 D0FF FE00"
    $"01FF FF00 03FF FF80 07FF FFC0 0FFF FFE0"
    $"1FFF FFF0 3FFF FFF8 7FFF FFFC FFFF FFFE"
    $"7FFF FFFF 3FFF FFFE 1FFF FFFC 0FFF FFFF"
    $"07FF FFFF 03FF FFFF 01FF FFFF 00FF FFFF"
    $"007F FFFF 003F FElF 001F FC07 000F F800"
    $"0007 F00D 0003 E000 0001 C000 0000 80"
  }
};

resource 'MBAR' (128) {
  {
    /* array MenuArray: 4 elements */ 
    /* [l] */
    128,
    /* [2] */
    129,
    /* [3] */
    130,
    /* [4] */
    131
  }
};

data 'SCBL' (0) {
    $"1F53 6372 6962 626C 652C 4665 6220 3135" 
    /* .Scribble,Feb 15 */
    $"2C31 3939 3020 4461 6E20 5765 7374 6F6E" 
    /* ,1990 Dan Weston */
};

