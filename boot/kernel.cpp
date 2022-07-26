#include "TeletypeVideoBuffer.hpp"

void initializeIDT();
extern "C" void kernel_start()
{
	initializeIDT();
	TeletypeVideoBuffer::clear();
	/*int a = 5;
	int b = 0;
	int c = a / b;
	TeletypeVideoBuffer::printf("%l", c);*/
	//TeletypeVideoBuffer::printf("Test %% %i %c %s", 0, 'z', "asd");
}