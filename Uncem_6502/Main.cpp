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
	TAX = 0xAA, //Transfer Accumulator to X
	TAY = 0xA8, //Transfer Accumulator to Y
	TSX = 0xBA, //Transfer Stack Pointer to X
	TXA = 0x8A, //Transfer X to Accumulator
	TXS = 0x9A, //Transfer X to Stack Pointer
	TYA = 0x98, // Transfer Y to Accumulator

	BCC = 0x90, //Branch on Carry Clear
	BCS = 0xB0, //Branch on Carry Set
	BEQ = 0xF0, //Branch on Result Zero
	BMI = 0x30,  //Branch on result minus
	BNE = 0xD0,  //Branch on result non zero
	BPL = 0x10,  //Branch on result plus NONE OF BRANCHES IMPLEMENTED YET
	BVC = 0x50,
	BVS = 0x70,

	CPXImmediate = 0xE0, //Compare X With Memory
	CPXZeroP = 0xE4,
	CPXAbs = 0xEC,

	CPYImmediate = 0xC0, //Compare Y With Memory
	CPYZeroP = 0xC4,
	CPYAbs = 0xCC
};

class MOS6502 {
public:
	bool ISDEBUG = true; // change this manually in code to set it to either debug or usual mode (true for debug, false for usual)

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

	void printRegisterInfo()
	{
		std::cout << std::hex << "\t" << ";" << " A:" << (int)mAccumulator << " X:" << (int)mRegisterX << " Y:" << (int)mRegisterY << " ST: CZIDBVN " << (int)C << (int)Z << (int)I << (int)D << (int)B << (int)V << (int)N << " PC:" << mProgramCounter << " SP:" << mStackPointer << "\n";
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
			std::cout << "RegisterX: " << std::hex << (int)mRegisterX << ", (" << mProgramCounter << ")" << std::endl;
			break;
		case DEX:
			if (ISDEBUG) { std::cout << "\t" << "DEX"; }
			mRegisterX--;
			setZeroAndNegativeFlags(mRegisterX);
			break;
		case DEY:
			if (ISDEBUG) { std::cout << "\t" << "DEY"; }
			mRegisterY--;
			setZeroAndNegativeFlags(mRegisterY);
			break;
		case CLC:
			if (ISDEBUG) { std::cout << "\t" << "CLC"; }
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
		case TAX:
			transferAccToX();
			break;
		case TAY:
			transferAccToY();
			break;
		case TSX:
			transferStackToX();
			break;
		case TYA:
			transferYToAcc();
			break;
		case TXA:
			transferXToAcc();
			break;
		case TXS:
			transferXToStack();
			break;
		case BNE:
			branchNonZero();
			break;
		case BCS:
			branchCarrySet();
			break;
		case BCC:
			branchCarryClear();
			break;
		case BEQ:
			branchZero();
			break;
		case BMI:
			branchMinus();
			break;
		case BPL:
			branchPlus();
			break;
		case BVC:
			branchOverflowClear();
			break;
		case BVS:
			branchOverflowSet();
			break;
		case CPXImmediate:
			compareXImmediate();
			break;
		case CPXAbs:
			compareXAbsolute();
			break;
		case CPXZeroP:
			compareXZeroPage();
			break;
		case CPYImmediate:
			compareYImmediate();
			break;
		case CPYAbs:
			compareYAbsolute();
			break;
		case CPYZeroP:
			compareYZeroPage();
			break;
		case NOP:
			break;
		default:
			//printMemory();
			std::cerr << "Unknown opcode: " << std::hex << (int)opcode << std::endl;
			std::exit(1);
		}
		if (ISDEBUG)
		{
			printRegisterInfo();
		}
	}

	void transferAccToX()
	{
		std::cout << "Transferring Accumulator to X" << mProgramCounter << std::endl;
		mRegisterX = mAccumulator;
		setZeroAndNegativeFlags(mRegisterX);
	}

	void transferAccToY()
	{
		std::cout << "Transferring Accumulator to Y" << mProgramCounter << std::endl;
		mRegisterY = mAccumulator;
		setZeroAndNegativeFlags(mRegisterY);
	}

	void transferStackToX()
	{
		std::cout << "Transferring StackPointer to X" << mProgramCounter << std::endl;
		mRegisterX = mStackPointer;
		setZeroAndNegativeFlags(mRegisterX);
	}

	void transferXToAcc()
	{
		std::cout << "Transferring X to Accumulator" << mProgramCounter << std::endl;
		mAccumulator = mRegisterX;
		setZeroAndNegativeFlags(mAccumulator);
	}

	void transferYToAcc()
	{
		std::cout << "Transferring Y to Accumulator" << mProgramCounter << std::endl;
		mAccumulator = mRegisterY;
		setZeroAndNegativeFlags(mAccumulator);
	}

	void transferXToStack()
	{
		std::cout << "Transferring X to StackPointer" << mProgramCounter << std::endl;
		mStackPointer = mRegisterX;
	}

	uint8_t add(uint8_t valueA, uint8_t valueB, bool carry, bool bcd)
	{
		uint16_t result;

		if (bcd) {
			bool bcdCarry = false;
			result = (valueA & 0xF) + (valueB & 0xF) + carry;
			if (result > 9) {
				result -= 10;
				bcdCarry = true;
			}

			uint16_t leftNibble = (valueA & 0xF0) + (valueB & 0xF0) + bcdCarry;
			if (leftNibble > 9) {
				leftNibble -= 10;
				leftNibble += 0x10;
			}

			result += leftNibble << 4;
		}
		else {
			result = valueA + valueB + (carry ? 1 : 0);
		}

		C = (result & 0x100) != 0;
		V = ((valueA ^ valueB) & 0x80) == 0 && ((valueA ^ result) & 0x80) != 0;
		Z = (result & 0xFF) == 0;
		N = (result & 0x80) != 0;

		return static_cast<uint8_t>(result & 0xFF);
	}

	uint8_t sub(uint8_t valueA, uint8_t valueB, bool carry, bool bcd)
	{
		uint16_t result;

		if (bcd) {
			bool bcdCarry = true;
			result = (valueA & 0xF) + ((~valueB) & 0xF) + (bcdCarry ? 1 : 0);
			if ((valueA & 0xF) < (valueB & 0xF))
				bcdCarry = false;

			uint16_t leftNibble = (valueA & 0xF0) + ((~valueB) & 0xF0) + (bcdCarry ? 1 : 0);
			result += leftNibble << 4;
		}
		else {
			result = valueA + (~valueB) + carry;
		}

		C = (result & 0x100) != 0;
		V = ((valueA ^ valueB) & 0x80) == 0 && ((valueA ^ result) & 0x80) != 0;
		Z = (result & 0xFF) == 0;
		N = (result & 0x80) != 0;

		return static_cast<uint8_t>(result & 0xFF);
	}

	void compareBase(uint8_t valueA, uint8_t valueB)
	{
		uint8_t tempVSave = V;
		uint8_t result = sub(valueA, valueB, false, false);
		V = tempVSave;
	}

	void compareXImmediate()
	{
		uint8_t value = fetch();
		compareBase(mRegisterX, value);
	}

	void compareXAbsolute()
	{
		uint8_t value = mMemory[fetch() + (fetch() << 8)];
		compareBase(mRegisterX, value);
	}

	void compareXZeroPage()
	{
		uint8_t value = mMemory[fetch()];
		compareBase(mRegisterX, value);
	}

	void compareYImmediate()
	{
		uint8_t value = fetch();
		compareBase(mRegisterY, value);
	}

	void compareYAbsolute()
	{
		uint8_t value = mMemory[fetch() + (fetch() << 8)];
		compareBase(mRegisterY, value);
	}

	void compareYZeroPage()
	{
		uint8_t value = mMemory[fetch()];
		compareBase(mRegisterY, value);
	}

	void branchNonZero() {
		int8_t fetchedByte = branchBase("BNE");
		if (Z == 0)
		{
			mProgramCounter += fetchedByte;
			branchDebugPrint("BNE", fetchedByte);
		}
	}

	void branchCarrySet() {
		int8_t fetchedByte = branchBase("BCS");
		if (C == 1)
		{
			mProgramCounter += fetchedByte;
			branchDebugPrint("BCS", fetchedByte);
		}
	}

	void branchCarryClear() {
		int8_t fetchedByte = branchBase("BCC");
		if (C == 0)
		{
			mProgramCounter += fetchedByte;
			branchDebugPrint("BCC", fetchedByte);
		}
	}

	void branchZero() {
		int8_t fetchedByte = branchBase("BEQ");
		if (Z == 1)
		{
			mProgramCounter += fetchedByte;
			branchDebugPrint("BEQ", fetchedByte);
		}
	}

	void branchMinus() {
		int8_t fetchedByte = branchBase("BMI");
		if (N == 1)
		{
			mProgramCounter += fetchedByte;
			branchDebugPrint("BMI", fetchedByte);
		}
	}

	void branchPlus() {
		int8_t fetchedByte = branchBase("BPL");
		if (N == 0)
		{
			mProgramCounter += fetchedByte;
			branchDebugPrint("BPL", fetchedByte);
		}
	}

	void branchOverflowClear() {
		int8_t fetchedByte = branchBase("BVC");
		if (V == 0)
		{
			mProgramCounter += fetchedByte;
			branchDebugPrint("BVC", fetchedByte);
		}
	}

	void branchOverflowSet() {
		int8_t fetchedByte = branchBase("BVS");
		if (V == 1)
		{
			mProgramCounter += fetchedByte;
			branchDebugPrint("BVS", fetchedByte);
		}
	}

	int8_t branchBase(char const* instruction) {
		std::cout << instruction << " started, PC: " << mProgramCounter << std::endl;
		return fetch();
	}

	void branchDebugPrint(char const* instruction, int8_t fetchedByte) {
		if (ISDEBUG) {
			std::cout << "\t" << instruction << " Offset: "  << (static_cast<int>(fetchedByte) & 0xFF) << std::endl;
		}
	}

	uint16_t jumpAbsolute() {
		std::cout << "JMP (absolute) started, PC: " << mProgramCounter << std::endl;

		uint16_t jumpAddress = fetch() + (fetch() << 8);

		std::cout << "New Address: " << jumpAddress << std::endl;
		if (ISDEBUG) { std::cout << "\t" << "JMP" << "\t" << "#" << jumpAddress; }
		return jumpAddress;
	}

	uint16_t jumpIndirect() {
		std::cout << "JMP (indirect) started, PC: " << mProgramCounter << std::endl;

		uint16_t lookupAddress = fetch() + (fetch() << 8);
		mProgramCounter = lookupAddress;
		std::cout << "Lookup Address: " << lookupAddress << std::endl;

		uint16_t jumpAddress = fetch() + (fetch() << 8);
		std::cout << "New Address: " << jumpAddress << std::endl;
		if (ISDEBUG) { std::cout << "\t" << "JMP" << "\t" << "" << lookupAddress; }
		return jumpAddress;
	}

	uint8_t ldaIndirectX() {
		uint8_t lookupAddress = fetch() + mRegisterX;
		std::cout << "Lookup address: " << std::hex << (int)lookupAddress << ", x being: " << std::hex << (int)mRegisterX << std::endl;

		uint16_t address = mMemory[lookupAddress] + (mMemory[lookupAddress + 1] << 8);
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
		0xE8,
		0x50, 0x00,
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