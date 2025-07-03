#include <glm/ext/vector_float3.hpp>
#include <readFile/readFile.hpp>
#include <brvLib/vehicle.hpp>

int main(){
  Vehicle vehicle(readFile("Vehicle.brv"));

  float k = 0.5;

  std::vector<DataTokenPtr> vec3Ptrs;
  std::vector<DataTokenPtr> vec2Ptrs;
  std::vector<DataTokenPtr> floatPtrs;

  for(int i = 0; i < vehicle.bricks.size(); i++){
    vehicle.bricks[i].pos *= k;
    for(int j = 0; j < vehicle.bricks[i].data.size(); j++){
      std::string name = vehicle.bricks[i].data[j].getName();
      if(name == "BrickSize"){
        for(int k = 0; k < vec3Ptrs.size(); k++){
          if(vec3Ptrs[k].vectorPtr == vehicle.bricks[i].data[j].vectorPtr && vec3Ptrs[k].tokenIdx == vehicle.bricks[i].data[j].tokenIdx && vec3Ptrs[k].valueIdx == vehicle.bricks[i].data[j].valueIdx){
            goto next_brick;
          }
        }
        vec3Ptrs.push_back(vehicle.bricks[i].data[j]);
      }
      else if(name == "SpinnerRadius" || name == "SpinnerSize"){
        for(int k = 0; k < vec2Ptrs.size(); k++){
          if(vec2Ptrs[k].vectorPtr == vehicle.bricks[i].data[j].vectorPtr && vec2Ptrs[k].tokenIdx == vehicle.bricks[i].data[j].tokenIdx && vec2Ptrs[k].valueIdx == vehicle.bricks[i].data[j].valueIdx){
            goto next_brick;
          }
        }
        vec2Ptrs.push_back(vehicle.bricks[i].data[j]);
      }
      else if (name == "WheelDiameter" || name == "WheelWidth"){
        for(int k = 0; k < floatPtrs.size(); k++){
          if(floatPtrs[k].vectorPtr == vehicle.bricks[i].data[j].vectorPtr && floatPtrs[k].tokenIdx == vehicle.bricks[i].data[j].tokenIdx && floatPtrs[k].valueIdx == vehicle.bricks[i].data[j].valueIdx){
            goto next_brick;
          }
        }
        floatPtrs.push_back(vehicle.bricks[i].data[j]);
      }
    }
    next_brick:
  }

  for(int i = 0; i < vec3Ptrs.size(); i++){
    float* sxw = (float*)vec3Ptrs[i].data();
    float* syw = (float*)vec3Ptrs[i].data()+1;
    float* szw = (float*)vec3Ptrs[i].data()+2;

    *sxw *= k;
    *syw *= k;
    //*szw *= k;
  }

  for(int i = 0; i < vec2Ptrs.size(); i++){
    float* sxw = (float*)vec2Ptrs[i].data();
    float* syw = (float*)vec2Ptrs[i].data()+1;

    *sxw *= k;
    *syw *= k;
  }

  for(int i = 0; i < floatPtrs.size(); i++){
    float* sxw = (float*)floatPtrs[i].data();

    *sxw *= k;
  }

  vehicle.exportToFile("Vehicle.brv");
}
