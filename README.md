# C++ Trie that generates code<br>
Generating C++ code to search for terms from a Trie<br>
If you need to search for a small amount of hard coded strings then this might be a good solution. Whilst the Trie is fast you have to contend with the overhead of initialising the Trie data structure and the memory usage. This version of the Trie has a dump() method that will generate a search() function capable of performing a parallel search of all the words in the Trie. Some simple tests have shown this to be considerably faster than using the Trie directly. The generated search() function is pretty ugly and unwieldy but so long as you don't have too many terms it could prove useful.
<br><br>
More documentation here: https://soswin-techbits.blogspot.com/2013/11/generating-code-from-trie.html<br>
To build the sample app you just need cmake and g++. The steps are simply:<br>
cmake .<br>
make<br>
<br>
This creates an executable called trie_code_gen. It only has one parameter which is the filename of a dictionary file, which is just a new line separated list of key words (ASCII). Example:<br>
./trie_code_gen example_dictionary.txt<br>
This will create 2 files, search.cpp and search.h.<br>
An example main that utilises the search method looks like this (this example has also been added as example_main.cpp):<br>
```
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

```
Example output:
```
Found word :cat
At position :6
Found word :sat
At position :10
Found word :on
At position :13
Found word :the
At position :17
Found word :mat
At position :21
Found word :cat
At position :46
```
NB The position is the end of the match.<br>
<br>
