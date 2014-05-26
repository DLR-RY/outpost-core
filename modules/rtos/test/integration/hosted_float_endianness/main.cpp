
#include <stdio.h>
#include <stdint.h>

#include <inttypes.h>

int
main(void)
{
	printf("Endianness Test\n\n");

	uint32_t i = 133124;
	printf("Test integer: %d\n", i);

	const uint8_t* ptr = (const uint8_t *) &i;

	printf("byte array  : %02X:%02X:%02X:%02X\n", ptr[0], ptr[1], ptr[2], ptr[3]);

	if (ptr[0] == 0x00 && ptr[1] == 0x02 && ptr[2] == 0x08 && ptr[3] == 0x04) {
		printf("-> Big endian\n");
	}
	else if (ptr[0] == 0x04 && ptr[1] == 0x08 && ptr[2] == 0x02 && ptr[3] == 0x00) {
		printf("-> Little endian\n");
	}
	else {
		printf("-> Invalid encoding!\n");
	}
	printf("\n");

	float f = 3.14159;

	printf("Test float  : %f\n", f);
	ptr = (const uint8_t *) &f;

	printf("byte array  : %02X:%02X:%02X:%02X\n", ptr[0], ptr[1], ptr[2], ptr[3]);

	if (ptr[0] == 0x40 && ptr[1] == 0x49 && ptr[2] == 0x0F && ptr[3] == 0xD0) {
		printf("-> Big endian\n");
	}
	else if (ptr[0] == 0xD0 && ptr[1] == 0x0F && ptr[2] == 0x49 && ptr[3] == 0x40) {
		printf("-> Little endian\n");
	}
	else {
		printf("-> Invalid encoding!\n");
	}
	printf("\n");

	return 0;
}
