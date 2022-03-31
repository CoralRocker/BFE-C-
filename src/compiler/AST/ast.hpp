#include <iostream>
#include <ostream>
#include <stdexcept>
#include <vector>
#include <optional>
#include <utility>

using namespace std;

class AST {
  private:
    class astNode;

  public:
    using node_ptr = astNode*;
    using node = astNode;
    using const_node = const astNode;
    using const_node_ptr = const astNode*;

    AST();
    
    ~AST();

    enum TYPE{
      ROOT,
      INC, DEC,
      RSHIFT, LSHIFT,
      GETC, PUTC,
      LOOP, DEBUG,
      PRINTNUM,
      MULT,
      SET, MOVE,
    };
    void print(ostream &os) const;
    void insertOp(TYPE op, bool close = false);
    
    void genAsm(ostream &os, int ARRSIZE = 1024, unsigned fdin = 0, unsigned fdout = 1, bool debug = false, bool err = true) const;
    
    void optimize();
  private:
    void delete_node(node_ptr n);

    void print_node(ostream &os, const_node_ptr n) const;
    
    void gen_asm_node(ostream &os, const_node_ptr n) const;
    
    bool is_add_sub_loop(const_node_ptr n) const;
    
    bool is_move_loop(const_node_ptr n) const;

    void optimize_node(node_ptr n);
    
    void optimize_loop_body(node_ptr n);

    class astNode {
      public:
        TYPE getType() const { return type; }
        astNode(TYPE t, node_ptr parent, int val = 0) : type(t), parent(parent), value(val) { } 
        node_ptr curChild() const {
          if( children.empty() ) return nullptr;
          return children.back();
        }
        node_ptr getParent() const {
          if( !parent ) throw runtime_error("Cannot get parent of root node!");
          return parent;
        }
        void incVal() {
          value++;
        }

        int val() const { return value; }
        void addChild(node_ptr c) { if( !c ) throw runtime_error("Cannot add null child!"); children.push_back(c); }
        
        const vector<node_ptr>& getChildren() const { return children; }
        
        const vector<pair<int, int>>& getOffsets() const { return offset_pairs; }
        
        friend void AST::optimize_node(node_ptr n);
        
      private:
        TYPE type;
        vector<node_ptr> children;
        node_ptr parent;
        int value;
        vector<pair<int,int>> offset_pairs;
    };

    node _root;
    node_ptr current;
};

