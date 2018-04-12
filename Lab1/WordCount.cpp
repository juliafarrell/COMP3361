/* 
 * File:   WordCount.cpp
 * Author: julia farrell
 * 
 * Created on March 30, 2018, 4:53 PM
 */

#include "WordCount.h"
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>

using namespace std;

WordCount::WordCount() {
    fileName = "none";
    wordCount = -1;
    lineCount = -1;
    maxInLine = -1;
}

WordCount::WordCount(string fileInput) {
    fileName = fileInput;
    ifstream ifs;
    ifs.open(fileName);
    // check if file opened
    if (!ifs) {
        throw runtime_error("File '" + fileInput + "' could not open properly");
    }
    wordCount = 0;
    lineCount = 0;
    maxInLine = 0;
    int curLine = 0;
    string currLine;
    string curWord;
    if (ifs.is_open()) {
        while(!ifs.eof()) {
            std::getline(ifs, currLine);
            lineCount++;
            std::istringstream line(currLine);
            while (line >> curWord) {
                curLine++;
                wordCount++;
            }
            if (curLine > maxInLine) {
                maxInLine = curLine;
                curLine = 0;
            } else {
                curLine = 0;
            }
        }
    }
}

WordCount::WordCount(const WordCount& orig) {
    throw runtime_error("No copy constructor");
}

WordCount::~WordCount() {
    // empty destructor
}

int WordCount::getWordCount() {
    return wordCount;
}

int WordCount::getLineCount() {
    return lineCount;
}

int WordCount::getMaxWordsInLine(){
    return maxInLine;
}


//for (int i = 0; i < me.size(); i++) {     
//                if ((i < me.size() - 1 
//                        && me.substr(i,1) == " " 
//                        && me.substr(i+1,1) != " ")
//                        || i == me.size() - 1) {
//                    wordCount++;
//                    curLine++;
//                }
//            }
//            if  (curLine > maxInLine)  {
//                maxInLine = curLine;
//                curLine = 0;
//            } else {
//                curLine = 0;
//            }