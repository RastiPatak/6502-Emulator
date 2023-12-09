#include <iostream>

static char registerX;
static char registerY;
static short programCounter;

int main()
{
	unsigned char c[65536];

	c[0] = 0xEA;
	c[1] = 0xEA;
	c[2] = 0xEA;
	c[3] = 0xE8;
	c[4] = 0xEA;
	c[5] = 0xE8;

	bool isValid = true;
	while (true)
	{
		switch (c[programCounter])
		{
		case 0xE8:
			registerX++;
			programCounter++;
			break;
		case 0xEA:
			programCounter++;
			break;
		default:
			isValid = false;
			std::cout << "End of instructions at: " << programCounter << ", value: " << c[programCounter] << std::endl;
			break;
		}
		if (!isValid)
			break;
	}

	std::cout << c[0] << std::endl;
	std::cout << c << std::endl;
	std::cout << "Hello! Yes, this message is utterly pointless, but don't judge me leaving it here." << std::endl;
	//for (const unsigned char& a : c)
	//	std::cout << (a + 0) << ",";
	//std::cout << std::endl;

	std::cout << "Register X: " << (registerX + 0) << std::endl;

	//char b;
	//std::cin >> b;
}