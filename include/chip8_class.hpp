#pragma once

// When the compiler encounters #pragma once, 
// it keeps track of the file and ensures it is 
// only included once, even if there are multiple 
// #include statements for that file throughout your project.

#include<cstdint>
#include<random>

const unsigned int KEY_COUNT 		= 16;
const unsigned int MEMORY_SIZE 		= 4096;
const unsigned int STACK_LEVELS	 	= 16;
const unsigned int REGISTER_COUNT 	= 16;
const unsigned int VIDEO_HEIGHT 	= 32;	// in pixels
const unsigned int VIDEO_WIDTH 		= 64;	// in pixels

class chip8_class
{
public: 
	chip8_class();	// Constructor Declaration
	void LoadROM(char const* filename);	
	void InstrCycle(); 	
	
	uint8_t  keypad[KEY_COUNT]{};	// 16 input keys mapped to first 16 hexes: 0-F
	uint32_t video_display[64*32]{};		// screenPixel will point here
private:
	// Declaring Functions for Secondary Tables
	void Table0Decode();
	void Table8Decode();
	void TableEDecode();
	void TableFDecode();
	
	// Instruction Declarations
	void OP_NULL();

	void OP_00E0();
        // CLS
	//Clears screen 

	void OP_1nnn();	
	// JP addr
	// Jump Function

	void OP_6xkk();
	// LD Vx, byte
	// set register VX

	void OP_7xkk();
	// ADD Vx, byte
	// add value to register Vx

	void OP_Annn();
	// LD I, addr
	// load addr into Index Register

	void OP_Dxyn();
	// DRW Vx, Vy, hieght
	// Display Draw

	uint8_t  registers[REGISTER_COUNT]{};		// 16 8-bit registers for CPU operations
	uint8_t  memory[MEMORY_SIZE]{};			// 4KB memory for storing program instructions, 
							// long term and short term data [4096 addresses for 8 bit arrays] 
	uint16_t index{};		// stores memory addresses for use in operations - 16 bit cuz highest address 0xFFF is 3 nibbles (12bit) long	
	uint16_t pc{};			// program counter
	uint16_t stack[STACK_LEVELS]{};			// 16 level stack, i.e. it can hold the RET address for 16 CALLs, 
							// and thus can nest multiple CALLs 
	uint8_t  sp{};			// 8-bit stack pointer to keep track of the current stack level 
					//(as stack is 16 levels deep, even 4-bits are enough to keep track of it)
	uint8_t  delayTimer{};		/* ####### READ ABOUT BRACE INITIALIZATION ####### */		
	uint8_t  soundTimer{};
	uint16_t opcode;		// 34 instructions in a chip8
	
	std::default_random_engine randomGenerator;
	std::uniform_int_distribution<uint8_t> randomByte;
	
	typedef void (chip8_class :: *FnPtrChipInterpreter)(); 	
	// we are (type)defining a fnPtr to a function that returns 'void' and takes no parameters

	FnPtrChipInterpreter tablePrimary[17];	
	// The Chip8 Instruction set has opcodes with the initial char including all digits from 0 upto F (17 unique chars)
	FnPtrChipInterpreter table0[0xE + 1]; 	// we have only two opcodes: 00E0 & 00EE (making table based on the unique last chars 0 and E)
	FnPtrChipInterpreter table8[0xE + 1];
	FnPtrChipInterpreter tableE[0xE +1];	
	// There are two opcodes ExA1 and Ex9E: i.e. index [1] and [E] are necessary, and we know arrays are 0-indexed so we need E+1 long array
	FnPtrChipInterpreter tableF[0x65+10];	
};


