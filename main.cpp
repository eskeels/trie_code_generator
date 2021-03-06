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

int myCallback(const char * pStart, const char * pbuff, const char * resultString, const char * position)
{
     std::cout << "Found word :" << resultString << std::endl;
      if (pStart && position)
           std::cout << "At position :" << std::distance(pStart, position) << std::endl;
       return 0;
}

using namespace TDS;

template <typename CharType>
void AddWord( const std::basic_string<CharType>& s, Trie<CharType>& t, std::map<const void *,std::basic_string<CharType> >& d)
{
	d[t.AddWord(s)]=s;
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

int ReadFile(const std::string& filename, 	std::map<const void *,std::string> dictionary, Trie<char>& t)
{
    std::fstream inputFile(filename.c_str());
    if (inputFile.fail())
    {
        std::cerr << "Failed to open " << filename << std::endl;
        return EXIT_FAILURE;
    }

    for( std::string line; std::getline(inputFile,line); )
    {
    	AddWord<char>(line, t, dictionary);
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

	Trie<char> t;
	std::map<const void *,std::string> dictionary;

    if (0 != ReadFile(fname, dictionary, t))
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
    hpp << "typedef int (CallbackFunction)(const char * pStart, const char * pbuff, const char * resultString, const char * position, void * data);" << std::endl;
    hpp << "const char * search(const char * pStart,  const char * pbuff, CallbackFunction cf, void * data = NULL);" << std::endl;
    hpp.close();
    
    system("g++ file_search.cpp -o file_search");
    
	return 0;
}


