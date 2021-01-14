# C++ Trie that generates code<br>
Generating C++ code from a Trie
If you need to search for a small amount of hard coded strings then this might be a good solution. Whilst the Trie is fast you have to contend with the overhead of initialising the Trie data structure and the memory usage. This version of the Trie has a dump() method that will generate a search() function capable of performing a parallel search of all the words in the Trie. Some simple tests have shown this to be considerably faster than using the Trie directly. The generated search() function is pretty ugly and unwieldy but so long as you don't have too many terms it could prove useful.
<br><br>
More documentation here: https://soswin-techbits.blogspot.com/2013/11/generating-code-from-trie.html<br>
