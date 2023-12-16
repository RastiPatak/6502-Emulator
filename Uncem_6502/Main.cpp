#include <iostream>

enum OpCode {
	BRK = 0,
	JMPAbs = 0x4C,
	JMPInd = 0x6C,
	LDAIndX = 0xA1,
	LDAZeroP = 0xA5,
	LDAImmediate = 0xA9,
	LDAAbs = 0xAD,
	LDAIndY = 0xB1,
	LDAZeroPX = 0xB5,
	LDAAbsY = 0xB9,
	LDAAbsX = 0xBD,
	INY = 0xC8,
	INX = 0xE8,
	DEX = 0xCA, // Decrement X by 1
	DEY = 0x88, // Decrement Y by 1
	CLC = 0x18, //Clear carry
	CLD = 0xD8, //Clear Decimal Mode
	CLI = 0x58, //Clear Interrupt Disable Bit
	CLV = 0xB8, //Clear Overflow flag
	NOP = 0xEA,
};

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
			executeOpcode((OpCode)opcode);
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

	void executeOpcode(OpCode opcode) {
		switch (opcode) {
		case JMPAbs:
			mProgramCounter = jumpAbsolute();
			break;
		case JMPInd:
			mProgramCounter = jumpIndirect();
			break;
		case LDAIndX:
			mAccumulator = ldaIndirectX();
			setZeroAndNegativeFlags(mAccumulator);
			break;
		case LDAZeroP:
			mAccumulator = ldaZeroPage();
			setZeroAndNegativeFlags(mAccumulator);
			break;
		case LDAImmediate:
			mAccumulator = ldaImmediate();
			setZeroAndNegativeFlags(mAccumulator);
			break;
		case LDAAbs:
			mAccumulator = ldaAbsolute();
			setZeroAndNegativeFlags(mAccumulator);
			break;
		case LDAIndY:
			mAccumulator = ldaIndirectY();
			setZeroAndNegativeFlags(mAccumulator);
			break;
		case LDAZeroPX:
			mAccumulator = ldaZeroPageX();
			setZeroAndNegativeFlags(mAccumulator);
			break;
		case LDAAbsY:
			mAccumulator = ldaAbsoluteY();
			setZeroAndNegativeFlags(mAccumulator);
			break;
		case LDAAbsX:
			mAccumulator = ldaAbsoluteX();
			setZeroAndNegativeFlags(mAccumulator);
			break;
		case INY:
			mRegisterY++;
			setZeroAndNegativeFlags(mRegisterY);
			break;
		case INX:
			mRegisterX++;
			setZeroAndNegativeFlags(mRegisterX);
			std::cout << "RegisterX: " << std::hex << (int)mRegisterX << ", (" << mProgramCounter << "-1)" << std::endl;
			break;
		case DEX:
			mRegisterX--;
			setZeroAndNegativeFlags(mRegisterX);
			break;
		case DEY:
			mRegisterY--;
			setZeroAndNegativeFlags(mRegisterY);
			break;
		case CLC:
			C = 0;
			break;
		case CLD:
			D = 0;
			break;
		case CLI:
			I = 0;
			break;
		case CLV:
			V = 0;
			break;
		case NOP:
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

	uint8_t ldaIndirectX() {
		uint8_t lookupAddress = fetch() + mRegisterX;
		std::cout << "Lookup address: " << std::hex << (int)lookupAddress << ", x being: " << std::hex << (int)mRegisterX << std::endl;

		uint16_t address = mMemory[lookupAddress] + mMemory[lookupAddress + 1] << 8;
		std::cout << "Address: " << std::hex << (int)address << " lookupA: " << std::hex << (int)lookupAddress << std::endl;

		uint8_t result = mMemory[address];
		std::cout << "Loading " << std::hex << (int)result << " into A" << std::endl;
		return result;
	}

	uint8_t ldaZeroPage() {
		uint8_t result = mMemory[fetch()];
		std::cout << "Loading " << std::hex << (int)result << " into A" << std::endl;
		return result;
	}

	uint8_t ldaImmediate() {
		uint8_t result = fetch();
		std::cout << "Loading " << std::hex << (int)result << " into A" << std::endl;
		return result;
	}

	uint8_t ldaAbsolute() {
		uint8_t result = mMemory[fetch() + (fetch() << 8)];
		std::cout << "Loading " << std::hex << (int)result << " into A" << std::endl;
		return result;
	}

	uint8_t ldaIndirectY() {
		uint8_t lookupAddress = fetch();
		std::cout << "Lookup address: " << std::hex << (int)lookupAddress << ", y: " << std::hex << (int)mRegisterY << std::endl;

		uint16_t address = (mMemory[lookupAddress] + (mMemory[lookupAddress + 1] << 8)) + mRegisterY;
		std::cout << "Address: " << std::hex << (int)address << std::endl;

		uint8_t result = mMemory[address];
		std::cout << "Loading " << std::hex << (int)result << " into A" << std::endl;
		return result;
	}

	uint8_t ldaZeroPageX() {
		uint8_t	address = mRegisterX + fetch();
		uint8_t result = mMemory[address];
		std::cout << "Loading " << std::hex << (int)result << " into A, address: " << std::hex << (int)address << std::endl;
		return result;
	}

	uint8_t ldaAbsoluteY() {
		uint8_t result = mMemory[(fetch() + (fetch() << 8)) + mRegisterY];
		std::cout << "Loading " << std::hex << (int)result << " into A" << std::endl;
		return result;
	}

	uint8_t ldaAbsoluteX() {
		uint8_t result = mMemory[(fetch() + (fetch() << 8)) + mRegisterX];
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
		0xA9, 0x09,
		0xE8,
		0xA5, 0x40,
		0xE8,
		0xA1, 0xF0,
		0xC8,
		0xB1, 0xB0,
		0x6C, 0x20, 0x01
	};

	uint8_t startingB0[] = {
		0x41, 0x08
	};

	uint8_t starting0842[] = {
		0xAB
	};

	uint8_t startingF2[] = {
		0x00, 0x05
	};

	uint8_t starting0500[] = {
		0xBB
	};

	uint8_t twoProgram[] = {
		0x50, 0x01
	};

	uint8_t threeProgram[] = {
		0xE8,
		0xE8,
		0xBD, 0x20, 0x0A
	};

	uint8_t memory[] = {
		0x45
	};

	MOS6502 cpu;
	cpu.loadProgram(program, sizeof(program), 0x0000);
	cpu.loadProgram(startingB0, sizeof(startingB0), 0x00B0);
	cpu.loadProgram(starting0842, sizeof(starting0842), 0x0842);
	cpu.loadProgram(startingF2, sizeof(startingF2), 0x00F2);
	cpu.loadProgram(starting0500, sizeof(starting0500), 0x0500);
	cpu.loadProgram(twoProgram, sizeof(twoProgram), 0x0120);
	cpu.loadProgram(threeProgram, sizeof(threeProgram), 0x0150);
	cpu.loadProgram(memory, sizeof(memory), 0x0A24);
	cpu.execute();
}