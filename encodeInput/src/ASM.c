/*
* FILE          : ASM.c
* PROJECT       : SENG2030 - Assignment 2
* PROGRAMMER    : Matthew Goodman
* FIRST VERSION : 2021-02-17
* DESCRIPTION :
* this file contains the logic and supporting functions for the 
* utility when being used in Assembly output mode.
*/

#include <stdio.h>
#include <string.h>
#include <stdbool.h>

//include custom header file
#include "../inc/encodeInput.h"

#define DCB_ENCODED_LENGTH 256  //length for a line of DC.B assembly
/*
 * Function: int assemblyLogic(FILE* input, FILE* output)
 * Description: 
 * this function reads binary bytes from the input stream, which are 
 * then formatted to DC.B Assembly format, and output to the output stream
 * Paramenters: 
 * FILE* input: the input stream
 * FILE* output: the output stream
 * Returns: 0 on success, or -1 on failure
*/
int assemblyLogic(FILE* input, FILE* output) {
    bool outputComplete = false;
    while(outputComplete == false){
        unsigned char inputData[DATA_RAW_MAX_LENGTH] = {0};     //the unconverted data (binary bytes)
        char dcbLine[DCB_ENCODED_LENGTH] = {'\0'};              //the formatted output line
        int dataLength = 0;                                     //number of bytes successfully read from input stream

        strcpy(dcbLine, "dc.b");   // add dc.b indicator to beginning of output line

        dataLength += fread(inputData, 1, DATA_RAW_MAX_LENGTH, input);
        //indicate that there is no more data in the file after this loop finishes
        if(feof(input)){
            outputComplete= true;
        }

        //write all data values to output line
        for(int i = 0; i < dataLength; i++){
            writeHexASM(dcbLine, inputData[i]);
            //add a comma if there are more instructions on the line
            if(i < dataLength-1){
                strcat(dcbLine, ",");
            }
        }
        //add newline
        strcat(dcbLine, "\n");
        printf("%s\n", dcbLine);    //DEBUG
        //write record to file
        fputs(dcbLine, output);
    }
}



/*
 * Function: void writeHexASM(char* destination, unsigned char c)
 * Description: 
 * Writes the DC.B instruction equivalent of a given byte to the
 * designated destination string
 * Paramenters: 
 * char* destination: reference to the c-string to fill
 * int pos: index in array to put first character
 * unsigned char c: byte to be converted to hexidecimal
 * Returns: nothing
*/
void writeHexASM(char* destination, unsigned char c) {
    char converted[5]; 
    sprintf(converted, " $%02X", c);
    strcat(destination, converted);
    return;
}