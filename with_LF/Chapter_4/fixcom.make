# File: fixcom.make
# Target: fixcom
# Sources: fixcom.cp
# Created: Monday, January 29, 1990 10:49:49 AM

OBJECTS fixcom.cp.o

fixcom ff fixcom.make {OBJECTS} 
    Link -w -c 'MPS I -t MPST a
        {OBJECTS} ∂
        "{CLibraries}"CSANELib.o ∂
        "{CLibraries}"Math.o ∂
        "{CLibraries}"CplusLib.o ∂
        #"{CLibraries}"Complex.o ∂
        "{CLibraries}"StdCLib.o ∂
        "{CLibraries}"Cinterface.o ∂
        "{Libraries}"Stubs.o ∂
        "{CLibraries}"CRuntime.o ∂
        "{Libraries}"Interface.o ∂
        "{Libraries}"ToolLibs.o ∂
        -o fixcom
fixcom.cp.o f fixcom.make fixcom.cp 
    CPlus fixcom.cp