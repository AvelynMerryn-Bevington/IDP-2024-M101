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
    Right
  };

  ::std::vector<Direction> FetchRoute(Node, Node);

private:
  ::std::map<::std::pair<Node, Node>, ::std::vector<Direction>> DirectionMap 
  {
//  | StartNode | EndNode            | Move 1 | Move 2 | Move 3 | Move 4 | Move 5 | Move 6 | Move 7 | Move 8 | Move 9 | ...
//  | --------- | ------------------ | ------ | ------ | ------ | ------ | ------ | ------ | ------ | ------ | ------ | ...
    {{Start,      Factory1         }, {Forward, Forward, Left,    Forward, Right,   Forward, Right,   Forward, Right}},
    {{Factory1,   Box4Middle       }, {Left,    Forward, Left,    Forward, Left,    Forward}},
    {{Factory1,   ContaminationSite}, {Right,   Right,   Forward, Forward}},
    {{Factory1,   Box2Right        }, {Right,   Right,   Forward, Right}},
    {{Factory1,   Box3Left         }, {Left,    Forward, Left,    Left}},
    {{Factory1,   Factory2         }, {Left,    Left,    Right}},
    {{Factory1,   Box1Right        }, {Right,   Right,   Right}},
    {{Factory1,   Box1Left         }, {Left,    Left,    Left}},
    {{Factory1,   Box3Right        }, {Left,    Left,    Right}},
    {{Box4Middle, Start            }, {Right,   Forward}}
  };
};

#endif // MAPPING_H
