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

namespace fs = std::filesystem;
using namespace std;
void prepare_asm(ostream &of);

void usage(char* prog) {
  cerr << "usage: " << prog << " [-s] [-o outputname] [-a array size] filename" << endl;
  cerr << "\t -s: don't compile, return assembly code." << endl;
  cerr << "\t -o [name]: output to given file" << endl;
  cerr << "\t -a [size]: size of default bf array" << endl;
  cerr << "\t -h: print this help dialogue" << endl;
  cerr << "\t -d: enable debug messages in the output program" << endl;
  cerr << "\t -I [fd]: default file descriptor for input. Must already be open." << endl;
  cerr << "\t -O [fd]: default file descriptor for output. Must already be open." << endl;
  cerr << "\t -e: don't include error management. Dynamic memory is disabled. works with -a." << endl;
  exit(-1);
}

bool validate_fname(string_view fname) {
  return fname.ends_with(".b") || fname.ends_with(".bf");
} 

int main(int argc, char** argv) {

  if( argc < 2 ) {
    cerr << "Must have at least one argument!" << endl;
    usage(argv[0]);
  }

  bool compile = true;
  bool err_manage = true;
  string ofname = "", ifname = ""; 
  int opt;
  int ARR_SIZE = 1024;
  int default_i = 0, default_o = 1;
  while( (opt = getopt(argc, argv, "so:a:heI:O:")) != -1 ) {
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
      case 'h':
        if( optarg ){
          
        }
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


  int loopCount = 0;
  long long shiftn = 0;
  stack<int> loop;


  ifstream file(ifname);
  stringstream of;

// TODO: Use correct exit codes on errors. 
  of << "  .section .data" << endl;
  of << "fdin:" << endl;
  of << "  .long " << default_i << endl;
  of << "fdout:" << endl;
  of << "  .long " << default_o << endl;
  of << "  .section .rodata" << endl;
  of << "fail_alloc:" << endl;
  of << "  .string \"Failed to allocate memory! (malloc/realloc returned nullptr)\"" << endl;
  of << "fail_write:" << endl;
  of << "  .string \"Failed to write byte to fdout! (call write returned 0)\"" << endl;
  of << "fail_read:" << endl;
  of << "  .string \"Failed to read byte from fdin! (call read returned 0)\"" << endl;
  of << "print_num:" << endl;
  of << "  .string \"%llu\"" << endl;
  of << "  .section .text" << endl;
  of << "  .globl main" << endl;
  of << "xmalloc:" << endl;
  of << "  mov $1, %rsi" << endl;
  of << "  call calloc" << endl;
  of << "  cmp $0, %rax" << endl;
  of << "  je .Lxmerr" << endl;
  of << "  ret" << endl;
  of << ".Lxmerr:" << endl;
  of << "  lea fail_alloc(%rip), %rdi" << endl;
  of << "  call puts" << endl;
  of << "  mov $1, %rax" << endl;
  of << "  call exit" << endl;
  of << "xrealloc:" << endl;
  of << "  lea (,%r14,2), %rdi" << endl;
  of << "  call xmalloc" << endl;
  of << "  mov %rax, %rdi" << endl;
  of << "  mov %r13, %rsi" << endl;
  of << "  mov %r14, %rdx" << endl;
  of << "  call memmove" << endl;
  of << "  sub %r13, %r12 # get index of r12" << endl;
  of << "  add %rax, %r12 " << endl;
  of << "  mov %r13, %rdi" << endl;
  of << "  push %rax" << endl;
  of << "  call free" << endl;
  of << "  pop %rax" << endl;
  of << "  mov %rax, %r13" << endl;
  of << "  lea (,%r14,2), %r14 " << endl;
  of << "  ret" << endl;
  of << "err_read:" << endl;
  of << "  lea fail_read(%rip), %rdi" << endl;
  of << "  call puts" << endl;
  of << "  mov $2, %rax" << endl;
  of << "  call exit" << endl;
  of << "err_write:" << endl;
  of << "  lea fail_write(%rip), %rdi" << endl;
  of << "  call puts" << endl;
  of << "  mov $2, %rax" << endl;
  of << "  call exit" << endl;
  of << "main:" << endl;
  of << "\tmov $" << ARR_SIZE << ", %rdi" << endl;
  of << "\tcall xmalloc" << endl;
  of << "\tcmp $0, %rax" << endl;
  of << "\tmov %rax, %r12" << endl;
  of << "\tmov %rax, %r13" << endl;
  of << "\tmov $" << ARR_SIZE << ", %r14" << endl;
  of << "\tmov %rax, %rdi" << endl;
  of << "\tmov $0, %rsi" << endl;
  of << "\tmov $" << ARR_SIZE << ", %rdx" << endl;
  of << "\tcall memset" << endl;


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
        of << "\tincb (%r12)" << endl;
        break;
      case '-':
        of << "\tdecb (%r12)" << endl;
        if( err_manage ) { // protect against wrap-around
          of << "\tmov (%r12), %cl" << endl;
          of << "\tmov $0, %rax" << endl;
          of << "\tcmovsq %rax, %rcx" << endl;
          of << "\tmovb %cl, (%r12)" << endl;
        }
        break;
      case '[':
        loop.push(loopCount++);
        of << "\tjmp .L" << loop.top() << "e" << endl;
        of << ".L" << loop.top() << ":" << endl;
        break;
      case ']':
        of << ".L" << loop.top() << "e:" << endl;
        of << "\tcmpb $0, (%r12)" << endl;
        of << "\tjne .L" << loop.top() << endl;
        loop.pop();
        break;
      case '.':
        of << "\tmov fdout(%rip), %edi" << endl;
        of << "\tmov %r12, %rsi" << endl;
        of << "\tmov $1, %rdx" << endl;
        of << "\tcall write" << endl;
        if( err_manage ) {
          of << "\tcmp $0, %rax" << endl;
          of << "\tje err_write" << endl;
        }
        break;
      case ',':
        of << "\tmov fdin(%rip), %edi" << endl;
        of << "\tmov %r12, %rsi" << endl;
        of << "\tmov $1, %rdx" << endl;
        of << "\tcall read" << endl;
        if( err_manage ) {
          of << "\tcmp $0, %rax" << endl;
          of << "\tje err_write" << endl;
        }
        break;
      case '>':
        of << "\tinc %r12" << endl;
        if( err_manage ) { // Expand memory on extensive right shift.
          of << "\tcmpq (%r14,%r13,), %r12" << endl;
          of << "\tjl .Ls" << shiftn << endl;
          of << "\tcall xrealloc" << endl;
          of << ".Ls" << shiftn++ << ":" << endl;
        }
        break;
      case '<':
        of << "\tdec %r12" << endl;
        if( err_manage ) {
          of << "\tcmp %r13, %r12" << endl;
          of << "\tcmovl %r13, %r12" << endl;
        }
        break;
      case '#':
        of << "\txor %rax, %rax" << endl;
        of << "\tlea print_num(%rip), %rdi" << endl;
        of << "\tmovzb (%r12), %rsi" << endl;
        of << "\tcall printf" << endl;
    }
  }

  of << "\tmov $0, %rax" << endl;
  of << "\tret" << endl;
  
  if( compile ) {
    
    char * templ = (char*)malloc(128);
    snprintf(templ, 128, "/tmp/cmpfXXXXXX.s");
    int tmpf = mkstemps(templ, 2);
    if( tmpf == -1 ) {
      throw runtime_error(string("Error creating temporary compilation file: ") + strerror(errno));
    }

    write(tmpf, of.str().data(), of.str().size());

    cout << "Compiling " << templ << endl;

    
    if( ofname.empty() ) {
      char* const comparg[] = {(char*)"clang", templ, NULL };
      execv("/usr/bin/clang", comparg); 
    }else{
      char * const comparg[] = {(char*)"clang", (char*)templ, (char*)"-o", (char*) ofname.c_str(), NULL };
      execv("/usr/bin/clang", comparg); 
    }
    
    unlink(templ);
    free(templ);
  }else{
    string oname;
    if( ofname.empty() ) oname = ifname + ".s";
    else oname = ofname;
    
    ofstream out(oname);
    out << of.rdbuf();
    out.close();

  }
}

