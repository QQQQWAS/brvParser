#pragma once

#include <cstring>
#include <string>
#include <vector>
#include <cstdint>
#include <iostream>

#include "dataTypes.hpp"

struct DataToken{
  std::string name;
  tokenTypes tokenType;
  dataTypes dataType;
  std::vector<std::string> data;
  DataToken(){}
  DataToken(std::string i_name, tokenTypes i_tokenType, dataTypes i_dataType, std::vector<std::string> i_data = {}){
    name = i_name;
    tokenType = i_tokenType;
    dataType = i_dataType;
    data = i_data;
  }
  DataToken(std::string i_name, dataTypes i_dataType, void* i_data){
    name = i_name;
    tokenType = VALUE;
    dataType = i_dataType;
    uint numBytes;
    switch(dataType){
      case BOOL:
        numBytes = 1;
        break;
      case UINT8:
        numBytes = 1;
        break;
      case UINT16:
        numBytes = 2;
        break;
      case COLOR3:
        numBytes = 3;
        break;
      case COLOR4:
        numBytes = 4;
        break;
      case UINT32:
        numBytes = 4;
        break;
      case FLOAT:
        numBytes = 4;
        break;
      case VEC3:
        numBytes = 12;
        break;
      default:
        numBytes = 0;
        break;
    }
    if(numBytes == 0){
      std::cout<<"string"<<std::endl;
      std::string str = *(std::string*)i_data;
      data.push_back(str);
    }
    else{
      std::string push;
      push.resize(numBytes);
      memcpy(push.data(), i_data, numBytes);
      data.push_back(push);
    }
  }
  uint16_t getNumValues(){
    return data.size();
  }
  uint32_t getSize(){
    uint32_t size = 0;
    for(std::string &d : data){
      if(dataType == STR)
        size += 1;
      else if(dataType == LONG_STR)
        size += 2;
      size += d.length();
    }
    return size;
  }
  std::vector<uint16_t> getTail(){
    if(data.size() == 1)
      return {};
    switch(tokenType){
      case ARRAY:
        return {uint16_t(data[0].length())};
        break;
      default:
        std::vector<uint16_t> r(data.size()+1);
        r[0] = 0;
        for(uint i = 0; i < data.size(); i++){
          r[i+1] = data[i].size()+(tokenType==STR_ARRAY?1:2);
        }
        return r;
    }
  }

  std::string getNameStr(){
    uint8_t nameLen = uint8_t(name.length());
    return ((char*)&nameLen)+name;
  }
  bool operator==(DataToken& dt){
    bool nameEq = name == dt.name;
    bool dtEq = dataType == dt.dataType;
    bool sizeEq = data.size() == dt.data.size();
    if(!(nameEq&&dtEq)){
      return false;
    }
    if(sizeEq){
      for(int i = 0; i < data.size(); i++){
        if(data[i] != dt.data[i]){
          return false;
        }
      }
    }
    return true;
  }
  uint findData(std::string dataStr){
    for(uint i = 0; i <data.size(); i++){
      if(data[i] == dataStr){
        return i;
      }
    }
    return data.size();
  }
};
