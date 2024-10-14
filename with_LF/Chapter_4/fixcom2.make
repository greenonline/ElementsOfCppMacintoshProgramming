# File: fixcom2.make
# Target: fixcom2
# Sources: fixcom2.cp
# Created: Friday, January 19, 1990 11:58:21 AM

OBJECTS =fixcom2.cp.o

fixcom2 f f fixcom2.make {OBJECTS) 
    Link -w -c 'MPS I -t MPST ∂
        {OBJECTS) ∂
        "{CLibraries)"CS.ANELib.o ∂
        "{CLibraries}"Math.o ∂
        "{CLibraries}"CplusLib.o ∂
        #"{CLibraries}"Complex.o ∂
        "{CLibraries}"StdCLib.o ∂
        "{CLibraries)"Cinterface.o ∂
        "{Libraries)"Stubs.o ∂
        "{CLibraries)"CRuntime.o ∂
        "{Libraries)"Interface.o ∂
        "{Libraries)"ToolLibs.o ∂
        - o fixcom2
fixcom2.cp.o f fixcom2.make fixcom2.cp TTool.h 
    CPlus fixcom2.cp