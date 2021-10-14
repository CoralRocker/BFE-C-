#include <stdexcept>
#include <vector>
#include <iostream>
#include <algorithm>
#include "ScratchPad.hpp"

using namespace std;

ScratchPad::ScratchPad(uint32_t id) {
	padId = id;
	bf_vec.push_back(0);
	pos = 0;
}

ScratchPad& ScratchPad::operator++() {
	bf_vec.at(pos)++;
	return *this;
}

ScratchPad& ScratchPad::operator--() {
	if( bf_vec.at(pos) != 0 )
		bf_vec.at(pos)--;
	return *this;
}

ScratchPad& ScratchPad::operator=(unsigned char x) {
	bf_vec.at(pos) = x;
	return *this;
}

ScratchPad& ScratchPad::operator<<(unsigned char x) {
	bf_vec.at(pos) = x;
	return *this;
}

bool ScratchPad::loopStart(std::ifstream& f){
	if( get() != 0 ){
		loop_vec.push_back(f.tellg());
		return true;
	}else{
		char c;
		while( !f.get(c).fail() ){
			if( c == ']' ) break; //  Continue to end of loop
		}
		return false;
	}
}

bool ScratchPad::loopEnd(std::ifstream& f){
	if( get() != 0 ){
		f.seekg(loop_vec.back());
		return false;
	}else{
		loop_vec.pop_back();
		return true;
	}
}

void ScratchPad::next() {
	if( ++pos >= bf_vec.size() ){
		bf_vec.push_back(0);
	}
}

void ScratchPad::prev() {
	if( pos-- <= 0 ){
		pos = 0;
	}
}

void ScratchPad::populate(ScratchPad& next){
	uint8_t cur = get();
	if( cur == 0 ){
		return;
	}else{
		for( uint8_t i = 1; i <= cur; i++ ){
			next.get(i) = get(pos+i);
			next.next();
		}
		next.to_start();
	}
}

void ScratchPad::trimMemory(){
	int x = get();
	auto new_end = std::remove_if(bf_vec.begin(), bf_vec.end(), [&](uint8_t c) -> bool { return c == x; });	
	bf_vec.erase(new_end, bf_vec.end());	
}

void ScratchPad::printPad() {
	cout << "ScratchPad " << padId << endl;
	cout << "[ ";
	for(uint8_t &c: bf_vec) {
		cout << +c << " ";
	}
	cout << "]" << endl;
	
	cout << "  ";
	for( size_t i = 0; i < bf_vec.size(); i++ ){
		if( i == pos ) std::cout << "^ ";
		else std::cout << "  ";
	}
	cout << endl;
}

void ScratchPad::print() {
	cout << bf_vec.at(pos);
}
