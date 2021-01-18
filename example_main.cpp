// This source was written by Stephen Oswin, and is placed in the
// public domain. The author hereby disclaims copyright to this source
// code.

#include <iostream>

#include "search.h"
#include "search.cpp"

int myCallback(const char * pStart, const char * pbuff, const char * resultString, const char * position)
{
    std::cout << "Found word :" << resultString << std::endl;
    if (pStart && position)
        std::cout << "At position :" << std::distance(pStart, position) << std::endl;
    return 0;
}

int main(int argc, char* argv[])
{
    const char * textToSearch = "The cat sat on the mat. it was a very happy cat.";

    const char * p = textToSearch;

    while( p && (*p != '\0') )
    {
        p = search(textToSearch, p, &myCallback );
    }
	return 0;
}

