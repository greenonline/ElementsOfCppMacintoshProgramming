#---------------------------------------------------------------- # Make file for a simple program using TTEDoc
# To use it, use the MPW "Build ... " command from the build menu, # specifying "TEApp" and the target file

#© 1990 Dan Weston, All rights reserved

# tell cplus and rez where to find included files for TApp,TDoc, 
# TScrollDoc, and TTEDoc
AppObjectDir = ::App-Doc:
ScrollObjDir = ::TScrollDoc:
TEObjDir = ::TEDoc:

# use SADE symbol generation, -sym off will result in faster builds
SymOpts = -sym on

# options for C++, where to look for include files 
CPlusOptions = {SymOpts} ∂
-i "{AppObjectDir}"∂
-i "{TEObjDir}"∂
-i "{ScrollObjDir}"

# options for the linker 
LinkOptions = -msg nodup {SymOpts}

# options for rez, where to look for include and #include files RezOptions = -s "{AppObjectDir}" -s "{ScrollObjDir}"∂
-i "{AppObjectDir}" -i "{ScrollObjDir}"

# We need to change this rule to include CPlusOptions 
.cp.o ƒ .cp
   CPlus {default}.cp -o {default}.cp.o {CPlusOptions}

Objects = ∂
 "{AppObjectDir}"TApp.cp.o ∂
 "{AppObjectDir}"TDoc.cp.o ∂
 "{ScrollObjDir}"TScrollDoc.cp.o ∂
 "{TEObjDir}"TTEDoc.cp.o ∂
 TEApp.cp.o

ResourceFiles = ∂
"{AppObjectDir}"TApp.rsrc ∂
"{AppObjectDir}"TDoc.rsrc ∂
"{ScrollObjDir}"TScrollDoc.rsrc ∂
TEApp.rsrc

# dependency rules for TDoc and TApp 
"{AppObjectDir}"TDoc.cp.o ƒ "{AppObjectDir}"TDoc.cp ∂
"{AppObjectDir}"TDoc.h ∂
"{AppObjectDir}"AppDocMenus.h



"{AppObjectDir}"TApp.cp.o ƒ "{AppObjectDir}"TApp.cp ∂ "{AppObjectDir}"T App.h ∂
"{AppObjectDir}"TDoc.h ∂
"{AppObjectDir}"AppDocMenus.h
# dependency rules for TScrollDoc 
"{ScrollObjDir}"TScrollDoc.cp.o ƒ ∂
"{ScrollObjDir}"TScrollDoc.cp ∂
"{ScrollObjDir}"TScrollDoc.h ∂
"{AppObjectDir}"TDoc.h

# dependency rules for TTEDoc 
"{TEObjDir}"TTEDoc.cp.o ƒ "{TEObjDir}"TTEDoc.cp ∂
"{TEObjDir}"TTEDoc.h ∂
 "{ScrollObjDir}"TScrollDoc.h ∂
 "{AppObjectDir}"TDoc.h

# dependency rules for TEApp TEApp.cp.o ƒ TEApp.cp ∂
"{AppObjectDir}"T App.h ∂
 "{AppObjectDir}"TDoc.h ∂
 "{ScrollObjDir}"TScrollDoc.h ∂
 "[TEObjDir}"TTEDoc.h ∂
 TEApp.make

TEApp ƒƒ {Objects} TEApp.make
    Link -o {Targ} {LinkOptions} ∂
{Objects} a "{CLibraries}"CPlusLib.o ∂
 "{CLibraries}"CRuntime.o ∂
 "{CLibraries}"StdCLib.o ∂
 "{CLibraries}"Cinterface.o ∂
 "{Libraries}"Interface.o
    SetFile {Targ} -t APPL -c '????' -a B

TEApp ƒƒ TEApp.r ∂
         {ResourceFiles}
Rez -append -o {Targ} {RezOptions} TEApp.r


