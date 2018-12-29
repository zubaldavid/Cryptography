// David Zubal
// P52 - 48 Byte Permutation

#include <stdio.h>
#include <string.h>

void P52(unsigned s[12]);

int main() {
    unsigned char buf[48] = {0};
    unsigned char expected[48] = {
        0xC4,0xD8,0x67,0x64,0x3B,0xF8,0xDC,0x07,0xD4,0xB0,0x0B,0x3B,0x4C,0x36,
        0x21,0x1B,0xDC,0x31,0x34,0x08,0x8E,0xBE,0xFB,0x0E,0x84,0xE8,0x54,0x00,
        0x55,0xD9,0x8B,0x64,0x2E,0xB4,0x5D,0x4A,0xCB,0x41,0x06,0xCA,0xC2,0xD2,
        0x73,0x86,0x09,0xD8,0x30,0x2E};
    P52((unsigned *)buf);
    if (memcmp(buf,expected,48)==0)
        printf("Pass\n");
    else
        printf("No Pass\n");
    return 0;
}
// Bits that fall off at left end are put back at right end
unsigned leftRotate(unsigned value, unsigned int shift) {
  return (value << shift) | (value >> (32 - shift));
}

void P52(unsigned s[12]) {
 
  int i,j;
  unsigned x,y,z;

  for (i = 0; i < 24; i++ ) {
    for (j = 0; j < 4; j++ ) {
     
     x = leftRotate(s[j], 24);
      y = leftRotate(s[j + 4],9);
      z = s[j + 8];

      s[j + 8] =  x ^ (z << 1) ^ ((y & z) << 2);
      s[j + 4] =  y ^ x ^ ((x | z) << 1);
      s[j]     =  z ^ y ^ ((x & y) << 3);
    
    }

    if(i % 4 == 0) {
      int k;
      unsigned array[] = {s[1],s[0],s[3],s[2]};

      for(k = 0; k < 4; k++) 
        s[k] = array[k];
      
      
      s[0] = s[0] ^ (0x9e377900 ^ (24 - i));
    }

    else if(i % 4 == 2) {
      int l;
      unsigned index[] = {s[2],s[3],s[0],s[1]};
      
      for(l = 0; l < 4; l++) {
        s[l] = index[l];
      }
    }
  }
}
