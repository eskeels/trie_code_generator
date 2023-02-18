// This source was written by Stephen Oswin, and is placed in the
// public domain. The author hereby disclaims copyright to this source
// code.

#include <map>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <random>
#include <set>
#include <time.h>
#include <stdlib.h>

#include "Trie.h"

int myCallback(const char * pStart, const char * pbuff, const char * resultString, const char * matchEnd)
{
     std::cout << "Found word :" << resultString << std::endl;
      if (pStart && matchEnd)
           std::cout << "At position :" << std::distance(pStart, matchEnd) << std::endl;
       return 0;
}

using namespace TDS;

template <typename CharType>
void AddWord(const std::basic_string<CharType>& s, 
             Trie<CharType>& t,
             const std::string& dictionaryName,
             int16_t score,
             bool caseSensitive,
             bool distinct)
{
	t.AddWord(s, dictionaryName, score, caseSensitive, distinct);
}

int ParseCmdLine(int argc, char* argv[], std::string& fname)
{
    if (argc<=1)
    {
        std::cerr << "Invalid command line options. Need file to process." << std::endl;
        return EXIT_FAILURE;
    }
    else
    {
        fname = argv[1];
    }

    return 0;
}

int ReadFile(const std::string& filename, Trie<char>& t)
{
    std::fstream inputFile(filename.c_str());
    if (inputFile.fail())
    {
        std::cerr << "Failed to open " << filename << std::endl;
        return EXIT_FAILURE;
    }

    int16_t score = -1;
    std::string dictionaryName = "dictionary1";
    bool caseSensitive = false;
    bool distinct = false;

    for( std::string line; std::getline(inputFile,line); )
    {
        std::cout << line << " " << score << std::endl;
    	AddWord<char>(line, t, dictionaryName, score, caseSensitive, distinct);
        ++score;
    }

    return 0;
}

typedef char CharT;

int main(int argc, char* argv[])
{
    std::string fname;

    if (0 != ParseCmdLine(argc, argv, fname))
    {
        return EXIT_FAILURE;
    }

	Trie<char> t;

    if (0 != ReadFile(fname, t))
    {
        return EXIT_FAILURE;
    }
            
	std::vector<SearchResult<char> > searchResults;

    std::stringstream ss;

    t.dump(ss);

    std::ofstream cpp("search.cpp");
    cpp << ss.str() << std::endl;
    cpp.close();

    std::ofstream hpp("search.h");
    hpp << "#pragma once" << std::endl;
    hpp << "typedef char CharT;" << std::endl;
    hpp << "typedef int (MatchFunction)(const CharT * pStart, const CharT * resultString, const CharT * matchStart, const CharT * matchEnd, const char * dictionaryName, int16_t score, bool distinct, void * data);" << std::endl;
    hpp << "const CharT * search(const CharT * pStart, const CharT * pbuff, MatchFunction match, void * data = NULL);" << std::endl;
    hpp.close();
    
    system("g++ -O3 file_search.cpp -o file_search");
    
	return 0;
}


