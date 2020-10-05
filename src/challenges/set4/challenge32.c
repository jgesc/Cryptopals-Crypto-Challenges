#include "../../utils/crypto.h"
#include "../../utils/stringutils.h"

#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <poll.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>
#include <x86intrin.h>

#define SLEEP_US 5000
#define PORT 8081

static int httppid = 0;

void httpmain()
{
  // Config
  const struct sockaddr_in addr = {
    .sin_family = AF_INET,
    .sin_addr.s_addr = inet_addr("127.0.0.1"),
    .sin_port = htons(PORT)
  };

  // Open socket
  int sock;
  int opt = 1;
  if(((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) ||
    (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
      &opt, sizeof(opt)) < 0) ||
    (bind(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0) ||
    (listen(sock, 0) < 0))
  {
    perror("Can not open socket");
    return;
  }

  // Get random key
  srand(time(NULL));
  uint8_t k[64], k_i;
  for(k_i = 0; k_i < 64; k_i++) k[k_i] = rand();

  // Print result cheats xd lol
  uint8_t res[20];
  puts("Result:");
  hmacsha1("example_file.bin", 16, res, k);
  printhex(res, 20);


  // Main loop
  signal(SIGCHLD, SIG_IGN); // Avoid zombie subprocesses
  int clisock;
  while(1)
  {
    // Accept connection
    struct sockaddr_in cliaddr = {0};
    socklen_t addrlen = 0;

    clisock = accept(sock, (struct sockaddr *)&cliaddr, (socklen_t *)&addrlen);
    if(clisock < 0)
    {
      perror("Error accepting connection");
      printf("Consider using 'ulimit -n 16384' to increase max open sockets");
      exit(EXIT_FAILURE);
    }

    if(!fork()) break;
  }

  // Serve request
  {
    const char * RES_200 = "OK";
    const char * RES_500 = "Internal Server Error";

    #define BUFSIZE 8192
    char buf[BUFSIZE] = {0};
    char res[256] = {0};

    if(recv(clisock, buf, BUFSIZE, 0) > 0 &&
      strncmp(buf, "GET /test?file=", 15) == 0)
    {
      // Get data and mac from URL
      char * data = buf+15;
      char * macstr = strchr(buf+15, '&')+1;

      // Check for MAC
      if(!macstr)
      {
        int resl = snprintf(res, 256, "HTTP/1.1 %d %s\r\n", 500, RES_500);
        send(clisock, res, resl, 0);
        close(clisock);
        exit(0);
      }

      *(macstr+40) = '\0';
      size_t l = macstr-1-data;
      uint8_t mac[20];
      if(readhex(macstr, mac, 20) == 20)
      {
        uint8_t calcmac[20];
        hmacsha1(data, l, calcmac, k);

        int i;
        for(i = 0; i < 20; i++)
        {
          if(mac[i] != calcmac[i]) break;
          usleep(SLEEP_US);
        }

        if(i == 20)
        {
          int resl = snprintf(res, 256, "HTTP/1.1 %d %s\r\n", 200, RES_200);
          send(clisock, res, resl, 0);
          close(clisock);
          exit(0);
        }
      }
    }

    int resl = snprintf(res, 256, "HTTP/1.1 %d %s\r\n", 500, RES_500);
    send(clisock, res, resl, 0);
    close(clisock);
  }
  close(clisock);
  exit(0);
}

int main(void)
{
  // Fork HTTP server subprocess
  if(!(httppid = fork()))
    httpmain();

   setbuf(stdout, NULL); // Avoid stdout buffering

   // Discover HMAC
   const char * file = "example_file.bin";
   char buf[8192] = {0};
   snprintf(buf, 8192, "GET /test?file=%s&%40s HTTP/1.1", file, "");
   size_t l = strlen(buf);

   char * hmac = strchr(buf, '&')+1;
   memset(hmac, '0', 40);

   // Setup socket
   int sock, opt=1;
   struct sockaddr_in addr = {
     .sin_family = AF_INET,
     .sin_port = htons(PORT),
     .sin_addr.s_addr = inet_addr("127.0.0.1")
   };
   if((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0 ||
     (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
       &opt, sizeof(opt)) < 0))
   {
     perror("Unexpected error:");
     kill(httppid, SIGINT);
     return 1;
   }

   // Wait for server to be available
   int retries, maxretries = 5, retrytime = 1;
   for(retries = 0; retries < maxretries; retries++)
   {
     if(connect(sock, (struct sockaddr*)&addr, sizeof(addr)) == 0) break;
     perror("Failed to connect, retrying...");
     sleep(retrytime);
   }

   if(retries >= maxretries)
   {
     perror("Can't reach server");
     kill(httppid, SIGINT);
     return 1;
   }

   char tmp;
   int k_i, byte;
   for(k_i = 0; k_i < 20; k_i++)
   {
     char * hexptr = hmac + k_i * 2;
     int mtbyte = 0; // Most time byte
      uint64_t mt = 0; // Most time

     printf("00");
     for(byte = 0; byte < 256; byte++)
     {
       // Generate next URL
       tmp = hexptr[2];
       snprintf(hexptr, 3, "%02x", byte);
       hexptr[2] = tmp;
       printf("\b\b%02x", byte);

       uint64_t tval_result = 0;
       int flag = 0;

       int j;
       for(j = 0; j < 5; j++)
       {
         // Connect
         if((sock = socket(AF_INET, SOCK_STREAM, 0)) <= 0 ||
           (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
             &opt, sizeof(opt)) < 0) ||
           connect(sock, (struct sockaddr*)&addr, sizeof(addr)) < 0)
         {
           perror("Can't connect");
           kill(httppid, SIGINT);
           return 1;
         }

         uint64_t tval_before, tval_after;
         char res[128];
         send(sock, buf, l, 0);
         tval_before = __rdtsc();
         recv(sock, res, 128, 0);
         tval_after = __rdtsc();
         shutdown(sock, 2);

         if(k_i == 19) // Bruteforce last byte
         {
           if(strncmp(res, "HTTP/1.1 200", 12) == 0)
           {
             mtbyte = byte;
             flag = 1;
             break;
           }
         }
         tval_result += tval_after - tval_before;
      }
      if(flag) break;
       if(tval_result > mt)
       {
         mt = tval_result;
         mtbyte = byte;
       }
     }

     printf("\b\b%02x", mtbyte);
     tmp = hexptr[2];
     snprintf(hexptr, 3, "%02x", mtbyte);
     hexptr[2] = tmp;
   }
   putchar('\n');

   /// Check HMAC
   if((sock = socket(AF_INET, SOCK_STREAM, 0)) <= 0 ||
     (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
       &opt, sizeof(opt)) < 0) ||
     connect(sock, (struct sockaddr*)&addr, sizeof(addr)) < 0)
   {
     perror("Can't connect");
     kill(httppid, SIGINT);
     return 1;
   }

   char res[128];
   send(sock, buf, l, 0);
   recv(sock, res, 128, 0);
   if(strncmp(res, "HTTP/1.1 200", 12) == 0) puts("OK");
   else puts("FAIL");


   kill(httppid, SIGINT); // Kill HTTP subprocess
   return 0;
 }
