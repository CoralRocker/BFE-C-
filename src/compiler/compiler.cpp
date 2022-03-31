#include <string_view>
#include <unistd.h> // getopt

#include <fstream>
#include <sstream>
#include <iostream>
#include <stdexcept>
#include <stack>
#include <string>
#include <cstdio>
#include <errno.h>
#include <cstring>
#include <filesystem>

#include "AST/ast.hpp"

namespace fs = std::filesystem;
using namespace std;

void usage(char* prog) {
  cerr << "usage: " << prog << " [options] [-o outputname] filename" << endl;
  cerr << "\t -s: don't compile, return assembly code." << endl;
  cerr << "\t -o [name]: output to given file" << endl;
  cerr << "\t -a [size]: size of default bf array" << endl;
  cerr << "\t -h: print this help dialogue" << endl;
  cerr << "\t -d: enable debug messages in the output program" << endl;
  cerr << "\t -I [fd]: default file descriptor for input. Must already be open." << endl;
  cerr << "\t -O [fd]: default file descriptor for output. Must already be open." << endl;
  cerr << "\t -e: don't include error management. Dynamic memory is disabled. works with -a." << endl;
  cerr << "\t -g: compile with debug symbols for gdb" << endl;
  cerr << "\t -t: Print out abstract syntax tree for program." << endl;
  exit(-1);
}

bool validate_fname(string_view fname) {
  return fname.ends_with(".b") || fname.ends_with(".bf");
} 

string output_file(string fname, string suffix = "s"){
  size_t pos = fname.rfind(".b");
  string result = fname.substr(0, pos+1);
  return result + suffix;
}

int main(int argc, char** argv) {

  if( argc < 2 ) {
    cerr << "Must have at least one argument!" << endl;
    usage(argv[0]);
  }

  bool compile = true;
  bool err_manage = true;
  bool gdb = false;
  string ofname = "", ifname = ""; 
  int opt;
  int ARR_SIZE = 1024;
  int default_i = 0, default_o = 1;
  bool dbg = false;
  bool print_ast = false;
  while( (opt = getopt(argc, argv, "so:a:heI:O:gdt")) != -1 ) {
    switch(opt) {
      case 's':
        compile = false;
        break;
      case 'o':
        ofname = optarg;
        break;
      case 'a':
        ARR_SIZE = stoi(optarg);
        break;
      case 'e':
        err_manage = false;
        cerr << "Warning: error mitigation systems disabled!" << endl;
        cerr << "Instruction pointer will be able to go before and after the memory block!" << endl;
        cerr << "Values will not be lower bounded! (0 will roll over to 255)" << endl;
        cerr << "Reads and writes will not check for errors!" << endl;
        break;
      case 'I':
        default_i = stoi(optarg);
        break;
      case 'O':
        default_o = stoi(optarg);
        break;
      case 'g':
        gdb = true;
        break;
      case 'd':
        dbg = true;
        break;
      case 't':
        print_ast = true;
        break;
      case 'h':
      default:
        usage(argv[0]);
        break;
    }
  }
  
  if( optind == argc ){
    cerr << "Err: need filename" << endl;
    return -1;
  }
  
  ifname = argv[optind];
  if( !validate_fname(ifname) ){
    cerr << ifname << ": invalid input file." << endl;
    cerr << "Input file(s) must end with \'.b\' or \'.bf\'" << endl;
    exit(1);
  }else if( !fs::exists(ifname) ){
    cerr << ifname << ": no file exists with this name" << endl;
    exit(1);
  }

  ifstream file(ifname);
  stringstream of;
  
  AST ast;

  char c;
  while( !file.get(c).eof() ){
    switch(c) {
      case '/': // Comments
        if( file.peek() == '/' ){ // Single line comment
          while( !file.get(c).eof() )
            if( c == '\n' ) break;
        }else if( file.peek() == '*' ){ // multi line comment
          while( !file.get(c).eof() )
            if( c == '*' && file.peek() == '/' ) break;
        }
        break;

      case '+':
        ast.insertOp(AST::INC);
        break;
      case '-':
        ast.insertOp(AST::DEC);
        break;
      case '[':
        ast.insertOp(AST::LOOP, false);
        break;
      case ']':
        ast.insertOp(AST::LOOP, true);
        break;
      case '.':
        ast.insertOp(AST::PUTC);
        break;
      case ',':
        ast.insertOp(AST::GETC);
        break;
      case '>':
        ast.insertOp(AST::RSHIFT);
        break;
      case '<':
        ast.insertOp(AST::LSHIFT);
        break;
      case '#':
        ast.insertOp(AST::PRINTNUM);
        break;
      case 'd':
        ast.insertOp(AST::DEBUG);
        break;
    }
  }
  
  ast.optimize();
  if( print_ast )
    ast.print(cout);
  ast.genAsm(of, ARR_SIZE, default_i, default_o, dbg, err_manage);

  
  if( compile ) {
    
    char * templ = (char*)malloc(128);
    snprintf(templ, 128, "/tmp/cmpfXXXXXX.s");
    int tmpf = mkstemps(templ, 2);
    if( tmpf == -1 ) {
      throw runtime_error(string("Error creating temporary compilation file: ") + strerror(errno));
    }

    write(tmpf, of.str().data(), of.str().size());

    cout << "Compiling " << templ << endl;

    
    if( ofname.empty() ) 
      ofname = output_file(fs::path(ifname).filename(), "o");
    
    char * const comparg[] = {(char*)"clang", (char*)templ, (char*)"-o", (char*) ofname.c_str(), ( gdb ? (char*)"-ggdb" : NULL ), NULL };
    execv("/usr/bin/clang", comparg); 
    
    unlink(templ);
    free(templ);
  }else{
    string oname;
    if( ofname.empty() ) oname = output_file(fs::path(ifname).filename());
    else oname = ofname;
    
    ofstream out(oname);
    out << of.rdbuf();
    out.close();

  }
}

