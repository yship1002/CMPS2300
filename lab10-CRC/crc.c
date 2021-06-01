#include <stdio.h>
#include <string.h>
#include <stdlib.h>

const char *unsigned_to_binary(unsigned long msg, int size)
{//convert number to string
	char* b = (char *)malloc(size + 1);//Allocate memory to perform transformation
	b[0] = '\0';

	int z;
	for (z = (1<<(size-1)); z > 0; z >>= 1)
	{//convert number bit by bit to a string i.e if string is 1 then append 1 to b
		strcat(b, ((msg & z) == z) ? "1" : "0");
	}

	return b;
}

unsigned long crc_remainder(unsigned long msg, unsigned long check, int mlen, int clen) {
	unsigned long newmsg = msg << (clen-1);//add the missing 0s to the end of the original message
	unsigned long n;
	int i;
	for (i = mlen; i > 0; i--) {//go over every bit of message
		if ((newmsg & (1 << (i+clen-2))) != 0) {//if the marching bit is not 0 then do the following otherwise skip if statement
			n = check << (i - 1);//padding 0 to check in order to XOR the newmsg
			newmsg = newmsg ^ n;//XOR n with newmsg
		}
	}
	return newmsg & ((1 << clen) - 1);//I think this is a redundant step since it is simply returning the newmsg
}

int main() {
    //Main method to to test the functionality of crc algorithm
	unsigned long msg = 0xC74A;
	unsigned long divisor = 0xB;
	unsigned long newmsg = (msg << 3) + crc_remainder(msg, divisor, 16, 4);
    //rem is the original remainder
    //newrem is the remainder after applying algorithm and it should be 0 in this case
	unsigned long rem, newrem;
	printf("Message is: ");
	printf("%s\n", unsigned_to_binary(msg, 16));
	printf("Divisor is: ");
	printf("%s\n", unsigned_to_binary(divisor, 4));
	rem = crc_remainder(msg, divisor, 16, 4);
	printf("Remainder of message and divisor: ");
	printf("%s\n", unsigned_to_binary(rem, 3));
	printf("Message with added bits is: ");
	printf("%s\n", unsigned_to_binary(newmsg, 19));
	newrem = crc_remainder(newmsg, divisor, 19, 4);
	printf("Remainder is: ");
	printf("%s\n", unsigned_to_binary(newrem, 3));

//Basically the same thing here
	printf("\nEXAMPLE FROM BOOK:\n\n");
	char * ptr;
	msg = strtoul("10011010", &ptr, 2);
	divisor = strtoul("1101", &ptr, 2);

	printf("Message is: ");
	printf("%s\n", unsigned_to_binary(msg, 8));
	printf("Divisor is: ");
	printf("%s\n", unsigned_to_binary(divisor, 4));
	rem = crc_remainder(msg, divisor, 8, 4);
	printf("Remainder of message and divisor: ");
	printf("%s\n", unsigned_to_binary(rem, 3));
	newmsg = (msg << 3) + rem;
	printf("Message with added bits is: ");
	printf("%s\n", unsigned_to_binary(newmsg, 11));
	newrem = crc_remainder(newmsg, divisor, 11, 4);
	printf("Remainder is: ");
	printf("%s\n", unsigned_to_binary(newrem, 3));
	return 0;
}
