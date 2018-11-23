#include "Xor.h"

#define KEY_SIZE    (24)

static unsigned char g_key_map[KEY_SIZE] = {
	 0xE3, 0x82, 0x49, 0x42, 0x39, 0x32, 0x55, 0x88,
	 0x99, 0x25, 0x38, 0x58, 0x41, 0x58, 0x44, 0x11,
	 0x27, 0x83, 0x48, 0x36, 0x36, 0x28, 0x33, 0x22};
			
void XorEncode(unsigned char* buffer, int len)
{
	for (int i = 0; i < len; i++) {
		buffer[i] -= g_key_map[i % KEY_SIZE];
	}
}

void XorDecode(unsigned char* buffer, int len)
{
	for (int i = 0; i < len; i++) {
		buffer[i] += g_key_map[i % KEY_SIZE];
	}
}
