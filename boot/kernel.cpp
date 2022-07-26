#include "TeletypeVideoBuffer.hpp"

void initializeIDT();
extern "C" void kernel_start()
{
	initializeIDT();
	TeletypeVideoBuffer::clear();
}