// pch.cpp: source file corresponding to the pre-compiled header
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "pch.h"
// When you are using pre-compiled headers, this source file is necessary for compilation to succeed.
// C program to display hostname 
// and IP address 
#include <stdio.h> 
#include <stdlib.h>
#include <errno.h> 
#include <sys/types.h>



// Returns hostname for the local computer 
void checkHostName(int hostname)
{
    if (hostname == -1)
    {
        perror("gethostname");
        exit(1);
    }
}

// Returns host information corresponding to host name 
void checkHostEntry(struct hostent* hostentry)
{
    if (hostentry == NULL)
    {
        perror("gethostbyname");
        exit(1);
    }
}

// Converts space-delimited IPv4 addresses 
// to dotted-decimal format 
void checkIPbuffer(char* IPbuffer)
{
    if (NULL == IPbuffer)
    {
        perror("inet_ntoa");
        exit(1);
    }
}

// Driver code 
char* getIpAddress()
{
    char hostbuffer[256];
    char* IPbuffer;
    struct hostent* host_entry;
    int hostname;

    // To retrieve hostname 
    hostname = gethostname(hostbuffer, sizeof(hostbuffer));
    checkHostName(hostname);

    // To retrieve host information 
    host_entry = gethostbyname(hostbuffer);
    checkHostEntry(host_entry);

    // To convert an Internet network 
    // address into ASCII string 
    IPbuffer = inet_ntoa(*((struct in_addr*)
        host_entry->h_addr_list[0]));

    return IPbuffer;
}