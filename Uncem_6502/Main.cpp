#include <iostream>
#include <iomanip>

enum OpCode {
	BRK = 0,
	JMPAbs = 0x4C,
	JMPInd = 0x6C,
	NOP = 0xEA,

	//Load Instructions

	LDAIndX = 0xA1,
	LDAZeroP = 0xA5,
	LDAImmediate = 0xA9,
	LDAAbs = 0xAD,
	LDAIndY = 0xB1,
	LDAZeroPX = 0xB5,
	LDAAbsY = 0xB9,
	LDAAbsX = 0xBD,

	LDXImmediate = 0xA2,
	LDXZeroP = 0xA6,
	LDXZeroPY = 0xB6,
	LDXAbsY = 0xAE,
	LDXAbs = 0xBE,

	LDYImmediate = 0xA0,
	LDYZeroP = 0xA4,
	LDYZeroPX = 0xB4,
	LDYAbsX = 0xAC,
	LDYAbs = 0xBC,

	//Save instructions

	STAZeroP = 0x85,
	STAZeroPX = 0x95,
	STAAbs = 0x8D,
	STAAbsX = 0x9D,
	STAAbsY = 0x99,
	STAIndX = 0x81,
	STAIndY = 0x91,

	STXZeroP = 0x86,
	STXZeroPY = 0x96,
	STXAbs = 0x8E,

	STYZeroP = 0x84,
	STYZeroPX = 0x94,
	STYAbs = 0x8C,

	//increment, decrement instructions

	INY = 0xC8,
	INX = 0xE8,
	DEX = 0xCA, // Decrement X by 1
	DEY = 0x88, // Decrement Y by 1

	//flag instructions

	CLC = 0x18, //Clear carry
	CLD = 0xD8, //Clear Decimal Mode
	CLI = 0x58, //Clear Interrupt Disable Bit
	CLV = 0xB8, //Clear Overflow flag
	SEC = 0x38, //Set carry flag
	SEI = 0x78, //Set Interruption flag
	SED = 0xF8, //Set Decimal Flag

	//Transfer instructions

	TAX = 0xAA, //Transfer Accumulator to X
	TAY = 0xA8, //Transfer Accumulator to Y
	TSX = 0xBA, //Transfer Stack Pointer to X
	TXA = 0x8A, //Transfer X to Accumulator
	TXS = 0x9A, //Transfer X to Stack Pointer
	TYA = 0x98, // Transfer Y to Accumulator

	// Logical and arithmetical instructions

	ROLAcc = 0x2A, // Rotate left
	ROLZeroP = 0x26,
	ROLZeroPX =0x36,
	ROLAbs =   0x2E,
	ROLAbsX =  0x3E,

	RORAcc = 0x6A, // Rotate right
	RORZeroP = 0x66,
	RORZeroPX =0x76,
	RORAbs =   0x6E,
	RORAbsX =  0x7E,

	//Branch instructions

	BCC = 0x90, //Branch on Carry Clear
	BCS = 0xB0, //Branch on Carry Set
	BEQ = 0xF0, //Branch on Result Zero
	BMI = 0x30,  //Branch on result minus
	BNE = 0xD0,  //Branch on result non zero
	BPL = 0x10,  //Branch on result plus
	BVC = 0x50,
	BVS = 0x70,

	//Compare instructions

	CMPImmediate = 0xC9,
	CMPZeroP = 0xC5,
	CMPZeroPX = 0xD5,
	CMPAbs = 0xCD,
	CMPAbsX = 0xDD,
	CMPAbsY = 0xD9,
	CMPIndX = 0xC1,
	CMPIndY = 0xD1,

	CPXImmediate = 0xE0, //Compare X With Memory
	CPXZeroP = 0xE4,
	CPXAbs = 0xEC,

	CPYImmediate = 0xC0, //Compare Y With Memory
	CPYZeroP = 0xC4,
	CPYAbs = 0xCC
};

enum addressMode // Prototype enum for addressing modes, used for unified output function.
{
	IMD = 1,
	ZPG = 2,
	ZPX = 3,
	ZPY = 4,
	ABS = 5,
	ABX = 6,
	ABY = 7,
	INX = 8,
	INY = 9,
	NON = 10,
	A = 11 //Accumulator as address mode, used in few commands
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
		memcpy(mMemory + offset, program, size);
	}

	void execute() {
		while (true) {
			uint8_t opcode = fetch();
			executeOpcode((OpCode)opcode);
		}
	}

	void printMemory() {
		for (std::size_t i = 0; i < sizeof(mMemory); i++) {
			std::cout << std::hex << static_cast<int>(mMemory[i]) << ", ";
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

	uint16_t fetch16() {
		return fetch() + (fetch() << 8);
	}

	void printRegisterInfo()
	{
		std::cout << std::hex << "\t" << ";"
			<< std::setfill('0')
			<< " A:" << std::setw(2) << (unsigned int)mAccumulator
			<< " X:" << std::setw(2) << (unsigned int)mRegisterX
			<< " Y:" << std::setw(2) << (unsigned int)mRegisterY
			<< " ST: CZIDBVN " << std::setw(1) << (int)C << (int)Z << (int)I << (int)D << (int)B << (int)V << (int)N
			<< " PC:" << std::setw(4) << (uint16_t)mProgramCounter
			<< " SP:" << std::setw(2) << (int)mStackPointer
			<< std::setw(0) << std::setfill(' ')
			<< "\n";
	}

	void executeOpcode(OpCode opcode) {
		if (ISDEBUG)
		{
			// fetch already performed before, so we write PC before fetch
			std::cout << std::hex << std::setw(4) << std::setfill('0') << (mProgramCounter - 1) << std::setfill(' ') << std::setw(0) << "\t";
		}
		switch (opcode) {
		case JMPAbs:
			mProgramCounter = jumpAbsolute();
			break;
		case JMPInd:
			mProgramCounter = jumpIndirect();
			break;

			//LOAD OPERATIONS

		case LDAIndX:
			mAccumulator = loadIndirectX("LDA");
			setZeroAndNegativeFlags(mAccumulator);
			break;
		case LDAZeroP:
			mAccumulator = loadZeroPage("LDA");
			setZeroAndNegativeFlags(mAccumulator);
			break;
		case LDAImmediate:
			mAccumulator = loadImmediate("LDA");
			setZeroAndNegativeFlags(mAccumulator);
			break;
		case LDAAbs:
			mAccumulator = loadAbsolute("LDA");
			setZeroAndNegativeFlags(mAccumulator);
			break;
		case LDAIndY:
			mAccumulator = loadIndirectY("LDA");
			setZeroAndNegativeFlags(mAccumulator);
			break;
		case LDAZeroPX:
			mAccumulator = loadZeroPageX("LDA");
			setZeroAndNegativeFlags(mAccumulator);
			break;
		case LDAAbsY:
			mAccumulator = loadAbsoluteY("LDA");
			setZeroAndNegativeFlags(mAccumulator);
			break;
		case LDAAbsX:
			mAccumulator = loadAbsoluteX("LDA");
			setZeroAndNegativeFlags(mAccumulator);
			break;

		case LDXAbsY:
			mRegisterX = loadAbsoluteY("LDX");
			setZeroAndNegativeFlags(mRegisterX);
			break;
		case LDXZeroP:
			mRegisterX = loadZeroPage("LDX");
			setZeroAndNegativeFlags(mRegisterX);
			break;
		case LDXZeroPY:
			mRegisterX = loadZeroPageY("LDX");
			setZeroAndNegativeFlags(mRegisterX);
			break;
		case LDXAbs:
			mRegisterX = loadAbsolute("LDX");
			setZeroAndNegativeFlags(mRegisterX);
			break;
		case LDXImmediate:
			mRegisterX = loadImmediate("LDX");
			setZeroAndNegativeFlags(mRegisterX);
			break;

		case LDYAbsX:
			mRegisterY = loadAbsoluteX("LDY");
			setZeroAndNegativeFlags(mRegisterY);
			break;
		case LDYZeroP:
			mRegisterY = loadZeroPage("LDY");
			setZeroAndNegativeFlags(mRegisterY);
			break;
		case LDYZeroPX:
			mRegisterY = loadZeroPageX("LDY");
			setZeroAndNegativeFlags(mRegisterY);
			break;
		case LDYAbs:
			mRegisterY = loadAbsolute("LDY");
			setZeroAndNegativeFlags(mRegisterY);
			break;
		case LDYImmediate:
			mRegisterY = loadImmediate("LDY");
			setZeroAndNegativeFlags(mRegisterY);
			break;

			//SAVE OPERATIONS

		case STAZeroP:
			saveZeroPage("STA", mAccumulator);
			break;
		case STAZeroPX:
			saveZeroPageX("STA", mAccumulator);
			break;
		case STAAbs:
			saveAbsolute("STA", mAccumulator);
			break;
		case STAAbsX:
			saveAbsoluteX("STA", mAccumulator);
			break;
		case STAAbsY:
			saveAbsoluteY("STA", mAccumulator);
			break;
		case STAIndX:
			saveIndirectX("STA", mAccumulator);
			break;
		case STAIndY:
			saveIndirectY("STA", mAccumulator);
			break;


		case STXZeroP:
			saveZeroPage("STX", mRegisterX);
			break;
		case STXZeroPY:
			saveZeroPageY("STX", mRegisterX);
			break;
		case STXAbs:
			saveAbsolute("STX", mRegisterX);
			break;


		case STYZeroP:
			saveZeroPage("STY", mRegisterY);
			break;
		case STYZeroPX:
			saveZeroPageX("STY", mRegisterY);
			break;
		case STYAbs:
			saveAbsolute("STY", mRegisterY);
			break;

			//INCREMENT AND DECREMENT

		case INY:
			if (ISDEBUG) { std::cout << "INY" << "\t"; }
			mRegisterY++;
			setZeroAndNegativeFlags(mRegisterY);
			break;
		case INX:
			if (ISDEBUG) { std::cout << "INX" << "\t"; }
			mRegisterX++;
			setZeroAndNegativeFlags(mRegisterX);
			break;
		case DEX:
			if (ISDEBUG) { std::cout << "DEX" << "\t"; }
			mRegisterX--;
			setZeroAndNegativeFlags(mRegisterX);
			break;
		case DEY:
			if (ISDEBUG) { std::cout << "DEY" << "\t"; }
			mRegisterY--;
			setZeroAndNegativeFlags(mRegisterY);
			break;

			//FLAG OPERATIONS

		case CLC:
			if (ISDEBUG) { std::cout << "CLC" << "\t"; }
			C = 0;
			break;
		case CLD:
			if (ISDEBUG) { std::cout << "CLD" << "\t"; }
			D = 0;
			break;
		case CLI:
			if (ISDEBUG) { std::cout << "CLI" << "\t"; }
			I = 0;
			break;
		case CLV:
			if (ISDEBUG) { std::cout << "CLV" << "\t"; }
			V = 0;
			break;
		case SEC:
			if (ISDEBUG) { std::cout << "SEC" << "\t"; }
			C = 1;
			break;
		case SEI:
			if (ISDEBUG) { std::cout << "SEI" << "\t"; }
			I = 1;
			break;
		case SED:
			if (ISDEBUG) { std::cout << "SED" << "\t"; }
			D = 1;
			break;

			//LOGICAL AND ARITHMETICAL OPERATIONS

		case RORAcc:
			rotateRightAccumulator();
		case RORZeroP:
			rotateRightAccumulator();
		case RORZeroPX:
			rotateRightAccumulator();
		case RORAbs:
			rotateRightAccumulator();
		case RORAbsX:
			rotateRightAccumulator();

		case ROLAcc:
			rotateLeftAccumulator();
		case ROLZeroP:
			rotateLeftAccumulator();
		case ROLZeroPX:
			rotateLeftAccumulator();
		case ROLAbs:
			rotateLeftAccumulator();
		case ROLAbsX:
			rotateLeftAccumulator();

			//TRANSFER OPERATIONS

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

			//BRANCHING OPERATIONS

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

			//COMPARE OPERATIONS

		case CPXImmediate:
			compareImmediate("CPX", mRegisterX);
			break;
		case CPXAbs:
			compareAbsolute("CPX", mRegisterX);
			break;
		case CPXZeroP:
			compareZeroPage("CPX", mRegisterX);
			break;

		case CPYImmediate:
			compareImmediate("CPY", mRegisterY);
			break;
		case CPYAbs:
			compareAbsolute("CPY", mRegisterY);
			break;
		case CPYZeroP:
			compareZeroPage("CPY", mRegisterY);
			break;

		case CMPImmediate:
			compareImmediate("CMP", mAccumulator);
			break;
		case CMPZeroP:
			compareZeroPage("CMP", mAccumulator);
			break;
		case CMPZeroPX:
			compareZeroPageX("CMP", mAccumulator);
			break;
		case CMPAbs:
			compareAbsolute("CMP", mAccumulator);
			break;
		case CMPAbsX:
			compareAbsoluteX("CMP", mAccumulator);
			break;
		case CMPAbsY:
			compareAbsoluteY("CMP", mAccumulator);
			break;
		case CMPIndX:
			compareIndX("CMP", mAccumulator);
			break;
		case CMPIndY:
			compareIndY("CMP", mAccumulator);
			break;

			//MISCELANNEOUS OPERATIONS

		case NOP:
			if (ISDEBUG) { std::cout << "NOP" << "\t"; }
			break;
		default:
			printMemory();
			std::cerr << "Unknown opcode: " << std::hex << static_cast<int>(opcode) << std::endl;
			std::exit(1);
		}
		if (ISDEBUG)
		{
			printRegisterInfo();
		}
	}

	uint8_t rotateright(uint8_t value)
	{
		uint8_t resultingvalue = value >> 1 + (C ? 0x80 : 0); //I rotate the entered value by 1 position right, replacing the left-most bit of the ROTATED VALUE with carry (either 1 or 0)
		C = value & 0x1;                                      //I store the bit that disappears due to shifting of the number in the carry flag
		setZeroAndNegativeFlags(resultingvalue);              //I also set the correct flags if the resulting value after shifting appears to be zero or negative
		return resultingvalue;                                //I return the value 
	}

	uint8_t rotateleft(uint8_t value)
	{
		uint8_t resultingvalue = value << 1 + (C ? 1 : 0);    //I rotate the entered value 1 position left, replacing the right-most bit of the ROTATED VALUE with value of carry 
		C = value & 0x80;                                     //I store the left-most bit that disappears due to shifting into carry flag
		setZeroAndNegativeFlags(resultingvalue);              //I check if the value is negative or zero
		return resultingvalue;                                //I return the value
	}

	void OutForComAndMode(std::string instruction, std::string addrmode, uint16_t addr)
	{
		// addrmode should be made to enum, if you wish. This version is already working one, but i didnt implement it widely
		if (ISDEBUG)
		{
			if (addrmode == "IMD")
			{
				std::cout << instruction << "\t" << "#" << (int)addr;
			}
			else if (addrmode == "ZPG")
			{
				std::cout << instruction << "\t" << std::hex << std::setw(2) << std::setfill('0') << addr;
			}
			else if (addrmode == "ZPX")
			{
				std::cout << instruction << "\t" << std::hex << std::setw(2) << std::setfill('0') << addr << ",x";
			}
			else if (addrmode == "ZPY")
			{
				std::cout << instruction << "\t" << std::hex << std::setw(2) << std::setfill('0') << addr << ",y";
			}
			else if (addrmode == "ABS")
			{
				std::cout << instruction << "\t" << std::hex << std::setw(4) << std::setfill('0') << addr;
			}
			else if (addrmode == "ABX")
			{
				std::cout << instruction << "\t" << std::hex << std::setw(4) << std::setfill('0') << addr << ",x";
			}
			else if (addrmode == "ABY")
			{
				std::cout << instruction << "\t" << std::hex << std::setw(4) << std::setfill('0') << addr << ",y";
			}
			else if (addrmode == "INX")
			{
				std::cout << instruction << "\t" << "(" << std::hex << std::setw(4) << std::setfill('0') << addr << ",x)";
			}
			else if (addrmode == "INY")
			{
				std::cout << instruction << "\t" << "(" << std::hex << std::setw(4) << std::setfill('0') << addr << "),y";
			}
			else if (addrmode == "A")
			{
				std::cout << instruction << "\t" << "A";
			}
			else
			{
				std::cout << instruction << "\t";
			}
		}
	}

	//VERSION OF UNIFIED OUTPUT FUNCTION WITH ENUMS USED
	void OutForComAndModeENUM(std::string instruction, addressMode mode, uint16_t addr)
	{
		// addrmode should be made to enum, if you wish. This version is already working one, but i didnt implement it widely
		if (ISDEBUG)
		{
			if (mode == IMD)
			{
				std::cout << instruction << "\t" << "#" << (int)addr;
			}
			else if (mode == ZPG)
			{
				std::cout << instruction << "\t" << std::hex << std::setw(2) << std::setfill('0') << addr;
			}
			else if (mode == ZPX)
			{
				std::cout << instruction << "\t" << std::hex << std::setw(2) << std::setfill('0') << addr << ",x";
			}
			else if (mode == ZPY)
			{
				std::cout << instruction << "\t" << std::hex << std::setw(2) << std::setfill('0') << addr << ",y";
			}
			else if (mode == ABS)
			{
				std::cout << instruction << "\t" << std::hex << std::setw(4) << std::setfill('0') << addr;
			}
			else if (mode == ABX)
			{
				std::cout << instruction << "\t" << std::hex << std::setw(4) << std::setfill('0') << addr << ",x";
			}
			else if (mode == ABY)
			{
				std::cout << instruction << "\t" << std::hex << std::setw(4) << std::setfill('0') << addr << ",y";
			}
			else if (mode == INX)
			{
				std::cout << instruction << "\t" << "(" << std::hex << std::setw(4) << std::setfill('0') << addr << ",x)";
			}
			else if (mode == INY)
			{
				std::cout << instruction << "\t" << "(" << std::hex << std::setw(4) << std::setfill('0') << addr << "),y";
			}
			else if (mode == A)
			{
				std::cout << instruction << "\t" << "A";
			}
			else
			{
				std::cout << instruction << "\t";
			}
		}
	}



	void rotateLeftZeroPage()
	{
		uint8_t addr = fetch();                               // ADDR WILL BE NEEDED FOR OUTPUT, NO QUESTIONS ASKED, IT WONT WORK OTHER WAY
		mMemory[addr] = rotateleft(mMemory[addr]);
		//OutForComAndMode("instructionname", "addressingmode", "addr");
		OutForComAndModeENUM("ROL", ZPG, addr);
		//if (ISDEBUG) { std::cout << "ROL" << "\t" << std::hex << std::setw(2) << std::setfill('0') << addr; }
	}

	void rotateLeftAccumulator()
	{
		mAccumulator = rotateleft(mAccumulator);
		OutForComAndModeENUM("ROL", A, 0);
		//if (ISDEBUG) { std::cout << "ROL" << "\t" << "A"; }
	}

	void rotateLeftZeroPageX()
	{
		uint8_t addr = fetch();  
		mMemory[addr + mRegisterX] = rotateleft(mMemory[addr + mRegisterX]);
		OutForComAndModeENUM("ROL", ZPG, addr);
		//if (ISDEBUG) { std::cout << "ROL" << "\t" << std::hex << std::setw(2) << std::setfill('0') << addr << ",x"; }
	}

	void rotateLeftAbsoluteX()
	{
		uint16_t addr = fetch16();
		mMemory[addr + mRegisterX] = rotateleft(mMemory[addr + mRegisterX]);
		OutForComAndModeENUM("ROL", ABX, addr);
		//if (ISDEBUG) { std::cout << "ROL" << "\t" << std::hex << std::setw(4) << std::setfill('0') << addr << ",x"; }
	}

	void rotateLeftAbsolute()
	{
		uint8_t addr = fetch16();
		mMemory[addr] = rotateleft(mMemory[addr]);
		OutForComAndModeENUM("ROL", ABS, addr);
		//if (ISDEBUG) { std::cout << "ROL" << "\t" << std::hex << std::setw(2) << std::setfill('0') << addr; }
	}

	void rotateRightZeroPage()
	{
		uint8_t addr = fetch();                              
		mMemory[addr] = rotateright(mMemory[addr]);
		OutForComAndModeENUM("ROR", ZPG, addr);
		//if (ISDEBUG) { std::cout << "ROL" << "\t" << std::hex << std::setw(2) << std::setfill('0') << addr; }
	}

	void rotateRightAccumulator()
	{
		mAccumulator = rotateright(mAccumulator);
		OutForComAndModeENUM("ROR", A, 0);
	}

	void rotateRightZeroPageX()
	{
		uint8_t addr = fetch();
		mMemory[addr + mRegisterX] = rotateright(mMemory[addr + mRegisterX]);
		OutForComAndModeENUM("ROR", ZPX, addr);
		//if (ISDEBUG) { std::cout << "ROL" << "\t" << std::hex << std::setw(2) << std::setfill('0') << addr << ",x"; }
	}

	void rotateRightAbsoluteX()
	{
		uint16_t addr = fetch16();
		mMemory[addr + mRegisterX] = rotateright(mMemory[addr + mRegisterX]);
		OutForComAndModeENUM("ROR", ABX, addr);
		//if (ISDEBUG) { std::cout << "ROL" << "\t" << std::hex << std::setw(4) << std::setfill('0') << addr << ",x"; }
	}

	void rotateRightAbsolute()
	{
		uint8_t addr = fetch16();
		mMemory[addr] = rotateright(mMemory[addr]);
		OutForComAndModeENUM("ROR", ABS, addr);
		//if (ISDEBUG) { std::cout << "ROL" << "\t" << std::hex << std::setw(2) << std::setfill('0') << addr; }
	}



	void transferAccToX()
	{
		if (ISDEBUG) { std::cout << "TAX" << "\t"; }
		mRegisterX = mAccumulator;
		setZeroAndNegativeFlags(mRegisterX);
	}

	void transferAccToY()
	{
		if (ISDEBUG) { std::cout << "TAY" << "\t"; }
		mRegisterY = mAccumulator;
		setZeroAndNegativeFlags(mRegisterY);
	}

	void transferStackToX()
	{
		if (ISDEBUG) { std::cout << "TSX" << "\t"; }
		mRegisterX = mStackPointer;
		setZeroAndNegativeFlags(mRegisterX);
	}

	void transferXToAcc()
	{
		if (ISDEBUG) { std::cout << "TXA" << "\t"; }
		mAccumulator = mRegisterX;
		setZeroAndNegativeFlags(mAccumulator);
	}

	void transferYToAcc()
	{
		if (ISDEBUG) { std::cout << "TYA" << "\t"; }
		mAccumulator = mRegisterY;
		setZeroAndNegativeFlags(mAccumulator);
	}

	void transferXToStack()
	{
		if (ISDEBUG) { std::cout << "TXS" << "\t"; }
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

			uint16_t leftNibble = ((valueA & 0xF0) >> 4) + ((valueB & 0xF0) >> 4) + bcdCarry;
			if (leftNibble > 9) {
				leftNibble -= 10;
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
			result = (valueA & 0xF) + ((~valueB) & 0xF) + (bcdCarry ? 1 : 0); //BCDcarry will always be true here, if i am not mistaken?
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



	//"VAL" IN ALL COMPARE OPERATIONS IS VALUE OF THE CHOSEN REGISTER AND IS NOT THE VALUE IT IS BEING COMPARED WITH.
	//VALUE THAT IT IS BEING COMPARED TO IS NAMED "VALUE" IN CODE



	void compareImmediate(std::string instruction, uint8_t val)
	{
		uint8_t value = fetch();
		if (ISDEBUG) { std::cout << instruction << "\t" << "#" << (int)value; }
		compareBase(val, value);
	}

	void compareAbsolute(std::string instruction, uint8_t val)
	{
		uint16_t addr = fetch16();
		uint8_t value = mMemory[addr];
		if (ISDEBUG) { std::cout << instruction << "\t" << std::hex << std::setw(4) << std::setfill('0') << addr; }
		compareBase(val, value);
	}

	void compareZeroPage(std::string instruction, uint8_t val)
	{
		uint8_t addr = fetch();
		uint8_t value = mMemory[addr];
		if (ISDEBUG) { std::cout << instruction << "\t" << std::hex << std::setw(2) << std::setfill('0') << addr; }
		compareBase(val, value);
	}

	void compareZeroPageX(std::string instruction, uint8_t val)
	{
		uint8_t addr = fetch();
		uint8_t value = mMemory[addr + mRegisterX];
		if (ISDEBUG) { std::cout << instruction << "\t" << std::hex << std::setw(2) << std::setfill('0') << addr << ",x"; }
		compareBase(val, value);
	}

	void compareAbsoluteX(std::string instruction, uint8_t val)
	{
		uint16_t addr = fetch16();
		uint8_t value = mMemory[addr + mRegisterX];
		if (ISDEBUG) {std::cout << instruction << "\t" << std::hex << std::setw(4) << std::setfill('0') << addr << ",x";}
		compareBase(val, value);
	}

	void compareAbsoluteY(std::string instruction, uint8_t val)
	{
		uint16_t addr = fetch16();
		uint8_t value = mMemory[addr + mRegisterY];
		if (ISDEBUG) {std::cout << instruction << "\t" << std::hex << std::setw(4) << std::setfill('0') << addr << ",y";}
		compareBase(val, value);
	}

	void compareIndY(std::string instruction, uint8_t val)
	{
		uint8_t lookupaddress = fetch();

		//uint16_t addr = (mMemory[lookupaddress] + mMemory[lookupaddress + 1] << 8) + mRegisterY;
		uint8_t value = mMemory[(mMemory[lookupaddress] + mMemory[lookupaddress + 1] << 8) + mRegisterY];
		if (ISDEBUG) {std::cout << instruction << "\t" << "(" << std::hex << std::setw(4) << std::setfill('0') << lookupaddress << "),y";}
		compareBase(val, value);
	}

	void compareIndX(std::string instruction, uint8_t val)
	{
		uint8_t lookupaddress = fetch() + mRegisterX;

		//uint16_t addr = (mMemory[lookupaddress + mRegisterX] + mMemory[lookupaddress + mRegisterX + 1] << 8);
		uint8_t value = mMemory[mMemory[lookupaddress] + mMemory[lookupaddress + 1] << 8];
		if (ISDEBUG) {std::cout << instruction << "\t" << "(" << std::hex << std::setw(4) << std::setfill('0') << lookupaddress << ",x)";}
		compareBase(val, value);
	}



	void branchNonZero() {
		int8_t fetchedByte = branchBase("BNE");
		if (Z == 0)
		{
			mProgramCounter += fetchedByte;
			//branchDebugPrint();
		}
	}

	void branchCarrySet() {
		int8_t fetchedByte = branchBase("BCS");
		if (C == 1)
		{
			mProgramCounter += fetchedByte;
			//branchDebugPrint();
		}
	}

	void branchCarryClear() {
		int8_t fetchedByte = branchBase("BCC");
		if (C == 0)
		{
			mProgramCounter += fetchedByte;
			//ranchDebugPrint();
		}
	}

	void branchZero() {
		int8_t fetchedByte = branchBase("BEQ");
		if (Z == 1)
		{
			mProgramCounter += fetchedByte;
			//branchDebugPrint("BEQ", fetchedByte);
		}
	}

	void branchMinus() {
		int8_t fetchedByte = branchBase("BMI");
		if (N == 1)
		{
			mProgramCounter += fetchedByte;
			//branchDebugPrint("BMI", fetchedByte);
		}
	}

	void branchPlus() {
		int8_t fetchedByte = branchBase("BPL");
		if (N == 0)
		{
			mProgramCounter += fetchedByte;
			//branchDebugPrint("BPL", fetchedByte);
		}
	}

	void branchOverflowClear() {
		int8_t fetchedByte = branchBase("BVC");
		if (V == 0)
		{
			mProgramCounter += fetchedByte;
			//branchDebugPrint("BVS", fetchedByte);
		}
	}

	void branchOverflowSet() {
		int8_t fetchedByte = branchBase("BVS");
		if (V == 1)
		{
			mProgramCounter += fetchedByte;
			//branchDebugPrint("BVS", fetchedByte);
		}
	}

	int8_t branchBase(char const* instruction) {
		if (ISDEBUG) { std::cout << instruction; }
		int8_t offset = fetch();
		if (ISDEBUG) { std::cout << "\t" << offset; }
		return offset;
	}

	/*void branchDebugPrint(std::string instruction, int8_t fetchedByte) {
		
		if (ISDEBUG)
		{
			std::cout << "\t" << (int)fetchedByte;
		}
	}*/

	uint16_t jumpAbsolute() {
		//std::cout << "JMP (absolute) started, PC: " << mProgramCounter << std::endl;

		uint16_t jumpAddress = fetch16();

		//std::cout << "New Address: " << jumpAddress << std::endl;
		if (ISDEBUG) { std::cout << "JMP" << "\t" << "#" << jumpAddress; }
		return jumpAddress;
	}

	uint16_t jumpIndirect() {
		//std::cout << "JMP (indirect) started, PC: " << mProgramCounter << std::endl;

		uint16_t lookupAddress = fetch16();
		mProgramCounter = lookupAddress;
		//std::cout << "Lookup Address: " << lookupAddress << std::endl;

		uint16_t jumpAddress = fetch16();
		//std::cout << "New Address: " << jumpAddress << std::endl;
		if (ISDEBUG) { std::cout << "JMP" << "\t" << "(" << "$" << std::hex << std::setw(4) << std::setfill('0') << lookupAddress << ")"; }
		return jumpAddress;
	}

	uint8_t loadIndirectX(const char* instruction) {
		uint8_t lookupAddress = fetch();

		if (ISDEBUG) { std::cout << instruction << "\t" << "(" << (int)lookupAddress << ",x)"; }

		lookupAddress += mRegisterX;
		//if (ISDEBUG) { std::cout << instruction << "\t" << "(" << (int)lookupAddress << ",x)"; }
		//std::cout << "Lookup address: " << std::hex << static_cast<int>(lookupAddress) << ", x being: " << std::hex << static_cast<int>(mRegisterX) << std::endl;

		uint16_t address = mMemory[lookupAddress] + (mMemory[lookupAddress + 1] << 8);
		//std::cout << "Address: " << std::hex << static_cast<int>(address) << " lookupA: " << std::hex << static_cast<int>(lookupAddress) << std::endl;

		uint8_t result = mMemory[address];
		//std::cout << "Loading " << std::hex << static_cast<int>(result) << " into A" << std::endl;

		return result;
	}

	uint8_t loadZeroPage(std::string instruction) {
		uint8_t addr = fetch();
		uint8_t result = mMemory[addr];
		if (ISDEBUG) { std::cout << instruction << "\t" << (int)addr; }
		//std::cout << "Loading " << std::hex << static_cast<int>(result) << " into A" << std::endl;
		return result;
	}

	uint8_t loadImmediate(std::string instruction) {
		uint8_t result = fetch();
		if (ISDEBUG) { std::cout << instruction << "\t" << "#" << (int)result; }
		//std::cout << "Loading " << std::hex << static_cast<int>(result) << " into A" << std::endl;
		return result;
	}

	uint8_t loadAbsolute(std::string instruction) {
		uint16_t addr = fetch16();
		uint8_t result = mMemory[addr];
		if (ISDEBUG) { std::cout << instruction << "\t" << (int)addr; }
		//std::cout << "Loading " << std::hex << static_cast<int>(result) << " into A" << std::endl;
		return result;
	}

	uint8_t loadIndirectY(std::string instruction) {
		uint8_t lookupAddress = fetch();
		if (ISDEBUG) { std::cout << instruction << "\t" << "(" << (int)lookupAddress << "),y"; }
		//std::cout << "Lookup address: " << std::hex << static_cast<int>(lookupAddress) << ", y: " << std::hex << static_cast<int>(mRegisterY) << std::endl;

		uint16_t address = (mMemory[lookupAddress] + (mMemory[lookupAddress + 1] << 8)) + mRegisterY;
		//std::cout << "Address: " << std::hex << static_cast<int>(address) << std::endl;

		uint8_t result = mMemory[address];
		//std::cout << "Loading " << std::hex << static_cast<int>(result) << " into A" << std::endl;
		return result;
	}

	uint8_t loadZeroPageX(std::string instruction) {
		uint8_t base = fetch();
		uint8_t	address = mRegisterX + base;
		uint8_t result = mMemory[address];
		if (ISDEBUG) { std::cout << instruction << "\t" << (int)base << ",x"; }
		//std::cout << "LDA " << std::hex << static_cast<int>(result) << " into A, address: " << std::hex << static_cast<int>(address) << std::endl;
		return result;
	}

	uint8_t loadZeroPageY(std::string instruction) {
		uint8_t base = fetch();
		uint8_t	address = mRegisterY + base;
		uint8_t result = mMemory[address];
		if (ISDEBUG) { std::cout << instruction << "\t" << (int)base << ",y"; }
		//std::cout << "LDA " << std::hex << static_cast<int>(result) << " into A, address: " << std::hex << static_cast<int>(address) << std::endl;
		return result;
	}

	uint8_t loadAbsoluteY(std::string instruction) {
		uint16_t base = fetch16();
		uint8_t result = mMemory[base + mRegisterY];
		if (ISDEBUG) { std::cout << instruction << "\t" << (int)base << ",y"; }
		//std::cout << "Loading " << std::hex << static_cast<int>(result) << " into A" << std::endl;
		return result;
	}

	uint8_t loadAbsoluteX(std::string instruction) {
		uint16_t base = fetch16();
		uint8_t result = mMemory[base + mRegisterX];
		if (ISDEBUG) { std::cout << instruction << "\t" << (int)base << ",x"; }
		//std::cout << "Loading " << std::hex << static_cast<int>(result) << " into A" << std::endl;
		return result;
	}



	//save instructions 

	void saveIndirectY(std::string instruction, uint8_t value) {
		uint8_t lookupAddress = fetch();
		if (ISDEBUG) { std::cout << instruction << "\t" << "(" << (int)lookupAddress << "),y"; }

		uint16_t address = (mMemory[lookupAddress] + (mMemory[lookupAddress + 1] << 8)) + mRegisterY;

		mMemory[address] = value;
	}

	void saveIndirectX(std::string instruction, uint8_t value) {
		uint8_t lookupAddress = fetch();

		if (ISDEBUG) { std::cout << instruction << "\t" << "(" << (int)lookupAddress << ",x)"; }
		lookupAddress += mRegisterX;
		uint16_t address = mMemory[lookupAddress] + (mMemory[lookupAddress + 1] << 8);
		mMemory[address] = value;
	}

	void saveZeroPage(std::string instruction, uint8_t value) {
		uint8_t addr = fetch();
		mMemory[addr] = value;
		if (ISDEBUG) { std::cout << instruction << "\t" << (int)addr; }
	}

	void saveAbsolute(std::string instruction, uint8_t value) {
		uint16_t addr = fetch16();
		mMemory[addr] = value;
		if (ISDEBUG) { std::cout << instruction << "\t" << (int)addr; }
	}

	void saveZeroPageX(std::string instruction, uint8_t value) {
		uint8_t base = fetch();
		uint8_t	address = mRegisterX + base;
		mMemory[address] = value;
		if (ISDEBUG) { std::cout << instruction << "\t" << (int)base << ",x"; }
	}

	void saveZeroPageY(std::string instruction, uint8_t value) {
		uint8_t base = fetch();
		uint8_t	address = mRegisterY + base;
		mMemory[address] = value;
		if (ISDEBUG) { std::cout << instruction << "\t" << (int)base << ",y"; }
	}

	void saveAbsoluteY(std::string instruction, uint8_t value) {
		uint16_t base = fetch16();
		mMemory[base + mRegisterY] = value;
		if (ISDEBUG) { std::cout << instruction << "\t" << (int)base << ",y"; }
	}

	void saveAbsoluteX(std::string instruction, uint8_t value) {
		uint16_t base = fetch16();
		mMemory[base + mRegisterX] = value;
		if (ISDEBUG) { std::cout << instruction << "\t" << (int)base << ",x"; }
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
		0xE0, 0x09,
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

	auto cpu = std::make_shared<MOS6502>();
	cpu->loadProgram(program, sizeof(program), 0x0000);
	cpu->loadProgram(startingB0, sizeof(startingB0), 0x00B0);
	cpu->loadProgram(starting0842, sizeof(starting0842), 0x0842);
	cpu->loadProgram(startingF2, sizeof(startingF2), 0x00F2);
	cpu->loadProgram(starting0500, sizeof(starting0500), 0x0500);
	cpu->loadProgram(twoProgram, sizeof(twoProgram), 0x0120);
	cpu->loadProgram(threeProgram, sizeof(threeProgram), 0x0150);
	cpu->loadProgram(memory, sizeof(memory), 0x0A24);
	cpu->execute();
}