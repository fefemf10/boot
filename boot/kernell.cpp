#include "TeletypeVideoBuffer.hpp"
#include "Framebuffer.hpp"
import vec;
void initializeIDT();
extern "C" void kernel_start()
{
	initializeIDT();
	TeletypeVideoBuffer::clear();
	u8vec4 p = u8vec4(5, 5, 5, 5);
	TeletypeVideoBuffer::printf("%lx", *reinterpret_cast<u32*>(&p));
}