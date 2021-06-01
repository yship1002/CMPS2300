#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <strings.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#define SERVER_PORT  5432
#define MAX_PENDING  5
#define MAX_LINE     1256

int main()
{
  struct sockaddr_in sin;
  char buf[MAX_LINE];
  int buf_len, addr_len;
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
  printf("Waiting for client connections...\n");
  int client_number = 1;
  while(1) {
      l = sizeof(sin);
      int index = 0;
      char question[8][5000] = {"Tell me more... ","I'm sorry to hear that.", "How does that make you feel?", "Is it really?", "Are you sure?","What makes you think ","How long since ","Did you come to me because "};//First 5 are always correct questions
      char answer[1][5000];
    if ((new_s = accept(s, (struct sockaddr *)&sin, &addr_len)) < 0) {
      perror("simplex-talk: accept");
      exit(1);
    }
    printf("Client %d connected.\n", client_number);
    while ((buf_len = recv(new_s, buf, sizeof(buf), 0))){
    fputs(buf, stdout);
    buf[strcspn(buf,"\n")]=0;
    srand(time(0));
    if (strchr(buf, '?')!=NULL){//message containes a ? mark
          strcpy(answer[0], question[0]);
          strcat(answer[0], "\n");
          send(new_s, answer[0], strlen(answer[0]), 0);
    }else{
          index=(rand()%8);
          if (index == 6){//How long since question
              strcpy(answer[0], question[6]);
              strcat(answer[0], buf);
              strcat(answer[0], " started?\n");
              send(new_s, answer[0], strlen(answer[0]), 0);
          }else if (index == 5 | index ==7){//questions contains users' response
              strcpy(answer[0], question[index]);
              strcat(answer[0], buf);
              strcat(answer[0], " ?\n");
              send(new_s, answer[0], strlen(answer[0]), 0);
          }else{//generic questions
              strcpy(answer[0], question[index]);
              strcat(answer[0], "\n");
              send(new_s, answer[0], strlen(answer[0]), 0);
          }
      }
    }
    close(new_s);
    client_number++;
  }
}
