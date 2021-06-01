#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <strings.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#define SERVER_PORT 5432
#define MAX_PENDING 1
#define MAX_LINE 256

unsigned long crc_remainder(unsigned long msg, unsigned long check, int mlen, int clen) {//create the remainder for crc
	unsigned long newmsg = msg << (clen-1);//add (clen-1) 0s to the end of the message and store the remained value in a variable called newmsg
	unsigned long n;// create an unsigned long type variable n
	int i;//create an int type variable n
	for (i = mlen; i > 0; i--) {//create a for loop to run mlen times
		if ((newmsg & (1 << (i+clen-2))) != 0) {//if the digits before the last (clen-1) of newmsg is not 0, doing the following command
			n = check << (i - 1);//add (i-1) 0s to check and assign the value to n
			newmsg = newmsg ^ n;//use XOR to cut the first digit of newmsg
		}
	}
	return newmsg & ((1 << clen) - 1);//return the remainder (clen digits£©
}

int main(){
	struct sockaddr_in sin;
	char buf[MAX_LINE];
	int len;
  	unsigned int l;
	int s, new_s;

	/* build address data structure */
	//bzero((char *)&sin, sizeof(sin));
	memset((char *)&sin, 0, sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = INADDR_ANY;
	sin.sin_port = htons(SERVER_PORT);

	/* setup passive open */
	if ((s = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
		perror("simplex-talk: socket");
		exit(1);
	}
	if ((bind(s, (struct sockaddr *)&sin, sizeof(sin))) < 0) {
		perror("simplex-talk: bind");
		exit(1);
	}
	listen(s, MAX_PENDING);

	/* wait for connection, then receive and print text */
	while(1) {
		l = sizeof(sin);
		if ((new_s = accept(s, (struct sockaddr *)&sin, &l)) < 0) {
			perror("simplex-talk: accept");
			exit(1);
		}
		while ((len = recv(new_s, buf, sizeof(buf), 0))){
			printf("The received value: %s\n", buf);

			char *point;
			unsigned long message = strtoul(buf, &point, 2);
			unsigned long divisor = strtoul("1101", &point, 2);
			unsigned long remainder = crc_remainder(message, divisor, strlen(buf) , 4); 
			printf("Remainder: %lu\n", remainder);

			for(int i=0;i<sizeof(buf);i++){
				buf[i] = (char) 0;//initialize buf
			}

			if (remainder != 0){
				printf("ERROR\n");
			}else{
				printf("Validated\n");
			}
			fputs(buf, stdout);
		}
		close(new_s);
	}
}

