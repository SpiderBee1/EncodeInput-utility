/*
* FILE          : SREC.c
* PROJECT       : SENG2030 - Assignment 2
* PROGRAMMER    : Matthew Goodman
* FIRST VERSION : 2021-02-17
* DESCRIPTION :
* this file contains the logic and supporting functions for the 
* utility when being used in S-Record output mode.
*/

#include <stdio.h>
#include <string.h>
#include <stdbool.h>

//include custom header file
#include "../inc/encodeInput.h"

enum RecordType{ header=0, data=1, summation=5, trailer=9 };  //filter output type
#define SREC_ENCODED_LENGTH 44          //maximum number of bytes in an S-Rec encoded line
#define MIN_BYTE_COUNT 3                //bytes to add to bytecount to accomodate for 2 address bytes, and checksum byte.
const char HEADERTEXT[] = "Matthew";    //static data content for the header record



/*
 * Function: int sRecordLogic(FILE* input, FILE* output)
 * Description: 
 * this function reads binary bytes from the input stream, which are 
 * then formatted to S-Record format and output to the output stream
 * Paramenters: 
 * FILE* input: the input stream
 * FILE* output: the output stream
 * Returns: 0 on success, or -1 on failure
*/
int sRecordLogic(FILE* input, FILE* output) {
    enum RecordType rType = header;     //the what type record to create on the next loop
    unsigned short dataCount = 0;       //counter of data lines in the output file 
    unsigned short memPosition = 0;     //the address in memory to store the data ()
    bool outputComplete = false;        //exit condition for while loop
    

    //loop to output records one at a time
    while(outputComplete == false){
        unsigned char inputData[DATA_RAW_MAX_LENGTH] = {0};     //the unconverted data section (binary bytes)
        unsigned char dataLength = 0;                           //the number of bytes in the data section alone
        unsigned short address = 0;                              //address field, 
        char sRecord[SREC_ENCODED_LENGTH] = {'\0'};             //the formated S-Record line (ascii bytes)
        unsigned short byteSum = 0;                             //the sum of bytes used to calculate the checksum
        
        //fill data according to record type
        switch (rType) {
        case header:
            strcpy(&sRecord[0], "S0");   //set record type indicator to header
            strcpy(inputData, HEADERTEXT);      //fill data section 
            dataLength += strlen(HEADERTEXT);   //set accompanying dataLength value
            
            //add all data values to bytesum
            for(int i = 0; i < dataLength; i++){
                byteSum += inputData[i];
            }

            //address is always 0x0000 for header record
            address = 0;

            //make the next record a data record 
            rType = data;
            break;
        case data:
            strcpy(&sRecord[0], "S1");   //set record type indicator to data
            //get up to 16 characters from input stream
            dataLength += fread(inputData, 1, DATA_RAW_MAX_LENGTH, input);
            //add all data values to bytesum
            for(int i = 0; i < dataLength; i++){
                byteSum += inputData[i];
            }
            
            //set address field value
            address = memPosition;
            //increment memory position
            memPosition += 16;

            ++dataCount;    //increment the data record counter
            //make the next record a summation if the end of file was reached
            if(feof(input)){
                rType = summation;
            }
            break;
        case summation:
            strcpy(&sRecord[0], "S5");   //set record type indicator to summation
            //summation has no data section

            //set address field value to count of data records
            address = dataCount;

            //make the next record a trailer record 
            rType = trailer;
            break;
        case trailer:
            strcpy(&sRecord[0], "S9");   //set record type indicator to trailer
            //summation has no data section

            //address is the same as the first data record (always 0x0000)
            address = 0;

            //indicate that the final record has been written out
            outputComplete = true;
            break;
        default:
            //unexpected value of rType. 
            return -1;
            break;
        }

        //format data into s-record format

        

        //fill count field
        writeHex(sRecord, 1, (unsigned char)(MIN_BYTE_COUNT + dataLength));
        //fill address field
        writeHex(sRecord, 2, (unsigned char)(address / 256));   //upper byte
        writeHex(sRecord, 3, (unsigned char)(address & 0xff));  //lower byte
        //fill data section
        if(dataLength > 0){
            //there is some data to be written
            for(int i = 0; i < dataLength; i++){
                writeHex(sRecord, 4+i, inputData[i]);
            }
        }
        //add byte values from count field and address field
        byteSum += (MIN_BYTE_COUNT+dataLength) + (address/256) + (address&0xff);
        //fill checksum field
        writeHex(sRecord, 4+dataLength, (unsigned char)CalculateSum(byteSum));

        //add newline
        strcat(sRecord, "\n");
        
        //write record to file
        fputs(sRecord, output);
    }

    return 0;
}



/*
 * Function: unsigned char CalculateSum(unsigned short byteSum)
 * Description: 
 * this function takes a 2 byte decimal and calculates the SREC checksum value
 * Paramenters: 
 * unsigned short byteSum: decimal value representing the sum of all byte values
 * Returns: the result of the calculation (char)
*/
unsigned char CalculateSum(unsigned short byteSum) {
    byteSum = ~byteSum; //set byteSum to its one's compliment
    return (byteSum & 0xFF);    //take only the least significant byte
}



/*
 * Function: void writeHex(char* destination, int pos, unsigned char c)
 * Description: 
 * Writes the 2 character hexadecimal equivalent of a given byte to the
 * designated destination string
 * Paramenters: 
 * char* destination: reference to the c-string to fill
 * int pos: index in array to put first character
 * unsigned char c: byte to be converted to hexidecimal
 * Returns: nothing
*/
void writeHex(char* destination, int pos, unsigned char c) {
    char converted[3]; 
    sprintf(converted, "%02X", c);
    strcpy(&destination[pos*2], converted);
    return;
}
