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
    for (i = mlen; i > 0; i--) {
        if ((newmsg & (1 << (i+clen-2))) != 0) {//if the marching bit is not 0 then do the following otherwise skip if statement
            n = check << (i - 1);//padding 0 to check in order to XOR the newmsg
            newmsg = newmsg ^ n;//XOR n with newmsg
        }
    }
    return newmsg & ((1 << clen) - 1);//I think this is a redundant step since it is simply returning the newmsg
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
  if (hp == NULL) {
    fprintf(stderr, "simplex-talk: unknown host: %s\n", host);
    exit(1);
  }

  /* build address data structure */
  bzero((char *)&sin, sizeof(sin));
  sin.sin_family = AF_INET;
  bcopy(hp->h_addr, (char *)&sin.sin_addr, hp->h_length);
  sin.sin_port = htons(SERVER_PORT);

  /* active open */
  if ((s = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
    perror("simplex-talk: socket");
    exit(1);
  }
  printf("Attempting to connect to %s.\n", hp->h_name);
  if (connect(s, (struct sockaddr *)&sin, sizeof(sin)) < 0)
  {
    perror("simplex-talk: connect");
    close(s);
    exit(1);
  }
  /* main loop: get and send lines of text */
  printf("Connected to %s.\n", hp->h_name);
  printf("Send a message to the server: ");
  while (fgets(buf, sizeof(buf), stdin)) {
    buf[MAX_LINE-1] = '\0';
    len = strlen(buf) + 1;
    char *dummy;
      //strtoul requires a pointer but we don't need it here so a dummy variable has been created
      //Strtoul converts message to binary
    unsigned long message = strtoul(buf, &dummy, 2);
    unsigned long divisor = strtoul("1101", &dummy, 2);//by default we use 1101 as divisor polynomial
    unsigned long remainder = crc_remainder(message, divisor, len, 4);
    srand(time(0));
    if(rand()%10 < 3){
  //chooses one bits to flip randomly
        message = (1<<rand()%(len)) ^ message;
    }
    unsigned long newMessage = (message << 3) + remainder;
    const char *binary = unsigned_to_binary(newMessage, len+3);
    int i=0;
    for(i = 0; i<len+3; i++){//transfer every bit of processed message to the buf so that it can be sent across network
        buf[i]=binary[i];
    }
    buf[i] = '\0'; //end string
    send(s, buf, strlen(buf), 0);
    printf("Send a message to the server: ");
  }
}
