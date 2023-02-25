// This source was written by Stephen Oswin, and is placed in the
// public domain. The author hereby disclaims copyright to this source
// code.

#include <iostream>
#include <fstream>
#include <string>

#include "search.h"
#include "search.cpp"

// pStart = start of buffer being scanned
// resultString = matched text from the dictionary
// matchStart = start of match
// matchEnd = end of match (last character)
// dictionaryName = name of dictionary this term belongs to
// score = score for term
int myCallback(const CharT* pStart,
               const CharT* resultString, 
               const CharT* matchStart,
               const CharT* matchEnd,
               const char* dictionaryName,
               int16_t score,
               bool distinct,
               bool caseSensitive,
               bool wholeWord,
               void * data)
{
    if (caseSensitive) {
        if (0 != memcmp(matchStart, resultString, 1+(matchEnd - matchStart))) {
            return 0;
        }
    }

    std::cout << "Found word [" << resultString << "] line :" << *(size_t *)data;
    if (pStart && matchEnd) {
        std::cout << " column :" << std::distance(pStart, matchStart) << std::endl;
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
            const char* newp = search(line.c_str(), p, &myCallback, &lineCount);
            // for single character matches. I should sort out the search method really...
            p = (newp == p ? newp + 1 : newp);
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

