#include <fstream>
#include <iostream>

#include "readFile.hpp"

std::string readFile(std::string path){
  std::ifstream file(path);
  if(!file){
    std::cerr<<"File at path "<<path<<" not found"<<std::endl;
    return "";
  }
  std::string line, text;
  while (std::getline(file, line)) {
    text.append(line + "\n");
  }
  file.close();
  return text;
}
