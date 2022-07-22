#include "TeletypeVideoBuffer.hpp"

void initializeIDT();
extern "C" void kernel_start()
{
	initializeIDT();
	TeletypeVideoBuffer::clear();
	TeletypeVideoBuffer::printf("Test %% %s %c", "abc", 'z');
}