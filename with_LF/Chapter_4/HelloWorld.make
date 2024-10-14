# File: HelloWorld.make
# Target: HelloWorld
# Sources: HelloWorld.cp
# Created: Monday, January 29, 1990 9:36:51 AM



OBJECTS = HelloWorld.cp.o

HelloWorld ƒƒ HelloWorld.make {OBJECTS}
    Link -w -c 'MPS ' -t MPST ∂
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
        -o HelloWorld

HelloWorld.cp.o ƒ HelloWorld.make HelloWorld.cp 
    #CPlus HelloWorld.cp
    SCpp HelloWorld.cp