/* modelessapp.rsrc.r
*
* rez source for modelessapp resources
*
* © 1990 Dan Weston All rights reserved
*
* Build it with the following rez command
*
* rez modelessapp.rsrc.r -o modelessapp.rsrc -t rsrc -c RSED */
*
*/

#include "types.r"

resource 'DITL' (1000){
  {
    /* array DITLarray: 2 elements */
    /* [l] */
    {99, 194, 119, 254},
    Button {
      enabled,
      "OK"
    },
    /* [2] */
    {11, 33, 135, 177),
    Useritem {
      enabled
    }
  }
};

resource 'DLOG' (1000) { 
  {40, 40, 190, 324},
  documentProc, invisible, goAway,
  0x0,
  1000,
  "New Dialog"
};