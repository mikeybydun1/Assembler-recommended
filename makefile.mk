assembler.out: utilities.o preAssembler.o symbolTable.o firstRide.o secondRide.o utilRide2.o outputFiles.o assembler.o debugger.o dataUtils.o
	gcc utilities.o preAssembler.o symbolTable.o firstRide.o secondRide.o utilRide2.o outputFiles.o assembler.o debugger.o dataUtils.o

utilities.o: utilities.c
	gcc -c utilities.c

preAssembler.o: preAssembler.c
	gcc -c preAssembler.c

symbolTable.o: symbolTable.c
	gcc -c symbolTable.c

firstRide.o: firstRide.c
	gcc -c firstRide.c

secondRide.o: secondRide.c
	gcc -c secondRide.c

utilRide2.o: utilRide2.c
	gcc -c utilRide2.c

outputFiles.o: outputFiles.c
	gcc -c outputFiles.c

assembler.o: assembler.c
	gcc -c assembler.c

debugger.o: debugger.c
	gcc -c debugger.c

dataUtils.o: dataUtils.c
	gcc -c dataUtils.c
