#pragma once

#include <iostream>
#include <cstdint>
#include <string>
#include <vector>
#include <fstream>

#include "brick.hpp"
#include "brvLib/dataTokenPtr.hpp"
#include "brvLib/dataTypes.hpp"
#include "dataToken.hpp"
#include <serial/deserialize.hpp>
#include <serial/serialize.hpp>

struct Vehicle{
  uint8_t gameVersion = 14;

  std::vector<Brick> bricks;
  std::vector<DataToken> dataTokens;
  Vehicle(){}
  Vehicle(std::string brv, bool v = false){
    if(v)std::cout<<"Starting import"<<std::endl;
    char* cstr = brv.data();
    uint ptr = 0;

    gameVersion = getUint8(cstr, ptr);
    uint16_t numBricksUsed = getUint16(cstr, ptr);
    uint16_t numBrickTypes = getUint16(cstr, ptr);
    uint16_t numDataTokens = getUint16(cstr, ptr);

    // brickTypes
    if(v)std::cout<<"Getting brick types"<<std::endl;
    std::vector<std::string> brickTypes(numBrickTypes);
    for(int i = 0; i <numBrickTypes; i++){
      brickTypes[i] = getStrL(cstr, ptr);
      if(v)std::cout<<" adding "<<brickTypes[i]<<std::endl;
    }

    // dataTokens
    if(v)std::cout<<"Getting data tokens"<<std::endl;
    dataTokens.resize(numDataTokens);
    for(int i = 0; i < numDataTokens; i++){
      DataToken* dt = &dataTokens[i];

      dt->name = getStrL(cstr, ptr);
      if(v)std::cout<<" Adding "<<dt->name<<std::endl;

      uint16_t numElements = getUint16(cstr, ptr);
      uint32_t dataSize = getUint32(cstr, ptr);
      if(v)std::cout<<"  numElements: "<<numElements<<std::endl;
      if(v)std::cout<<"  dataSize:    "<<dataSize<<std::endl;
      dt->data.resize(numElements);

      // deduce type
      if(numElements == 1){
        dt->tokenType = VALUE;
        if(v)std::cout<<"  Token type: value"<<std::endl;
      }
      else{
        uint tmpPtr = ptr + dataSize;
        uint16_t elementSize = getUint16(cstr, tmpPtr);
        if(dataSize == elementSize * numElements){
          dt->tokenType = ARRAY;
          if(v)std::cout<<"  Token type: array"<<std::endl;
        }
        else{
          dt->tokenType = STR_ARRAY;
          if(v)std::cout<<"  Token type: string array"<<std::endl;
        }
      }

      //deduce data type
      if(dt->tokenType == STR_ARRAY){
        dt->dataType = STR;
        if(v)std::cout<<"  Data type: string"<<std::endl;
      }
      else{
        if(dt->tokenType == VALUE && getUint8(cstr, ptr, 0) == dataSize-1){
          dt->dataType = STR;
          if(v)std::cout<<"  Data type: string"<<std::endl;
        }
        else if(dt->tokenType == VALUE && getUint16(cstr, ptr, 0) == dataSize-2){
          dt->dataType = LONG_STR;
          if(v)std::cout<<"  Data type: long string"<<std::endl;
        }
        uint16_t elementSize = dataSize / numElements;
        switch(elementSize){
          case 1:
            if(dt->name[0] == 'b'){
              dt->dataType = BOOL;
              if(v)std::cout<<"  Data type: bool"<<std::endl;
            }
            else{
              dt->dataType = UINT8;
              if(v)std::cout<<"  Data type: uint8"<<std::endl;
            }
            break;
          case 2:
            dt->dataType = UINT16;
            if(v)std::cout<<"  Data type: uint16"<<std::endl;
            break;
          case 3:
            dt->dataType = UINT16;
            if(v)std::cout<<"  Data type: color3"<<std::endl;
            break;
          case 4:{
            if(dt->name.find("Color") != std::string::npos){
              dt->dataType = COLOR4;
              if(v)std::cout<<"  Data type: color4"<<std::endl;
              break;
            }
            //std::cout<<"  ohno it has 4 bytes!!!!!!!!!!!!!!!!!!!!!!!!"<<std::endl;
            std::vector<float> floatCandidates(numElements);
            //std::vector<uint32_t> uint32Candidates(numElements);
            uint tmpPtr = ptr;
            //search for bad things
            bool found = false;
            for(int j = 0; j < numElements; j++){
              floatCandidates[j] = getFloat(cstr, tmpPtr);
              if(isnanf(floatCandidates[j]) || isinff(floatCandidates[j])){
                found = true;
                dt->dataType = UINT32;
                if(v)std::cout<<"  Data type: uint32"<<std::endl;
                break;
              }
            }
            if(found == false){
              dt->dataType = FLOAT;
              if(v)std::cout<<"  Data type: float"<<std::endl;
            }
            break;
          }
          case 12:
            dt->dataType = VEC3;
            if(v)std::cout<<"  Data type: vec3"<<std::endl;
            break;
        }
      }

      uint16_t elementSizeArray = dataSize / numElements;

      // read data
      for(int j = 0; j < numElements; j++){
        if(dt->dataType == STR){
          dt->data[j] = getStrL(cstr, ptr);

        }
        else if(dt->dataType == LONG_STR){
          dt->data[j] = getStrL16(cstr, ptr);
        }
        else{
          dt->data[j] = getStr(cstr, ptr, elementSizeArray);
        }
        if(v)std::cout<<"  Read: "<<dt->data[j]<<std::endl;
      }

      // not now tho
      //ptr += dataSize;

      switch(dt->tokenType){
        case VALUE:
          break;
        case ARRAY:
          ptr += 2;
          break;
        case STR_ARRAY:
          ptr += 2 + 2*numElements;
          break;
      }
    }

    //bricks
    bricks.resize(numBricksUsed);
    for(int i = 0; i < numBricksUsed; i++){
      Brick* br = &bricks[i];
      uint16_t id = getUint16(cstr, ptr);
      uint32_t size = getUint32(cstr, ptr);
      uint8_t dtHeadSize = getUint8(cstr, ptr);

      br->name = brickTypes[id];
      if(v)std::cout<<" Adding "<<br->name<<", size "<<size<<std::endl;
      br->data.resize(dtHeadSize);

      for(int j = 0; j < dtHeadSize; j++){
        br->data[j].tokenIdx = getUint16(cstr, ptr);
        br->data[j].valueIdx = getUint16(cstr, ptr);
        br->data[j].vectorPtr = &dataTokens;
        if(v)std::cout<<"  Data token "<<br->data[j].getName()<<std::endl;
      }
      br->pos.x = getFloat(cstr, ptr);
      br->pos.y = getFloat(cstr, ptr);
      br->pos.z = getFloat(cstr, ptr);
      br->rot.x = getFloat(cstr, ptr);
      br->rot.y = getFloat(cstr, ptr);
      br->rot.z = getFloat(cstr, ptr);
      if(v)std::cout<<"  Position "<<br->pos.x<<" "<<br->pos.y<<" "<<br->pos.z<<std::endl;
      if(v)std::cout<<"  Rotation "<<br->rot.x<<" "<<br->rot.y<<" "<<br->rot.z<<std::endl;
    }
  }
  void exportToFile(std::string filePath, bool v = false){
    if(v)std::cout<<"Starting export"<<std::endl;
    std::ofstream output(filePath);
    output.write((const char*)&gameVersion, 1);

    uint16_t numBricksUsed = bricks.size();
    std::vector<std::string> brickTypes;
    for(Brick& br : bricks){
      bool found = false;
      for(std::string& s : brickTypes){
        if(br.name == s){
          found = true;
          break;
        }
      }
      if(!found){
        brickTypes.push_back(br.name);
      }
    }
    uint16_t numBrickTypes = brickTypes.size();
    uint16_t numDataTokens = dataTokens.size();
    output.write((const char*)&numBricksUsed, 2);
    output.write((const char*)&numBrickTypes, 2);
    output.write((const char*)&numDataTokens, 2);

    for(std::string& s : brickTypes){
      output<<(char)s.length()<<s;
    }

    for(DataToken& dt : dataTokens){
      output<<(char)dt.name.length()<<dt.name;
      uint16_t numElements = dt.data.size();
      output.write((const char*)&numElements, 2);

      uint32_t dataSize = dt.getSize();
      output.write((const char*)&dataSize, 4);

      for(std::string& d : dt.data){
        if(dt.dataType == STR){
          uint8_t len = d.length();
          output.write((const char*)&len, 1);
        }
        else if(dt.dataType == LONG_STR){
          uint16_t len = d.length();
          output.write((const char*)&len, 2);
        }
        output<<d;
      }

      std::vector<uint16_t> tail = dt.getTail();
      for(uint16_t& t : tail){
        output.write((const char*)&t, 2);
      }
    }

    for(Brick &br : bricks){
      uint16_t id = 0;
      for(std::string& s : brickTypes){
        if(br.name == s){
          break;
        }
        id++;
      }
      output.write((const char*)&id, 2);
      uint8_t dtHeadSize = br.data.size();
      uint32_t size = 1+6*4+dtHeadSize*4;
      output.write((const char*)&size, 4);
      output.write((const char*)&dtHeadSize, 1);
      for(DataTokenPtr& dtp : br.data){
        output.write((const char*)&dtp.tokenIdx, 2);
        output.write((const char*)&dtp.valueIdx, 2);
      }
      output.write((const char*)&br.pos.x, 4);
      output.write((const char*)&br.pos.y, 4);
      output.write((const char*)&br.pos.z, 4);
      output.write((const char*)&br.rot.x, 4);
      output.write((const char*)&br.rot.y, 4);
      output.write((const char*)&br.rot.z, 4);
    }
    output<<(char)0<<(char)0;
  }
  uint dataTokenPos(DataToken& dt){
    for(uint i = 0; i < dataTokens.size(); i++){
      if(dt == dataTokens[i]){
        return i;
      }
    }
    return dataTokens.size();
  }
  Brick* addBrick(Brick b){
    bricks.push_back(b);
    return &bricks[bricks.size()-1];
  }
  Brick* addBrick(Brick& b){
    bricks.push_back(b);
    return &bricks[bricks.size()-1];
  }
  void addDataTokenToBrick(DataToken dt, uint brickId){
    std::cout<<brickId<<std::endl;
    if(dt.tokenType != VALUE){
      std::cerr<<"Incompatible token type"<<std::endl;
      return;
    }
    uint dtPos = dataTokenPos(dt);
    if(dtPos == dataTokens.size()){
      dataTokens.push_back(dt);
      DataTokenPtr dtPtr(&dataTokens, dataTokens.size()-1, 0);
      bricks[brickId].data.push_back(dtPtr);
    }
    else{
      DataToken* dtp = &dataTokens[dtPos];
      uint dataPos = dtp->findData(dt.data[0]);

      uint tokenIdx, valueIdx;

      if(dataPos == dtp->data.size()){
        if(dtp->tokenType == VALUE){
          if(dtp->dataType == STR || dtp->dataType == LONG_STR)
            dtp->tokenType = STR_ARRAY;
          else
            dtp->tokenType = ARRAY;
        }
        dtp->data.push_back(dt.data[0]);
        tokenIdx = dtPos;
        valueIdx = dtp->data.size()-1;
      }
      else{
        tokenIdx = dtPos;
        valueIdx = dataPos;
      }

      bricks[brickId].data.push_back(DataTokenPtr(&dataTokens, tokenIdx, valueIdx));
    }
  }
};
