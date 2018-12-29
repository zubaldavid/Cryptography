#include <stdio.h>
#include <string.h>

void P52(unsigned s[12]);
void P52_hash(unsigned char m[], unsigned mbytes, unsigned char res[], unsigned rbytes);

void P52_hash(unsigned char m[], unsigned mbytes, unsigned char res[], unsigned rbytes) {
	int i = 0, j = 0;
	unsigned char bitRate[16] = {0};
	unsigned char state[48] = {0};
	int block = (mbytes / 16 ) + 1;
	int remain = mbytes%16;

	// ABSORB - take message and xor it with state
	while (i < block){ // iterate by block of 16 bits till done.
		// Store message by 16 bits into buffer else store remainder
		if((i+1)*16 <= mbytes) {	memcpy(bitRate, &m[i*16], 16);}
		else{	memcpy(bitRate, &m[i*16],mbytes - (i*16));}
	//	memcpy(bitRate, &m[i*16], ((i+1)*16 <= mbytes )? 16 : mbytes - (i*16) );
		// Apply padding to the last block
		if (i == block - 1){
			bitRate[remain] = 0x80;
			// fill in everything in between with 00's
			for (j = remain + 1; j < 15; j++) {	bitRate[j] = 0x0; }
			bitRate[15] = 0x01;
		}

		// Xor the first 16 bytes of state with bitRate
		for (j = 0; j < 16; j++) { state[j] ^= bitRate[j]; }
		P52((unsigned *)state);
		i += 1; // Iterate to next block
	}

	i = 0;
	// SQUEEZE - store data into res[]
	while (i < rbytes){
		// Send back to result if we have a full block, else the remainder
		if(i + 16 <= rbytes) { memcpy(&res[i], state, 16);}
		else { memcpy(&res[i], state,rbytes-i);}
		//memcpy(&res[i], state, (i + 16 <= rbytes)?16:rbytes-i);
		P52((unsigned *)state);
		i+=16;
	}
}
