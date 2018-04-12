/* 
 * File:   main.cpp
 * Author: julia farrell
 *
 * Created on March 30, 2018, 4:33 PM
 */

#include <cstdlib>
#include <iostream>

#include "WordCount.h"

using namespace std;

/*This program has the user input a text file 
 * and finds the word count, line count, and max number of words in a line.
 */
int main(int argc, char** argv) {
    string myFile = argv[1];
    WordCount* wc = new WordCount(myFile);
    cout << wc->getWordCount();
    cout << " words, ";
    cout << wc->getLineCount();
    cout << " lines, ";
    cout << wc->getMaxWordsInLine();
    cout << " max words in line";
    return 0;
}

