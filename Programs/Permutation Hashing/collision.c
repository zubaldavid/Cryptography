#include<stdio.h>
#include<string.h>
#define SZ 4

void P52_hash(unsigned char m[], unsigned mbytes, unsigned char res[], unsigned rbytes);

void printRes(unsigned char arr[], int size){
    int i;
    for(i=0;i<size;i++){
        printf("%02x",arr[i]);
    }
    printf("\n");
}

int main() {
    unsigned char res1[SZ] = {0}, res2[SZ] = {0};
    unsigned i = 0;

    do { // first loop to find the cycle pattern
        P52_hash(res1, sizeof(res1), res1, sizeof(res1)); // hash once
        P52_hash(res2, sizeof(res2), res2, sizeof(res2)); // hash twice
        P52_hash(res2, sizeof(res2), res2, sizeof(res2)); // hash trice

    } while(memcmp(res1,res2,SZ)!=0); //we find pattern of cycle and afterwards store the values where found

    memset(res1,0,sizeof(res1)); //reset the res1 hash

    unsigned char oldRes1[SZ]; // store values where we found collision
    unsigned char oldRes2[SZ]; // store for fast value as well

    do { // second loop to find the cycle pattern
        memcpy(oldRes2, res2, SZ);
        memcpy(oldRes1, res1, SZ);
        P52_hash(res1,SZ,res1,SZ);
        P52_hash(res2,SZ,res2,SZ);
    } while(memcmp(res1,res2,SZ) !=0); //2nd loop finally finds collision

	printRes(oldRes1,SZ);
	printRes(oldRes2, SZ);
    printf("Theese collide to get\n");
    printRes(res1,SZ);

}
