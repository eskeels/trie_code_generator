// trie.cpp : Defines the entry point for the console application.
//
#include <map>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <random>
#include <set>
#include <time.h>

#include "Trie.h"

using namespace TDS;

template <typename CharType>
void AddWord( const std::basic_string<CharType>& s, Trie<CharType>& t, std::map<const void *,std::basic_string<CharType> >& d)
{
	d[t.AddWord(s)]=s;
}

int main(int argc, char* argv[])
{
	Trie<char> t;
	std::map<const void *,std::string> dictionary;
	std::vector<SearchResult<char> > searchResults;

	AddWord<char>("cat", t, dictionary);
	AddWord<char>("cats", t, dictionary);
	AddWord<char>("peter", t, dictionary);
	AddWord<char>("wendy", t, dictionary);
	AddWord<char>("hook", t, dictionary);
	AddWord<char>("hooks", t, dictionary);
	AddWord<char>("party", t, dictionary);
	AddWord<char>("pillows", t, dictionary);

    t.dump();

	return 0;
}

