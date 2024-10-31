#include "mapping.h"
#include <Arduino.h>
#include <Arduino_LSM6DS3.h>
#include "DFRobot_VL53L0X.h"
#include "ArxContainer.h"

std::array<Mapping::Direction, 10> Mapping::FetchRoute(Mapping::Node StartNode, Mapping::Node EndNode)
{
  switch (StartNode)
  {
  case Mapping::Node::Start:
  case Mapping::Node::Factory1:
  case Mapping::Node::Box4Middle:
    return DirectionMap[std::array<Mapping::Node, 2>{StartNode, EndNode}];

  default:
    std::array<Mapping::Direction, 10> returnRoute = DirectionMap[std::array<Mapping::Node, 2>{EndNode, StartNode}];
    for (int i = 0; i < 10; i++)
    {
      if (returnRoute[i] == Left)
        returnRoute[i] = Right;
      else if (returnRoute[i] == Right)
        returnRoute[i] = Left;
    }
    return returnRoute;
  }
}
