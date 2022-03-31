#include "ast.hpp"
#include <algorithm>
#include <ostream>
#include <vector>
#include <iostream>
#include <utility>

// #########################################################
// ###   ABSTRACT SYNTAX TREE FUNCTIONS                  ###
// #########################################################

AST::AST() : _root(ROOT, nullptr), current(&_root) {}

AST::~AST() {
  delete_node(&_root);
}


void AST::insertOp(TYPE op, bool close) {
  node_ptr cur;
  if( (cur = current->curChild()) ){
    if( op == LOOP && close ) { // Check if parent is loop, close if so.
      if( current->getType() != LOOP ) throw runtime_error("Cannot close loop! Parent is not loop");
      current->incVal();
      current = current->getParent();
    }else{
      switch( op ) {
        case INC:
        case DEC:
          if( cur->getType() == op ){ // Increment inc/dec counter if there.
            cur->incVal();
          }else{
            node_ptr c = new astNode(op, current, 1); // Create node if not.
            current->addChild(c);
          }
          break;
        case LOOP:{
          node_ptr c = new astNode(op, current);
          current->addChild(c);
          current = c;
          }
          break;
        case LSHIFT:
        case RSHIFT:
          if( cur->getType() == op ) // increment shift counters
            cur->incVal();
          else{
            node_ptr c = new astNode(op, current, 1);
            current->addChild(c);
          }
          break;
        case GETC:
        case PUTC:
        case DEBUG:
        case PRINTNUM:{
          node_ptr c = new astNode(op, current);
          current->addChild(c);
          }
          break;
        default:
          break; 
      }
    }
  }else{
    node_ptr c = new astNode(op, current, 1);
    current->addChild(c);
    if( op == LOOP ) current = c;
  }
}

void AST::delete_node(node_ptr n) {
  for( node_ptr child : n->getChildren() ){
    delete_node(child);
    delete child;
  }
}

int tabnum;

void AST::print(std::ostream &os) const{
  tabnum = 0;
  print_node(os, &_root);
}

string tabs(int tabnum) {
  string res;
  for( int i = 0; i < tabnum; i++ ){
    res.push_back('\t');
  }
  return res;
}

void AST::print_node(ostream &os, const_node_ptr n) const {
  switch(n->getType() ) {
    case INC:
      os << tabs(tabnum) << "+" << n->val() << endl;
      break;
    case DEC:
      os << tabs(tabnum) << "-" << n->val() << endl;
      break;
    case RSHIFT:
      os << tabs(tabnum) << ">" << n->val() << endl;
      break;
    case LSHIFT:
      os << tabs(tabnum) << "<" << n->val() << endl;
      break;
    case LOOP: 
      os  << tabs(tabnum++) << "{ " << endl;;
      for(const_node_ptr child : n->getChildren()) {
        print_node(os, child);
      }
      os << tabs(--tabnum) << "} " << endl;
      break;
    case PUTC:
      os << tabs(tabnum) << ". " << endl;
      break;
    case GETC:
      os << tabs(tabnum) << ", " << endl;
      break;
    case ROOT:
      os << "ROOT: " << endl;
      for(const_node_ptr child : n->getChildren() ){
        print_node(os, child);
      }
      os << "END" << endl;
      break;
    case DEBUG:
      os << tabs(tabnum) << "DEBUG_INFO " << endl;
      break;
    case PRINTNUM:
      os << tabs(tabnum) << "PRINT_NUM " << endl;
      break;
    case MULT:
      os << tabs(tabnum) << "*";
      for( const pair<int,int>& p : n->getOffsets() ){
        os<< "(" << p.first << "," << p.second << ")";
      }
      os <<endl;
      break;
    case SET:
      os << tabs(tabnum) << "=" << n->val() << endl;
      break;
    case MOVE:
      os << tabs(tabnum) << "=>" << n->val() << "(";
      for( const pair<int,int>& p : n->getOffsets() ){
        os << " " << p.first;
      }
      os << " )" << endl;
      break;
  }
}

bool AST::is_add_sub_loop(const_node_ptr n) const{
  if( n->getType() != LOOP ) return false;
  
  for( const_node_ptr c : n->getChildren() ){
    switch( c->getType()){
      case RSHIFT:
      case LSHIFT:
      case INC:
      case DEC:
        break;
      default:
        return false;
        break;
    }
  }

  return true;
}

bool AST::is_move_loop(const_node_ptr n) const {
  if( n->getType() != LOOP ) return false;
  
  for( const_node_ptr c : n->getChildren() ){
    switch(c->getType()) {
      case RSHIFT:
      case LSHIFT:
      case DEC:
      case MOVE:
        break;
      default:
        return false;
        break;
    }
  }
  return true;
}

void AST::optimize() {
  optimize_node(&_root);
}

void AST::optimize_node(node_ptr n){
  if( is_add_sub_loop(n) ){
    // Mult/Div optimization
    int offset = 0;
    bool non_one = false;
    for( const_node_ptr c : n->getChildren() ){
      switch( c->getType() ){// TODO: offset
        case INC:
          if( !offset ) break;
          n->offset_pairs.push_back({offset, c->val()});
          if( c->val() != 1 ) non_one = true;
          break;
        case DEC:
          if( !offset ) break;
          n->offset_pairs.push_back({offset, c->val()});
          if( c->val() != 1 ) non_one = true;
          break;
        case RSHIFT:
          offset += c->val();
          break;
        case LSHIFT:
          offset -= c->val();
          break;
        default:
          break;
      }
    }
    if( offset != 0) {
      n->offset_pairs.clear();
      return;
    } if( n->offset_pairs.empty() ) {
      n->type = SET;
      n->value = 0;
    }else if( !non_one ){
      n->type = MOVE;
      n->value = 0;
    }else{
      n->type = MULT;
      n->value = 0;
    }

  }else{
    optimize_loop_body(n);
    for( node_ptr c : n->getChildren() ){
      optimize_node(c);
    }
  }
};

void AST::optimize_loop_body(node_ptr n) {
  if( n->getType() != LOOP ) return;
   
  const vector<node_ptr>& children = n->getChildren();
  node_ptr lookahead = nullptr, cur = nullptr;
  vector<node_ptr> newchildren;
  
  bool move_loop = is_move_loop(n);

  for( auto it = children.begin(); it != children.end()-1; ++it ){
    lookahead = *(it+1); cur = *it; 
    if( move_loop ) {
      
    }
  }
  
  

}

// #########################################################
// ###   ASSEMBLY CODE GENERATION                        ###
// #########################################################

int loop;
int shift;
bool debug, errmng;
void AST::genAsm(std::ostream &os, int ARRSIZE, unsigned fdin, unsigned fdout, bool debug, bool err) const {
  loop = 0;
  shift = 0;
  ::debug = debug;
  errmng = err;

  os << "\t.section .data" << endl;
  os << "fdin:" << endl;
  os << "\t.long " << fdin << endl;
  os << "fdout:" << endl;
  os << "\t.long " << fdout << endl;
  os << "\t.section .rodata" << endl;
  os << "fail_alloc:" << endl;
  os << "\t.string \"Failed to allocate memory! (malloc/realloc returned nullptr)\"" << endl;
  os << "fail_write:" << endl;
  os << "\t.string \"Failed to write byte to fdout! (call write returned 0)\"" << endl;
  os << "fail_read:" << endl;
  os << "\t.string \"Failed to read byte from fdin! (call read returned 0)\"" << endl;
  os << "print_num:" << endl;
  os << "\t.string \"%llu\"" << endl;
  os << "debug:" << endl;
  os << "\t.string \"(%lld, cell %llu, %llu cells)\\n\"" << endl; 
  os << "\t.section .text" << endl;
  os << "\t.globl main" << endl;
  os << "xmalloc:" << endl;
  os << "\tlea (,%rdi,8), %rdi" << endl;
  os << "\tmov $1, %rsi" << endl;
  os << "\tcall calloc" << endl;
  os << "\tcmp $0, %rax" << endl;
  os << "\tje .Lxmerr" << endl;
  os << "\tret" << endl;
  os << ".Lxmerr:" << endl;
  os << "\tlea fail_alloc(%rip), %rdi" << endl;
  os << "\tcall puts" << endl;
  os << "\tmov $1, %rax" << endl;
  os << "\tcall exit" << endl;
  os << "xrealloc:" << endl;
  os << "\tlea (,%r14,2), %rdi" << endl;
  os << "\tcall xmalloc" << endl;
  os << "\tmov %rax, %rdi" << endl;
  os << "\tmov %r13, %rsi" << endl;
  os << "\tlea (,%r14,8), %rdx" << endl;
  os << "\tcall memmove" << endl;
  os << "\tsub %r13, %r12 # get index of r12" << endl;
  os << "\tshrq $3, %r12" << endl;
  os << "\tlea (,%r12,8), %r12" << endl;
  os << "\tadd %rax, %r12 " << endl;
  os << "\tmov %r13, %rdi" << endl;
  os << "\tpush %rax" << endl;
  os << "\tcall free" << endl;
  os << "\tpop %rax" << endl;
  os << "\tmov %rax, %r13" << endl;
  os << "\tlea (,%r14,2), %r14 " << endl;
  os << "\tlea (%r13,%r14,8), %rax" << endl;
  os << "\tcmp %rax, %r12" << endl;
  os << "\tjb .Lxrealloc" << endl;
  os << "\tcall xrealloc" << endl;
  os << ".Lxrealloc:" << endl;
  os << "\tret" << endl;
  os << "err_read:" << endl;
  os << "\tlea fail_read(%rip), %rdi" << endl;
  os << "\tcall puts" << endl;
  os << "\tmov $2, %rax" << endl;
  os << "\tcall exit" << endl;
  os << "err_write:" << endl;
  os << "\tlea fail_write(%rip), %rdi" << endl;
  os << "\tcall puts" << endl;
  os << "\tmov $2, %rax" << endl;
  os << "\tcall exit" << endl;
  os << "debug_print:" << endl;
  os << "\tmov $0, %rax" << endl;
  os << "\tlea debug(%rip), %rdi" << endl;
  os << "\tmovq (%r12), %rsi" << endl;
  os << "\tmovq %r12, %rdx" << endl;
  os << "\tsubq %r13, %rdx" << endl;
  os << "\tshrq $3, %rdx" << endl;
  os << "\tmovq %r14, %rcx" << endl;
  os << "\tcall printf" << endl;
  os << "\tret" << endl;
  os << "main:" << endl;
  os << "\tmov $" << ARRSIZE << ", %rdi" << endl;
  os << "\tcall xmalloc" << endl;
  os << "\tmov %rax, %r12" << endl;
  os << "\tmov %rax, %r13" << endl;
  os << "\tmov $" << ARRSIZE << ", %r14" << endl;
  os << "\tmov %rax, %rdi" << endl;
  os << "\tmov $0, %rsi" << endl;
  os << "\tmov $" << ARRSIZE << ", %rdx" << endl;
  os << "\tcall memset"  << endl;


  gen_asm_node(os, &_root);
}

void AST::gen_asm_node(std::ostream &os, const_node_ptr n) const {
  switch( n->getType() ){
    case INC:
      os << endl;
      os << "\taddq $" << n->val() << ", (%r12)" << endl; 
      break;
    case DEC:
      os << endl;
      os << "\tsubq $" << n->val() << ", (%r12)" << endl;
      if( errmng ) 
         os << "\tmovq (%r12), %rcx" << endl
            << "\tmovq $0, %rax" << endl
            << "\tcmovsq %rax, %rcx" << endl
            << "\tmovq %rcx, (%r12)" << endl;
      break;
    case LOOP: {
      os << endl;
      int loopn = loop++;
      os << "\tjmp .L" << loopn  <<"e"<<endl
         << ".L"<<loopn<<":"<<endl;
      
      for( const_node_ptr child : n->getChildren() ){
        gen_asm_node(os, child);
      }
      
      os << endl;
      
      os << ".L"<<loopn<<"e:"<<endl
         << "\tcmpq $0, (%r12)" << endl
         << "\tjne .L"<<loopn<<endl;
      }
      break;

    case PUTC:
      os << endl;
      os << "\tmov fdout(%rip), %edi" << endl
         << "\tmov %r12, %rsi" << endl
         << "\tmov $1, %rdx" << endl
         << "\tcall write" << endl;
      if( errmng )
         os << "\tcmp $0, %rax" << endl
            << "\tje err_write" << endl;
      break;
    
    case GETC:
      os << endl;
      os << "\tmov fdin(%rip), %edi" << endl
         << "\tmov %r12, %rsi" << endl
         << "\tmovq $0, (%r12)" << endl
         << "\tmovq $1, %rdx" << endl
         << "\tcall read" << endl;
      if( errmng )
         os << "\tcmp $0, %rax" << endl
            << "\tje err_read" << endl;
      break;

    case RSHIFT:
      os << endl;
      os << "\tadd $" << 8*n->val() <<", %r12" << endl;
      if( errmng )
         os << "\tlea (%r13, %r14,8), %rax" << endl
            << "\tcmpq %rax, %r12" << endl
            << "\tjb .Ls"<< shift <<endl
            << "\tcall xrealloc" << endl
            << ".Ls"<< shift << ":" << endl;
      shift++;
      break;
    case LSHIFT:
      os << endl;
      os << "\tsub $" << n->val()*8 << ", %r12" << endl;
      if( errmng )
         os << "\tcmp %r13, %r12" << endl
            << "\tcmovl %r13, %r12" << endl;
      break;

    case DEBUG:
      os << endl;
      if( debug ) 
        os << "\tcall debug_print" << endl;
      break;
    case PRINTNUM:
      os << endl;
      os << "\txor %rax, %rax" << endl
         << "\tlea print_num(%rip), %rdi" << endl
         << "\tmovq (%r12), %rsi" << endl
         << "\tcall printf" << endl;
      break;
    case ROOT:
      for( const_node_ptr child : n->getChildren() ){
        gen_asm_node(os, child);
      }
      
      os << "\tmov %r13, %rdi" << endl
         << "\tcall free" << endl
         << "\txor %rax, %rax" << endl
         << "\tret" << endl;
      break;
    case MULT:
      os << endl;
      if( errmng ) {
        int max_off = 0;
        for( const pair<int,int>& p : n->getOffsets() ){
          if( p.first > max_off ) max_off = p.first;
        }
        if( max_off > 0 ) {
          os << "\tlea (%r13,%r14,8), %rax" << endl
             << "\tlea " << max_off * 8 << "(,%r12,), %rcx" << endl
             << "\tcmp %rax, %rcx" << endl
             << "\tjb .Ls" << shift << endl
             << "\tcall xrealloc" << endl
             << ".Ls" << shift << ":" << endl;
          shift++;
        }
      }

      os << "\tmovq (%r12), %rax" << endl;
      for( const pair<int,int>& p : n->getOffsets() ){
        os << "\tmovq $"<< p.second << ", %rcx" << endl
           << "\timulq %rax, %rcx" << endl
           << "\taddq %rcx, " << p.first*8 << "(%r12)" << endl;
      }
      os << "\tmovq $" << n->val() <<  ", (%r12)" << endl;
      break;
    case SET:
      os << endl;
      os << "\tmovq $" << n->val() << ", (%r12)" << endl;
      break;

    case MOVE:
      os << endl;
      if( errmng ) {
        int max_off = 0;
        for( const pair<int,int>& p : n->getOffsets() ){
          if( p.first > max_off ) max_off = p.first;
        }
        if( max_off > 0 ) {
          os << "\tlea (%r13,%r14,8), %rax" << endl
             << "\tlea " << max_off * 8 << "(,%r12,), %rcx" << endl
             << "\tcmp %rax, %rcx" << endl
             << "\tjb .Ls" << shift << endl
             << "\tcall xrealloc" << endl
             << ".Ls" << shift << ":" << endl;
          shift++;
        }
      }
      
      if( n->val() != 0 )
        os << "\tmovq " << 8*n->val() << "(%r12), %rax" << endl;
      else
        os << "\tmovq (%r12), %rax" << endl;

      for( const pair<int,int>& p : n->getOffsets() ){
        os << "\taddq %rax, " << 8*p.first << "(%r12)" << endl;
      }
      os << "\tmovq $0, (%r12)" << endl;
      break;
    default: 
      // TODO
      break;
  }

}

