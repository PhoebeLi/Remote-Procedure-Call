#ifndef RPCBUFFER_H
#define RPCBUFFER_H

#include "constants.h"

/*
 * reorganized based on the rwbuffer in https://github.com/kkevinchou/CS454-A3
 */

class RPCBuffer
{
private:
		DataTypeConversion _convertor;
		char * copyBufferToConvertor(char * bufferP, unsigned int len);
		char * copyConvertorToBuffer(char * bufferP, unsigned int len);
public:
		// every function that returns char * returns the new starting position of the buffer pointer
  
		// insertion methods, insert data to the buffer and update the buffer pointer.
		char *addShortToBuffer(short s, char *bufferP);
	    char *addIntToBuffer(int i, char *bufferP);
	    char *addStringToBuffer(string s, char *bufferP);
	    char *addUnsignedIntToBuffer(unsigned int u, char *bufferP);
	    char *addCharToBuffer(char c, char *bufferP);
	    char *addDoubleToBuffer(double d, char *bufferP);
	    char *addFloatToBuffer(float f, char *bufferP);
	    char *addLongToBuffer(long l, char *bufferP);
	    char *addUnsignedShortToBuffer(unsigned short us, char *bufferP);
  
    	// insert arrays of data
		char * addCStringToBuffer(const char * c, char *bufferP );
    	char * addIntArrayToBuffer(int intBuf[], int numInts, char *bufferP);
		char * addDoubleArrayToBuffer(double doubleArray[], int len, char *bufferP);
		char * addFloatArrayToBuffer(float floatArray[], int len, char *bufferP) ;
		char * addCharArrayToBuffer(const char charArray[], int len, char *bufferP) ;
		char * addShortArrayToBuffer(short shortArray[], int len, char *bufferP);
		char * addLongArrayToBuffer(long longArray[], int len, char *bufferP);
  
    	// extraction methods, removes data from the buffer to a variable and
		char * extractCStringFromBuffer(char * bufferP, string &s);
		char * extractUnsignedIntFromBuffer(char * bufferP, unsigned int &i);
		char * extractIntFromBuffer(char * bufferP, int &i);
		char * extractShortFromBuffer(char * bufferP, short &i);
		char * extractUnsignedShortFromBuffer(char * bufferP, unsigned short &us);
		char * extractLongFromBuffer(char * bufferP, long &l);
		char * extractFloatFromBuffer(char * bufferP, float &f);
		char * extractDoubleFromBuffer(char * bufferP, double &d);
		char * extractCharFromBuffer(char * bufferP, char &c);
  
    	// extract arrays of data
		char * extractIntArrayFromBuffer(char * bufferP, int intArray[], unsigned int len);
		char * extractShortArrayFromBuffer(char * bufferP, short shortArray[], unsigned int len);
		char * extractCharArrayFromBuffer(char * bufferP, char charArray[], unsigned int len);
		char * extractLongArrayFromBuffer(char * bufferP, long longArray[], unsigned int len);
		char * extractDoubleArrayFromBuffer(char * bufferP, double doubleArray[], unsigned int len);
		char * extractFloatArrayFromBuffer(char * bufferP, float floatArray[], unsigned int len);


    	//extract metadata for array of data
    	//extractArgTypes
		char * getArgTypes(char * bufferP, int argTypes[]);
		unsigned int getArgTypesLength(char * bufferP);
		unsigned int getArgTypesLength(int * array);
  
    	//extract the length of array of data from argType
		unsigned short getArrayLengthFromArgumentType(int argType);
  
    	//extract the data type from argType
		int getTypeFromArgumentType(int argType);
};




#endif