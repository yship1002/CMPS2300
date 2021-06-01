#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

#define SERVER_PORT 5432
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
}
    const char *unsigned_to_binary(unsigned long msg, int size)//convert unsigned long to string
{
    char* b = (char *)malloc(size + 1);//allocate memory for b
    b[0] = '\0';//initialize b

    int z;//create an integer z
    for (z = (1<<(size-1)); z > 0; z >>= 1)//for loop create a binary string
    {
        strcat(b, ((msg & z) == z) ? "1" : "0");//store the string in b
    }
    return b;//return the value of b
}
int main(int argc, char * argv[])
{
	FILE *fp;
	struct hostent *hp;
	struct sockaddr_in sin;
	char *host;
	char buf[MAX_LINE];
	int s;
	int len;

	if (argc==2) {
		host = argv[1];
	}
	else {
		fprintf(stderr, "usage: simplex-talk host\n");
		exit(1);
	}

	/* translate host name into peer's IP address */
	hp = gethostbyname(host);
	if (!hp) {
		fprintf(stderr, "simplex-talk: unknown host: %s\n", host);
		exit(1);
	}

	/* build address data structure */
	//bzero((char *)&sin, sizeof(sin));
	memset((char *)&sin, 0, sizeof(sin));
	sin.sin_family = AF_INET;
	//bcopy(hp->h_addr, (char *)&sin.sin_addr, hp->h_length);
	memcpy(hp->h_addr, (char *)&sin.sin_addr, hp->h_length);
	sin.sin_port = htons(SERVER_PORT);

	/* active open */
	if ((s = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
		perror("simplex-talk: socket");
		exit(1);
	}
	if (connect(s, (struct sockaddr *)&sin, sizeof(sin)) < 0)
	{
		perror("simplex-talk: connect");
		close(s);
		exit(1);
	}
	/* main loop: get and send lines of text */
  printf("Send a message to the server: ");
	while (fgets(buf, sizeof(buf), stdin)) {
		buf[MAX_LINE-1] = '\0';
		len = strlen(buf) + 1;

		char *point;
		unsigned long message = strtoul(buf, &point, 2);
		unsigned long divisor = strtoul("1101", &point, 2);
		unsigned long remainder = crc_remainder(message, divisor, len, 4);
		//30% chance to change a random bit
		srand(time(0)); 
		if(rand()%10 < 3){
		//chooses one bits to flip randomly 
			message = (1<<rand()%(len)) ^ message;
		}

		unsigned long newMessage = (message << 3) + remainder;

		const char *binary = unsigned_to_binary(newMessage, len+3); 
		char toSend[MAX_LINE];

		int i = 0;
		for(i = 0; i<len+3; i++){ 
			buf[i]=binary[i];
		}
		buf[i] = '\0'; //end string

		send(s, buf, strlen(buf), 0);
    		printf("Send a message to the server: ");
	}
}

