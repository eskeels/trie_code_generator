// This source was written by Stephen Oswin, and is placed in the
// public domain. The author hereby disclaims copyright to this source
// code.

#ifdef _WINDOWS
#include <windows.h>
#pragma execution_character_set( "utf-8" )
#endif

#include <iostream>
#include <fstream>
#include <string>
#include <codecvt>
#include <iterator>
#include <locale>

#include "search.h"
#include "search.cpp"
#include "scan_result.h"

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

    ScanResult sr;

    size_t lineCount = 0;

    std::cout << "Converting to wide.." << std::endl;
    std::wstring_convert<std::codecvt_utf8<int32_t>, int32_t> convert;

    std::u32string buffer;
    buffer.reserve(1024 * 1024 * 2);
    std::string line;
    for (; std::getline(inputFile, line); lineCount++) {
        buffer.append((const CharT*)convert.from_bytes(line).c_str());
        buffer += U'\n';
    }

    sr.setMaxMatches(1000);

    std::cout << "Starting scan.." << std::endl;
    const CharT* p = buffer.c_str();
    const CharT* bufferStart = p;
    const CharT* bufferEnd = &buffer[buffer.size()];
    while( p && (*p != '\0') ) {
        const CharT* newp = search(&buffer[0], &buffer[buffer.size()], p, sr);

        // for single character matches. I should sort out the search method really...
        p = (newp == p ? newp + 1 : newp);
    }

#ifdef _WINDOWS
    // set console to utf-8
    SetConsoleOutputCP(CP_UTF8);
    // if this doesnt work call chcp 65001 from command prompt
#endif
    
    sr.dump(bufferStart, bufferEnd, 5);
  
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

