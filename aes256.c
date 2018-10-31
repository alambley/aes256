#include "aes256.h"

#define KEY_LENGTH 8
#define BLOCK_SIZE 4
#define NUMB_ROUNDS 14

uint8_t S_Box[256] = {
  0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5,
  0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,
  0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0,
  0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,
  0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc,
  0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,
  0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a,
  0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,
  0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0,
  0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,
  0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b,
  0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,
  0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85,
  0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,
  0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5,
  0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,
  0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17,
  0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,
  0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88,
  0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,
  0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c,
  0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,
  0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9,
  0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,
  0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6,
  0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,
  0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e,
  0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,
  0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94,
  0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,
  0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68,
  0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16,
};

uint8_t Inverse_S_Box[256] = {
  0x52, 0x09, 0x6a, 0xd5, 0x30, 0x36, 0xa5, 0x38,
  0xbf, 0x40, 0xa3, 0x9e, 0x81, 0xf3, 0xd7, 0xfb,
  0x7c, 0xe3, 0x39, 0x82, 0x9b, 0x2f, 0xff, 0x87,
  0x34, 0x8e, 0x43, 0x44, 0xc4, 0xde, 0xe9, 0xcb,
  0x54, 0x7b, 0x94, 0x32, 0xa6, 0xc2, 0x23, 0x3d,
  0xee, 0x4c, 0x95, 0x0b, 0x42, 0xfa, 0xc3, 0x4e,
  0x08, 0x2e, 0xa1, 0x66, 0x28, 0xd9, 0x24, 0xb2,
  0x76, 0x5b, 0xa2, 0x49, 0x6d, 0x8b, 0xd1, 0x25,
  0x72, 0xf8, 0xf6, 0x64, 0x86, 0x68, 0x98, 0x16,
  0xd4, 0xa4, 0x5c, 0xcc, 0x5d, 0x65, 0xb6, 0x92,
  0x6c, 0x70, 0x48, 0x50, 0xfd, 0xed, 0xb9, 0xda,
  0x5e, 0x15, 0x46, 0x57, 0xa7, 0x8d, 0x9d, 0x84,
  0x90, 0xd8, 0xab, 0x00, 0x8c, 0xbc, 0xd3, 0x0a,
  0xf7, 0xe4, 0x58, 0x05, 0xb8, 0xb3, 0x45, 0x06,
  0xd0, 0x2c, 0x1e, 0x8f, 0xca, 0x3f, 0x0f, 0x02,
  0xc1, 0xaf, 0xbd, 0x03, 0x01, 0x13, 0x8a, 0x6b,
  0x3a, 0x91, 0x11, 0x41, 0x4f, 0x67, 0xdc, 0xea,
  0x97, 0xf2, 0xcf, 0xce, 0xf0, 0xb4, 0xe6, 0x73,
  0x96, 0xac, 0x74, 0x22, 0xe7, 0xad, 0x35, 0x85,
  0xe2, 0xf9, 0x37, 0xe8, 0x1c, 0x75, 0xdf, 0x6e,
  0x47, 0xf1, 0x1a, 0x71, 0x1d, 0x29, 0xc5, 0x89,
  0x6f, 0xb7, 0x62, 0x0e, 0xaa, 0x18, 0xbe, 0x1b,
  0xfc, 0x56, 0x3e, 0x4b, 0xc6, 0xd2, 0x79, 0x20,
  0x9a, 0xdb, 0xc0, 0xfe, 0x78, 0xcd, 0x5a, 0xf4,
  0x1f, 0xdd, 0xa8, 0x33, 0x88, 0x07, 0xc7, 0x31,
  0xb1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xec, 0x5f,
  0x60, 0x51, 0x7f, 0xa9, 0x19, 0xb5, 0x4a, 0x0d,
  0x2d, 0xe5, 0x7a, 0x9f, 0x93, 0xc9, 0x9c, 0xef,
  0xa0, 0xe0, 0x3b, 0x4d, 0xae, 0x2a, 0xf5, 0xb0,
  0xc8, 0xeb, 0xbb, 0x3c, 0x83, 0x53, 0x99, 0x61,
  0x17, 0x2b, 0x04, 0x7e, 0xba, 0x77, 0xd6, 0x26,
  0xe1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0c, 0x7d,
};

uint8_t R_Con[30] = {
  0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 
  0x40, 0x80, 0x1b, 0x36, 0x6c, 0xc0, 
  0xab, 0x4d, 0x9a, 0x2f, 0x5e, 0xbc, 
  0x63, 0xc6, 0x97, 0x35, 0x6a, 0xd4, 
  0xb3, 0x7d, 0xfa, 0xef, 0xc5, 0x91,
};

void AES256MainECB(uint8_t *key, uint8_t *data, uint32_t datalen, uint8_t *returndata, uint32_t *returnlen, bool encrypt){
  int temp = datalen;
  int numbBlocks = ceil(datalen/16.0);
  int dataBytes = 0;
  int returnBytes = 0;
  int i,x,y;
  for(i = 0; i < numbBlocks; i++){
    AES256_State state;
    for(y = 0; y < 4; y++){
      for(x = 0; x < 4; x++){
        if(dataBytes < datalen){
          state.x[y].x[x] = data[dataBytes++];
        }else{
          state.x[y].x[x] = 0x00;
        }
      }
    }
    if(encrypt)
      state = AES256Encrypt(state, key);
    else
      state = AES256Decrypt(state, key);
    for(y = 0; y < 4; y++){
      for(x = 0; x < 4; x++){
        returndata[returnBytes++] = state.x[y].x[x];
      }
    }
  }
  *returnlen = returnBytes;
}

void AES256MainCBC(uint8_t *key, uint8_t *data, uint8_t *iv, uint32_t datalen, uint8_t *returndata, uint32_t *returnlen, bool encrypt){
  int temp = datalen;
  int numbBlocks = ceil(datalen/16.0);
  int dataBytes = 0;
  int returnBytes = 0;
  int i,x,y;
  AES256_State ivState;
  for(y = 0; y < 4; y++){
    for(x = 0; x < 4; x++){
      ivState.x[y].x[x] = iv[y * 4 + x];
    }
  }
  for(i = 0; i < numbBlocks; i++){
    AES256_State state;
    for(y = 0; y < 4; y++){
      for(x = 0; x < 4; x++){
        if(dataBytes < datalen){
          state.x[y].x[x] = data[dataBytes++];
        }else{
          state.x[y].x[x] = 0x00;
        }
      }
    }
    if(encrypt){
      state = XorState(state, ivState);
      state = AES256Encrypt(state, key);
      ivState = state;
    }else{
      AES256_State temp = state;
      state = AES256Decrypt(state, key);
      state = XorState(state, ivState);
      ivState = temp;
    }
    for(y = 0; y < 4; y++){
      for(x = 0; x < 4; x++){
        returndata[returnBytes++] = state.x[y].x[x];
      }
    }
  }
  *returnlen = returnBytes;
}

void AES256MainCTR(uint8_t *key, uint8_t *data, uint8_t *iv, uint32_t datalen, uint8_t *returndata, uint32_t *returnlen, bool encrypt){
  int temp = datalen;
  int numbBlocks = ceil(datalen/16.0);
  int dataBytes = 0;
  int returnBytes = 0;
  int i,x,y;
  AES256_State ivState;
  for(y = 0; y < 4; y++){
    for(x = 0; x < 4; x++){
      ivState.x[y].x[x] = iv[y * 4 + x];
    }
  }
  for(i = 0; i < numbBlocks; i++){
    AES256_State state;
    for(y = 0; y < 4; y++){
      for(x = 0; x < 4; x++){
        if(dataBytes < datalen){
          state.x[y].x[x] = data[dataBytes++];
        }else{
          state.x[y].x[x] = 0x00;
        }
      }
    }
    AES256_State temp = AES256Encrypt(ivState, key);
    state = XorState(state,temp);
    ivState = IncState(ivState);
    for(y = 0; y < 4; y++){
      for(x = 0; x < 4; x++){
        if(returnBytes == datalen)
          break;
        returndata[returnBytes++] = state.x[y].x[x];
      }
    }
  }
  *returnlen = returnBytes;
}

AES256_State AES256Encrypt(AES256_State state, uint8_t key[32]){
  AES256_Word expanse[60];
  KeyExpansion(key, expanse);
  state = AddRoundKey(state, expanse, 0);
  int i;
  for(i = 0; i < NUMB_ROUNDS - 1; i++){
    state = SubBytes(state, false);
    state = ShiftRows(state);
    state = MixColumns(state, false);
    state = AddRoundKey(state, expanse, i+1);
  }
  state = SubBytes(state, false);
  state = ShiftRows(state);
  state = AddRoundKey(state, expanse, NUMB_ROUNDS);
  return state;
}

AES256_State AES256Decrypt(AES256_State state, uint8_t key[32]){
  AES256_Word expanse[60];
  KeyExpansion(key, expanse);
  state = AddRoundKey(state, expanse, NUMB_ROUNDS);
  int i;
  for(i = NUMB_ROUNDS - 1; i > 0; i--){
    state = InvShiftRows(state);
    state = SubBytes(state, true);
    state = AddRoundKey(state, expanse, i);
    state = MixColumns(state, true);
  }
  
	state = InvShiftRows(state);
  state = SubBytes(state, true);
  state = AddRoundKey(state, expanse, 0);
  return state;
}

char * _Print_Word(AES256_Word word){
  static char buffer[10];
  sprintf(buffer, "%02x%02x%02x%02x",word.x[0],word.x[1],word.x[2],word.x[3]);
  return buffer;
}

char * _Print_State(AES256_State state){
  static char buffer[40];
  sprintf(buffer, "%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x",
          state.x[0].x[0],state.x[0].x[1],state.x[0].x[2],state.x[0].x[3],
          state.x[1].x[0],state.x[1].x[1],state.x[1].x[2],state.x[1].x[3],
          state.x[2].x[0],state.x[2].x[1],state.x[2].x[2],state.x[2].x[3],
          state.x[3].x[0],state.x[3].x[1],state.x[3].x[2],state.x[3].x[3]
          );
  return buffer;
}

AES256_Word SubWord(AES256_Word word, bool inverse){
  int i;
  for(i = 0; i < 4; i++){
    uint8_t higher = word.x[i] >> 4;
    uint8_t lower = word.x[i] & 0x0f;
    if(inverse)
      word.x[i] = Inverse_S_Box[higher * 16 + lower];
    else
      word.x[i] = S_Box[higher * 16 + lower];
  }
  return word;
}

AES256_Word RotWord(AES256_Word word, uint8_t shift){
  AES256_Word temp;
  int i,j;
  j = shift;
  for(i = 0; i < 4; i++){
    temp.x[i] = word.x[j];
    j++;
    if(j >= 4)
      j = 0;
  }
  return temp;
}

AES256_Word XorRcon(AES256_Word word, uint8_t rcon){
  uint8_t temp = R_Con[rcon];
  word.x[0] = word.x[0] ^ temp;
  return word;
}

AES256_Word XorWord(AES256_Word a, AES256_Word b){
  AES256_Word temp;
  int i;
  for(i = 0; i < 4; i++)
    temp.x[i] = a.x[i] ^ b.x[i];
  return temp;
}

void KeyExpansion(uint8_t key[32], AES256_Word *expanse){
  int i;
  for(i = 0; i < 60; i++){
    if(i < KEY_LENGTH){
      expanse[i].x[0] = key[4*i];
      expanse[i].x[1] = key[4*i+1];
      expanse[i].x[2] = key[4*i+2];
      expanse[i].x[3] = key[4*i+3];
      // printf("[%02d]%s\n",i,_Print_Word(expanse[i]));
    }else{
      AES256_Word temp = expanse[i-1];
      if(i % KEY_LENGTH == 0){
        temp = XorRcon(SubWord(RotWord(temp,1), false),i/KEY_LENGTH-1);
      }else if(KEY_LENGTH > 6 && i % KEY_LENGTH == 4){
        temp = SubWord(temp, false);
      }
      expanse[i] = XorWord(expanse[i-KEY_LENGTH],temp);
      // printf("[%02d]%s\n",i,_Print_Word(expanse[i]));
    }
  }
}

AES256_State AddRoundKey(AES256_State in, AES256_Word *expanse, uint8_t round){
  int i;
  for(i = 0; i < 4; i++){
    in.x[i] = XorWord(in.x[i],expanse[round * 4 + i]);
  }
  return in;
}

AES256_State SubBytes(AES256_State in, bool inverse){
  int i;
  for(i = 0; i < 4; i++)
    in.x[i] = SubWord(in.x[i],inverse);
  return in;
}

AES256_State ShiftRows(AES256_State in){
  AES256_State temp;
  int i,j,k;
  for(j = 0; j < 4; j++)
    for(i = 0; i < 4; i++){
      k = i+j;
      if(k >= 4)
        k -= 4;
      temp.x[i].x[j] = in.x[k].x[j];
    }  
  return temp;  
}

AES256_State MixColumns(AES256_State in, bool inverse){
  int i;
  for(i = 0; i < 4; i++){
    if(!inverse)
      in.x[i] = MixColumn(in.x[i]);
    else
      in.x[i] = InvMixColumn(in.x[i]);
  }
  return in;
}

uint8_t gmul(uint8_t a, uint8_t b) {
	uint8_t p = 0; 
	while (a && b) {
    if (b & 1) 
      p ^= a;
    if (a & 0x80) 
      a = (a << 1) ^ 0x11b; 
    else
      a <<= 1;
    b >>= 1;
	}
	return p;
}

AES256_Word MixColumn(AES256_Word r){
  AES256_Word a;
  a.x[0] = gmul(r.x[0],0x02) ^ gmul(r.x[1],0x03) ^ gmul(r.x[2],0x01) ^ gmul(r.x[3],0x01);
  a.x[1] = gmul(r.x[0],0x01) ^ gmul(r.x[1],0x02) ^ gmul(r.x[2],0x03) ^ gmul(r.x[3],0x01);
  a.x[2] = gmul(r.x[0],0x01) ^ gmul(r.x[1],0x01) ^ gmul(r.x[2],0x02) ^ gmul(r.x[3],0x03);
  a.x[3] = gmul(r.x[0],0x03) ^ gmul(r.x[1],0x01) ^ gmul(r.x[2],0x01) ^ gmul(r.x[3],0x02);
  return a;
}

AES256_Word InvMixColumn(AES256_Word r){
  AES256_Word a;
  a.x[0] = gmul(r.x[0],0x0e) ^ gmul(r.x[1],0x0b) ^ gmul(r.x[2],0x0d) ^ gmul(r.x[3],0x09);
  a.x[1] = gmul(r.x[0],0x09) ^ gmul(r.x[1],0x0e) ^ gmul(r.x[2],0x0b) ^ gmul(r.x[3],0x0d);
  a.x[2] = gmul(r.x[0],0x0d) ^ gmul(r.x[1],0x09) ^ gmul(r.x[2],0x0e) ^ gmul(r.x[3],0x0b);
  a.x[3] = gmul(r.x[0],0x0b) ^ gmul(r.x[1],0x0d) ^ gmul(r.x[2],0x09) ^ gmul(r.x[3],0x0e);
  return a;
}

AES256_State InvShiftRows(AES256_State in){
  AES256_State temp;
  int i,j,k;
  for(j = 0; j < 4; j++)
    for(i = 0; i < 4; i++){
      k = i-j;
      if(k < 0)
        k += 4;
      temp.x[i].x[j] = in.x[k].x[j];
    }
  return temp;
}

AES256_State XorState(AES256_State a, AES256_State b){
  AES256_State temp;
  int i;
  for(i = 0; i < 4; i++)
    temp.x[i] = XorWord(a.x[i], b.x[i]);
  return temp;
}

AES256_State IncState(AES256_State a){
  int x,y;
  bool breakevent = false;
  for(y = 3; y >= 0; y--){
    for(x = 3; x >= 0; x--){
      a.x[y].x[x] += 1;
      if(!a.x[y].x[x] == 0)
        breakevent = true;
      if(breakevent)
        break;
    }
    if(breakevent)
      break;
  }
  return a;
}