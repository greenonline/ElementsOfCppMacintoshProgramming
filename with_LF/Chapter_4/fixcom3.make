# File: fixcom3.make
# Target: fixcom3
# Sources: fixcom3.cp
# Created: Friday, January 19, 1990 11:58:21 AM

OBJECTS fixcom3.cp.o

fixcom3 ff fixcom3.make {OBJECTS} 
    Link -w -c 'MPS I -t MPST ∂
        {OBJECTS} ∂ "{CLibraries}"CSANELib.o ∂
        "{CLibraries}"Math.o ∂
        "{CLibraries}"CplusLib.o ∂
        #"{CLibraries}"Complex.o ∂
        "{CLibraries}"StdCLib.o ∂
        "{CLibraries}"Cinterface.o ∂
        "{Libraries}"Stubs.o ∂
        "{CLibraries}"CRuntime.o ∂
        "{Libraries}"Interface.o ∂
        "{Libraries}"ToolLibs.o ∂
        -o fixcom3
fixcom3.cp.o f fixcom3.make fixcom3.cp TTool.h 
    CPlus fixcom3.cp