/*
* FILE          : encodeInput.c
* PROJECT       : SENG2030 - Assignment 2
* PROGRAMMER    : Matthew Goodman
* FIRST VERSION : 2021-02-17
* DESCRIPTION :
* this file contains the main execution and logic code for the
* encodeInput filter utility. The utility is used to encode binary input
* into S-Record or DC.B output.
*/

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h> //used for access() method
#include <errno.h>  //used for detailed errors

//include custom header file
#include "../inc/encodeInput.h"

enum Mode{Assembly, SRecord};  //filter output type
#define MAX_PATH 4096          //maximum path length for input/output file


int main( int argc, char *argv[] ) {

    enum Mode mode = Assembly;          //filter output mode
    char inFile[MAX_PATH] = {'\0'};     //input filename string
    char outFile[MAX_PATH] = {'\0'};    //output filename string
    FILE* inPtr;                        //pointer to input stream
    FILE* outPtr;                       //pointer to output stream

    //read command line arguments
    for(int i=0; i < argc; ++i){
        if(strcmp(argv[i], "-srec") == 0 ){
            //set the filter to output S-Record format
            mode = SRecord;
        } else if(strcmp(argv[i], "-h") == 0 ) {
            //output usage statement and return
            printf("Usage: encodeInput [-i<InputFilename>] [-o<OutputFilename>] [-srec] [-h]\n");
            i = argc;
            return 0;
        } else if (strncmp(argv[i], "-i", 2) == 0) { //the argument begins with the input flag
            strcpy(inFile, &argv[i][2]);
        } else if (strncmp(argv[i], "-o", 2) == 0) { //the argument begins with the output flag
            strcpy(outFile, &argv[i][2]);
        }
    }

    //check if the input file is specified
    if(inFile[0] != 0){     
        //make sure the input file exists and can be read
        if(access(inFile, R_OK) != 0){ 
            //file does not exist, or can't read
            perror("Input File Error");
            return -1;
        } else {
            //the file meets the qualifications, update input file pointer
            inPtr = fopen(inFile, "r");
        }

        if(outFile[0] == 0){    //if output file is not specified
            //append extension to infile as outfile
            strcpy(outFile, inFile);
            if(mode == Assembly){
                strcat(outFile, ".asm");
            } else {
                strcat(outFile, ".srec");
            }
        }
    } else {
        //get input from console instead
        inPtr = stdin;
    }

    if(outFile[0] != 0){    //if output file is specified
        //create or open specified output file
        if((outPtr = fopen(outFile, "w")) == NULL) {
            //file did not have write permission
            perror("Output File Error");
            return -1;
        }
    }  else {    //no input or output specified
        //output to stdout instead
        outPtr = stdout;
    }

    //do output based on selected filter mode
    if(mode == SRecord){
        //do S-Record mode logic
        if (sRecordLogic(inPtr, outPtr) != 0) {
            //an error occured in SREC function
            printf("S-Record Filter Stopped Unexpectedly\n");
            return -1;
        }
    }else{
        //do Assembly mode logic
        if (assemblyLogic(inPtr, outPtr) != 0) {
            //an error occured in ASM function
            printf("Assembly Filter Stopped Unexpectedly\n");
            return -1;
        }
    }

    //end of program
    return 0;
}
