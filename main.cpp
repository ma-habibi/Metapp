#include "yaml-cpp/yaml.h"
#include <iostream>

int main()
{
  YAML::Node config = YAML::LoadFile("hello_world.yaml");
  const std::string hello = config["hello_world"].as<std::string>();
  std::cout << hello << std::endl;
  return 0;
}
