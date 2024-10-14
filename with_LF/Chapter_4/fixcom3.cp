////////////////////////////////////////////////////////////////
//
// fixcom3.cp
//
// Changes C++ style comments to C comments
//
// Uses MPW command line for input and output file
//
// invoke with input file name and -o output file names:
//
// fixcorn3 foo.cp -o foo.c
//
// ©1990 Dan Weston, all rights reserved
// ////////////////////////////////////////////////////////////////

#include "TTool.h"

class TFixComment3 : public TTool {

protected:
  istream *fIn;
  ostream *fOut; 

public:
  void ITool(int argc, char* argv[]);
  int DoWork(void); 

protected:
  int SetOption(char *option);
  int HandleArg(char * arg);
  int Filter(istream& in,ostream& out);
};


void TFixComment3::ITool(int argc, char* argv[]) { 
  // do the inherited stuff first
  TTool::ITool(argc, argv) ;
  // hook up default input and output
  fIn = &cin; 
  fOut = &cout;
}

int TFixComment3::DoWork(void) { 
  return Filter(*fIn,*fOut);
}

int TFixComment3::SetOption(char *option){ 
  char * arg;

  if(*(++option) == 'o'){
    // get the output file name 
    if((arg = GetNextArg()) != 0){
      // open the file and create a stream for it
      fstream * fs = MakeStream(arg,O_WRONLY);
      if(fs != 0) {
        fOut = fs;
        return 1; 
      } else
        return 0;
    } else{
      cerr << "### " << " missing file name\n";
      return 0;
    }
  } else {
    cerr << "### "<<option<< " is not a valid option\n"; 
    return 0;
  }
}



int TFixComment3::HandleArg (char * arg) {
  // open the file and create a stream for it
  fstream * fs = MakeStream(arg, O_RDONLY);
  if(fs != 0) {
    fIn = fs;
    return 1;
  } else return 0;
}


int TFixComment3::Filter(istream& in, ostream& out) {

  char c;
  char nextc;

  while (in.get(c)) {
    SpinCursor(1);
    if (c !='/'){
      // most chars just pass right through filter 
      out << c;
    } else {
      // this may be a double slash comment... 
      // check next char following first '/'
      in.get(nextc);
      if (nextc != '/'){
        // not a double slash comment,
        // just output the '/' and the following char
        out << c << nextc;
      } else {
        // it is a double slash comment,
        // substitute opening C comment 
        out<< '/' << '*';

        // pass chars through until end of line
        in.get(c);
        while (c != '\n'){
          out << c;
          in.get (c);
        }

        // now insert a closing comment
        out << ' ' << '*' << '/';

        // and send the newline char out too
        out << c;

      } // end nextc != '/' else
    } // end of c != '/' else
  } // end while

  // make sure all output is flushed 
  out << flush;
  return 0;
}

int main(int argc, char* argv[]) { 

  TFixComment3 fixComTool;

  fixComTool.ITool(argc,argv);
  return fixComTool.Run();
}