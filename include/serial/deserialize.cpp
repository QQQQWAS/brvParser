#include <string>
#include <string.h>
#include <cstdint>

char getChar(char* mem, uint& ptr, bool s = 1){
  char r;
  memcpy(&r, mem + ptr, 1);
  if(s) ptr+=1;
  return r;
}
uint8_t getUint8(char* mem, uint& ptr, bool s = 1){
  uint8_t r;
  memcpy(&r, mem + ptr, 1);
  if(s) ptr+=1;
  return r;
}
uint16_t getUint16(char* mem, uint& ptr, bool s = 1){
  uint16_t r;
  memcpy(&r, mem + ptr, 2);
  if(s) ptr+=2;
  return r;
}
uint32_t getUint32(char* mem, uint& ptr, bool s = 1){
  uint32_t r;
  memcpy(&r, mem + ptr, 4);
  if(s) ptr+=4;
  return r;
}

float getFloat(char* mem, uint& ptr, bool s = 1){
  float r;
  memcpy(&r, mem + ptr, 4);
  if(s) ptr+=4;
  return r;
}
std::string getStr(char* mem, uint& ptr, uint len, bool s = 1){
  std::string r;
  r.resize(len);
  memcpy(r.data(), mem + ptr, len);
  if(s) ptr+=len;
  return r;
}
std::string getStrL(char* mem, uint& ptr){
  uint8_t len = getUint8(mem, ptr);
  return getStr(mem, ptr, len);
}
std::string getStrL16(char* mem, uint& ptr){
  uint8_t len = getUint16(mem, ptr);
  return getStr(mem, ptr, len);
}
