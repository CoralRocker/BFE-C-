#include <stdexcept>
#include <vector>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <filesystem>
#include <string>
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
	if( bf_vec.at(pos) > 0 )
		bf_vec.at(pos)--;
	return *this;
}

ScratchPad& ScratchPad::operator=(uint64_t x) {
	bf_vec.at(pos) = x;
	return *this;
}

ScratchPad& ScratchPad::operator<<(uint64_t x) {
	bf_vec.at(pos) = x;
	return *this;
}

ScratchPad& ScratchPad::operator+=(uint64_t x) {
	bf_vec.at(pos) += x;
	return *this;
}

ScratchPad& ScratchPad::operator-=(uint64_t x) {
	if( x > bf_vec.at(pos) )
		bf_vec.at(pos) = 0;
	else
		bf_vec.at(pos) -= x;
	return *this;
}

bool ScratchPad::loopStart(std::istringstream& f){
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

bool ScratchPad::loopEnd(std::istringstream& f){
	if( get() != 0 ){
		f.seekg(loop_vec.back());
		return false;
	}
	loop_vec.pop_back();
	return true;
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
	uint64_t cur = get();
	if( bf_vec.size() <= pos + cur ){
		cerr << "Err populating Pad " << next.getId() << " from Pad " << getId() << endl;
		throw runtime_error("Error! Attempting to populate a scratchpad with more information than you have!");
	}

	if( cur == 0 ){
		return;
	}else{
		for( uint64_t i = 1; i <= cur; i++ ){
			next.get(i-1) = get(pos+i);
			next.next();
		}
		next.to_start();
	}
}

void ScratchPad::trimMemory(){
	uint64_t x = get();
	auto new_end = std::remove_if(bf_vec.begin(), bf_vec.end(), [&](uint64_t c) -> bool { return c == x; });	
	bf_vec.erase(new_end, bf_vec.end());	
}

void ScratchPad::printPad() {
	cout << "ScratchPad " << padId << endl;
	cout << "[ ";
	for(uint64_t &c: bf_vec) {
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
	cout << static_cast<char>(bf_vec.at(pos)%256);
}
