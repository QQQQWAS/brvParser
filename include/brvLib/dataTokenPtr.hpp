#pragma once

#include <string>
#include <vector>
#include <cstdint>

#include "brvLib/dataTypes.hpp"
#include "dataToken.hpp"

struct DataTokenPtr{
  std::vector<DataToken>* vectorPtr;
  uint16_t tokenIdx;
  uint16_t valueIdx;
  DataTokenPtr(){}
  DataTokenPtr(std::vector<DataToken>* i_vec, uint16_t i_tix, uint16_t i_vix){
    vectorPtr = i_vec;
    tokenIdx = i_tix;
    valueIdx = i_vix;
  }
  std::string getName(){
    return vectorPtr->at(tokenIdx).name;
  }
  std::string* strPtr(){
    return &vectorPtr->at(tokenIdx).data[valueIdx];
  }
  char* data(){
    return vectorPtr->at(tokenIdx).data[valueIdx].data();
  }
  uint32_t dataSize(){
    return vectorPtr->at(tokenIdx).data[valueIdx].size();
  }
  dataTypes dataType(){
    return vectorPtr->at(tokenIdx).dataType;
  }
  tokenTypes tokenType(){
    return vectorPtr->at(tokenIdx).tokenType;
  }
  bool operator==(DataTokenPtr& d){
    return vectorPtr==d.vectorPtr && tokenIdx==d.tokenIdx && valueIdx==d.valueIdx;
  }
};
