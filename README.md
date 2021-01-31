# C++ Trie that generates code<br>
Generating C++ code to search for terms from a Trie<br>
If you need to search for a fixed amount of hard coded strings then this might be a good solution. Whilst the Trie is fast you have to contend with the overhead of initialising the Trie data structure and the memory usage. This version of the Trie has a dump() method that will generate a search() function capable of performing a parallel search of all the words in the Trie. Some simple tests have shown this to be considerably faster than using the Trie directly. The search() function will find partial matches.
<br><br>
More documentation here: https://soswin-techbits.blogspot.com/2013/11/generating-code-from-trie.html<br>
To build the sample app you just need cmake and g++. The steps are simply:<br>
cmake .<br>
make<br>
<br>
This creates an executable called trie_code_gen. It only has one parameter which is the filename of a dictionary file, which is just a new line separated list of key words (ASCII). Example:<br>
./trie_code_gen example_dictionary.txt<br>
This will create 2 files, search.cpp and search.h and build a new executable called file search.<br>
There is only one parameter, the name of the file to search:<br>
```
./file_search example_file.txt
```
Example output:
```
Found word [on] line :3 column :26
Found word [on] line :3 column :40
Found word [on] line :6 column :29
Found word [on] line :8 column :52
Found word [the] line :12 column :2
```
NB The position is the end of the match.<br>
<br>
