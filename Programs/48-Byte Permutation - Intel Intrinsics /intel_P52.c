#include <stdio.h>
#include <string.h>
#include <emmintrin.h> // SEE2 Intrinsinsic

void P52(unsigned *);

int main(int argc, char *argv[]) {
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

void P52(unsigned s[12]) {

	int i;
	for(i = 0; i < 24; i++) {
		// Rotates by 24 and stores values s[0-4] into register r0
		__m128i r0 = _mm_loadu_si128 ((__m128i *)s);
		r0 =  _mm_or_si128( _mm_slli_epi32(r0, 24), // shifts left 24
				 		    _mm_srli_epi32 (r0, 8)  // shifts right (32-24)
		);
		_mm_store_si128((__m128i *)s,r0);

		// Rotates by 9 and stores values s[4-7] into a register r1
		__m128i r1 = _mm_loadu_si128 ((__m128i *)&s[4]);
		r1 =  _mm_or_si128( _mm_slli_epi32(r1, 9), // shifts left 9
				 		    _mm_srli_epi32 (r1, 23)  // shifts right (32-9)
		);
		_mm_store_si128((__m128i *)&s[4],r1);

		// Stores values s[8-11] into a register r2
		__m128i r2 = _mm_loadu_si128 ((__m128i *)&s[8]);

		// Storing values back into s[8-11]
		// s[8]  = x1 ^ (z1 << 1) ^ ((y1 & z1) << 2);
		__m128i a8 = _mm_loadu_si128 ((__m128i *)&s[8]);
		a8 = _mm_xor_si128 (_mm_xor_si128 (r0, _mm_slli_epi32(r2,1)),
		                  _mm_slli_epi32(_mm_and_si128(r1,r2),2)
		);
		_mm_store_si128((__m128i *)&s[8],a8);

		// Storing values back into s[4-7]
		// s[4] = y1 ^ x1 ^ ((x1 | z1) << 1);
		__m128i a4 = _mm_loadu_si128 ((__m128i *)&s[4]);
		a4 = _mm_xor_si128 (_mm_xor_si128(r1,r0),
							_mm_slli_epi32(_mm_or_si128(r0,r2), 1)
		);
		_mm_store_si128((__m128i *)&s[4],a4);

		// Storing values back into s[4-7]
		// s[0] = z1 ^ y1 ^ ((x1 & y1) << 3);
		__m128i a0 = _mm_loadu_si128 ((__m128i *)&s[0]);
		a0 = _mm_xor_si128 (_mm_xor_si128(r2,r1),
							_mm_slli_epi32(_mm_and_si128(r0,r1), 3)

		);
		_mm_store_si128((__m128i *)s,a0);

	if(i % 4 == 0) {
		__m128i a9 = _mm_loadu_si128 ((__m128i *)s);
		a9 = _mm_shuffle_epi32(a0,_MM_SHUFFLE(2, 3, 0, 1));
		_mm_store_si128((__m128i *)s,a9);

		s[0] = s[0] ^ (0x9e377900 ^ (24 - i));
	}

	else if(i % 4 == 2) {
		__m128i a10 = _mm_loadu_si128 ((__m128i *)s);
		a10 = _mm_shuffle_epi32(a10,_MM_SHUFFLE(1,0,3,2));
		_mm_store_si128((__m128i *)s,a10);
	}
  }
}
