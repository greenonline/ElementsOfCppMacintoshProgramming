//////////////////////////////////////////////////////////////////////////////////////////
//
// TTool.h
//
// A simple class for writing MPW tools 
//
// Include TTool.h in your tool program, 
// derive a class from TTool,
// Override SetOption to process arguments that begin with '-' 
// Override HandleArg to process all other arguments
// Override DoWork to do the actual work of the tool
//
// ©1990 Dan Weston, all rights reserved
// 
//////////////////////////////////////////////////////////////////////////////////////////
#include <Quickdraw.h> 
#include <Fonts.h> 
#include <CursorCtl.h> 
#include <iostream.h> 
#include <fstream.h> 
#include <FCntl.h>

//////////////////////////////////////////////////////////////////////////////////////////
//
// class TTool
// 
//////////////////////////////////////////////////////////////////////////////////////////


class TTool {

protected:
  int fArgc;
  int fCurrentArg; char ** fArgv; char * fProgName;
  char * fNextArg;
public:
  virtual void ITool(int argc, char* argv[]);
  virtual int Run(void);
protected:
  virtual char* GetNextArg(void);
  virtual fstream * MakeStream(char * fileName, int permission); 
  virtual int ParseArguments(void);
  virtual int SetOption(char * /*option*/) {return 1;}
  virtual int HandleArg(char * /*arg*/) {return 1;}
  virtual int DoWork(void) {return 0;}
};

//////////////////////////////////////////////////////////////////////////////////////////
//
// TTool::ITool
// 
//////////////////////////////////////////////////////////////////////////////////////////

void TTool::ITool(int argc, char* argv[]) {

  fArgc = argc;
  fArgv = argv;

  fProgName = *fArgv++;
  fCurrentArg = 1;
  fNextArg = 0;

  // just in case you want to use Quickdraw InitGraf(&qd.thePort);
  // MPW tool documentation says to call this next function 
  SetFScaleDisable(true);
  InitCursorCtl(nil);
  SpinCursor(1);
}

//////////////////////////////////////////////////////////////////////////////////////////
//
// TTool::Run
// 
//////////////////////////////////////////////////////////////////////////////////////////
int TTool::Run(void) {

  if(ParseArguments())
    return DoWork();
  else
    return 1;
}
//////////////////////////////////////////////////////////////////////////////////////////
//
// TTool::ParseArguments
// 
//////////////////////////////////////////////////////////////////////////////////////////
int TTool::ParseArguments(void) {

  char * arg;

  while ( (arg = GetNextArg ()) != 0) {
    if(*arg == '-') { 
      if(SetOption(arg) == 0)
        return 0;
    } else {
      if(HandleArg(arg) == 0)
        return 0;
    }
  }
  // signal success
  return 1;
}

//////////////////////////////////////////////////////////////////////////////////////////
//
// TTool::GetNextArg
// 
//////////////////////////////////////////////////////////////////////////////////////////
char* TTool: :GetNextArg(void) {

  if(fCurrentArg++ < fArgc) {
    fNextArg = *fArgv++;
    return fNextArg;
  } else
    return 0;
}

//////////////////////////////////////////////////////////////////////////////////////////
//
// TTool::MakeStream
// 
//////////////////////////////////////////////////////////////////////////////////////////
fstream * TTool::MakeStream(char * fileName, int permission) {

  const int BUFFSIZE = 1024;

  int fd = open(fileName,permission);
  if(fd == EOF){
    fd = creat(fileName);
  }
  if (fd != EOF) {
    char* buff= new char[BUFFSIZE];
    if (buff == 0) {
      cerr << "### " << "error making stream\n";
      return 0;
    }
    fstream * fs = new fstream(fd,buff, BUFFSIZE);
    if(fs == 0){
      cerr << "### " << "error making stream\n";
      return 0;
    }
    return fs;
  }
  cerr << "### " << "error opening file " << fileName <<"\n"; 
  return 0; // failed to open file
}



