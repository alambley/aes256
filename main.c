#include <stdio.h>
#include <unistd.h>
#include "aes256.h"

#define MAX_MSG_LGTH 1000000000 //1 GB

void str_to_bytes(char *str, uint8_t *bytes){
  int str_len = strlen(str);
  int i;
  for(i = 0; i < str_len; i++){
    bytes[i] = str[i];
  }
}

char* bytes_to_hexstr(uint8_t *bytes, uint32_t length){
  static char buffer[(uint32_t)MAX_MSG_LGTH * 2];
  memset(buffer, 0, strlen(buffer));
  int i;
  for(i = 0; i < length; i++){
    sprintf(buffer + strlen(buffer), "%02x", bytes[i]);
    if((i + 1) % 16 == 0){
      sprintf(buffer + strlen(buffer),"\n");
    }
  }
  return buffer;
}

char* bytes_to_str(uint8_t *bytes, uint32_t length){
  static char buffer[(uint32_t)MAX_MSG_LGTH];
  memset(buffer, 0, strlen(buffer));
  int i;
  for(i = 0; i < length; i++){
    if(bytes[i] == 0 || 32 <= bytes[i] && bytes[i] <= 126)
      sprintf(buffer + strlen(buffer), "%c", bytes[i]);
    else
      sprintf(buffer + strlen(buffer), "%c", '_');
  }
  return buffer;
}

void parse_02X(uint8_t *key, char *arg, int bytes){
  int iter;
  for(iter = 0; iter < bytes; iter ++){
    uint8_t tempbit;
    char temp[2] = "";
    temp[0] = (char*)arg[iter*2];
    temp[1] = (char*)arg[iter*2+1];
    sscanf(temp, "%02hhX", &tempbit);
    key[iter] = tempbit;
  }
}

int main(int argc, char *argv[]){

  uint8_t key[32] = {
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
    0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
    0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 
    0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f,
  };

  uint8_t proofdata[16] = {
    0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77,
    0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff,
  };

  uint8_t iv[16] = {
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
    0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
  };

  bool proof = false;

  char * message = "";

  if(argc == 4){
    message = argv[1];
    char *keystring = argv[2];
    if(strlen(keystring) != 64){
      printf("key must be 32 bytes (%d)\n",strlen(keystring)/2);
      exit(-1);
    }
    parse_02X(key,keystring,32);
    char *ivstring = argv[3];
    if(strlen(ivstring) != 32){
      printf("iv must be 16 bytes (%d)\n",strlen(ivstring)/2);
      exit(-1);
    }
    parse_02X(iv,ivstring,16);
  }else{
    printf("Usage:\n%s [Message to encrypt] [Key in 02x hex] [IV in 02x hex]\n",argv[0]);
    exit(-1);
  }

  uint8_t *byteBuffer = (uint8_t*)malloc((uint32_t)MAX_MSG_LGTH);
  uint8_t *returnBuffer = (uint8_t*)malloc((uint32_t)MAX_MSG_LGTH);

  if(byteBuffer == NULL || returnBuffer == NULL){
    printf("error allocating buffers\n");
    return(-1);
  }

  uint32_t returnlen = 0;
  
  printf("Key is...\n%s", bytes_to_hexstr(key, 32));
  printf("IV is...\n%s", bytes_to_hexstr(iv, 16));
  str_to_bytes(message, byteBuffer);
  printf("Message is... \n%s\n", message);
  AES256MainCBC(key, byteBuffer, iv, strlen(message), returnBuffer, &returnlen, true);
  printf("Encrypted data is...\n%s",bytes_to_hexstr(returnBuffer, returnlen));
  // uint8_t *newBuffer = (uint8_t*)malloc((uint32_t)MAX_MSG_LGTH);
  // AES256MainECB(key, returnBuffer, returnlen, newBuffer, &returnlen, false);
  // printf("Unencrypted data is...\n%s",bytes_to_hexstr(newBuffer, returnlen));
  // if(!proof)
  //   printf("Unencrypted message is...\n%s\n",bytes_to_str(newBuffer, returnlen));
}
