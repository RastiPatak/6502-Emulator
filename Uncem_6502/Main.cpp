#include <iostream>
#include <iomanip>

enum OpCode
{
	BRK = 0,
	JMPAbs = 0x4C,
	JMPInd = 0x6C,
	JSRAbs = 0x20,
	NOP = 0xEA,
	PHP = 0x08,
	PLP = 0x28,
	PLA = 0x68,
	PHA = 0x48,
	RTS = 0x60,
	RTI = 0x40,

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
	INCZeroP = 0xE6,
	INCZeroPX = 0xF6,
	INCAbs = 0xEE,
	INCAbsX = 0xFE,

	DEX = 0xCA, // Decrement X by 1
	DEY = 0x88, // Decrement Y by 1
	DECZeroP = 0xC6,
	DECZeroPX = 0xD6,
	DECAbs = 0xCE,
	DECAbsX = 0xDE,


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

	ADCImmediate = 0x69, // immediate	ADC #oper	69	2	2
	ADCZeroP = 0x65,      // zeropage	ADC oper	65	2	3
	ADCZeroPX = 0x75,    //zeropage, X	ADC oper, X	75	2	4
	ADCAbs = 0x6D,        //absolute	ADC oper	6D	3	4
	ADCAbsX = 0x7D,       //absolute, X	ADC oper, X	7D	3	4 *
	ADCAbsY = 0x79,       //absolute, Y	ADC oper, Y	79	3	4 *
	ADCIndX = 0x61,        //(indirect, X)	ADC(oper, X)	61	2	6
	ADCIndY = 0x71,		//(indirect), Y	ADC(oper), Y	71	2	5 *

	SBCImmediate = 0xE9,//immediate	SBC #oper	E9	2	2
	SBCZeroP = 0xE5,//zeropage	SBC oper	E5	2	3
	SBCZeroPX = 0xF5,//zeropage, X	SBC oper, X	F5	2	4
	SBCAbs = 0xED,//absolute	SBC oper	ED	3	4
	SBCAbsX = 0xFD,//absolute, X	SBC oper, X	FD	3	4 *
	SBCAbsY = 0xF9,//absolute, Y	SBC oper, Y	F9	3	4 *
	SBCIndX = 0xE1,//(indirect, X)	SBC(oper, X)	E1	2	6
	SBCIndY = 0xF1,//(indirect), Y	SBC(oper), Y	F1	2	5 *


	ROLAcc = 0x2A, // Rotate left
	ROLZeroP = 0x26,
	ROLZeroPX = 0x36,
	ROLAbs = 0x2E,
	ROLAbsX = 0x3E,

	RORAcc = 0x6A, // Rotate right
	RORZeroP = 0x66,
	RORZeroPX = 0x76,
	RORAbs = 0x6E,
	RORAbsX = 0x7E,

	ASLAcc = 0x0A, //Arithmetic shift left
	ASLZeroP = 0x06,
	ASLZeroPX = 0x16,
	ASLAbs = 0x0E,
	ASLAbsX = 0x1E,

	LSRAcc = 0x4A, //Logical shift right
	LSRZeroP = 0x46,
	LSRZeroPX = 0x56,
	LSRAbs = 0x4E,
	LSRAbsX = 0x5E,

	ORAImmediate = 0x09,  //or with memory or accumulator
	ORAZeroP = 0x05,
	ORAZeroPX = 0x15,
	ORAAbs = 0x0D,
	ORAAbsX = 0x1D,
	ORAAbsY = 0x19,
	ORAIndX = 0x01,
	ORAIndY = 0x11,

	EORImmediate = 0x49,  //xor with memory or accumulator
	EORZeroP = 0x45,
	EORZeroPX = 0x55,
	EORAbs = 0x4D,
	EORAbsX = 0x5D,
	EORAbsY = 0x59,
	EORIndX = 0x41,
	EORIndY = 0x51,

	ANDImmediate = 0x29, //and with memory or accumulator
	ANDZeroP = 0x25,
	ANDZeroPX = 0x35,
	ANDAbs = 0x2D,
	ANDAbsX = 0x3D,
	ANDAbsY = 0x39,
	ANDIndX = 0x21,
	ANDIndY = 0x31,

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
	CPYAbs = 0xCC,

	BITAbs = 0x2C,
	BITZeroP = 0x24,

	HALT = 0xFF // Undocumented code, used for testing
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
	INDX = 8,
	INDY = 9,
	NON = 10,
	A = 11 //Accumulator as address mode, used in few commands
};

class MOS6502
{
public:
	bool ISDEBUG = true; // change this manually in code to set it to either debug or usual mode (true for debug, false for usual)

	MOS6502()
		: mAccumulator(0), mRegisterX(0), mRegisterY(0), mProgramCounter(0), mStackPointer(0xFF), C(0), Z(0), I(0), D(0), B(0), V(0), N(0)
	{
		for (std::size_t i = 0; i < sizeof(mMemory); i++)
		{
			mMemory[i] = 0;
		}
	}

	void loadProgram(const uint8_t* program, std::size_t size, uint16_t offset)
	{
		memcpy(mMemory + offset, program, size);
	}

	void reset()
	{
		mProgramCounter = 0xFFFE;
		mProgramCounter = fetch16();
	}

	void executeFrom(uint16_t start)
	{
		mProgramCounter = start;
		while (true)
		{
			uint8_t opcode = fetch();
			if (opcode == HALT) { return; }
			if (!executeOpcode((OpCode)opcode))
			{
				printMemory();
				std::cerr << "Unknown opcode: " << std::hex << static_cast<int>(opcode) << std::endl;
				return;
			}
		}
	}

	void execute()
	{
		// execute from current mProgramCounter
		while (true)
		{
			uint8_t opcode = fetch();
			if (opcode == HALT) { return; }
			if (!executeOpcode((OpCode)opcode))
			{
				printMemory();
				std::cerr << "Unknown opcode: " << std::hex << static_cast<int>(opcode) << std::endl;
				return;
			}
		}
	}

	void printMemory()
	{
		for (std::size_t i = 0; i < sizeof(mMemory); i++)
		{
			std::cout << std::hex << static_cast<int>(mMemory[i]) << ", ";
		}
		std::cout << std::endl;
	}

protected:
	uint8_t mMemory[65536];
	uint8_t mAccumulator, mRegisterX, mRegisterY;
	uint16_t mProgramCounter;
	uint8_t mStackPointer;
	uint8_t C, Z, I, D, B, V, N;

	static constexpr uint16_t stackOffset = 0x100;

	bool executeOpcode(OpCode opcode)
	{
		if (ISDEBUG)
		{
			// fetch already performed before, so we write PC before fetch
			std::cout << std::hex << std::setw(4) << std::setfill('0') << (mProgramCounter - 1) << std::setfill(' ') << std::setw(0) << "\t";
		}
		switch (opcode)
		{
		case JMPAbs:
			mProgramCounter = jumpAbsolute();
			break;
		case JMPInd:
			mProgramCounter = jumpIndirect();
			break;
		case JSRAbs:
			mProgramCounter = jumpAbsoluteSavingAddress();
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
		case INCZeroP:
			incrementZeroPage();
			break;
		case INCZeroPX:
			incrementZeroPageX();
			break;
		case INCAbs:
			incrementAbsolute();
			break;
		case INCAbsX:
			incrementAbsoluteX();
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
		case DECZeroP:
			decrementZeroPage();
			break;
		case DECZeroPX:
			decrementZeroPageX();
			break;
		case DECAbs:
			decrementAbsolute();
			break;
		case DECAbsX:
			decrementAbsoluteX();
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

		case ORAImmediate:                        //or with memory or accumulator
			orWithMemoryOrAccImmediate();
			break;
		case ORAZeroP:
			orWithMemoryOrAccZeroP();
			break;
		case ORAZeroPX:
			orWithMemoryOrAccZeroPX();
			break;
		case ORAAbs:
			orWithMemoryOrAccAbs();
			break;
		case ORAAbsX:
			orWithMemoryOrAccAbsX();
			break;
		case ORAAbsY:
			orWithMemoryOrAccAbsY();
			break;
		case ORAIndX:
			orWithMemoryOrAccIndX();
			break;
		case ORAIndY:
			orWithMemoryOrAccIndY();
			break;

		case EORImmediate:                        //or with memory or accumulator
			orWithMemoryOrAccImmediate();
			break;
		case EORZeroP:
			orWithMemoryOrAccZeroP();
			break;
		case EORZeroPX:
			orWithMemoryOrAccZeroPX();
			break;
		case EORAbs:
			orWithMemoryOrAccAbs();
			break;
		case EORAbsX:
			orWithMemoryOrAccAbsX();
			break;
		case EORAbsY:
			orWithMemoryOrAccAbsY();
			break;
		case EORIndX:
			orWithMemoryOrAccIndX();
			break;
		case EORIndY:
			orWithMemoryOrAccIndY();
			break;


		case ANDImmediate:
			andWithMemoryOrAccImmediate();
			break;
		case ANDZeroP:
			andWithMemoryOrAccZeroP();
			break;
		case ANDZeroPX:
			andWithMemoryOrAccZeroPX();
			break;
		case ANDAbs:
			andWithMemoryOrAccAbs();
			break;
		case ANDAbsX:
			andWithMemoryOrAccAbsX();
			break;
		case ANDAbsY:
			andWithMemoryOrAccAbsY();
			break;
		case ANDIndX:
			andWithMemoryOrAccIndX();
			break;
		case ANDIndY:
			andWithMemoryOrAccIndY();
			break;


		case ADCImmediate:
			adcWithMemoryOrAccImmediate();
			break;
		case ADCZeroP:
			adcWithMemoryOrAccZeroP();
			break;
		case ADCZeroPX:
			adcWithMemoryOrAccZeroPX();
			break;
		case ADCAbs:
			adcWithMemoryOrAccAbs();
			break;
		case ADCAbsX:
			adcWithMemoryOrAccAbsX();
			break;
		case ADCAbsY:
			adcWithMemoryOrAccAbsY();
			break;
		case ADCIndX:
			adcWithMemoryOrAccIndX();
			break;
		case ADCIndY:
			adcWithMemoryOrAccIndY();
			break;

		case SBCImmediate:
			sbcWithMemoryOrAccImmediate();
			break;
		case SBCZeroP:
			sbcWithMemoryOrAccZeroP();
			break;
		case SBCZeroPX:
			sbcWithMemoryOrAccZeroPX();
			break;
		case SBCAbs:
			sbcWithMemoryOrAccAbs();
			break;
		case SBCAbsX:
			sbcWithMemoryOrAccAbsX();
			break;
		case SBCAbsY:
			sbcWithMemoryOrAccAbsY();
			break;
		case SBCIndX:
			sbcWithMemoryOrAccIndX();
			break;
		case SBCIndY:
			sbcWithMemoryOrAccIndY();
			break;


		case RORAcc:
			rotateRightAccumulator();
			break;
		case RORZeroP:
			rotateRightAccumulator();
			break;
		case RORZeroPX:
			rotateRightAccumulator();
			break;
		case RORAbs:
			rotateRightAccumulator();
			break;
		case RORAbsX:
			rotateRightAccumulator();
			break;

		case ROLAcc:
			rotateLeftAccumulator();
			break;
		case ROLZeroP:
			rotateLeftAccumulator();
			break;
		case ROLZeroPX:
			rotateLeftAccumulator();
			break;
		case ROLAbs:
			rotateLeftAccumulator();
			break;
		case ROLAbsX:
			rotateLeftAccumulator();
			break;

		case LSRAcc:
			shiftLeftAccumulator();
			break;
		case LSRZeroP:
			shiftLeftZeroPage();
			break;
		case LSRZeroPX:
			shiftLeftZeroPageX();
			break;
		case LSRAbs:
			shiftLeftAbsolute();
			break;
		case LSRAbsX:
			shiftLeftAbsoluteX();
			break;

		case ASLAcc:
			shiftRightAccumulator();
			break;
		case ASLZeroP:
			shiftRightZeroPage();
			break;
		case ASLZeroPX:
			shiftRightZeroPageX();
			break;
		case ASLAbs:
			shiftRightAbsolute();
			break;
		case ASLAbsX:
			shiftRightAbsoluteX();
			break;

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


		case BITAbs:
			bitTestAbsolute();
			break;
		case BITZeroP:
			bitTestAbsolute();
			break;

			//MISCELANNEOUS OPERATIONS
		case BRK:
			breakCPU();          //Its Breaking Bad time!
			break;
		case RTI:
			returnFromInterrupt();
			break;
		case RTS:
			returnFromSubroutine();
			break;
		case PLP:
			pullStatusFromStack();
			break;
		case PHA:
			pushAccToStack();
			break;
		case PLA:
			pullAccFromStack();
			break;
		case PHP:
			pushStatusToStack();
			break;
		case NOP:
			if (ISDEBUG) { std::cout << "NOP" << "\t"; }
			break;
		default:
			return false;
		}
		if (ISDEBUG)
		{
			printRegisterInfo();
		}

		return true;
	}

private:

	uint8_t fetch()
	{
		uint8_t data = mMemory[mProgramCounter];
		mProgramCounter++;
		return data;
	}

	uint16_t fetch16()
	{
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

	void breakCPU()
	{
		mMemory[stackOffset + mStackPointer] = (mProgramCounter & 0xF0) >> 8;
		mStackPointer--;
		mMemory[stackOffset + mStackPointer] = mProgramCounter & 0x0F;
		mStackPointer--;
		uint8_t Status = 0x00;
		Status += (N ? 0x80 : 0);
		Status += (V ? 0x40 : 0);
		Status += 0x20;
		Status += 0x10;
		Status += (D ? 0x08 : 0);
		Status += 0x04;
		Status += (Z ? 0x02 : 0);
		Status += (C ? 0x01 : 0);
		mMemory[stackOffset + mStackPointer] = Status;
		mStackPointer--;
		if (ISDEBUG) { std::cout << "BRK" << "\t"; }
	}



	void incrementZeroPage()
	{
		uint8_t addr = fetch();
		mMemory[addr]++;
		OutForComAndModeENUM("INC", ZPG, addr);
		setZeroAndNegativeFlags(mMemory[addr]);
	}

	void incrementZeroPageX()
	{
		uint8_t addr = fetch();
		mMemory[addr + mRegisterX]++;
		OutForComAndModeENUM("INC", ZPG, addr);
		setZeroAndNegativeFlags(mMemory[addr + mRegisterX]);
	}

	void incrementAbsoluteX()
	{
		uint16_t addr = fetch16();
		mMemory[addr + mRegisterX]++;
		OutForComAndModeENUM("INC", ABX, addr);
		setZeroAndNegativeFlags(mMemory[addr + mRegisterX]);
	}

	void incrementAbsolute()
	{
		uint16_t addr = fetch16();
		mMemory[addr]++;
		OutForComAndModeENUM("INC", ABS, addr);
		setZeroAndNegativeFlags(mMemory[addr]);
	}


	void decrementZeroPage()
	{
		uint8_t addr = fetch();
		mMemory[addr]--;
		OutForComAndModeENUM("DEC", ZPG, addr);
		setZeroAndNegativeFlags(mMemory[addr]);
	}

	void decrementZeroPageX()
	{
		uint8_t addr = fetch();
		mMemory[addr + mRegisterX]--;
		OutForComAndModeENUM("DEC", ZPG, addr);
		setZeroAndNegativeFlags(mMemory[addr + mRegisterX]);
	}

	void decrementAbsoluteX()
	{
		uint16_t addr = fetch16();
		mMemory[addr + mRegisterX]--;
		OutForComAndModeENUM("DEC", ABX, addr);
		setZeroAndNegativeFlags(mMemory[addr + mRegisterX]);
	}

	void decrementAbsolute()
	{
		uint16_t addr = fetch16();
		mMemory[addr]--;
		OutForComAndModeENUM("DEC", ABS, addr);
		setZeroAndNegativeFlags(mMemory[addr]);
	}


	uint8_t rotateright(uint8_t value)
	{
		uint8_t resultingvalue = (value >> 1) | (C ? 0x80 : 0); //I rotate the entered value by 1 position right, replacing the left-most bit of the ROTATED VALUE with carry (either 1 or 0)
		C = value & 0x1;                                      //I store the bit that disappears due to shifting of the number in the carry flag
		setZeroAndNegativeFlags(resultingvalue);              //I also set the correct flags if the resulting value after shifting appears to be zero or negative
		return resultingvalue;                                //I return the value 
	}

	uint8_t rotateleft(uint8_t value)
	{
		uint8_t resultingvalue = (value << 1) | (C ? 1 : 0);  //I rotate the entered value 1 position left, replacing the right-most bit of the ROTATED VALUE with value of carry 
		C = value & 0x80;                                     //I store the left-most bit that disappears due to shifting into carry flag
		setZeroAndNegativeFlags(resultingvalue);              //I check if the value is negative or zero
		return resultingvalue;                                //I return the value
	}

	uint8_t shiftleft(uint8_t value)
	{
		uint8_t resultingvalue = value << 1;                  //I rotate the entered value 1 position left, replacing the right-most bit of the ROTATED VALUE with 0
		C = value & 0x80;                                     //I store the left-most bit that disappears due to shifting into carry flag
		setZeroAndNegativeFlags(resultingvalue);              //I check if the value is negative or zero
		return resultingvalue;                                //I return the value
	}

	uint8_t shifteright(uint8_t value)
	{
		uint8_t resultingvalue = value >> 1;                  //I rotate the entered value by 1 position right, replacing the left-most bit of the ROTATED VALUE with 0
		C = value & 0x1;                                      //I store the bit that disappears due to shifting of the number in the carry flag
		setZeroAndNegativeFlags(resultingvalue);              //I also set the correct flags if the resulting value after shifting appears to be zero or negative
		return resultingvalue;                                //I return the value 
	}

	//VERSION OF UNIFIED OUTPUT FUNCTION WITH ENUMS USED
	void OutForComAndModeENUM(const char* instruction, addressMode mode, uint16_t addr)
	{
		// addrmode should be made to enum, if you wish. This version is already working one, but i didnt implement it widely
		if (ISDEBUG)
		{
			switch (mode)
			{
			case IMD:
				std::cout << instruction << "\t" << "#" << "$" << std::hex << std::setw(2) << std::setfill('0') << addr;
				break;
			case ZPG:
				std::cout << instruction << "\t" << "$" << std::hex << std::setw(2) << std::setfill('0') << addr;
				break;
			case ZPX:
				std::cout << instruction << "\t" << "$" << std::hex << std::setw(2) << std::setfill('0') << addr << ",x";
				break;
			case ZPY:
				std::cout << instruction << "\t" << "$" << std::hex << std::setw(2) << std::setfill('0') << addr << ",y";
				break;
			case ABS:
				std::cout << instruction << "\t" << "$" << std::hex << std::setw(4) << std::setfill('0') << addr;
				break;
			case ABX:
				std::cout << instruction << "\t" << "$" << std::hex << std::setw(4) << std::setfill('0') << addr << ",x";
				break;
			case ABY:
				std::cout << instruction << "\t" << "$" << std::hex << std::setw(4) << std::setfill('0') << addr << ",y";
				break;
			case INDX:
				std::cout << instruction << "\t" << "(" << "$" << std::hex << std::setw(4) << std::setfill('0') << addr << ",x)";
				break;
			case INDY:
				std::cout << instruction << "\t" << "(" << "$" << std::hex << std::setw(4) << std::setfill('0') << addr << "),y";
				break;
			case A:
				std::cout << instruction << "\t" << "A";
				break;
			default:
				std::cout << instruction << "\t";
				break;
			}
		}
	}


	// NV1BDIZC -> flags register (byte construction)
	// to add Carry -> directly add 0x01
	// to add Zero -> directly add 0x02
	// to add Interrupt disable -> directly add 0x04
	// to add Decimal -> directly add 0x08
	// to add Break -> directly add 0x10
	// to add 5th bit -> directly add 0x20
	// to add Overflow -> directly add 0x40
	// to add Negative -> directly add 0x80
	void pushStatusToStack()
	{
		uint8_t Status = 0x00;
		Status += (N ? 0x80 : 0);
		Status += (V ? 0x40 : 0);
		Status += 0x20;
		Status += 0x10;
		Status += (D ? 0x08 : 0);
		Status += (I ? 0x04 : 0);
		Status += (Z ? 0x02 : 0);
		Status += (C ? 0x01 : 0);
		mMemory[stackOffset + mStackPointer] = Status;
		mStackPointer--;
		if (ISDEBUG) { std::cout << "PHP" << "\t"; }
	}

	void pullStatusFromStack()
	{
		mStackPointer++;
		uint8_t Status = mMemory[stackOffset + mStackPointer];
		C = (Status & 0x01) != 0;
		Z = (Status & 0x02) != 0;
		I = (Status & 0x04) != 0;
		D = (Status & 0x08) != 0;
		V = (Status & 0x40) != 0;
		N = (Status & 0x80) != 0;
		if (ISDEBUG) { std::cout << "PLP" << "\t"; }
	}

	void pushAccToStack()
	{
		mMemory[stackOffset + mStackPointer] = mAccumulator;
		mStackPointer--;
		if (ISDEBUG) { std::cout << "PHA" << "\t"; }
	}

	void pullAccFromStack()
	{
		mStackPointer++;
		mAccumulator = mMemory[stackOffset + mStackPointer];
		if (ISDEBUG) { std::cout << "PLA" << "\t"; }
	}

	void returnFromInterrupt()
	{
		mStackPointer++;
		uint8_t Status = mMemory[stackOffset + mStackPointer];
		C = (Status & 0x01) != 0;
		Z = (Status & 0x02) != 0;
		I = (Status & 0x04) != 0;
		D = (Status & 0x08) != 0;
		V = (Status & 0x40) != 0;
		N = (Status & 0x80) != 0;
		mStackPointer++;
		uint16_t ProgramCounter = mMemory[stackOffset + mStackPointer];
		mStackPointer++;
		ProgramCounter += (mMemory[stackOffset + mStackPointer] << 8);
		mProgramCounter = ProgramCounter;
		if (ISDEBUG) { std::cout << "RTI" << "\t"; }
	}

	void returnFromSubroutine()
	{
		mStackPointer++;
		uint16_t ProgramCounter = mMemory[stackOffset + mStackPointer];
		mStackPointer++;
		ProgramCounter += (mMemory[stackOffset + mStackPointer] << 8);
		mProgramCounter = ProgramCounter + 1;
		if (ISDEBUG) { std::cout << "RTS" << "\t"; }
	}

	void rotateLeftAccumulator()
	{
		mAccumulator = rotateleft(mAccumulator);
		OutForComAndModeENUM("ROL", A, 0);
		//if (ISDEBUG) { std::cout << "ROL" << "\t" << "A"; }
	}

	void rotateLeftZeroPage()
	{
		uint8_t addr = fetch();                               // ADDR WILL BE NEEDED FOR OUTPUT, NO QUESTIONS ASKED, IT WONT WORK OTHER WAY
		mMemory[addr] = rotateleft(mMemory[addr]);
		//OutForComAndMode("instructionname", "addressingmode", "addr");
		OutForComAndModeENUM("ROL", ZPG, addr);
		//if (ISDEBUG) { std::cout << "ROL" << "\t" << std::hex << std::setw(2) << std::setfill('0') << addr; }
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
		uint16_t addr = fetch16();
		mMemory[addr] = rotateleft(mMemory[addr]);
		OutForComAndModeENUM("ROL", ABS, addr);
		//if (ISDEBUG) { std::cout << "ROL" << "\t" << std::hex << std::setw(2) << std::setfill('0') << addr; }
	}



	void shiftLeftAccumulator()
	{
		mAccumulator = shiftleft(mAccumulator);
		OutForComAndModeENUM("ASL", A, 0);
		//if (ISDEBUG) { std::cout << "ASL" << "\t" << "A"; }
	}

	void shiftLeftZeroPage()
	{
		uint8_t addr = fetch();                               // ADDR WILL BE NEEDED FOR OUTPUT, NO QUESTIONS ASKED, IT WONT WORK OTHER WAY
		mMemory[addr] = shiftleft(mMemory[addr]);
		//OutForComAndMode("instructionname", "addressingmode", "addr");
		OutForComAndModeENUM("ASL", ZPG, addr);
		//if (ISDEBUG) { std::cout << "ASL" << "\t" << std::hex << std::setw(2) << std::setfill('0') << addr; }
	}

	void shiftLeftZeroPageX()
	{
		uint8_t addr = fetch();
		mMemory[addr + mRegisterX] = shiftleft(mMemory[addr + mRegisterX]);
		OutForComAndModeENUM("ASL", ZPG, addr);
		//if (ISDEBUG) { std::cout << "ASL" << "\t" << std::hex << std::setw(2) << std::setfill('0') << addr << ",x"; }
	}

	void shiftLeftAbsoluteX()
	{
		uint16_t addr = fetch16();
		mMemory[addr + mRegisterX] = shiftleft(mMemory[addr + mRegisterX]);
		OutForComAndModeENUM("ASL", ABX, addr);
		//if (ISDEBUG) { std::cout << "ASL" << "\t" << std::hex << std::setw(4) << std::setfill('0') << addr << ",x"; }
	}

	void shiftLeftAbsolute()
	{
		uint16_t addr = fetch16();
		mMemory[addr] = shiftleft(mMemory[addr]);
		OutForComAndModeENUM("ASL", ABS, addr);
		//if (ISDEBUG) { std::cout << "ASL" << "\t" << std::hex << std::setw(2) << std::setfill('0') << addr; }
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
		uint16_t addr = fetch16();
		mMemory[addr] = rotateright(mMemory[addr]);
		OutForComAndModeENUM("ROR", ABS, addr);
		//if (ISDEBUG) { std::cout << "ROL" << "\t" << std::hex << std::setw(2) << std::setfill('0') << addr; }
	}



	void shiftRightZeroPage()
	{
		uint8_t addr = fetch();
		mMemory[addr] = rotateright(mMemory[addr]);
		OutForComAndModeENUM("LSR", ZPG, addr);
		//if (ISDEBUG) { std::cout << "LSR" << "\t" << std::hex << std::setw(2) << std::setfill('0') << addr; }
	}

	void shiftRightAccumulator()
	{
		mAccumulator = rotateright(mAccumulator);
		OutForComAndModeENUM("LSR", A, 0);
	}

	void shiftRightZeroPageX()
	{
		uint8_t addr = fetch();
		mMemory[addr + mRegisterX] = rotateright(mMemory[addr + mRegisterX]);
		OutForComAndModeENUM("LSR", ZPX, addr);
		//if (ISDEBUG) { std::cout << "LSR" << "\t" << std::hex << std::setw(2) << std::setfill('0') << addr << ",x"; }
	}

	void shiftRightAbsoluteX()
	{
		uint16_t addr = fetch16();
		mMemory[addr + mRegisterX] = rotateright(mMemory[addr + mRegisterX]);
		OutForComAndModeENUM("LSR", ABX, addr);
		//if (ISDEBUG) { std::cout << "LSR" << "\t" << std::hex << std::setw(4) << std::setfill('0') << addr << ",x"; }
	}

	void shiftRightAbsolute()
	{
		uint16_t addr = fetch16();
		mMemory[addr] = rotateright(mMemory[addr]);
		OutForComAndModeENUM("LSR", ABS, addr);
		//if (ISDEBUG) { std::cout << "LSR" << "\t" << std::hex << std::setw(2) << std::setfill('0') << addr; }
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

		if (bcd)
		{
			bool bcdCarry = false;
			result = (valueA & 0xF) + (valueB & 0xF) + carry;
			if (result > 9)
			{
				result -= 10;
				bcdCarry = true;
			}

			uint16_t leftNibble = ((valueA & 0xF0) >> 4) + ((valueB & 0xF0) >> 4) + bcdCarry;
			if (leftNibble > 9)
			{
				leftNibble -= 10;
			}

			result += leftNibble << 4;
		}
		else
		{
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

		if (bcd)
		{
			bool bcdCarry = true;
			result = (valueA & 0xF) + ((~valueB) & 0xF) + (bcdCarry ? 1 : 0); //BCDcarry will always be true here, if i am not mistaken?
			if ((valueA & 0xF) < (valueB & 0xF))
				bcdCarry = false;

			uint16_t leftNibble = (valueA & 0xF0) + ((~valueB) & 0xF0) + (bcdCarry ? 1 : 0);
			result += leftNibble << 4;
		}
		else
		{
			result = valueA - valueB - (carry ? 0 : 1); // CF inverted on sub
		}

		C = (result & 0x100) == 0; // CF inverted on sub
		V = ((valueA ^ valueB) & 0x80) == 0 && ((valueA ^ result) & 0x80) != 0;
		Z = (result & 0xFF) == 0;
		N = (result & 0x80) != 0;

		return static_cast<uint8_t>(result & 0xFF);
	}

	void compareBase(uint8_t valueA, uint8_t valueB)
	{
		uint8_t tempVSave = V;
		uint8_t result = sub(valueA, valueB, true, false);
		V = tempVSave;
	}

	//OR WITH MEMORY OR ACCUMULATOR

	void orWithMemoryOrAccImmediate()
	{
		uint8_t value = fetch();
		mAccumulator = value | mAccumulator;
		if (ISDEBUG) { std::cout << "ORA" << "\t" << "#" << (int)value; }
		setZeroAndNegativeFlags(mAccumulator);
	}

	void orWithMemoryOrAccZeroP()
	{
		uint8_t addr = fetch();
		uint8_t value = mMemory[addr];
		mAccumulator = value | mAccumulator;
		if (ISDEBUG) { std::cout << "ORA" << "\t" << std::hex << std::setw(2) << std::setfill('0') << addr; }
		setZeroAndNegativeFlags(mAccumulator);
	}

	void orWithMemoryOrAccZeroPX()
	{
		uint8_t addr = fetch();
		uint8_t value = mMemory[addr + mRegisterX];
		mAccumulator = value | mAccumulator;
		if (ISDEBUG) { std::cout << "ORA" << "\t" << std::hex << std::setw(2) << std::setfill('0') << addr << ",x"; }
		setZeroAndNegativeFlags(mAccumulator);
	}

	void orWithMemoryOrAccAbs()
	{
		uint16_t addr = fetch16();
		uint8_t value = mMemory[addr];
		mAccumulator = value | mAccumulator;
		if (ISDEBUG) { std::cout << "ORA" << "\t" << std::hex << std::setw(4) << std::setfill('0') << addr; }
		setZeroAndNegativeFlags(mAccumulator);
	}

	void orWithMemoryOrAccAbsX()
	{
		uint16_t addr = fetch16();
		uint8_t value = mMemory[addr + mRegisterX];
		mAccumulator = value | mAccumulator;
		if (ISDEBUG) { std::cout << "ORA" << "\t" << std::hex << std::setw(4) << std::setfill('0') << addr << ",x"; }
		setZeroAndNegativeFlags(mAccumulator);
	}

	void orWithMemoryOrAccAbsY()
	{
		uint16_t addr = fetch16();
		uint8_t value = mMemory[addr + mRegisterY];
		mAccumulator = value | mAccumulator;
		if (ISDEBUG) { std::cout << "ORA" << "\t" << std::hex << std::setw(4) << std::setfill('0') << addr << ",y"; }
		setZeroAndNegativeFlags(mAccumulator);
	}

	void orWithMemoryOrAccIndX()
	{
		uint8_t lookupaddress = fetch() + mRegisterX;
		uint8_t value = mMemory[mMemory[lookupaddress] + (mMemory[lookupaddress + 1] << 8)];
		mAccumulator = value | mAccumulator;
		if (ISDEBUG) { std::cout << "ORA" << "\t" << "(" << std::hex << std::setw(4) << std::setfill('0') << lookupaddress << ",x)"; }
		setZeroAndNegativeFlags(mAccumulator);
	}

	void orWithMemoryOrAccIndY()
	{
		uint8_t lookupaddress = fetch();
		uint8_t value = mMemory[(mMemory[lookupaddress] + (mMemory[lookupaddress + 1] << 8)) + mRegisterY];
		mAccumulator = value | mAccumulator;
		if (ISDEBUG) { std::cout << "ORA" << "\t" << "(" << std::hex << std::setw(4) << std::setfill('0') << lookupaddress << "),y"; }
		setZeroAndNegativeFlags(mAccumulator);
	}

	//XOR WITH MEMORY OR ACCUMULATOR

	void xorWithMemoryOrAccImmediate()
	{
		uint8_t value = fetch();
		mAccumulator = value ^ mAccumulator;
		if (ISDEBUG) { std::cout << "EOR" << "\t" << "#" << (int)value; }
		setZeroAndNegativeFlags(mAccumulator);
	}

	void xorWithMemoryOrAccZeroP()
	{
		uint8_t addr = fetch();
		uint8_t value = mMemory[addr];
		mAccumulator = value ^ mAccumulator;
		if (ISDEBUG) { std::cout << "EOR" << "\t" << std::hex << std::setw(2) << std::setfill('0') << addr; }
		setZeroAndNegativeFlags(mAccumulator);
	}

	void xorWithMemoryOrAccZeroPX()
	{
		uint8_t addr = fetch();
		uint8_t value = mMemory[addr + mRegisterX];
		mAccumulator = value ^ mAccumulator;
		if (ISDEBUG) { std::cout << "EOR" << "\t" << std::hex << std::setw(2) << std::setfill('0') << addr << ",x"; }
		setZeroAndNegativeFlags(mAccumulator);
	}

	void xorWithMemoryOrAccAbs()
	{
		uint16_t addr = fetch16();
		uint8_t value = mMemory[addr];
		mAccumulator = value ^ mAccumulator;
		if (ISDEBUG) { std::cout << "EOR" << "\t" << std::hex << std::setw(4) << std::setfill('0') << addr; }
		setZeroAndNegativeFlags(mAccumulator);
	}

	void xorWithMemoryOrAccAbsX()
	{
		uint16_t addr = fetch16();
		uint8_t value = mMemory[addr + mRegisterX];
		mAccumulator = value ^ mAccumulator;
		if (ISDEBUG) { std::cout << "EOR" << "\t" << std::hex << std::setw(4) << std::setfill('0') << addr << ",x"; }
		setZeroAndNegativeFlags(mAccumulator);
	}

	void xorWithMemoryOrAccAbsY()
	{
		uint16_t addr = fetch16();
		uint8_t value = mMemory[addr + mRegisterY];
		mAccumulator = value ^ mAccumulator;
		if (ISDEBUG) { std::cout << "EOR" << "\t" << std::hex << std::setw(4) << std::setfill('0') << addr << ",y"; }
		setZeroAndNegativeFlags(mAccumulator);
	}

	void xorWithMemoryOrAccIndX()
	{
		uint8_t lookupaddress = fetch() + mRegisterX;
		uint8_t value = mMemory[mMemory[lookupaddress] + (mMemory[lookupaddress + 1] << 8)];
		mAccumulator = value ^ mAccumulator;
		if (ISDEBUG) { std::cout << "EOR" << "\t" << "(" << std::hex << std::setw(4) << std::setfill('0') << lookupaddress << ",x)"; }
		setZeroAndNegativeFlags(mAccumulator);
	}

	void xorWithMemoryOrAccIndY()
	{
		uint8_t lookupaddress = fetch();
		uint8_t value = mMemory[(mMemory[lookupaddress] + (mMemory[lookupaddress + 1] << 8)) + mRegisterY];
		mAccumulator = value ^ mAccumulator;
		if (ISDEBUG) { std::cout << "EOR" << "\t" << "(" << std::hex << std::setw(4) << std::setfill('0') << lookupaddress << "),y"; }
		setZeroAndNegativeFlags(mAccumulator);
	}

	// AND WITH MEMORY OR ACCUMULATOR

	void andWithMemoryOrAccImmediate()
	{
		uint8_t value = fetch();
		mAccumulator = value & mAccumulator;
		if (ISDEBUG) { std::cout << "AND" << "\t" << "#" << (int)value; }
		setZeroAndNegativeFlags(mAccumulator);
	}

	void andWithMemoryOrAccZeroP()
	{
		uint8_t addr = fetch();
		uint8_t value = mMemory[addr];
		mAccumulator = value & mAccumulator;
		if (ISDEBUG) { std::cout << "AND" << "\t" << std::hex << std::setw(2) << std::setfill('0') << addr; }
		setZeroAndNegativeFlags(mAccumulator);
	}

	void andWithMemoryOrAccZeroPX()
	{
		uint8_t addr = fetch();
		uint8_t value = mMemory[addr + mRegisterX];
		mAccumulator = value & mAccumulator;
		if (ISDEBUG) { std::cout << "AND" << "\t" << std::hex << std::setw(2) << std::setfill('0') << addr << ",x"; }
		setZeroAndNegativeFlags(mAccumulator);
	}

	void andWithMemoryOrAccAbs()
	{
		uint16_t addr = fetch16();
		uint8_t value = mMemory[addr];
		mAccumulator = value & mAccumulator;
		if (ISDEBUG) { std::cout << "AND" << "\t" << std::hex << std::setw(4) << std::setfill('0') << addr; }
		setZeroAndNegativeFlags(mAccumulator);
	}

	void andWithMemoryOrAccAbsX()
	{
		uint16_t addr = fetch16();
		uint8_t value = mMemory[addr + mRegisterX];
		mAccumulator = value & mAccumulator;
		if (ISDEBUG) { std::cout << "AND" << "\t" << std::hex << std::setw(4) << std::setfill('0') << addr << ",x"; }
		setZeroAndNegativeFlags(mAccumulator);
	}

	void andWithMemoryOrAccAbsY()
	{
		uint16_t addr = fetch16();
		uint8_t value = mMemory[addr + mRegisterY];
		mAccumulator = value & mAccumulator;
		if (ISDEBUG) { std::cout << "AND" << "\t" << std::hex << std::setw(4) << std::setfill('0') << addr << ",y"; }
		setZeroAndNegativeFlags(mAccumulator);
	}

	void andWithMemoryOrAccIndX()
	{
		uint8_t lookupaddress = fetch() + mRegisterX;
		uint8_t value = mMemory[mMemory[lookupaddress] + (mMemory[lookupaddress + 1] << 8)];
		mAccumulator = value & mAccumulator;
		if (ISDEBUG) { std::cout << "AND" << "\t" << "(" << std::hex << std::setw(4) << std::setfill('0') << lookupaddress << ",x)"; }
		setZeroAndNegativeFlags(mAccumulator);
	}

	void andWithMemoryOrAccIndY()
	{
		uint8_t lookupaddress = fetch();
		uint8_t value = mMemory[(mMemory[lookupaddress] + (mMemory[lookupaddress + 1] << 8)) + mRegisterY];
		mAccumulator = value & mAccumulator;
		if (ISDEBUG) { std::cout << "AND" << "\t" << "(" << std::hex << std::setw(4) << std::setfill('0') << lookupaddress << "),y"; }
		setZeroAndNegativeFlags(mAccumulator);
	}

	//ADC

	void adcWithMemoryOrAccImmediate()
	{
		uint8_t value = fetch();
		mAccumulator = add(value, mAccumulator, C, D);
		if (ISDEBUG) { std::cout << "ADC" << "\t" << "#" << (int)value; }
	}

	void adcWithMemoryOrAccZeroP()
	{
		uint8_t addr = fetch();
		uint8_t value = mMemory[addr];
		mAccumulator = add(value, mAccumulator, C, D);
		if (ISDEBUG) { std::cout << "ADC" << "\t" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(addr); }
	}

	void adcWithMemoryOrAccZeroPX()
	{
		uint8_t addr = fetch();
		uint8_t value = mMemory[addr + mRegisterX];
		mAccumulator = add(value, mAccumulator, C, D);
		if (ISDEBUG) { std::cout << "ADC" << "\t" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(addr) << ",x"; }
	}

	void adcWithMemoryOrAccAbs()
	{
		uint16_t addr = fetch16();
		uint8_t value = mMemory[addr];
		mAccumulator = add(value, mAccumulator, C, D);
		if (ISDEBUG) { std::cout << "ADC" << "\t" << std::hex << std::setw(4) << std::setfill('0') << static_cast<int>(addr); }
	}

	void adcWithMemoryOrAccAbsX()
	{
		uint16_t addr = fetch16();
		uint8_t value = mMemory[addr + mRegisterX];
		mAccumulator = add(value, mAccumulator, C, D);
		if (ISDEBUG) { std::cout << "ADC" << "\t" << std::hex << std::setw(4) << std::setfill('0') << static_cast<int>(addr) << ",x"; }
	}

	void adcWithMemoryOrAccAbsY()
	{
		uint16_t addr = fetch16();
		uint8_t value = mMemory[addr + mRegisterY];
		mAccumulator = add(value, mAccumulator, C, D);
		if (ISDEBUG) { std::cout << "ADC" << "\t" << std::hex << std::setw(4) << std::setfill('0') << static_cast<int>(addr) << ",y"; }
	}

	void adcWithMemoryOrAccIndX()
	{
		uint8_t lookupaddress = fetch() + mRegisterX;
		uint8_t value = mMemory[mMemory[lookupaddress] + (mMemory[lookupaddress + 1] << 8)];
		mAccumulator = add(value, mAccumulator, C, D);
		if (ISDEBUG) { std::cout << "ADC" << "\t" << "(" << std::hex << std::setw(4) << std::setfill('0') << static_cast<int>(lookupaddress) << ",x)"; }
	}

	void adcWithMemoryOrAccIndY()
	{
		uint8_t lookupaddress = fetch();
		uint8_t value = mMemory[(mMemory[lookupaddress] + (mMemory[lookupaddress + 1] << 8)) + mRegisterY];
		mAccumulator = add(value, mAccumulator, C, D);
		if (ISDEBUG) { std::cout << "ADC" << "\t" << "(" << std::hex << std::setw(4) << std::setfill('0') << static_cast<int>(lookupaddress) << "),y"; }
	}

	//SBC

	void sbcWithMemoryOrAccImmediate()
	{
		uint8_t value = fetch();
		mAccumulator = sub(mAccumulator, value, C, D);
		if (ISDEBUG) { std::cout << "SBC" << "\t" << "#" << "$" << std::hex << std::setw(2) << std::setfill('0') << (uint16_t)value; }
	}

	void sbcWithMemoryOrAccZeroP()
	{
		uint8_t addr = fetch();
		uint8_t value = mMemory[addr];
		mAccumulator = sub(mAccumulator, value, C, D);
		if (ISDEBUG) { std::cout << "SBC" << "\t" << "$" << std::hex << std::setw(2) << std::setfill('0') << (uint16_t)addr; }
	}

	void sbcWithMemoryOrAccZeroPX()
	{
		uint8_t addr = fetch();
		uint8_t value = mMemory[addr + mRegisterX];
		mAccumulator = add(mAccumulator, value, C, D);
		if (ISDEBUG) { std::cout << "SBC" << "\t" << "$" << std::hex << std::setw(2) << std::setfill('0') << (uint16_t)addr << ",x"; }
	}

	void sbcWithMemoryOrAccAbs()
	{
		uint16_t addr = fetch16();
		uint8_t value = mMemory[addr];
		mAccumulator = sub(mAccumulator, value, C, D);
		if (ISDEBUG) { std::cout << "SBC" << "\t" << "$" << std::hex << std::setw(4) << std::setfill('0') << addr; }
	}

	void sbcWithMemoryOrAccAbsX()
	{
		uint16_t addr = fetch16();
		uint8_t value = mMemory[addr + mRegisterX];
		mAccumulator = sub(mAccumulator, value, C, D);
		if (ISDEBUG) { std::cout << "SBC" << "\t" << "$" << std::hex << std::setw(4) << std::setfill('0') << addr << ",x"; }
	}

	void sbcWithMemoryOrAccAbsY()
	{
		uint16_t addr = fetch16();
		uint8_t value = mMemory[addr + mRegisterY];
		mAccumulator = sub(mAccumulator, value, C, D);
		if (ISDEBUG) { std::cout << "SBC" << "\t" << "$" << std::hex << std::setw(4) << std::setfill('0') << addr << ",y"; }
	}

	void sbcWithMemoryOrAccIndX()
	{
		uint8_t lookupaddress = fetch() + mRegisterX;
		uint8_t value = mMemory[mMemory[lookupaddress] + (mMemory[lookupaddress + 1] << 8)];
		mAccumulator = sub(mAccumulator, value, C, D);
		if (ISDEBUG) { std::cout << "SBC" << "\t" << "(" << "$" << std::hex << std::setw(4) << std::setfill('0') << (uint16_t)lookupaddress << ",x)"; }
	}

	void sbcWithMemoryOrAccIndY()
	{
		uint8_t lookupaddress = fetch();
		uint8_t value = mMemory[(mMemory[lookupaddress] + (mMemory[lookupaddress + 1] << 8)) + mRegisterY];
		mAccumulator = sub(mAccumulator, value, C, D);
		if (ISDEBUG) { std::cout << "SBC" << "\t" << "(" << "$" << std::hex << std::setw(4) << std::setfill('0') << (uint16_t)lookupaddress << "),y"; }
	}

	//"VAL" IN ALL COMPARE OPERATIONS IS VALUE OF THE CHOSEN REGISTER AND IS NOT THE VALUE IT IS BEING COMPARED WITH.
	//VALUE THAT IT IS BEING COMPARED TO IS NAMED "VALUE" IN CODE



	void compareImmediate(const char* instruction, uint8_t val)
	{
		uint8_t value = fetch();
		if (ISDEBUG) { std::cout << instruction << "\t" << "#" << "$" << std::hex << std::setw(2) << std::setfill('0') << (uint16_t)value; }
		compareBase(val, value);
	}

	void compareAbsolute(const char* instruction, uint8_t val)
	{
		uint16_t addr = fetch16();
		uint8_t value = mMemory[addr];
		if (ISDEBUG) { std::cout << instruction << "\t" << "$" << std::hex << std::setw(4) << std::setfill('0') << addr; }
		compareBase(val, value);
	}

	void compareZeroPage(const char* instruction, uint8_t val)
	{
		uint8_t addr = fetch();
		uint8_t value = mMemory[addr];
		if (ISDEBUG) { std::cout << instruction << "\t" << "$" << std::hex << std::setw(2) << std::setfill('0') << (uint16_t)addr; }
		compareBase(val, value);
	}

	void compareZeroPageX(const char* instruction, uint8_t val)
	{
		uint8_t addr = fetch();
		uint8_t value = mMemory[addr + mRegisterX];
		if (ISDEBUG) { std::cout << instruction << "\t" << "$" << std::hex << std::setw(2) << std::setfill('0') << (uint16_t)addr << ",x"; }
		compareBase(val, value);
	}

	void compareAbsoluteX(const char* instruction, uint8_t val)
	{
		uint16_t addr = fetch16();
		uint8_t value = mMemory[addr + mRegisterX];
		if (ISDEBUG) { std::cout << instruction << "\t" << "$" << std::hex << std::setw(4) << std::setfill('0') << addr << ",x"; }
		compareBase(val, value);
	}

	void compareAbsoluteY(const char* instruction, uint8_t val)
	{
		uint16_t addr = fetch16();
		uint8_t value = mMemory[addr + mRegisterY];
		if (ISDEBUG) { std::cout << instruction << "\t" << "$" << std::hex << std::setw(4) << std::setfill('0') << addr << ",y"; }
		compareBase(val, value);
	}

	void compareIndY(const char* instruction, uint8_t val)
	{
		uint8_t lookupaddress = fetch();

		//uint16_t addr = (mMemory[lookupaddress] + mMemory[lookupaddress + 1] << 8) + mRegisterY;
		uint8_t value = mMemory[(mMemory[lookupaddress] + (mMemory[lookupaddress + 1] << 8)) + mRegisterY];
		if (ISDEBUG) { std::cout << instruction << "\t" << "(" << "$" << std::hex << std::setw(4) << std::setfill('0') << (uint16_t)lookupaddress << "),y"; }
		compareBase(val, value);
	}

	void compareIndX(const char* instruction, uint8_t val)
	{
		uint8_t lookupaddress = fetch() + mRegisterX;

		//uint16_t addr = (mMemory[lookupaddress + mRegisterX] + mMemory[lookupaddress + mRegisterX + 1] << 8);
		uint8_t value = mMemory[mMemory[lookupaddress] + (mMemory[lookupaddress + 1] << 8)];
		if (ISDEBUG) { std::cout << instruction << "\t" << "(" << "$" << std::hex << std::setw(4) << std::setfill('0') << (uint16_t)lookupaddress << ",x)"; }
		compareBase(val, value);
	}




	void bitTestAbsolute()
	{
		uint16_t addr = fetch16();
		uint8_t value = mMemory[addr];
		N = (value & 0x80) != 0;
		V = (value & 0x40) != 0;
		Z = (value & mAccumulator) == 0;
		if (ISDEBUG) { std::cout << "BIT" << "\t" << std::hex << std::setw(4) << std::setfill('0') << addr; }
	}

	//THIS is never used, delete or change code
	void bitTestZeroPage()
	{
		uint8_t addr = fetch();
		uint8_t value = mMemory[addr];
		N = (value & 0x80) != 0;
		V = (value & 0x40) != 0;
		Z = (value & mAccumulator) == 0;
		if (ISDEBUG) { std::cout << "BIT" << "\t" << std::hex << std::setw(2) << std::setfill('0') << addr; }
	}



	void branchNonZero()
	{
		int8_t fetchedByte = branchBase("BNE");
		if (Z == 0)
		{
			mProgramCounter += fetchedByte;
			branchDebugPrint("BNE", fetchedByte);
		}
	}

	void branchCarrySet()
	{
		int8_t fetchedByte = branchBase("BCS");
		if (C == 1)
		{
			mProgramCounter += fetchedByte;
			branchDebugPrint("BCS", fetchedByte);
		}
	}

	void branchCarryClear()
	{
		int8_t fetchedByte = branchBase("BCC");
		if (C == 0)
		{
			mProgramCounter += fetchedByte;
			branchDebugPrint("BCC", fetchedByte);
		}
	}

	void branchZero()
	{
		int8_t fetchedByte = branchBase("BEQ");
		if (Z == 1)
		{
			mProgramCounter += fetchedByte;
			branchDebugPrint("BEQ", fetchedByte);
		}
	}

	void branchMinus()
	{
		int8_t fetchedByte = branchBase("BMI");
		if (N == 1)
		{
			mProgramCounter += fetchedByte;
			branchDebugPrint("BMI", fetchedByte);
		}
	}

	void branchPlus()
	{
		int8_t fetchedByte = branchBase("BPL");
		if (N == 0)
		{
			mProgramCounter += fetchedByte;
			branchDebugPrint("BPL", fetchedByte);
		}
	}

	void branchOverflowClear()
	{
		int8_t fetchedByte = branchBase("BVC");
		if (V == 0)
		{
			mProgramCounter += fetchedByte;
			branchDebugPrint("BVC", fetchedByte);
		}
	}

	void branchOverflowSet()
	{
		int8_t fetchedByte = branchBase("BVS");
		if (V == 1)
		{
			mProgramCounter += fetchedByte;
			branchDebugPrint("BVS", fetchedByte);
		}
	}

	int8_t branchBase(char const* instruction)
	{
		if (ISDEBUG) { std::cout << instruction; }
		int8_t offset = fetch();
		if (ISDEBUG) { std::cout << "\t" << (uint16_t)(mProgramCounter + offset); }
		return offset;
	}

	void branchDebugPrint(const char* instruction, int8_t fetchedByte)
	{
		if (ISDEBUG)
		{
			std::cout << ";+";
		}
	}

	uint16_t jumpAbsolute()
	{
		//std::cout << "JMP (absolute) started, PC: " << mProgramCounter << std::endl;

		uint16_t jumpAddress = fetch16();

		//std::cout << "New Address: " << jumpAddress << std::endl;
		if (ISDEBUG) { std::cout << "JMP" << "\t" << "#" << jumpAddress; }
		return jumpAddress;
	}

	uint16_t jumpAbsoluteSavingAddress()
	{
		//std::cout << "JMP (absolute) started, PC: " << mProgramCounter << std::endl;

		uint16_t jumpAddress = fetch16();
		uint16_t savedPosition = mProgramCounter - 1; // address of end of current instruction
		mMemory[stackOffset + mStackPointer] = ((savedPosition >> 8) & 0xFF);
		mStackPointer--;
		mMemory[stackOffset + mStackPointer] = savedPosition & 0xFF;
		mStackPointer--;
		//std::cout << "New Address: " << jumpAddress << std::endl;
		if (ISDEBUG) { std::cout << "JSR" << "\t" << "#" << jumpAddress; }
		return jumpAddress;
	}

	uint16_t jumpIndirect()
	{
		//std::cout << "JMP (indirect) started, PC: " << mProgramCounter << std::endl;

		uint16_t lookupAddress = fetch16();
		mProgramCounter = lookupAddress;
		//std::cout << "Lookup Address: " << lookupAddress << std::endl;

		uint16_t jumpAddress = fetch16();
		//std::cout << "New Address: " << jumpAddress << std::endl;
		if (ISDEBUG) { std::cout << "JMP" << "\t" << "(" << "$" << std::hex << std::setw(4) << std::setfill('0') << lookupAddress << ")"; }
		return jumpAddress;
	}

	uint8_t loadIndirectX(const char* instruction)
	{
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

	uint8_t loadZeroPage(const char* instruction)
	{
		uint8_t addr = fetch();
		uint8_t result = mMemory[addr];
		if (ISDEBUG) { std::cout << instruction << "\t" << (int)addr; }
		//std::cout << "Loading " << std::hex << static_cast<int>(result) << " into A" << std::endl;
		return result;
	}

	uint8_t loadImmediate(const char* instruction)
	{
		uint8_t result = fetch();
		if (ISDEBUG) { std::cout << instruction << "\t" << "#" << (int)result; }
		//std::cout << "Loading " << std::hex << static_cast<int>(result) << " into A" << std::endl;
		return result;
	}

	uint8_t loadAbsolute(const char* instruction)
	{
		uint16_t addr = fetch16();
		uint8_t result = mMemory[addr];
		if (ISDEBUG) { std::cout << instruction << "\t" << (int)addr; }
		//std::cout << "Loading " << std::hex << static_cast<int>(result) << " into A" << std::endl;
		return result;
	}

	uint8_t loadIndirectY(const char* instruction)
	{
		uint8_t lookupAddress = fetch();
		if (ISDEBUG) { std::cout << instruction << "\t" << "(" << (int)lookupAddress << "),y"; }
		//std::cout << "Lookup address: " << std::hex << static_cast<int>(lookupAddress) << ", y: " << std::hex << static_cast<int>(mRegisterY) << std::endl;

		uint16_t address = (mMemory[lookupAddress] + (mMemory[lookupAddress + 1] << 8)) + mRegisterY;
		//std::cout << "Address: " << std::hex << static_cast<int>(address) << std::endl;

		uint8_t result = mMemory[address];
		//std::cout << "Loading " << std::hex << static_cast<int>(result) << " into A" << std::endl;
		return result;
	}

	uint8_t loadZeroPageX(const char* instruction)
	{
		uint8_t base = fetch();
		uint8_t	address = mRegisterX + base;
		uint8_t result = mMemory[address];
		if (ISDEBUG) { std::cout << instruction << "\t" << (int)base << ",x"; }
		//std::cout << "LDA " << std::hex << static_cast<int>(result) << " into A, address: " << std::hex << static_cast<int>(address) << std::endl;
		return result;
	}

	uint8_t loadZeroPageY(const char* instruction)
	{
		uint8_t base = fetch();
		uint8_t	address = mRegisterY + base;
		uint8_t result = mMemory[address];
		if (ISDEBUG) { std::cout << instruction << "\t" << (int)base << ",y"; }
		//std::cout << "LDA " << std::hex << static_cast<int>(result) << " into A, address: " << std::hex << static_cast<int>(address) << std::endl;
		return result;
	}

	uint8_t loadAbsoluteY(const char* instruction)
	{
		uint16_t base = fetch16();
		uint8_t result = mMemory[base + mRegisterY];
		if (ISDEBUG) { std::cout << instruction << "\t" << (int)base << ",y"; }
		//std::cout << "Loading " << std::hex << static_cast<int>(result) << " into A" << std::endl;
		return result;
	}

	uint8_t loadAbsoluteX(const char* instruction)
	{
		uint16_t base = fetch16();
		uint8_t result = mMemory[base + mRegisterX];
		if (ISDEBUG) { std::cout << instruction << "\t" << (int)base << ",x"; }
		//std::cout << "Loading " << std::hex << static_cast<int>(result) << " into A" << std::endl;
		return result;
	}



	//save instructions 

	void saveIndirectY(const char* instruction, uint8_t value)
	{
		uint8_t lookupAddress = fetch();
		if (ISDEBUG) { std::cout << instruction << "\t" << "(" << (int)lookupAddress << "),y"; }

		uint16_t address = (mMemory[lookupAddress] + (mMemory[lookupAddress + 1] << 8)) + mRegisterY;

		mMemory[address] = value;
	}

	void saveIndirectX(const char* instruction, uint8_t value)
	{
		uint8_t lookupAddress = fetch();

		if (ISDEBUG) { std::cout << instruction << "\t" << "(" << (int)lookupAddress << ",x)"; }
		lookupAddress += mRegisterX;
		uint16_t address = mMemory[lookupAddress] + (mMemory[lookupAddress + 1] << 8);
		mMemory[address] = value;
	}

	void saveZeroPage(const char* instruction, uint8_t value)
	{
		uint8_t addr = fetch();
		mMemory[addr] = value;
		if (ISDEBUG) { std::cout << instruction << "\t" << (int)addr; }
	}

	void saveAbsolute(const char* instruction, uint8_t value)
	{
		uint16_t addr = fetch16();
		mMemory[addr] = value;
		if (ISDEBUG) { std::cout << instruction << "\t" << (int)addr; }
	}

	void saveZeroPageX(const char* instruction, uint8_t value)
	{
		uint8_t base = fetch();
		uint8_t	address = mRegisterX + base;
		mMemory[address] = value;
		if (ISDEBUG) { std::cout << instruction << "\t" << (int)base << ",x"; }
	}

	void saveZeroPageY(const char* instruction, uint8_t value)
	{
		uint8_t base = fetch();
		uint8_t	address = mRegisterY + base;
		mMemory[address] = value;
		if (ISDEBUG) { std::cout << instruction << "\t" << (int)base << ",y"; }
	}

	void saveAbsoluteY(const char* instruction, uint8_t value)
	{
		uint16_t base = fetch16();
		mMemory[base + mRegisterY] = value;
		if (ISDEBUG) { std::cout << instruction << "\t" << (int)base << ",y"; }
	}

	void saveAbsoluteX(const char* instruction, uint8_t value)
	{
		uint16_t base = fetch16();
		mMemory[base + mRegisterX] = value;
		if (ISDEBUG) { std::cout << instruction << "\t" << (int)base << ",x"; }
	}

	void setZeroAndNegativeFlags(uint8_t value)
	{
		Z = value == 0;
		N = (value & 0x80) != 0;
	}
};

class MOS6502Debug : public MOS6502
{
public:
	uint8_t  getAccumulator() { return mAccumulator; }
	uint16_t getProgramCounter() { return mProgramCounter; }
	uint8_t  getStackPointer() { return mStackPointer; }
	uint8_t  getRegisterX() { return mRegisterX; }
	uint8_t  getRegisterY() { return mRegisterY; }

	uint8_t  getMemory(uint16_t addr) { return mMemory[addr]; }
};

static bool TestBasicOps()
{
	bool isOk = true;

	uint8_t program[] = {
	 0xA9, 0x83, 0x8D, 0x01, 0x00, 0xA9, 0x05, 0x8D,
	 0x02, 0x00, 0xA9, 0x81, 0x8D, 0x03, 0x00, 0xA9,
	 0x73, 0x8D, 0x04, 0x00, 0x18, 0x20, 0x44, 0x10,
	 0xFF, 0xA9, 0xFA, 0x8D, 0x01, 0x00, 0xA9, 0x03, //first was 0xea but changed to 0xff (halt)
	 0x8D, 0x02, 0x00, 0x20, 0x51, 0x10, 0xFF, 0xA9, // seventh was ea but is now ff 
	 0x07, 0x8D, 0x01, 0x00, 0xA9, 0x06, 0x8D, 0x02,
	 0x00, 0x20, 0x74, 0x10, 0xEA, 0xA9, 0x0A, 0x8D,
	 0x01, 0x00, 0xA9, 0x05, 0x8D, 0x02, 0x00, 0x20,
	 0x91, 0x10, 0xFF, 0x00, 0xA5, 0x01, 0x65, 0x03, // third was EA, now is 0xff
	 0x85, 0x05, 0xA5, 0x02, 0x65, 0x04, 0x85, 0x06,
	 0x60, 0xA9, 0x00, 0x85, 0x03, 0x85, 0x04, 0xA5,
	 0x01, 0xC9, 0x00, 0xF0, 0x16, 0xA5, 0x02, 0xC9,
	 0x00, 0xF0, 0x10, 0xC6, 0x02, 0xA5, 0x03, 0x18,
	 0x65, 0x01, 0x85, 0x03, 0x90, 0xEF, 0xE6, 0x04,
	 0x4C, 0x5D, 0x10, 0x60, 0xA9, 0x00, 0x85, 0x03,
	 0xA5, 0x01, 0xC9, 0x00, 0xF0, 0x12, 0xA5, 0x02,
	 0xC9, 0x00, 0xF0, 0x0C, 0xC6, 0x02, 0xA5, 0x03,
	 0x18, 0x65, 0x01, 0x85, 0x03, 0x4C, 0x7E, 0x10,
	 0x60, 0xA9, 0x00, 0x85, 0x03, 0xA5, 0x01, 0x85,
	 0x04, 0xC5, 0x02, 0x30, 0x10, 0xE6, 0x03, 0xA5,
	 0x04, 0x38, 0xE5, 0x02, 0x85, 0x04, 0xC5, 0x02,
	 0x30, 0x03, 0x4C, 0x9D, 0x10, 0x60, 0xFF
	};

	auto cpu = std::make_shared<MOS6502Debug>();
	cpu->loadProgram(program, sizeof(program), 0x1000);

	if (isOk)
	{
		// ; test ADC_XY16
		cpu->executeFrom(0x1000);

		if (cpu->getMemory(0x05) != 0x04)
		{
			isOk = false;
		}

		if (cpu->getMemory(0x06) != 0x79)
		{
			isOk = false;
		}

		std::cout << "Test ADC_XY16:" << ((isOk) ? "OK" : "FAIL") << "\n";
	}

	if (isOk)
	{
		cpu->executeFrom(0x1019);

		if (cpu->getMemory(0x03) != 0xEE)
		{
			isOk = false;
		}

		if (cpu->getMemory(0x04) != 0x02)
		{
			isOk = false;
		}

		std::cout << "Test MUL_XY16:" << ((isOk) ? "OK" : "FAIL") << "\n";
	}

	if (isOk)
	{
		cpu->executeFrom(0x1035);

		if (cpu->getMemory(0x03) != 0x02)
		{
			isOk = false;
		}

		if (cpu->getMemory(0x04) != 0x00)
		{
			isOk = false;
		}

		std::cout << "Test DIV_XY:" << ((isOk) ? "OK" : "FAIL") << "\n";
	}

	return(isOk);
}

int main()
{

	TestBasicOps();

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