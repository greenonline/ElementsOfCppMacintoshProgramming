#------------------------------------------------------------------ # Make file for the simplest program using the ModelessDoc objects #T o use it, use the MPW "Build ... " command from the build menu,
# specifying "ModelessApp" and the target file
#© 1990 Dan Weston, All rights reserved
# tell cplus and rez where to find included files for TApp,TDoc, # and TModelessDoc
AppObjectDir = ::App-Doc:
ModelessObjDir = ::ModelessDoc:
# use SADE symbol generation, -sym off will result in faster builds SymOpts = -sym on
# options for C++, where to look for include files
CPlusOptions = {SymOpts} -i "{AppObjectDir}" -i "{ModelessObjDir}"
# options for the linker LinkOptions = -msg nodup {SymOpts}
# options for rez, where to look for include and #include files RezOptions = -s "{AppObjectDir}" -i "{AppObjectDir}"
# We need to change this rule to include CPlusOptions .cp.o f .cp
CPlus {default}.cp -o {default}.cp.o {CPlusOptions}
Objects = a
"{AppObjectDir}"T App.cp.o a "{AppObjectDir}"TDoc.cp.o d "{ModelessObjDir}"TModelessDoc.cp.o a ModelessApp.cp.o
ResourceFiles = d "{AppObjectDir}"T App.rsrc d "{AppObjectDir}"TDoc.rsrc a ModelessApp.rsrc
# dependency rules for TDoc and TApp "{AppObjectDir}"TDoc.cp.o f "{AppObjectDir}"TDoc.cp d
"{AppObjectDir}"TDoc.h a "{AppObjectDir}"AppDocMenus.h
"{AppObjectDir}"TApp.cp.o f "{AppObjectDir}"TApp.cp a "{AppObjectDir}"T App.h a "{AppObjectDir}"TDoc.h a
"{AppObjectDir}"AppDocMenus.h # dependency rules for TModelessDoc




"{ModelessObjDir}"TModelessDoc.cp.o f a "{ModelessObjDir}"TModelessDoc.cp a
"{ModelessObjDir}"TModelessDoc.h a "{AppObjectDir}"TDoc.h
# dependency rules for ModelessApp ModelessApp.cp.o f ModelessApp.cp a "{AppObjectDir}"T App.h a "{AppObjectDir}"TDoc.h a
"{ModelessObjDir}"TModelessDoc.h a ModelessApp.make
ModelessApp ff {Objects} ModelessApp.make Link -o {Targ} {LinkOptions} a
{Objects} a "{CLibraries}"CPlusLib.o a "{CLibraries}"CRuntime.o a "{CLibraries}"StdCLib.o a "{CLibraries}"Cinterface.o a "{L ibraries}"Interface.o
SetFile {Targ} -t APPL -c '????' -a B
ModelessApp ff ModelessApp.r a {ResourceFiles}
Rez -append -o {Targ} {RezOptions} ModelessApp.r