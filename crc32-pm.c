#include <stdio.h>
#include <stdlib.h>

unsigned reverse(unsigned x) {
  x = ((x & 0x55555555) <<  1) | ((x >>  1) & 0x55555555);
  x = ((x & 0x33333333) <<  2) | ((x >>  2) & 0x33333333);
  x = ((x & 0x0F0F0F0F) <<  4) | ((x >>  4) & 0x0F0F0F0F);
  x = (x << 24) | ((x & 0xFF00) << 8) |
    ((x >> 8) & 0xFF00) | (x >> 24);
  return x;
}

unsigned int fuzzification_crc32(unsigned char *message) {
  int i, j;
  unsigned int byte, crc;

  i = 0;
  crc = 0xFFFFFFFF;
  while (message[i] != 0) {
    byte = message[i];            // Get next byte.
    byte = reverse(byte);         // 32-bit reversal.
    for (j = 0; j <= 7; j++) {    // Do eight times.
      if ((int)(crc ^ byte) < 0)
	crc = (crc << 1) ^ 0x04C11DB7;
      else crc = crc << 1;
      byte = byte << 1;          // Ready next msg bit.
    }
    i = i + 1;
  }
  return reverse(~crc);
}


//char global_char0;
char global_str[LEN];
int main() {
  //global_str[0] = global_char0;
  global_str[LEN-1] = '\0';
  unsigned int crc = fuzzification_crc32(global_str);
  char mystr[LEN];
  srand(time(NULL));   // Initialization, should only be called once.
  for (int i = 0; i < LEN; i++) mystr[i] = (char) rand(); // Returns a pseudo-random integer between 0 and RAND_MAX.
  if (crc == fuzzification_crc32(mystr)) {
    printf("found a bug\n");
  }
  return 0;
}
