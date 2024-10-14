#------------------------------------------------------------------ 
# Make file for the Scribble program using the App and Doc objects 
# To use it, use the MPW "Build ... " command from the build menu,
# specifying "Scribble" and the target file
#© 1990 Dan Weston, All rights reserved

# tell cplus and rez where to find included files for TApp and TDoc
AppObjectDir = : :App-Doc:

# use SADE symbol generation, -sym off will result in faster builds
SymOpts = -sym on

# options for C++, where to look for include files
CPlusOptions = {SymOpts} -i "{AppObjectDir}" 

# options for the linker
LinkOptions = -msg nodup {SymOpts}

# options for rez, where to look for include and #include files 
RezOptions = -s "{AppObjectDir}" -i "{AppObjectDir}"

# We need to change this rule to include CPlusOptions 
.cp.o ƒ .cp
  CPlus {default}.cp -o {default} .cp.o {CPlusOptions}
.cp.o ƒ .h
  CPlus {default}.cp -o {default}.cp.o {CPlusOptions}

Objects = ∂
  "{AppObjectDir}"TApp.cp.o ∂ 
  "{AppObjectDir}"TDoc.cp.o ∂ 
  Scribble.cp.o

ResourceFiles = ∂ 
  "{AppObjectDir}"TApp.rsrc ∂
  "{AppObjectDir}"TDoc.rsrc ∂ 
  Scribble.rsrc

# dependency rules for  TDoc and TApp

"{AppObjectDir}"TDoc.cp.o ƒ "{AppObjectDir}"TDoc.cp ∂
                            "{AppObjectDir}"TDoc.h ∂
                            "{AppObjectDir}"AppDocMenus.h

"{AppObjectDir}"TApp.cp.o ƒ "{AppObjectDir}"TApp.cp ∂ 
                            "{AppObjectDir}"TApp.h ∂
                            "{AppObjectDir}"TDoc.h ∂
                            "{AppObjectDir}"AppDocMenus.h



# dependency rules for Scribble

Scribble.cp.o ƒ Scribble.cp ∂
            "{AppObjectDir}"T App.h ∂
            "{AppObjectDir}"TDoc.h ∂
            Scribble.make


Scribble ƒƒ {Objects} Scribble.make
    Link -o {Targ} {LinkOptions} ∂
         {Objects} d 
         "{CLibraries}"CPlusLib.o ∂
         "{CLibraries}"CRuntime.o ∂
         "{CLibraries}"StdCLib.o ∂
         "{CLibraries}"Cinterface.o ∂
         "{Libraries}"Interface.o
    SetFile {Targ} -t APPL -c SCBL -a B

Scribble ƒƒ Scribble.r ∂
         {ResourceFiles}
    Rez -append -o {Targ} {RezOptions} Scribble.r





