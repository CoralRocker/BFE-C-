#pragma once

#include <vector>
#include <iostream>
#include <fstream>

class ScratchPad {
	private:
		uint32_t padId;
		size_t pos;
		std::vector<uint8_t> bf_vec;
		std::vector<size_t> loop_vec;

	public:
		ScratchPad(uint32_t id = 0xFFFF);

		uint32_t getId() { return padId; }
		size_t getPos() { return pos; }

		std::vector<uint8_t>& getBf() { return bf_vec; }
		std::vector<size_t>& getLoop() { return loop_vec; }
		uint8_t& get() { return bf_vec.at(pos); }
		uint8_t& get(size_t where) { return bf_vec.at(where); }
		


		bool loopStart(std::ifstream& f);
		bool loopEnd(std::ifstream& f);

		void next();
		void prev();
		void to_start() { pos = 0; }
		void to_end() { pos = bf_vec.size()-1; }

		ScratchPad& operator++ (); // ++ Prefix
		ScratchPad& operator-- (); // -- Prefix
		ScratchPad& operator= (unsigned char x); // Assign current position
		ScratchPad& operator<< (unsigned char x);

		void populate(ScratchPad& next); // Load into a new temporary scratchpad
		void depopulate(ScratchPad& next); // Load from a temporary scratchpad
		
		void trimMemory(); // Searches for all of the current value in the memory array and removes it

		void printPad();
		void print();
};
