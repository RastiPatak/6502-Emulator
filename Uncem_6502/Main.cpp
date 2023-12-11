#include <iostream>

class MOS6502 {
public:
	MOS6502() 
		: mAccumulator(0), mRegisterX(0), mRegisterY(0), mProgramCounter(0), mStackPointer(0xFD), C(0), Z(0), I(0), D(0), B(0), V(0), N(0) {
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
			// Implement other logic such as handling interrupts, etc.
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
		case 0xE8:
			mRegisterX++;
			setZeroAndNegativeFlags(mRegisterX);
			std::cout << "RegisterX: " << std::hex << (int)mRegisterX << std::endl;
			break;
		case 0xEA:
			break;
		default:
			printMemory();
			std::cerr << "Unknown opcode: " << std::hex << (int)opcode << std::endl;
			std::exit(1);
		}
	}

	void setZeroAndNegativeFlags(uint8_t value) {
		Z = value == 0;
		N = (value & 0x80) != 0;
	}
};

int main() {
	uint8_t program[] = {
		0xEA,
		0xEA,
		0xEA,
		0xE8,
		0xEA,
		0xE8
	};

	MOS6502 cpu;
	cpu.loadProgram(program, sizeof(program), 0x0005);
	cpu.execute();

	//for (const unsigned char& a : c)
	//	std::cout << (a + 0) << ",";
	//std::cout << std::endl;
}