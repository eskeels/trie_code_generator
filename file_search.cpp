// This source was written by Stephen Oswin, and is placed in the
// public domain. The author hereby disclaims copyright to this source
// code.

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

std::u32string to_utf32(const std::string& s)
{
    std::wstring_convert<std::codecvt_utf8<int32_t>, int32_t> convert;
    auto asInt = convert.from_bytes(s);
    return std::u32string(reinterpret_cast<char32_t const*>(asInt.data()), asInt.length());
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

    std::u32string buffer;
    std::string line;
    for (; std::getline(inputFile, line); lineCount++) {
        buffer += to_utf32(line);
    }

    const CharT* p = buffer.c_str();
    const CharT* bufferStart = p;
    const CharT* bufferEnd = &buffer[buffer.size()];
    while( p && (*p != '\0') ) {
        const CharT* newp = search(&buffer[0], &buffer[buffer.size()], p, sr);

        // for single character matches. I should sort out the search method really...
        p = (newp == p ? newp + 1 : newp);
    }

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

