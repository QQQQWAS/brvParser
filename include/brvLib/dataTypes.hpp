#pragma once

enum tokenTypes{
  VALUE,     // does not have a tail
  ARRAY,     // tail of length 2
  STR_ARRAY, // tail of length 2 + 2*numElements;
};

enum dataTypes{
  BOOL,
  UINT8,
  UINT16,
  COLOR3,
  COLOR4,
  UINT32,
  FLOAT,
  VEC3,
  STR,
  LONG_STR,
}; //good luck lol
