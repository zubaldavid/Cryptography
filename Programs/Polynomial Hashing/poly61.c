#include <string.h>
#include <stdio.h>
#include <stdint.h>

uint64_t poly61(unsigned char m[], unsigned mbytes, uint64_t k);


int main() {
	uint64_t k1 = (1ULL << 61) - 129;
	uint64_t k2 = 0x0fedcba987654321ULL;
	unsigned char m1[] = {1,2,3,4};
	unsigned char m2[] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16};
	printf("Test 1: %d\n", poly61(m1, 0, k1)==0x0ULL);
	printf("Test 2: %d\n", poly61(m1, 3, k1)==0x80030181ULL);
	printf("Test 3: %d\n", poly61(m1, 4, k1)==0x1ffffffdfe7f3fffULL);
	printf("Test 4: %d\n", poly61(m2, 15, k2)==0x1b16a17f8138ae4cULL);
	printf("Test 5: %d\n", poly61(m2, 16, k2)==0x11defe0c9db2a7aeULL);

}

void printString(unsigned char *m, unsigned bytes ){
	int i;
	for(i = 0; i < 4; i++){
		printf("%0.2x ", m[i]);
	}
	printf("\n");
}

// Implement polynomial hashing using Horner’s method and divisionless mod
__uint64_t poly61(unsigned char *m, unsigned mbytes, __uint64_t k) {
	int i = 0, j, a;
	__int128_t lo, hi;
	__int128_t result128 = 1;
	unsigned char paddedArr[4] = {0}, remain = 0;
	unsigned temp = mbytes;
	unsigned mod  = mbytes % 4;

	if(mbytes == 0) {
		return (__uint64_t)0;
	}

	// Horners Method
	while(i + 4 <= mbytes) {
	unsigned nibble = 0;

		for(j = i; j <= i+3; j++) {
			nibble += m[j] << (8*(j-i));			//Builds up 32 bit
		}

		result128 *= k; 							// Accumulator is 61 bits.
		result128 += nibble;						// Padded message to 32 bits.
		lo = result128 & ((1ULL << 61) -1);			// Find the lower bound off divisionless mod
		hi = result128 >> 61; 						// Find the higher bound divisionless mod
		result128 = hi + lo;						// Add the bounds together
		//printf("1st - Result[%d]: %x\n",j ,result128);
		i+=4;										// Increment each time you run through
	}

	memcpy(paddedArr,m+i,mod);						// Copy the last bytes to a buffer
	paddedArr[mod] = 0x80; 							// Introduce the '1' padding to the 4 bytes input

	unsigned paddedNibble = 0;
	for(a = 0; a <= 3; a++) {						// Run the last few values through the same flow
		paddedNibble += paddedArr[a] << (8*a);
		printf("Pad nibble: %0.8x\n", paddedNibble);
	}

	result128 *= k;
	result128 += paddedNibble;
	lo = result128 & ((1ULL << 61) -1 );
	hi = result128 >> 61;
	result128 = hi + lo;

	// Extra divionless mod if value bigger than 64 bits
	int64_t x = result128;
	int64_t p = 2^61 - 1;
	int64_t t = x-p;        	// negative if x<p, non-negative if x>=p
	int64_t mask = t >> 63;  	// arithmetic shift: all 1 if x<p, all 0 if x>=p
	x = (mask & x) | (~mask & t);

	return (__uint64_t)result128;
}
