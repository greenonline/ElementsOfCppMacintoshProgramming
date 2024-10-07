//////////////////////////////////////////////////////////////// //
// fixcom.cp
//
// Changes C++ style comments to C comments 
//
// Uses cin and cout streams
//
// invoke with redirection from MPW, such as 
//
// fixcom < foo.cp > foo.fixed
//
// ©1989 Dan Weston, all rights reserved
// ////////////////////////////////////////////////////////////////
#include <iostream.h> 

int main (void) {
  char c;
  char nextc;

  while (cin.get(c)){
    if (c != 'I'l{
      // most chars just pass right through filter cout << c;
    else {
      // this may be a double-slash comment... 
      // check next char following first '/' 
      cin.get(nextc);
      if (nextc != 'I') {
        // not a double-slash comment,
        // just output the 'I' and the following char 
        cout << c << nextc;

      } else {
        // it is a double-slash comment,
        // substitute opening C comment
        cout << 'I' << '*';
        // pass chars through until end of line 
        cin.get(c);
        while (c != '\n'){
          cout << c; 
          cin.get(c);
        }
        // now insert a closing comment 
        cout<< ' ' << '*' <<'/';
        // and send the newline char out too
        cout << c;
      } // end nextc != '/' else
    } // end c != '/' else 
  } // end while
  // make sure a// output is flushed 
  cout << flush;
  return O;
}