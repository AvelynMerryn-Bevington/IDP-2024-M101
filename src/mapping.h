#ifndef MAPPING_H
#define MAPPING_H
#include "ArxContainer.h"

class Mapping
{
public:
  enum Node
  {
    Start, 
    Factory1,
    Factory2,
    ContaminationSite,
    IsolatedBox1Top,
    IsolatedBox2Bottom,
    Box1Left,
    Box1Right,
    Box2Left,
    Box2Right,
    Box3Left,
    Box3Right,
    Box4Middle,
  };

  enum Direction
  {
    Forward,
    Left,
    Right,
    End
  };

  std::array<Mapping::Direction, 10> FetchRoute(Mapping::Node, Mapping::Node);

private:
  std::map<std::array<Mapping::Node, 2>, std::array<Mapping::Direction, 10>> DirectionMap 
  { 
    { {Start, Factory1}, {Forward, Forward, Left, Forward, Right, Forward, Right, Forward, Right, End}},
    //{ {Start, Factory2}, {Forward, Left, Forward,Right, Right, Forward, Forward, Left, Left, End}},
    { {Factory1, Factory2}, {Left, Left, Right, End, End, End, End, End, End, End}},
    { {Factory1, ContaminationSite}, {Right, Right, Forward, Forward, End, End, End, End, End, End}},
    { {Factory1, Box1Right}, {Right, Right, Right, End, End, End, End, End, End, End}},
    { {Factory1, Box1Left}, {Left, Left, Left, End, End, End, End, End, End, End}},
    { {Factory1, Box2Right}, {Right, Right, Forward, Right, End, End, End, End, End, End}},
    { {Factory1, Box3Right}, {Left, Left, Right, End, End, End, End, End, End, End}},
    { {Factory1, Box3Left}, {Left, Forward, Left, Left, End, End, End, End, End, End}},
    { {Factory1, Box4Middle}, {Left, Forward, Left, Forward, Left, Forward, End, End, End, End}},
    { {Box4Middle, Start}, {Right, Forward, End, End, End, End, End, End, End, End}}
  };
};

#endif // MAPPING_H
