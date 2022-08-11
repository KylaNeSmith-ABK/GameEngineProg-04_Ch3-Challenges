#include "Utils.h"

void Utils::ClearLine()
{
	std::cout << "\x1b[1A\r";
}
