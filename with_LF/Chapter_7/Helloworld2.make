#-----------------------------------------------------------------------
# Make file for the simplest program using the App and Doc objects
# To use it, use the MPW "Build ... " command from the build menu,
# specifying "Helloworld2" and the target file

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
    CPlus {default}.cp -o {default}.cp.o {CPlusOptions}

Objects = ∂
    "{AppObjectDir}"T App.cp.o ∂ 
    "{AppObjectDir}"TDoc.cp.o ∂ 
    Helloworld2.cp.o

ResourceFiles = ∂
    "{AppObjectDir}"T App.rsrc ∂
    "{AppObjectDir}"TDoc.rsrc ∂
    Helloworld2.rsrc

# dependency rules for TDoc and TApp 
"{AppObjectDir)"TDoc.cp.o ƒ "{AppObjectDir}"TDoc.cp ∂
                            "{AppObjectDir}"TDoc.h ∂
                            "{AppObjectDir}"AppDocMenus.h
"{AppObjectDir}"TApp.cp.o ƒ "{AppObjectDir}"TApp.cp ∂
                            "{AppObjectDir}"TApp.h ∂ 
                            "{AppObjectDir}"TDoc.h ∂ 
                            "{AppObjectDir}"AppDocMenus.h

# dependency rules for Helloworld2 
Helloworld2.cp.o ƒ Helloworld2.cp ∂
          "{AppObjectDir}"T App.h ∂
           "{AppObjectDir}"TDoc.h ∂
           Helloworld2.make

Helloworld2 ƒƒ {Objects} Helloworld2.make 
    Link -o {Targ} {LinkOptions} ∂
         {Objects} ∂
         "{CLibraries}"CPlusLib.o ∂
         "{CLibraries}"CRuntime.o ∂
         "{CLibraries}"StdCLib.o ∂
         "{CLibraries}"Cinterface.o ∂
         "{L ibraries}"Interface.o
    SetFile {Targ} -t APPL -c '????' -a B

Helloworld2 ƒƒ Helloworld2.r ∂
        {ResourceFiles} ∂
        Helloworld2.make
    Rez -append -o {Targ} {RezOptions} Helloworld2.r
