#pragma once

#include <windows.h>

typedef struct _dfsStruct 
{
    BOOL  bFoundFile;
    BOOL  bCheckCreateFileA;
    BOOL  bResultCreateFileA;
    int   cDepth;
    int   tDepth;
    int   eVar;
} DFSStruct, *PDFSTRUCT;


// Forward declarations
size_t internalstrlen (const char *str);
int internalstrncmp (const char * s1, const char * s2, size_t n );


// Implementations
size_t internalstrlen(const char *str)
{
    const char *s;

    for (s = str; *s; ++s)
    {
        ;
    }
    
    return (s - str);
}


int internalstrncmp( const char * s1, const char * s2, size_t n )
{
    while ( n && *s1 && ( *s1 == *s2 ) )
    {
        ++s1;
        ++s2;
        --n;
    }
    
    if ( n == 0 )
    {
        return 0;
    }
    else
    {
        return ( *(unsigned char *)s1 - *(unsigned char *)s2 );
    }
}