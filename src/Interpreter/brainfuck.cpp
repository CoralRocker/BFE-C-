#include <iostream>
#include <iterator>
#include <string>
#include <vector>
#include <fstream>
#include <functional>
#include "ScratchPad.hpp"

int main(int argc, char** argv){
	
	std::string fname;

	if( argc < 2 ){
		std::cerr << "Err: no filename given." << std::endl;
		std::cout << "Enter filename to process: ";
		std::cin >> fname;
	}else{
		fname = argv[1];
	}

	std::ifstream file (fname);
	if( !file.is_open() ){
		std::cerr << "Err: file not opened." << std::endl;
		return -1;
	}

	// Store current filestream to a vector
	std::vector<std::ifstream*> file_vec;
	file_vec.push_back(&file);
	
	uint32_t scratch_id = 0;

	std::vector<ScratchPad*> prog_vec;
	ScratchPad *prog = new ScratchPad(scratch_id++);
	prog_vec.push_back(prog);

	char c;
	bool comment = false;
	while( !file.get(c).fail() ){ // Get C while stream isn't in failstate
		
		// Check for comments real quick...
		switch(c) {
			case '/':
			{
				int nxt = file.peek();
				if( nxt == '*' ){
					comment = true;
					file.get(c);
				}else if( nxt == '/' ){
					while( !file.get(c).fail() ){ if( c == '\n' ) break; }
					comment = false;
				}
				break;
			}
			case '*':
			{
				if( !comment ) break;
				int nxt = file.peek();
				if( nxt == '/' ){
					comment = false;
				}
				break;
			}
		}
		
		// Main program switch
		if( !comment ){
		switch(c) {
			case '>':
				prog->next();
				break;
			case '<':
				prog->prev();
				break;
			case '+':
				// std::cout << "incrementing" << std::endl;
				++*prog;
				break;
			case '-':
				--*prog;
				break;
			case '.':
				prog->print();
				break;
			case ',':
				std::cin >> c;
				*prog = c;
				break;
			case '[':
				prog->loopStart(file);
				break;
			case ']':
				prog->loopEnd(file);
				break;
			case '{':
			{
				ScratchPad *tmp = new ScratchPad(scratch_id++);
				prog_vec.push_back(tmp);
				prog->populate(*tmp);
				prog = prog_vec.back();
				break;
			}
			case '}':
			{
				// Remove program from vector
				prog_vec.pop_back();
				
				prog->populate(*prog_vec.back()); // Populate next scratchpad
				
				prog = prog_vec.back();

				break;
			}
			case 'd':
				prog->printPad();
				break;
			case '~':
				prog->to_end();
				break;
			case '|':
				prog->trimMemory();
				break;
			case '^':
				prog->to_start();
				break;
		}}
	}

	// Give us a nice new line, precious
	std::cout << std::endl;

	return 0;
}
