#pragma once

#include <string>
#include <cstdint>

char getChar(char* mem, uint& ptr, bool s = 1);
uint8_t getUint8(char* mem, uint& ptr, bool s = 1);
uint16_t getUint16(char* mem, uint& ptr, bool s = 1);
uint32_t getUint32(char* mem, uint& ptr, bool s = 1);

float getFloat(char* mem, uint& ptr, bool s = 1);
std::string getStr(char* mem, uint& ptr, uint len, bool s = 1);
std::string getStrL(char* mem, uint& ptr);
std::string getStrL16(char* mem, uint& ptr);
