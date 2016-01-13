#include "rpcbuffer.h"
#include <iostream>

/*
 * reorganized based on the rwbuffer in https://github.com/kkevinchou/CS454-A3
 */

using namespace std;

char * RPCBuffer::copyBufferToConvertor(char * bufferP, unsigned int len)
{
  for(unsigned int i = 0; i < len; i++)
  {
    _convertor.charArray[i] = *bufferP;
  }
  
  return bufferP+len;
}

char * RPCBuffer::copyConvertorToBuffer(char * bufferP, unsigned int len)
{
  for(unsigned int i = 0; i < len; i++)
  {
    bufferP[i] = _convertor.charArray[i];
  }
  return bufferP+len;
}


/*************************** insertion methods **************************/

char *RPCBuffer::addCStringToBuffer(const char * c, char *bufferP )
{
  while(*c != '\0')
  {
    *bufferP = *c;
    bufferP++;
    c++;
  }
  *bufferP = *c; // add the '\0'
  bufferP++;
  return bufferP;
}

char * RPCBuffer::addStringToBuffer(string s, char *bufferP)
{
  unsigned int i = 0;
  for (i = 0; i < s.size(); i++) {
    bufferP[i] = s[i];
  }
  bufferP[i] = '\0';
  
  return bufferP + i + 1;
}


char * RPCBuffer::addIntToBuffer(int i, char *bufferP)
{
  _convertor.i = i;
  return copyConvertorToBuffer(bufferP, sizeof(int));
}

char * RPCBuffer::addUnsignedIntToBuffer(unsigned int u, char *bufferP)
{
  _convertor.ui = u;
  return copyConvertorToBuffer(bufferP, sizeof(unsigned int));
}

char * RPCBuffer::addShortToBuffer(short s, char *bufferP)
{
  _convertor.s = s;
  return copyConvertorToBuffer(bufferP, sizeof(short));
}

char *RPCBuffer::addCharToBuffer(char c, char *bufferP) {
  bufferP[0] = c;
  return bufferP + 1;
}

char *RPCBuffer::addDoubleToBuffer(double d, char *bufferP) {
    _convertor.d = d;
  return copyConvertorToBuffer(bufferP, sizeof(double));
}

char *RPCBuffer::addFloatToBuffer(float f, char *bufferP) {
  _convertor.f = f;
  return copyConvertorToBuffer(bufferP, sizeof(float));
}

char *RPCBuffer::addLongToBuffer(long l, char *bufferP) {
  _convertor.l = l;
  return copyConvertorToBuffer(bufferP, sizeof(long));
}

char *RPCBuffer::addUnsignedShortToBuffer(unsigned short us, char *bufferP)
{
  _convertor.us = us;
  return copyConvertorToBuffer(bufferP, sizeof(unsigned short));
}

//insert array to buffer
char * RPCBuffer::addIntArrayToBuffer(int intArray[], int len, char *bufferP) {
  for (int i = 0; i < len; i++) {
    bufferP = addIntToBuffer(intArray[i], bufferP);
  }
  return bufferP;
}

char * RPCBuffer::addDoubleArrayToBuffer(double doubleArray[], int len, char *bufferP) {
  for (int i = 0; i < len; i++) {
    bufferP = addDoubleToBuffer(doubleArray[i], bufferP);
  }
  return bufferP;
}


char * RPCBuffer::addFloatArrayToBuffer(float floatArray[], int len, char *bufferP) {
  for (int i = 0; i < len; i++) {
    bufferP = addFloatToBuffer(floatArray[i], bufferP);
  }
  
  return bufferP;
}

char * RPCBuffer::addShortArrayToBuffer(short shortArray[], int len, char *bufferP) {
  for (int i = 0; i < len; i++) {
    bufferP = addShortToBuffer(shortArray[i], bufferP);
  }
  
  return bufferP;
}
char * RPCBuffer::addCharArrayToBuffer(const char charArray[], int len, char *bufferP)
{
  for (int i = 0; i < len; i++) {
    bufferP = addCharToBuffer(charArray[i], bufferP);
  }
  
  return bufferP;
}
char * RPCBuffer::addLongArrayToBuffer(long longArray[], int len, char *bufferP) {
  for (int i = 0; i < len; i++) {
    bufferP = addLongToBuffer(longArray[i], bufferP);
  }
  
  return bufferP;
}




/************************** Extraction methods **************************/


char * RPCBuffer::extractLongFromBuffer(char * bufferP, long &l)
{
  bufferP = copyBufferToConvertor(bufferP, sizeof(long));
  l = _convertor.l;
  return bufferP;
}
char * RPCBuffer::extractFloatFromBuffer(char * bufferP, float &f)
{
  bufferP = copyBufferToConvertor(bufferP, sizeof(float));
  f = _convertor.f;
  return bufferP;
}
char * RPCBuffer::extractCharFromBuffer(char * bufferP, char &c)
{
  c = *bufferP;
  bufferP++;
  return bufferP;
}
char * RPCBuffer::extractDoubleFromBuffer(char * bufferP, double &d)
{
  bufferP = copyBufferToConvertor(bufferP, sizeof(double));
  d = _convertor.d;
  return bufferP;
}
char * RPCBuffer::extractUnsignedIntFromBuffer(char * bufferP, unsigned int &i)
{
  bufferP = copyBufferToConvertor(bufferP, sizeof(unsigned int));
  i= _convertor.ui;
  return bufferP;
}
char * RPCBuffer::extractIntFromBuffer(char * bufferP, int &i)
{
  bufferP = copyBufferToConvertor(bufferP, sizeof(int));
  i = _convertor.i;
  return bufferP;
}

// extracts until '\0', you can use this to extract cstrings
char * RPCBuffer::extractCStringFromBuffer(char * bufferP, string &s)
{
  string r = "";
  while(*bufferP != '\0')
  {
    r += *bufferP;
    bufferP++;
  }
  s.assign(r);
  bufferP++;
  return bufferP;
}

char * RPCBuffer::extractShortFromBuffer(char * bufferP, short &i)
{
  bufferP = copyBufferToConvertor(bufferP, sizeof(short));
  i = _convertor.s;
  return bufferP;
}

char * RPCBuffer::extractUnsignedShortFromBuffer(char * bufferP, unsigned short &us)
{
  bufferP = copyBufferToConvertor(bufferP, sizeof(unsigned short));
  us = _convertor.us;
  return bufferP;
}


// extract arrays

char * RPCBuffer::extractIntArrayFromBuffer(char * bufferP, int intArray[], unsigned int len) // make sure enough memory is in intArray
{
  for(unsigned int i = 0; i < len; i++)
  {
    int n;
    bufferP = extractIntFromBuffer(bufferP, n);
    intArray[i] = n;
  }
  return bufferP;
}

char * RPCBuffer::extractShortArrayFromBuffer(char * bufferP, short shortArray[], unsigned int len)
{
  for(unsigned int i = 0; i < len; i++)
  {
    short n;
    bufferP = extractShortFromBuffer(bufferP, n);
    shortArray[i] = n;
  }
  return bufferP;
}

char * RPCBuffer::extractCharArrayFromBuffer(char * bufferP, char charArray[], unsigned int len)
{
  for(unsigned int i = 0; i < len; i++)
  {
    char n;
    bufferP = extractCharFromBuffer(bufferP, n);
    charArray[i] = n;
  }
  return bufferP;
}

char * RPCBuffer::extractLongArrayFromBuffer(char * bufferP, long longArray[], unsigned int len)
{
  for(unsigned int i = 0; i < len; i++)
  {
    long n;
    bufferP = extractLongFromBuffer(bufferP, n);
    longArray[i] = n;
  }
  return bufferP;
}

char * RPCBuffer::extractDoubleArrayFromBuffer(char * bufferP, double doubleArray[], unsigned int len)
{
  for(unsigned int i = 0; i < len; i++)
  {
    double n;
    bufferP = extractDoubleFromBuffer(bufferP, n);
    doubleArray[i] = n;
  }
  return bufferP;
}

char * RPCBuffer::extractFloatArrayFromBuffer(char * bufferP, float floatArray[], unsigned int len)
{
  for(unsigned int i = 0; i < len; i++)
  {
    float n;
    bufferP = extractFloatFromBuffer(bufferP, n);
    floatArray[i] = n;
  }
  return bufferP;
}


//Keep extracting integers which represent argument types until a 0 is found and include the 0
char * RPCBuffer::getArgTypes(char * bufferP, int argTypes[])
{
  int index = 0;
  while(true)
  {
    int i;
    bufferP = extractIntFromBuffer(bufferP, i);
    argTypes[index] = i;
    index++;
    if(i ==0) break;
    
  }
  return bufferP;
}

unsigned int RPCBuffer::getArgTypesLength(int * array)
{
  //keep checking all argument types until we find the terminator '0'
  unsigned int counter = 0;
  while(true)
  {
    counter++;
    int type = *array;
    if(type == 0) break;
    
    array++;
  }
  
  return counter;
}

unsigned int RPCBuffer::getArgTypesLength(char * bufferP)
{
  //keep checking argument types until we find the terminator '0'
  unsigned int counter = 0;
  while(true)
  {
    counter++;
    int i;
    bufferP = extractIntFromBuffer(bufferP, i);
    if(i == 0) break;
  }
  
  return counter;
}

// gets the third and fourth bytes, and translates it into a short
unsigned short RPCBuffer::getArrayLengthFromArgumentType(int argType)
{
  return argType & 65535;
}

// gets the second byte and translate it into an int
int RPCBuffer::getTypeFromArgumentType(int argType)
{
  return (argType >> 16) & 255;
}
