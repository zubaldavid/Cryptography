// David Zubal
// #1206
// Counter Mode encryption


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NONCE_LEN 12
#define BLK_LEN 48
#define KEY_LEN 48

void P52(unsigned s[12]);

int main(int argc, const char* argv[]) {
	size_t bytes_read;
	unsigned char key[KEY_LEN] = {0};  /* Auto init to all zeros */
	unsigned char nonce[NONCE_LEN];
	unsigned char blk[BLK_LEN];
	unsigned char counter[BLK_LEN] = {0};
	unsigned char tempBlk[BLK_LEN];
	/* Setup key and nonce. Report command line errors */
	if ((argc == 3) && (strlen(argv[1]) == 1) && (strlen(argv[2]) <= KEY_LEN) &&
					((argv[1][0] == 'e') || (argv[1][0] == 'd'))) {
		if (argv[1][0] == 'e') {
			FILE *rand = fopen("/dev/urandom", "rb");
			fread(nonce, 1, NONCE_LEN, rand);
			fclose(rand);
			fwrite(nonce, 1, NONCE_LEN, stdout);
		} else {
			// We decrypt here
			fread(nonce, 1, NONCE_LEN, stdin);
		}
		memcpy(counter, nonce, 12);
		memcpy(key, argv[2], strlen(argv[2]));
	}

	else {
		fprintf(stderr, "usage: %s d|e <pwd up to 48 chars>\n", argv[0]);
		return EXIT_FAILURE;
	}

	do {
		int i;
		bytes_read = fread(blk, 1, BLK_LEN, stdin);
		if (bytes_read > 0) {
			// process bytes_read bytes

			// Store nonce into temp block
			memcpy(tempBlk,counter,BLK_LEN);

			// Xor key with counter + nonce
			for(i = 0; i < 48; i++){
				tempBlk[i] = tempBlk[i] ^ key[i];
			}

			P52((unsigned *)tempBlk);

			// Xor with key after P52
			for(i = 0; i < 48; i++){
				tempBlk[i] = tempBlk[i] ^ key[i];
			}

			// Xor key with plaintext
			for(i = 0; i < 48; i++){
				blk[i] = blk[i] ^ tempBlk[i];
			}

			// adding the nonce and counter
			for(i = 47; i > 12; i--){
					counter[i] += 1;
					if(counter[i] == 0)
						continue;
					else
						break;
			}

			fwrite(blk, 1, bytes_read, stdout);

		}
	} while (bytes_read == BLK_LEN);
	return EXIT_SUCCESS;
}

unsigned leftRotate(unsigned value, unsigned int shift){
    return (value << shift) | (value >> (32 - shift));
}

void P52(unsigned s[12]) {

    int i,j;
    unsigned x,y,z;

    for (i = 0; i < 24; i++ ) {
        for (j = 0; j < 4; j++ ) {

            x = leftRotate(s[j], 24);
            y = leftRotate(s[j + 4],9);
            // x = s[j] << 24;
            // y = s[j + 4] << 9;
            z = s[j + 8];

            s[j + 8] =  x ^ (z << 1) ^ ((y & z) << 2);
            s[j + 4] =  y ^ x ^ ((x | z) << 1);
            s[j]     =  z ^ y ^ ((x & y) << 3);

        }

        if(i % 4 == 0) {
            int k;
            unsigned array[] = {s[1],s[0],s[3],s[2]};

            for(k = 0; k < 4; k++) {
                s[k] = array[k];
            }

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
