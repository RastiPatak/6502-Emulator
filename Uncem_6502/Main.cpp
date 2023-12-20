#include <iostream>


#include <conio.h> 

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
	BNE = 0x30,  //Branch on result non zero
	BPL = 0x30,  //Branch on result plus NONE OF BRANCHES IMPLEMENTED YET

	CPXImmedeate = 0xE0, //Compare X With Memory
	CPXZeroP = 0xE4,
	CPXAbs = 0xEC,

	CPYImmedeate = 0xC0, //Compare Y With Memory
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

	/*void clearConsole()
	{
		system("cls");
	}*/

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
		std::cout << std::hex << "\t" << ";" << " A:" << (int)mAccumulator << " X:" << (int)mRegisterX << " Y:" << (int)mRegisterY << " ST: CZIDBVN " << (int)C << (int)Z << (int)I << (int)D << (int)B << (int)V << (int)N  << " PC:" << mProgramCounter << " SP:" << mStackPointer << "\n";
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
			TransferAccToX();
			break;
		case TAY:
			TransferAccToY();
			break;
		case TSX:
			TransferStackToX();
			break;
		case TYA:
			TransferYToAcc();
			break;
		case TXA:
			TransferXToAcc();
			break;
		case TXS:
			TransferXToStack();
			break;
		case BNE: // on non zero

		case CPXImmedeate:
			CompareXWithMemoryImmedeate();
			break;
		case CPXAbs:
			CompareXWithMemoryAbsolute();
			break;
		case CPXZeroP:
			CompareXWithMemoryZeroPage();
			break;
		case CPYImmedeate:
			CompareYWithMemoryImmedeate();
			break;
		case CPYAbs:
			CompareYWithMemoryAbsolute();
			break;
		case CPYZeroP:
			CompareYWithMemoryZeroPage();
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

	void TransferAccToX()
	{
		std::cout << "Transferring Accumulator to X" << mProgramCounter << std::endl;
		mRegisterX = mAccumulator;
		setZeroAndNegativeFlags(mRegisterX);
	}

	void TransferAccToY()
	{
		std::cout << "Transferring Accumulator to Y" << mProgramCounter << std::endl;
		mRegisterY = mAccumulator;
		setZeroAndNegativeFlags(mRegisterY);
	}

	void TransferStackToX()
	{
		std::cout << "Transferring Accumulator to X" << mProgramCounter << std::endl;
		mRegisterX = mStackPointer;
		setZeroAndNegativeFlags(mRegisterX);
	}

	void TransferXToAcc()
	{
		std::cout << "Transferring X to Accumulator" << mProgramCounter << std::endl;
		mAccumulator = mRegisterX;
		setZeroAndNegativeFlags(mAccumulator);
	}

	void TransferYToAcc()
	{
		std::cout << "Transferring Y to Accumulator" << mProgramCounter << std::endl;
		mAccumulator = mRegisterY;
		setZeroAndNegativeFlags(mAccumulator);
	}

	void TransferXToStack()
	{
		std::cout << "Transferring Accumulator to X" << mProgramCounter << std::endl;
		mStackPointer = mRegisterX;
		setZeroAndNegativeFlags(mStackPointer);
	}

	//  2  1  0
	// 12 34 56<
	// 78 88 F9<
	// 8A BD 4F
	//     0  C

	uint8_t add(uint8_t val_a, uint8_t val_b, uint8_t& C, uint8_t& Z, uint8_t& N, uint8_t& V)
	{
		uint16_t res = val_a;

		res += val_b;
		
		if (C) { res += 1; }

		// unsigned 0..255
		// signed -128..0..+127

		if (res > 255)
		{
			C = 1;
		}
		else
		{ 
			C = 0; 
		}

		if (res == 0)
		{
			Z = 1;
		}
		else
		{
			Z = 0;
		}

		if (res > 127)
		{
			N = 1;
		}
		else
		{
			N = 0;
		}

		V = 0;
		if (val_a <= 127 && val_b <= 127)
		{
			if (res > 127)
			{
				V = 1;
			}
		}
		else if (val_a >= 128 && val_b >= 128)
		{
			if (res < 128)
			{
				V = 1;
			}
		}

		return res;
	}

	uint8_t sub(uint8_t val_a, uint8_t val_b, uint8_t& C, uint8_t& Z, uint8_t& N, uint8_t& V)
	{
		uint16_t res = val_a;
		res -= val_b;

		if (!C) { res -= 1; }

		// unsigned 0..255
		// signed -128..0..+127

		if (res > 255) // if the result if more than 255 that means the number looks like this, at least 0000 0001 0000 0000, which raises the carry flag in this case 
		{
			C = 0;
		}
		else
		{
			C = 1;
		}

		if (res == 0)
		{
			Z = 1;
		}
		else
		{
			Z = 0;
		}

		if (res > 127)
		{
			N = 1;
		}
		else
		{
			N = 0;
		}

		V = 0;

		int8_t val_as = val_a; //signing numbers
		int8_t val_bs = val_b;
		/*int8_t ress = res;*/

		//raising overflow flag. It happens if anomalous answer is produced
		if (val_a < 127 && val_b < 127) // if both numbers are greater than 0, but result of subtraction is less than 0 (can happen)
		{
			if (val_a > val_b)
			{
				if (res > 127)
				{
					V = 1;
				}
			}
		}
		else if (val_a > 127 && val_b > 127) // if both numbers are less than 0, but result of subtraction is greater than 0 (can happen)
		{
			if (val_a < val_b)
			{
				if (res < 127)
				{
					V = 1;
				}
			}
		}

		return res;
	}

	void compareTwoNumbers(uint8_t val_a, uint8_t val_b, uint8_t& C, uint8_t& Z, uint8_t& N)
	{
		C = 1; 
		uint8_t dummyV; // Overflow flag is not being raised during operations it normally is raised in. Thats why i let it be, but i dont use it during CMP operations, replacing it with a dummy.
		sub(val_a, val_b, C, Z, N, dummyV);
	}

	void CompareXWithMemoryImmedeate()
	{
		uint8_t valueToCompareTo = fetch();
		compareTwoNumbers(mRegisterX, valueToCompareTo, C, Z, N);
	}

	void CompareXWithMemoryAbsolute()
	{
		uint16_t lookupAddress = fetch() + (fetch() << 8);
		uint8_t valueToCompareTo = mMemory[lookupAddress];
		compareTwoNumbers(mRegisterX, valueToCompareTo, C, Z, N);
	}

	void CompareXWithMemoryZeroPage()
	{
		uint16_t lookupAddress = fetch();
		uint8_t valueToCompareTo = mMemory[lookupAddress];
		compareTwoNumbers(mRegisterX, valueToCompareTo, C, Z, N);
	}


	void CompareYWithMemoryImmedeate()
	{
		uint8_t valueToCompareTo = fetch();
		uint8_t comparisonResult = mRegisterY - valueToCompareTo;
		compareTwoNumbers(mRegisterY, valueToCompareTo, C, Z, N);
	}

	void CompareYWithMemoryAbsolute()
	{
		uint16_t lookupAddress = fetch() + (fetch() << 8);
		uint8_t valueToCompareTo = mMemory[lookupAddress];
		compareTwoNumbers(mRegisterY, valueToCompareTo, C, Z, N);
	}

	void CompareYWithMemoryZeroPage()
	{
		uint16_t lookupAddress = fetch();
		uint8_t valueToCompareTo = mMemory[lookupAddress];
		compareTwoNumbers(mRegisterY, valueToCompareTo, C, Z, N);
	}

	/*uint16_t BranchNonZero() {
		std::cout << "BNE started, PC: " << mProgramCounter << std::endl;
		uint16_t branchOffset = fetch() << 8;
		if (Z == 0)
		{

			std::cout << "New Address: " << branchOffset << std::endl;
			
			return branchOffset;
		}
		else
		{
			return mProgramCounter;
		}
		if (ISDEBUG) { std::cout << "\t" << "BNE" << "\t" << "#" << branchOffset; }
	}*/

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

	/*uint8_t programWithBranch[] = {
		//LDX 10, LDY 5, 
	};*/

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
	cpu.loadProgram(programWithBranch, sizeof(programWithBranch), 0x0210);
	cpu.loadProgram(memory, sizeof(memory), 0x0A24);
	cpu.execute();
}