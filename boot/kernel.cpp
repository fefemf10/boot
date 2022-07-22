#include "TeletypeVideoBuffer.hpp"

void initializeIDT();
extern "C" void kernel_start()
{
	initializeIDT();
	TeletypeVideoBuffer::clear();
	TeletypeVideoBuffer::printf("Test %% %i %c %s", 0, 'z', "asd");
}