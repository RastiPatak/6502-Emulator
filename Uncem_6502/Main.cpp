#include <iostream>

class MOS6502 {
public:
	MOS6502() 
		: mAccumulator(0), mRegisterX(0), mRegisterY(0), mProgramCounter(0), mStackPointer(0xFF), C(0), Z(0), I(0), D(0), B(0), V(0), N(0) {
		for (std::size_t i = 0; i < sizeof(mMemory); i++) {
			mMemory[i] = 0;
		}
	}

	void loadProgram(const uint8_t* program, std::size_t size, uint16_t offset) {
		std::size_t i = offset;
		for (std::size_t j = 0; j < size; j++) {
			mMemory[i++] = program[j];
		}
	}

	void execute() {
		while (true) {
			uint8_t opcode = fetch();
			executeOpcode(opcode);
		}
	}

	void printMemory() {
		for (std::size_t i = 0; i < sizeof(mMemory); i++) {
			std::cout << std::hex << (int)mMemory[i] << ", ";
		}
		std::cout << std::endl;
	}

private:
	uint8_t mMemory[65536];
	uint8_t mAccumulator, mRegisterX, mRegisterY;
	uint16_t mProgramCounter;
	uint8_t mStackPointer;
	uint8_t C, Z, I, D, B, V, N;

	uint8_t fetch() {
		uint8_t data = mMemory[mProgramCounter];
		mProgramCounter++;
		return data;
	}

	void executeOpcode(uint8_t opcode) {
		switch (opcode) {
		case 0x4C:
			mProgramCounter = jumpAbsolute();
			break;
		case 0x6C:
			mProgramCounter = jumpIndirect();
			break;
		case 0xA9:
			mAccumulator = ldaImmediate();
			break;
		case 0xE8:
			mRegisterX++;
			setZeroAndNegativeFlags(mRegisterX);
			std::cout << "RegisterX: " << std::hex << (int)mRegisterX << ", (" << mProgramCounter << ")" << std::endl;
			break;
		case 0xEA:
			break;
		default:
			printMemory();
			std::cerr << "Unknown opcode: " << std::hex << (int)opcode << std::endl;
			std::exit(1);
		}
	}

	uint16_t jumpAbsolute() {
		std::cout << "JMP (absolute) started, PC: " << mProgramCounter << std::endl;

		uint16_t jumpAddress = fetch() + (fetch() << 8);

		std::cout << "New Address: " << jumpAddress << std::endl;
		return jumpAddress;
	}

	uint16_t jumpIndirect() {
		std::cout << "JMP (indirect) started, PC: " << mProgramCounter << std::endl;
		
		uint16_t lookupAddress = fetch() + (fetch() << 8);
		mProgramCounter = lookupAddress;
		std::cout << "Lookup Address: " << lookupAddress << std::endl;

		uint16_t jumpAddress = fetch() + (fetch() << 8);
		std::cout << "New Address: " << jumpAddress << std::endl;
		return jumpAddress;
	}

	uint8_t ldaImmediate() {
		uint8_t result = fetch();
		std::cout << "Loading " << std::hex << (int)result << " into A" << std::endl;
		return result;
	}

	void setZeroAndNegativeFlags(uint8_t value) {
		Z = value == 0;
		N = (value & 0x80) != 0;
	}
};

int main() {
	uint8_t program[] = {
		0xEA,
		0xA9,
		0x09,
		0xE8,
		0xEA,
		0xE8,
		0x6C, 0x20, 0x01
	};

	uint8_t twoProgram[] = {
		0x50, 0x01
	};

	uint8_t threeProgram[] = {
		0xE8,
		0xE8
	};

	MOS6502 cpu;
	cpu.loadProgram(program, sizeof(program), 0x0000);
	cpu.loadProgram(twoProgram, sizeof(twoProgram), 0x0120);
	cpu.loadProgram(threeProgram, sizeof(threeProgram), 0x0150);
	cpu.execute();
}