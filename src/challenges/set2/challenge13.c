#include "../../utils/crypto.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Profile variables str
char emailstr[] = "email";
char uidstr[] = "uid";
char rolestr[] = "role";


unsigned char * K()
{
  static int kflag = 0;
  static unsigned char k[16];

  if(!kflag)
  {
    int i;
    for(i = 0; i < 16; i++) k[i] = rand() % 256;
    kflag = 1;
  }

  return k;
}

struct profile
{
  char email[256];
  unsigned int uid;
  char role[32];
};

struct profile parse(char * strprof)
{
  struct profile p = {0};

  char * param = strtok(strprof, "&");
  while(param != NULL)
  {
    // Read parameter
    char var[512] = {0};
    char val[512] = {0};
    sscanf(param, "%[^=]%*c%s", var, val);

    // Parse variable
    if(strcmp(var, emailstr) == 0)
      strcpy(p.email, val);
    else if(strcmp(var, uidstr) == 0)
      p.uid = (unsigned int)strtol(val, NULL, 10);
    else if(strcmp(var, rolestr) == 0)
      strcpy(p.role, val);

    // Fetch next parameter
    param = strtok(NULL, "&");
  }

  return p;
}

void printprof(struct profile * p)
{
  printf("{\n\temail: '%s',\n\tuid: %u,\n\trole: '%s'\n}\n", p->email, p->uid, p->role);
}

int profile_for(char * email, unsigned char * out)
{
  // Clean email from metacharacters
  char cleanemail[512] = {0};
  int i = -1;
  int j = 0;
  while(email[++i])
    if(email[i] != '&' && email[i] != '=')
      cleanemail[j++] = email[i];

  // Concatenate profile string
  char profstr[8192] = {0};
  strcat(profstr, "email=");
  strcat(profstr, cleanemail);
  strcat(profstr, "&uid=10&role=user");

  // Encrypt
  return encrypt_aes128ecb(profstr, strlen(profstr), out, K());
}

int is_admin(struct profile * p)
{
  return strcmp(p->role, "admin") == 0;
}

struct profile decprof(unsigned char * in, int inl)
{
  char buff[8192] = {0};
  int l = decrypt_aes128ecb(in, inl, buff, K());
  buff[l] = 0;
  return parse(buff);
}

int main(void)
{
  // Profile buffers
  unsigned char cprof1[8192];
  unsigned char cprof2[8192];
  unsigned char cprof3[8192];

  // Get blocks
  int l1 = profile_for("1234567890admin", cprof1); // Block starting with 'admin'
  int l2 = profile_for("123456789", cprof2); // 16 byte padding
  int l3 = profile_for("fakeadmin1234", cprof3); // Target

  // Replace blocks with crafted ones
  memcpy(cprof3 + 32, cprof1 + 16, 16);
  memcpy(cprof3 + 48, cprof2 + 32, 16);

  // Decode and print result
  struct profile p = decprof(cprof3, 64);
  printprof(&p);
  printf("\nIs admin?: %s\n", is_admin(&p) ? "True" : "False");

  return 0;
}
