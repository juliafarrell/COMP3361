/* 
 * File:   main.cpp
 * Author: julia
 *
 * Created on April 5, 2018, 11:10 AM
 */

#include <cstdlib>

#include "Process.h"

using namespace std;

/* This program runs from the command line, and takes one argument which is
 * the name of the trace file.
 */
int main(int argc, char** argv) {
    string myFile = argv[1];
    Process(myFile).Run();
    return 0;
}

