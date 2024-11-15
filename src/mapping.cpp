#include "mapping.h"
#include <Arduino.h>
#include <Arduino_LSM6DS3.h>
#include "DFRobot_VL53L0X.h"
#include "ArxContainer.h"

::std::vector<Mapping::Direction> Mapping::FetchRoute(Mapping::Node StartNode, Mapping::Node EndNode)
{
  switch (StartNode)
  {
  case Node::Start:
  case Node::Factory1:
  case Node::Box2Left:
    return DirectionMap[::std::pair<Node, Node>{StartNode, EndNode}];

  case Node::ContaminationSite:
    

  default:
    const ::std::vector<Direction> OutwardRoute = DirectionMap[::std::pair<Node, Node>{EndNode, StartNode}];
    ::std::vector<Direction> returnRoute;

    // For the contamination site, don't ignore the first direction
    if (StartNode == Mapping::Node::ContaminationSite)
      returnRoute.push_back(OutwardRoute[OutwardRoute.size() - 1]);

    // size() - 2 ignores the first direction
    for (int i = static_cast<int>(OutwardRoute.size() - 2); i >= 0; i--)
    {
      if (OutwardRoute[i] == Right)
        returnRoute.push_back(Left);
      else if (OutwardRoute[i] == Left)
        returnRoute.push_back(Right);
      else
        returnRoute.push_back(OutwardRoute[i]);
    }
    return returnRoute;
  }
}
