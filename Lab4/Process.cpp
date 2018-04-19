/*
 * Process implementation 
 * 
 * File:   Process.cpp
 * Author: Mike Goss <mikegoss@cs.du.edu>
 * 
 */

#include "Process.h"

#include <algorithm>
#include <cctype>
#include <iomanip>
#include <iostream>
#include <sstream>

using std::cin;
using std::cout;
using std::cerr;
using std::getline;
using std::istringstream;
using std::string;
using std::vector;

Process::Process(string file_name_) 
: file_name(file_name_), line_number(0) {
  // Open the trace file.  Abort program if can't open.
  trace.open(file_name, std::ios_base::in);
  if (!trace.is_open()) {
    cerr << "ERROR: failed to open trace file: " << file_name << "\n";
    exit(2);
  }
}

Process::~Process() {
  trace.close();
}

void Process::Run(void) {
  // Read and process commands
  string line;                // text line read
  string cmd;                 // command from line
  vector<uint32_t> cmdArgs;   // arguments from line
  
  // Select the command to execute
  while (ParseCommand(line, cmd, cmdArgs)) {
    if (cmd == "memsize" ) {
      CmdMemSize(line, cmd, cmdArgs);    // allocate memory
    } else if (cmd == "diff") {
      CmdDiff(line, cmd, cmdArgs);  // get and compare multiple bytes
    } else if (cmd == "store") {
      CmdStore(line, cmd, cmdArgs);      // put bytes
    } else if (cmd == "replicate") {
      CmdRepl(line, cmd, cmdArgs);     // fill bytes with value
    } else if (cmd == "duplicate") {
      CmdDupl(line, cmd, cmdArgs);     // copy bytes to dest from source
    } else if (cmd == "print") {
      CmdPrint(line, cmd, cmdArgs);     // dump byte values to output
    } else if (cmd != "#") {
      cerr << "ERROR: invalid command\n";
      exit(2);
    }
  }
}

bool Process::ParseCommand(
    string &line, string &cmd, vector<uint32_t> &cmdArgs) {
  cmdArgs.clear();
  line.clear();
  
  // Read next line
  if (std::getline(trace, line)) {
    ++line_number;
    cout << std::dec << line_number << ":" << line << "\n";
    
    // No further processing if comment or empty line
    if (line.size() == 0 || line[0] == '#') {
      cmd = "#";
      return true;
    }
    
    // Make a string stream from command line
    istringstream lineStream(line);
    
    // Get command
    if (!(lineStream >> cmd)) {
      // Blank line, treat as comment
      cmd = "#";
      return true;
    }
    
    // Get arguments
    uint32_t arg;
    while (lineStream >> std::hex >> arg) {
      cmdArgs.push_back(arg);
    }
    return true;
  } else if (trace.eof()) {
      return false;
  } else {
    cerr << "ERROR: getline failed on trace file: " << file_name 
            << "at line " << line_number << "\n";
    exit(2);
  }
}

void Process::CmdMemSize(const string &line, 
                         const string &cmd, 
                         const vector<uint32_t> &cmdArgs) {
  // Allocate the specified memory size
  mem.resize(cmdArgs.at(0), 0);
}

void Process::CmdDiff(const string &line,
                              const string &cmd,
                              const vector<uint32_t> &cmdArgs) {
  uint32_t addr = cmdArgs.back();

  // Compare specified byte values
  int count = cmdArgs.size() - 1;
  for (int i = 0; i < count; ++i) {
    if(mem.at(addr) != cmdArgs.at(i)) {
      cout << "diff error at address " << std::hex << addr
              << ", expected " << static_cast<uint32_t>(cmdArgs.at(i))
              << ", actual is " << static_cast<uint32_t>(mem.at(addr)) << "\n";
    }
    ++addr;
  }
}

void Process::CmdStore(const string &line,
                       const string &cmd,
                       const vector<uint32_t> &cmdArgs) {
  // Store multiple bytes starting at specified address
  uint32_t addr = cmdArgs.back();
  int count = cmdArgs.size() - 1;
  for (int i = 0; i < count; ++i) {
     mem.at(addr++) = cmdArgs.at(i);
  }
}

void Process::CmdDupl(const string &line,
                      const string &cmd,
                      const vector<uint32_t> &cmdArgs) {
  // Duplicate specified number of bytes to destination from source
  auto dst = mem.begin() + cmdArgs.at(2);
  auto src = mem.begin() + cmdArgs.at(1);
  auto src_end = src + cmdArgs.at(0);
  std::copy(src, src_end, dst);
}

void Process::CmdRepl(const string &line,
                      const string &cmd,
                      const vector<uint32_t> &cmdArgs) {
  // Replicate specified value in destination range
  uint8_t value = cmdArgs.at(0);
  uint32_t count = cmdArgs.at(1);
  uint32_t addr = cmdArgs.at(2);
  for (int i = 0; i < count; ++i) {
     mem.at(addr++) = value;
  }
}

void Process::CmdPrint(const string &line,
                       const string &cmd,
                       const vector<uint32_t> &cmdArgs) {
  uint32_t addr = cmdArgs.at(1);
  uint32_t count = cmdArgs.at(0);

  // Output the address
  cout << std::hex << addr;

  // Output the specified number of bytes starting at the address
  for(int i = 0; i < count; ++i) {
    if((i % 16) == 0) {  // line break every 16 bytes
      cout << "\n";
    }
    cout << " " << std::setfill('0') << std::setw(2)
            << static_cast<uint32_t> (mem.at(addr++));
  }
  cout << "\n";
}
