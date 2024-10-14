//////////////////////////////////////////////////////////////// 
//
//
//
//
// fixcom2.cp
//Changes C++ style comments to C comments
//Uses MPW input and output redirection
//invoke with redirected input file and output file names 
//
// fixcom2 < foo.cp > foo.c
// ©1990 Dan Weston, all rights reserved
//
//
//
//
//
//
//
// 
//////////////////////////////////////////////////////////////// 
#include "TTool.h"

class TFixComment2 : public TTool {

public:
  int DoWork(void)
    {return Filter(cin,cout);} 
protected:
  int Filter(istream& in,ostream& out); };


int TFixComment2::Filter(istream& in,ostream& out) { 
  char c;
  char nextc;
  while (in.get(c)){
    SpinCursor(1); 
    if (c != '/'){
      // most chars just pass right through filter
      out << c; 
    } else {
      // this may be a double-slash comment... 
      // check next char following first '/' 
      in.get (nextc) ;
      if (nextc !='/'){

        // not a double-slash comment,
        // just output the '/' and the following char 
        out << c << nextc;
      } else {

        // it is a double-slash comment,
        // substitute opening C comment
        out<< 'I' << '*';

        // pass chars through until end of line 
        in.get (c);


        while (c != '\n') { 
          out << c;
          in.get(c);

          // now insert a closing comment 
          out<< ' ' << '*' << '/';

          // and send the newline char out too 
          out << c;
        }
      } // end nextc != '/' else 
    } // end of c != '/' else
  } // end while

  // make sure all output is flushed 
  out << flush;
  return 0;
}


int main(int argc, char* argv[]) { 
  TFixComment2 fixComTool; 

  fixComTool.ITool(argc,argv); 
  return fixComTool.Run();
}