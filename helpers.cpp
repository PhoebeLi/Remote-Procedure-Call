#include "helpers.h"
#include <string>
#include <iostream>
#include <cstdlib>
#include <stdio.h>
#include <stdlib.h> //getenv
#include <unistd.h>
#include <string.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include "constants.h"
#include "rpc.h"
#include "rpcbuffer.h"
using namespace std;
static bool debug = false;
unsigned int sizeOfType(int type)
{
    switch(type)
    {
        case ARG_CHAR:
            return sizeof(char);
        break;
        case ARG_SHORT:
            return sizeof(short);
        break;
        case ARG_INT:
             return sizeof(int);
        break;
        case ARG_LONG:
            return sizeof(long);
        break;
        case ARG_DOUBLE:
            return sizeof(double);
        break;
        case ARG_FLOAT:
            return sizeof(float);
        break;
        default:
            return 0;
        break;
    }
    return 0;
}
MessageType getMessageTypeFromInt(int i)
{
    return static_cast<MessageType>(i);
    return (MessageType)i;
}

int setupSocketAndReturnDescriptor(const char * serverAddressString, int serverPort)
{
        int socketFileDescriptor;


    struct sockaddr_in serverAddressStruct;
    struct hostent *server;


    socketFileDescriptor = socket(AF_INET, SOCK_STREAM, 0);
    if (socketFileDescriptor < 0)
    {
        return SOCKET_OPEN_FAILURE;
    }
    server = gethostbyname(serverAddressString);

    if (server == NULL) {

       return SOCKET_UNKNOWN_HOST;

    }
    memset((char *) &serverAddressStruct, 0,sizeof(serverAddressStruct));

    serverAddressStruct.sin_family = AF_INET;
    bcopy((char *)server->h_addr,
         (char *)&serverAddressStruct.sin_addr.s_addr,
         server->h_length);
    serverAddressStruct.sin_port = htons(serverPort);

    if (connect(socketFileDescriptor,(struct sockaddr *) &serverAddressStruct,sizeof(serverAddressStruct)) < 0)
    {
        return SOCKET_CONNECTION_FALIURE;
    }

    return socketFileDescriptor;
}
int setupSocketAndReturnDescriptor(const char * serverAddressString, char * serverPortString)
{
    int serverPort = atoi(serverPortString);
    return setupSocketAndReturnDescriptor(serverAddressString, serverPort);
}

int createSocket() {
    return socket(AF_INET, SOCK_STREAM, 0);
}

int listenOnSocket(int localSocketFd) {
    struct sockaddr_in binderAddress;
    memset((struct sockaddr_in *)&binderAddress, 0, sizeof(binderAddress));

    binderAddress.sin_family = AF_INET;
    binderAddress.sin_addr.s_addr = INADDR_ANY;
    binderAddress.sin_port = 0;

    if (bind(localSocketFd, (struct sockaddr *) &binderAddress, sizeof(binderAddress)) < 0)
    {
        return SOCKET_LOCAL_BIND_FALIURE;
    }

    listen(localSocketFd, 5);
}

int acceptConnection(int localSocketFd) {
    struct sockaddr_in clientAddress;
    socklen_t clientAddressSize = sizeof(clientAddress);
    int newSocketFd = accept(localSocketFd, (struct sockaddr *) &clientAddress, &clientAddressSize);

    if (newSocketFd < 0)
    {
        return SOCKET_ACCEPT_CLIENT_FAILURE;
    }


    return newSocketFd;
}

unsigned short getPort(int localSocketFd) {
    struct sockaddr_in sin;
    socklen_t len = sizeof(sin);
    getsockname(localSocketFd, (struct sockaddr *)&sin, &len);
    return ntohs(sin.sin_port);
}

string getHostname() {
    char localHostName[256];
    gethostname(localHostName, 256);

    return string(localHostName);
}

void printSettings(int localSocketFd) {
    char localHostName[256];
    gethostname(localHostName, 256);
    cout << "BINDER_ADDRESS " << localHostName << endl;

    struct sockaddr_in sin;
    socklen_t len = sizeof(sin);
    getsockname(localSocketFd, (struct sockaddr *)&sin, &len);
    cout << "BINDER_PORT " << ntohs(sin.sin_port) << endl;
}


// Client Server helpers






int insertClientServerMessageToBuffer(char *messagePointer, const char* name, int* argTypes, void**args)
{
    RPCBuffer b;
    unsigned int argTypesLength = b.getArgTypesLength(argTypes);
    messagePointer = b.addUnsignedIntToBuffer(strlen(name)+1, messagePointer);
    messagePointer = b.addCStringToBuffer(name, messagePointer);
    messagePointer = b.addIntArrayToBuffer(argTypes, argTypesLength, messagePointer);

    for(unsigned int i = 0; i < argTypesLength-1; i++)
    {
        int argType = argTypes[i];

        unsigned short length = b.getArrayLengthFromArgumentType(argType);
        if(length == 0) length= 1; // treat scalars and arrays of length 1 the same

        int type = b.getTypeFromArgumentType(argType);

        void * arg = args[i];

        switch(type)
        {
            case ARG_CHAR:
            {
                char * chars = (char *)arg;
                for(int j = 0; j < length; j++)
                {
                    messagePointer = b.addCharToBuffer(chars[j], messagePointer);
                }
            }

            break;
            case ARG_SHORT:
            {
                short * shorts = (short *)arg;
                for(int j = 0; j < length; j++)
                {
                    messagePointer = b.addShortToBuffer(shorts[j], messagePointer);
                }
            }
            break;
            break;
            case ARG_INT:
            {
                int * ints = (int *)arg;
                for(int j = 0; j < length; j++)
                {
                    messagePointer = b.addIntToBuffer(ints[j], messagePointer);
                }
            }
            break;
            case ARG_LONG:
            {
                long * longs = (long *)arg;
                for(int j = 0; j < length; j++)
                {
                    messagePointer = b.addLongToBuffer(longs[j], messagePointer);
                }
            }
            break;
            case ARG_DOUBLE:
            {
                double * doubles = (double *)arg;
                for(int j = 0; j < length; j++)
                {
                    messagePointer = b.addDoubleToBuffer(doubles[j], messagePointer);
                }
            }
            break;
            case ARG_FLOAT:
            {
                float * floats = (float *)arg;
                for(int j = 0; j < length; j++)
                {
                    messagePointer = b.addFloatToBuffer(floats[j], messagePointer);
                }
            }
            break;
            break;
            default:
            break;
        }
    }
    return 0;
}
unsigned int getClientServerMessageLength(const char* name, int* argTypes, void**args)
{
    unsigned int argTypesLength = 0;
    unsigned int messageSize = 0;

    // calculate length of arguments
    int * argTypesP = argTypes;
    RPCBuffer b;
    while(*argTypesP != 0)
    {
        int argType = *argTypesP;

        unsigned short length = b.getArrayLengthFromArgumentType(argType);
        if(length == 0) length = 1; // if it's a scalar, it still takes up one room

        int type = b.getTypeFromArgumentType(argType);
        unsigned int size = sizeOfType(type);
        messageSize += length*size;

        argTypesP++;
        argTypesLength++;
    }
    argTypesLength++; //accout for the 0

    // calculate length of argTypes
    messageSize += 4*argTypesLength;

    // calculate name size
    const char * nameP = name;
    while(*nameP != '\0')
    {
        messageSize++;
        nameP++;
    }
    messageSize++; //accountfor null termination char

    messageSize += 4; // account for sending size of name

    return messageSize;
}
int cleanupArgumentsMessage(char * bufferPointer, int argTypes[], void * args[], unsigned int argTypesLength)
{
    RPCBuffer b;
    bufferPointer = b.getArgTypes(bufferPointer, argTypes);


    for(unsigned int i = 0; i < argTypesLength-1 /*ignores the last (0) int*/; i++)
    {
        int argType = argTypes[i];
        unsigned short int length = b.getArrayLengthFromArgumentType(argType);
        int type = b.getTypeFromArgumentType(argType);
        if(length==0)
        {
            delete args[i];
        }
        else
        {
            delete [] args[i];
        }
    }
    return 0;
}
int extractArgumentsMessage(char * bufferPointer, int argTypes[], void * args[], unsigned int argTypesLength, bool allocateMemory)
{
    RPCBuffer b;
    bufferPointer = b.getArgTypes(bufferPointer, argTypes);


    for(unsigned int i = 0; i < argTypesLength-1 /*ignores the last (0) int*/; i++)
    {
        int argType = argTypes[i];
        unsigned short int length = b.getArrayLengthFromArgumentType(argType);
        int type = b.getTypeFromArgumentType(argType);

        if(allocateMemory)
        {
            switch(type)
            {
                case ARG_CHAR:
                {
                    if(length == 0)
                    {

                        char * c = new char();
                        bufferPointer = b.extractCharFromBuffer(bufferPointer, *c);

                        args[i] = (void *)c;
                    }
                    else
                    {
                        char * cArray = new char[length];
                        bufferPointer = b.extractCharArrayFromBuffer(bufferPointer, cArray, length);
                        args[i] = (void *)cArray;
                    }

                }
                break;
                case ARG_SHORT:
                {
                    if(length == 0)
                    {
                        short * s = new short();
                        bufferPointer = b.extractShortFromBuffer(bufferPointer, *s);

                        args[i] = (void *)s;
                    }
                    else
                    {
                        short * sArray = new short[length];
                        bufferPointer = b.extractShortArrayFromBuffer(bufferPointer, sArray, length);
                        args[i] = (void *)sArray;
                    }
                }
                break;
                case ARG_INT:
                {
                    if(length == 0)
                    {
                        int * c = new int();
                        bufferPointer = b.extractIntFromBuffer(bufferPointer, *c);

                        args[i] = (void *)c;
                    }
                    else
                    {
                        int * cs = new int[length];
                        bufferPointer = b.extractIntArrayFromBuffer(bufferPointer, cs, length);
                        args[i] = (void *)cs;
                    }

                }
                break;
                case ARG_LONG:
                {
                    if(length == 0)
                    {
                        long * l = new long();
                        bufferPointer = b.extractLongFromBuffer(bufferPointer, *l);

                        args[i] = (void *)l;
                    }
                    else
                    {
                        long * lArray = new long[length];
                        bufferPointer = b.extractLongArrayFromBuffer(bufferPointer, lArray, length);
                        args[i] = (void *)lArray;
                    }
                }
                break;
                case ARG_DOUBLE:
                {
                    if(length == 0)
                    {
                        double * d = new double();
                        bufferPointer = b.extractDoubleFromBuffer(bufferPointer, *d);

                        args[i] = (void *)d;
                    }
                    else
                    {
                        double * dArray = new double[length];
                        bufferPointer = b.extractDoubleArrayFromBuffer(bufferPointer, dArray, length);
                        args[i] = (void *)dArray;
                    }
                }
                break;
                case ARG_FLOAT:
                {
                    if(length == 0)
                    {
                        float * f = new float();
                        bufferPointer = b.extractFloatFromBuffer(bufferPointer, *f);

                        args[i] = (void *)f;
                    }
                    else
                    {
                        float * fArray = new float[length];
                        bufferPointer = b.extractFloatArrayFromBuffer(bufferPointer, fArray, length);
                        args[i] = (void *)fArray;
                    }
                }
                break;
                default:
                break;
            }
        }
        else
        {
             switch(type)
        {
            case ARG_CHAR:
            {
                if(length == 0)
                {
                    bufferPointer = b.extractCharFromBuffer(bufferPointer, *((char *)args[i]));
                }
                else
                {

                    bufferPointer = b.extractCharArrayFromBuffer(bufferPointer, (char *)args[i], length);
                }

            }
            break;
            case ARG_SHORT:
            {
                if(length == 0)
                {
                    short * s = new short();
                    bufferPointer = b.extractShortFromBuffer(bufferPointer, *((short *)args[i]));
                }
                else
                {
                    bufferPointer = b.extractShortArrayFromBuffer(bufferPointer, (short *)args[i], length);
                }
            }
            break;
            case ARG_INT:
            {
                if(length == 0)
                {
                    bufferPointer = b.extractIntFromBuffer(bufferPointer, *((int *)args[i]));
                }
                else
                {
                    bufferPointer = b.extractIntArrayFromBuffer(bufferPointer, (int *)args[i], length);
                }

            }
            break;
            case ARG_LONG:
            {
                if(length == 0)
                {
                    bufferPointer = b.extractLongFromBuffer(bufferPointer, *((long *)args[i]));
                }
                else
                {
                    bufferPointer = b.extractLongArrayFromBuffer(bufferPointer, (long *)args[i], length);
                }
            }
            break;
            case ARG_DOUBLE:
            {
                if(length == 0)
                {
                    bufferPointer = b.extractDoubleFromBuffer(bufferPointer, *((double *)args[i]));
                }
                else
                {
                    bufferPointer = b.extractDoubleArrayFromBuffer(bufferPointer, (double *)args[i], length);
                }
            }
            break;
            case ARG_FLOAT:
            {
                if(length == 0)
                {
                    bufferPointer = b.extractFloatFromBuffer(bufferPointer, *((float *)args[i]));
                }
                else
                {
                    bufferPointer = b.extractFloatArrayFromBuffer(bufferPointer, (float *)args[i], length);
                }
            }
            break;
            default:
            break;
        }
        }
    }
    return 0;
}