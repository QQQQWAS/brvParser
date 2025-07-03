#pragma once

#include <glm/glm.hpp>

#include "dataTokenPtr.hpp"

struct Brick{
  std::string name;
  glm::vec3 pos;
  glm::vec3 rot;
  std::vector<DataTokenPtr> data;
  Brick(){}
  Brick(std::string i_name, glm::vec3 i_pos = glm::vec3(0), glm::vec3 i_rot = glm::vec3(0)){
    name = i_name;
    pos = i_pos;
    rot = i_rot;
  }
  bool hasDataTokenPtr(DataTokenPtr dt){
    for(DataTokenPtr& d :data){
      if(d == dt){
        return true;
      }
    }
    return true;
  }
};
