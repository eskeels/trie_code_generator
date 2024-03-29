// This source was written by Stephen Oswin, and is placed in the
// public domain. The author hereby disclaims copyright to this source
// code.

#include <map>
#include <iostream>
#include <fstream>
#include <sstream>
#include <codecvt>
#include <iterator>
#include <locale>
#include <algorithm>
#include <random>
#include <set>
#include <time.h>
#include <stdlib.h>

typedef char32_t CharType;

#include "Trie.h"

using namespace TDS;

template <typename CharType>
void AddWord(const std::basic_string<CharType>& s, 
             Trie<CharType>& t,
             const std::string& dictionaryName,
             int16_t score,
             bool caseSensitive,
             bool distinct,
             bool wholeWord)
{
	t.AddWord(s, dictionaryName, score, caseSensitive, distinct, wholeWord);
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

std::string getDictionaryName(const std::string& line) {
    if (line[0] == '[') {
        auto pos = line.find("]");
        if (pos != -1) {
            return line.substr(1, pos-1);
        }
    }
    return "";
}

bool processLine(const std::string& line,
                 std::string& term,
                 int16_t& score, 
                 bool& caseSensitive, 
                 bool& distinct, 
                 bool& wholeWord) {
    std::basic_stringstream<char> ss(line);
    std::vector<std::string> result;
    
    while (ss.good()) {
        std::string substr;
        getline(ss, substr, ',');
        result.push_back(substr);
    }

    if (result.size() == 5) {

        term = result[0];
        score = std::stoi(result[1]);
        caseSensitive = (result[2] == "t" || result[2] == "T");
        distinct = (result[3] == "t" || result[3] == "T");
        wholeWord = (result[4] == "t" || result[4] == "T");
    } else {
        std::cout << "Rejecting " << line << std::endl;
        return false;
    }
    return true;
}

int ReadFile(const std::string& filename, Trie<char32_t>& t, std::vector<std::string>& dictionaryNames) {
    std::fstream inputFile(filename.c_str());
    if (inputFile.fail())
    {
        std::cerr << "Failed to open " << filename << std::endl;
        return EXIT_FAILURE;
    }

    int16_t score = 0;
    std::string dictionaryName = "dictionary1";
    bool caseSensitive = false;
    bool distinct = false;
    bool wholeWord = false;

    int termCount = 0;
    int dictionaryCount = 0;
    int rejectedCount = 0;

    std::wstring_convert<std::codecvt_utf8<int32_t>, int32_t> convert;

    std::string term;
    for( std::string line; std::getline(inputFile,line); ) {
        
        if (line[0] == '[') {
            dictionaryName = getDictionaryName(line);
            std::cout << "Processing dictionary " << dictionaryName << std::endl;
            ++dictionaryCount;
            continue;
        }

        if (line[0] == '#') {
            continue;
        }

        if (processLine(line, term, score, caseSensitive, distinct, wholeWord)) {
            std::u32string wterm((const char32_t*)convert.from_bytes(term).c_str());
            AddWord<char32_t>(wterm, t, dictionaryName, score, caseSensitive, distinct, wholeWord);
            ++termCount;
        } else {
            ++rejectedCount;
        }
    }

    std::cout << std::endl
        << "Processed "
        << dictionaryCount << " dictionaries, "
        << termCount << " terms."
        << " Rejected " << rejectedCount << " terms." << std::endl;

    return 0;
}

int main(int argc, char* argv[])
{
    std::string fname;

    if (0 != ParseCmdLine(argc, argv, fname))
    {
        return EXIT_FAILURE;
    }

	Trie<char32_t> t;
    std::vector<std::string> dictionaryNames;

    if (0 != ReadFile(fname, t, dictionaryNames))
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
    hpp << "#include <cctype>" << std::endl;
    hpp << "typedef char32_t CharT;" << std::endl;
    hpp << "#include \"scan_result.h\"" << std::endl;
    hpp << "const CharT * search(const CharT * pStart, const CharT * pEnd, const CharT * pbuff, ScanResult& result);" << std::endl;
    hpp.close();
#ifdef _WINDOWS
    system("cl /EHsc file_search.cpp");
#else
    system("g++ -O3 file_search.cpp -o file_search");
#endif // _WINDOWS
    
	return 0;
}


