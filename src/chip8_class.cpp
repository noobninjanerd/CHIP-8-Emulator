#include "chip8_class.hpp"
#include <random>
#include <cstdint>
#include <chrono>
#include <cstring>	// for the memset function
#include <fstream>	// LoadROM	

const unsigned int FONTSET_SIZE = 80; 		// loading into the 4KB memory, starting from the address 0x50
const unsigned int FONTSET_START_ADDRESS = 0x50; 
const unsigned int START_ADDRESS = 0x200;

uint8_t fontset[FONTSET_SIZE] = 
{
	0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
	0x20, 0x60, 0x20, 0x20, 0x70, // 1
	0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
	0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
	0x90, 0x90, 0xF0, 0x10, 0x10, // 4
	0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
	0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
	0xF0, 0x10, 0x20, 0x40, 0x40, // 7
	0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
	0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
	0xF0, 0x90, 0xF0, 0x90, 0x90, // A
	0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
	0xF0, 0x80, 0x80, 0x80, 0xF0, // C
	0xE0, 0x90, 0x90, 0x90, 0xE0, // D
	0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
	0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

chip8_class :: chip8_class() 
	: randomGenerator((std :: chrono :: system_clock :: now().time_since_epoch()).count())
	// seeding the randomGenerator (made in default_random_engine) in the constructor 
	// member initialization list. now() is a function of the class system_clock
{
	// This is a constructor. It doesn't have a return type.
	
	// Initialize the program counter
	pc = START_ADDRESS;

	// Load fonts into memory
	for (unsigned int i =0; i < FONTSET_SIZE; ++i)
	{
		memory[FONTSET_START_ADDRESS + i] = fontset[i];	
		// Loading the font-bytes into memory [4kb] starting from address 0x50
	}
	
	// Initialize RNG
	randomByte = std::uniform_int_distribution<uint8_t>(0,255U);
	
	// Set up Primary Table for the unique functions

	tablePrimary[0x0] = &chip8_class::Table0Decode;

	tablePrimary[0x1] = &chip8_class::OP_1nnn;	// JP addr 
	/*	
	tablePrimary[0x2] = &chip8_class::OP_2nnn;	// CALL addr
	tablePrimary[0x3] = &chip8_class::OP_3xkk;	// SE Vx, byte
	tablePrimary[0x4] = &chip8_class::OP_4xkk;	// SNE Vx, byte
	tablePrimary[0x5] = &chip8_class::OP_5xy0;	// SE Vx, Vy
	*/
	tablePrimary[0x6] = &chip8_class::OP_6xkk;	// LD Vx, byte
	tablePrimary[0x7] = &chip8_class::OP_7xkk;	// ADD Vx, byte

	// tablePrimary[0x8] = chip8_class::Table8Decode;
	
	// tablePrimary[0x9] = chip8_class::OP_9xy0;	// SNE Vx, Vy
	tablePrimary[0xA] = &chip8_class::OP_Annn;	// LD I, addr
	/*
	tablePrimary[0xB] = &chip8_class::OP_Bnnn;	// JP V0, addr
	tablePrimary[0xC] = &chip8_class::OP_Cxkk;	// RND Vx, byte
	*/
	tablePrimary[0xD] = &chip8_class::OP_Dxyn;	// DRW Vx, Vy, hieght

	// tablePrimary[0xE] = chip8_class::TableEDecode;	
	// tablePrimary[0xF] = chip8_class::TableFDecode;

	for (size_t i = 0; i <= 0xE; i++)
	{
		table0[i] = &chip8_class::OP_NULL;
	}

	table0[0x0] = &chip8_class::OP_00E0;	// CLS
	// table0[0xE] = chip8_class::OP_00EE;	// RET
	/*	
	for (size_t i = 0; 1 <= 0x65; i++)
	{
		tableE[i] = chip8_class::OP_NULL;
	}

	tableE[0x1] = chip8_class::OP_ExA1;	// SKNP Vx
	tableE[0xE] = chip8_class::OP_Ex9E;	// SKP Vx
	
	for (size_t i = 0; 1 <= 0x65; i++)
	{
		tableF[i] = chip8_class::OP_NULL;
	}
	
	tableF[0x07] = chip8_class::OP_Fx07;	// LD Vx, DT	
	tableF[0x0A] = chip8_class::OP_Fx0A;	// LD Vx, K
	tableF[0x15] = chip8_class::OP_Fx15;	// LD DT, Vx
	tableF[0x18] = chip8_class::OP_Fx18;	// LD ST, Vx
	tableF[0x1E] = chip8_class::OP_Fx1E;	// ADD I, Vx
	tableF[0x29] = chip8_class::OP_Fx29; 	// LD F, Vx
	tableF[0x33] = chip8_class::OP_Fx33;	// LD B, Vx
	tableF[0x55] = chip8_class::OP_Fx55; 	// LD [I], Vx
	tableF[0x65] = chip8_class::OP_Fx65; 	// LD Vx, [I]
	*/
}

void chip8_class::Table0Decode()
{
	((*this).*(table0[opcode & 0x000Fu]))();	// the u suffix makes sure the hex is treated as unsigned	
}

void chip8_class::Table8Decode()
{
	((*this).*(table8[opcode & 0x000Fu]))();
}

void chip8_class::TableEDecode()
{
	((*this).*(tableE[opcode & 0x000Fu]))();
}

void chip8_class::TableFDecode()
{
	((*this).*(tableF[opcode & 0x00FFu]))();
}

void chip8_class::LoadROM(char const* filename)
// char const* or const char* implies that the pointer is pointing to 
// constant chars or array of char (c-type string), and the pointer 
// can't be used to change the value of the char(s)
{
	std::ifstream romFile(filename, std::ios::binary | std::ios::ate);
	// std::ios::binary : opens ROM file in binary mode
	// std::ios::ate    : positions the file pointer at the end of the file, when it is opened
	if (romFile.is_open())
	{
		std::streampos size = romFile.tellg();
		// streampos : type that represents a position within a file stream
		// tellg() : returns current position of file pointer as an offset from the beginning of the file

		char* buffer = new char[size];
		// allocates a dynamic array of characters (bytes) in memory to hold the contents of the ROM file

		romFile.seekg(0, std::ios::beg);
		// seekg(): moves the file pointer to a specified location
		// offset = 0 from the beginning (beg) of the file
		
		romFile.read(buffer, size);
		// the first arguement is a pointer to the buffer where data should be stored
		
		romFile.close();

		for (long i = 0; i < size; i++)
		{
			memory[START_ADDRESS + i] = buffer[i];
		}
		delete[] buffer;		
		// deallocates the dynamically allocated memory for the buffer, preventing memory leaks

		buffer = nullptr; // Prevents dangling pointer
	}
}

void chip8_class::InstrCycle()
{
	// Fetch
	opcode = memory[pc];			// store first byte of instruction
	opcode = opcode << 8u; 			// left shift by 1 byte to make space for latter part of instruction 
	opcode = opcode | memory[pc + 1];	// OR with second byte of instruction at PC+1
	
	// Increase the PC by 2, so that it points to the next instruction
	pc = pc + 2;

	// Decode & Execute
	((*this).*(tablePrimary[(opcode & 0xF000) >> 12u]))();	// right shift by 12 units to get 4bit code

	// Decrement delay timer if it's been set
	if (delayTimer > 0)
	{
		--delayTimer;
	}

	// Decrement the sound timer if it's been set
	if (soundTimer > 0)
	{
		--soundTimer;
	}
} 

void chip8_class::OP_00E0()
{
	// CLS
	std :: memset(video_display, 0, sizeof(video_display));
}

void chip8_class::OP_1nnn()
{
	uint16_t address = opcode & 0x0FFFu;
	pc = address; 	
}

void chip8_class::OP_6xkk()
{
	// Set Vx = kk
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t byte = opcode & 0x00FFu;

	registers[Vx] = byte;
}

void chip8_class::OP_7xkk()
{
	// Set Vx = Vx + kk
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t byte = opcode & 0x00FFu;

	registers[Vx] += byte;
}

void chip8_class::OP_Annn()
{
	// Set I = nnn
	uint16_t address = opcode & 0x0FFFu;

	index = address;
}

void chip8_class::OP_NULL()
{}

void chip8_class::OP_Dxyn()
{
	// Load n-byte sprite stored at memory location I , and display at (Vx, Vy), set VF = collission
	// n-byte is the hieght of the sprite, i.e. n rows of 8 bits	
	// Get x & y
	uint8_t Vx = ( opcode & 0x0F00u ) >> 8u;
	uint8_t Vy = ( opcode & 0x00F0u ) >> 4u;
	uint8_t height = opcode & 0x000Fu;
	
	// Getting xPos and yPos on video_display (wrap if going beyond screen boundaries)
	uint8_t xPos = registers[Vx] % VIDEO_WIDTH;	// The value in the registers will be already taken care of by the preceding code !!!
	uint8_t yPos = registers[Vy] % VIDEO_HEIGHT; 

	registers[0xF] = 0;	// The collision flag will be initially unset

	for (unsigned int row = 0; row < height; ++row) 	
	{
		uint8_t storedSpriteByte = memory[index + row];	
		// Note that index isn't being changed, only the well I + row is being passed
		// index points to the memory location where the sprite is stored, and copied from on to the screenn
		
		for (unsigned int col = 0; col < 8; ++col)
		{
			uint8_t spritePixel = storedSpriteByte & (0x80u >> col);	// spriteByte & (1000 0000 >> col)
			uint32_t* screenPixel = &video_display[ ( (yPos + row) * VIDEO_WIDTH ) + (xPos + col) ];
			// screenPixel points to the address of the first byte of the 4 Byte pixel element in the array
			// it's type uint34_t tells the computer to take 4 bytes starting from the address, and set (0xFFFFFFFF) or unset them
			
			// Yashaswini's Explanation
			// # # # # 
			// # # # # 
			// # # * # 
			// # # # 

			if (spritePixel)
			{
				// Screen Pixel also on : Collision
				if (*screenPixel == 0xFFFFFFFF)
				{
					registers[0xF] = 1; // collision case
				}
				*screenPixel = *screenPixel ^ 0xFFFFFFFF;
			}
		}
	}
}

