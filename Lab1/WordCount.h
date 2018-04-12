/* 
 * File:   WordCount.h
 * Author: julia farrell
 *
 * Created on March 30, 2018, 4:53 PM
 */

#ifndef WORDCOUNT_H
#define WORDCOUNT_H

#include <string>


/** 
 * This class is used to calculate various quantities from a text file 
 */
class WordCount {
    std::string fileName;
    int wordCount;
    int lineCount;
    int maxInLine;
public:
    WordCount();
    WordCount(std::string);
    WordCount(const WordCount& orig);
    virtual ~WordCount(); 
    /**
     * Gets the number of words in a file,
     * assuming lines begin with a word (not a space)
     * @return # of words in the file, -1 if no file
     */
    int getWordCount();
    /**
     * Gets the number of lines in a file,
     * assuming eof is on its own line
     * @return # of words in the file, -1 if no file
     */
    int getLineCount();
    /**
     * Gets the max number of words in a line of the file,
     * assuming lines begin with a word (not a space)
     * and that the eof is on its own line
     * @return # max of words in a line in the file, -1 if no file
     */
    int getMaxWordsInLine();  
};

#endif /* WORDCOUNT_H */

