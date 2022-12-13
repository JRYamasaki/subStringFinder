#include <iostream>
#include <vector>
#include <unordered_map>
#include <thread>
#include <mutex>
#include <algorithm>
#include <set>

#include <iterator>
#include <fstream>
#include <functional>

std::unordered_map<std::string, std::vector<std::string>> output;

void containsSubstring(
    const std::vector<std::string> wordsToCheck,
    const std::string substring,
    std::mutex& mutex) {
    for(std::string currentWord : wordsToCheck) {
        if(currentWord.find(substring) != std::string::npos) {
            mutex.lock();
            output[currentWord].push_back(substring);
            mutex.unlock();
        }
    }
}

//Things to check, does the file exist? any exceptions thrown?
std::set<std::string> fileToList (const std::string& fileName) {
    std::ifstream filein(fileName);

    // We use a set to remove duplicates from either list of words
    std::set<std::string> result;

    for (std::string line; std::getline(filein, line);) 
    {
        result.insert(line);
    }

    return result;
}

static const std::string ALL_WORDS_FILE_NAME = "allWords1.txt";
static const std::string COMMON_WORDS_FILE_NAME = "commonWords2.txt";

int main() {
    const std::set<std::string> allWords = fileToList(ALL_WORDS_FILE_NAME);
    const std::set<std::string> commonWords = fileToList(COMMON_WORDS_FILE_NAME);

    std::mutex outputMutex;
    std::vector<std::thread> threads;
    std::vector<std::string> allWordsCopy {allWords.begin(), allWords.end()};

    for(std::string word : commonWords) {
        threads.push_back(std::thread(containsSubstring, allWordsCopy, word, std::ref(outputMutex)));
    }

    for(auto& thread : threads) {
        thread.join();
    }

    std::for_each(
        output.begin(),
        output.end(),
        [](const std::pair<std::string, std::vector<std::string>> &p) {
            std::cout << p.first << ": ";
            std::cout << "[";
            for(auto substr : p.second) {
                std::cout << substr << ", ";
            }
            std::cout << "]\n";
        }
    );

    return 0;
}