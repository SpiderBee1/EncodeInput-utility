# FILE          : makefile
# PROJECT       : SENG2030 - Assignment 2
# PROGRAMMER    : Matthew Goodman
# FIRST VERSION : 2021-02-17
# DESCRIPTION :
# this makefile will compile and link all necessary source files
# in the encodeInput directory.

# =======================================================
#                  encodeInput
# =======================================================
# FINAL BINARY Target
./bin/encodeInput : ./obj/encodeInput.o ./obj/ASM.o ./obj/SREC.o
	cc ./obj/encodeInput.o ./obj/ASM.o ./obj/SREC.o -o ./bin/encodeInput

# =======================================================
#                     Dependencies
# =======================================================                     
./obj/encodeInput.o : ./src/encodeInput.c ./inc/encodeInput.h
	cc -c ./src/encodeInput.c -o ./obj/encodeInput.o

./obj/ASM.o : ./src/ASM.c ./inc/encodeInput.h
	cc -c ./src/ASM.c -o ./obj/ASM.o

./obj/SREC.o : ./src/SREC.c ./inc/encodeInput.h
	cc -c ./src/SREC.c -o ./obj/SREC.o

#
# =======================================================	
# Other targets
# =======================================================                     
all : ./bin/encodeInput

clean:
	rm -f ./bin/*
	rm -f ./obj/*.o
	


