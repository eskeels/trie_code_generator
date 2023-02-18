// This source was written by Stephen Oswin, and is placed in the
// public domain. The author hereby disclaims copyright to this source
// code.

#include <iostream>
#include <fstream>
#include <string>

#include "search.h"
#include "search.cpp"

// pStart = start of buffer being scanned
// pbuff = current position within the buffer being scanned
// resultString = matched text
// position = end of match (past last character)
// dictionaryName = name of dictionary this term belongs to
// score = score for term
int myCallback(const CharT* pStart,
               const CharT* pbuff, 
               const CharT* resultString, 
               const CharT * position,
               const char * dictionaryName,
               int16_t score,
               bool distinct,
               void * data)
{
    std::cout << "Found word [" << resultString << "] line :" << *(size_t *)data;
    if (pStart && position)
    {
        std::cout << " column :" << std::distance(pStart, position) << std::endl;
    }

    return 0;
}

int ParseCmdLine(int argc, char* argv[], std::string& fname)
{
    if (argc<=1)
    {
        std::cerr << "Invalid command line options. Need file to search." << std::endl;
        return EXIT_FAILURE;
    }
    else
    {
        fname = argv[1];
    }

    return 0;
}

int ReadFile(const std::string& filename)
{
    std::fstream inputFile(filename.c_str());
    if (inputFile.fail())
    {
        std::cerr << "Failed to open " << filename << std::endl;
        return EXIT_FAILURE;
    }

    size_t lineCount = 0;
    std::string line;
    for( ; std::getline(inputFile,line); lineCount++)
    {
        const char * p = line.c_str();
        while( p && (*p != '\0') )
        {
            p = search(line.c_str(), p, &myCallback, &lineCount);
            //++p;
        }
    }

    return 0;
}

int main(int argc, char* argv[])
{
    std::string fname;

    if (0 != ParseCmdLine(argc, argv, fname))
    {
        return EXIT_FAILURE;
    }

    ReadFile(fname);

    return EXIT_SUCCESS;
}

