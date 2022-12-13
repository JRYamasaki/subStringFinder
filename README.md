# subStringFinder

The goal of this code is to find determine which words in one text file are substrings of the other text file. To run the makefile you will need access to g++.

Running the makefile
```make```

Running the executable
```./main.exe ./allWords2.txt ./commonWords3.txt```

Optimizations that could've been made:
- Use fewer threads instead of one thread per word. If the commonwords text file is large, I imagine the program will crash
- Using a smarter data structure to limit the search. I was thinking of using a Trie to store the list from allWords.txt but I couldn't think of a smart enough way to store them in time. Or use the counts of each letter to allow a pruning of options instead of searching all the words
- If the text input files are large, parallize the read of each file across several reader threads
