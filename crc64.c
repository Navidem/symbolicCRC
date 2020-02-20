// taken from xz source code: src/liblzma/check/crc64_small.c at https://git.tukaani.org/?p=xz.git;a=summary

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
///////////////////////////////////////////////////////////////////////////////
//
/// \file       crc64_small.c
/// \brief      CRC64 calculation (size-optimized)
//
//  Author:     Lasse Collin
//
//  This file has been put into the public domain.
//  You can do whatever you want with this file.
//
///////////////////////////////////////////////////////////////////////////////

//#include "check.h"


typedef unsigned long long int	uint64_t;
typedef unsigned char		uint8_t;
static uint64_t crc64_table[256];


static void
crc64_init(void)
{
#  define UINT64_C(c)	c ## ULL
	static const uint64_t poly64 = UINT64_C(0xC96C5795D7870F42);

	for (size_t b = 0; b < 256; ++b) {
		uint64_t r = b;
		for (size_t i = 0; i < 8; ++i) {
			if (r & 1)
				r = (r >> 1) ^ poly64;
			else
				r >>= 1;
		}

		crc64_table[b] = r;
		printf("0x%Lx, ", r);
	}
	printf ("\n");
	return;
}


uint64_t
lzma_crc64(const uint8_t *buf, size_t size, uint64_t crc)
{
	//mythread_once(crc64_init);

	crc = ~crc;

	while (size != 0) {
		crc = crc64_table[*buf++ ^ (crc & 0xFF)] ^ (crc >> 8);
		--size;
	}

	return ~crc;
}

//char global_char0;
char global_str[LEN];
int main() {
  //global_str[0] = global_char0;
  //global_str[LEN] = '\0';
  crc64_init();
  uint64_t crc = lzma_crc64(global_str, LEN, 0xffffffffffffffffL);
  char mystr[LEN];
  srand(time(NULL));   // Initialization, should only be called once.
  for (int i = 0; i < LEN; i++) mystr[i] = (char) rand(); // Returns a pseudo-random integer between 0 and RAND_MAX.
  if (crc == lzma_crc64(mystr, LEN, 0xffffffffffffffffL)) {
    printf("found a bug\n");
  }
  return 0;
}
