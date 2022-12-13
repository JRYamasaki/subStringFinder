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
#include <sstream>

static const uint8_t NUMBER_OF_REQUIRED_ARGUMENTS = 3;
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

std::set<std::string> fileToList (char* fileName) {
    std::ifstream filein(fileName);

    if(!filein.is_open()) {
        std::ostringstream ss;
        ss << "Cannot open file" << fileName;
        throw std::runtime_error(ss.str());
    }

    std::set<std::string> result;

    for (std::string line; std::getline(filein, line);) 
    {
        result.insert(line);
    }

    return result;
}

// void sortOutput(std::unordered_map<std::string, std::vector<std::string>>& output) {
//     std::for_each(
//         output.begin(),
//         output.end(),
//         [](const std::pair<std::string, std::vector<std::string>> &p) {
//             std::sort(p.second.begin(), p.second.end());
//         }
//     );
// }

// void printOutput(
//     const std::set<std::string>& allWords,
//     const std::unordered_map<std::string, std::vector<std::string>>& output) {
    
//     for(std::string word : allWords) {
//         std::cout << word;
//         std::cout << "[";

//         for(std::string match : output.at(word)) {
//             std::cout << match << ", ";
//         }
        
//         std::cout << "]\n";
//     }
// }

int main(int argc, char *argv[]) {
    if(argc != NUMBER_OF_REQUIRED_ARGUMENTS) {
        throw std::runtime_error("Must pass two filenames as parameters, first parameter is the list of all words, second parameter is the list of common words");
    }

    const std::set<std::string> allWords = fileToList(argv[1]);
    const std::set<std::string> commonWords = fileToList(argv[2]);

    std::mutex outputMutex;
    std::vector<std::thread> threads;
    std::vector<std::string> allWordsCopy {allWords.begin(), allWords.end()};

    for(std::string word : commonWords) {
        threads.push_back(std::thread(containsSubstring, allWordsCopy, word, std::ref(outputMutex)));
    }

    for(auto& thread : threads) {
        thread.join();
    }

    //sortOutput(output);
    //printOutput(allWords, output);

    //Print output
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