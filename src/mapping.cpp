#include "mapping.h"
#include <Arduino.h>
#include <Arduino_LSM6DS3.h>
#include "DFRobot_VL53L0X.h"
#include "ArxContainer.h"

std::array<Mapping::Direction, 10> Mapping::FetchRoute(Mapping::Node StartNode, Mapping::Node EndNode)
//This function fetches the route from Start Node to End Node from the DirectionMap and returns it
//If the Map stores the route in the wrong direction we switch all Lefts with Rights and all Rights with Lefts
{
    std::array<Mapping::Direction, 10> Route;
    if (StartNode == Mapping::Node::Start){
        Route = DirectionMap[std::array<Mapping::Node, 2>{StartNode, EndNode}];
    } 
    else if (StartNode == Mapping::Node::Factory1){
        Route = DirectionMap[std::array<Mapping::Node, 2>{StartNode, EndNode}];
    } 
    else if (StartNode == Mapping::Node::Box4Middle){
        Route = DirectionMap[std::array<Mapping::Node, 2>{StartNode, EndNode}];
    } else{
        Mapping::Node temp = EndNode;
        EndNode = StartNode;
        StartNode = EndNode;
        Route = DirectionMap[std::array<Mapping::Node, 2>{StartNode, EndNode}];
        for (int i = 0; i < 10; i++){
            if (Route[i] == Left){
                Route[i] = Right;
            } else if(Route[i] == Right){
                Route[i] == Left;
            }
        }
    }
    return DirectionMap[std::array<Mapping::Node, 2>{StartNode, EndNode}];
}
