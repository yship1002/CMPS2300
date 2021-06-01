#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <strings.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#define SERVER_PORT  5432
#define MAX_PENDING  5
#define MAX_LINE     256
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
int main()
{
  struct sockaddr_in sin;
  char buf[MAX_LINE];
  unsigned int l;
  int buf_len, addr_len;
  int s, new_s;

  /* build address data structure */
  bzero((char *)&sin, sizeof(sin));
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
  printf("Waiting for client connections...\n");
  int client_number = 1;
  while(1) {
    l = sizeof(sin);
    if ((new_s = accept(s, (struct sockaddr *)&sin, &l)) < 0) {
      perror("simplex-talk: accept");
      exit(1);
    }
    printf("Client %d connected.\n", client_number);
    while (buf_len = recv(new_s, buf, sizeof(buf), 0)){
        printf("The received value: %s\n", buf);
        char *dummy;
        //strtoul needs a pointer but we don't need it here so a dummy variable has been created
        //stroul converts a message to binary
        unsigned long message = strtoul(buf, &dummy, 2);
        unsigned long divisor = strtoul("1101", &dummy, 2);
        unsigned long remainder = crc_remainder(message, divisor, strlen(buf) , 4);
        printf("Remainder: %lu\n", remainder);
        for(int i=0;i<sizeof(buf);i++){
            buf[i] = (char) 0;//initialize buf
        }

        if (remainder != 0){
            printf("Message has been corrupted!\n");
        }else{
            printf("message has been validated!\n");
        }
        fputs(buf, stdout);
    }
    close(new_s);
    client_number++;
      
  }
}
