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
    IsolatedBox1Top,
    IsolatedBox2Bottom,
    ContaminationSite,
    Box1Left,
    Box1Right,
    Box2Left,
    Box2Right,
    Box3Left,
    Box3Right,
    Box4Right,
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
//  | StartNode | EndNode             | Move 1 | Move 2 | Move 3 | Move 4 | Move 5 | Move 6 | Move 7 | Move 8 | Move 9 | Move 10| ...
//  | --------- | ------------------- | ------ | ------ | ------ | ------ | ------ | ------ | ------ | ------ | ------ | ------ | ...
//    {{Start,      Factory1          }, {Forward, Forward, Right,   Forward, Forward, Forward, Left,    Forward, Left,    Left}},
    {{Start,      Factory1          }, {Left}},
    {{Factory1,   ContaminationSite }, {Right,   Right,   Forward, Forward}},
    {{Factory1,   Box1Left          }, {Left,    Left,    Forward, Left,    Forward}},
    {{Factory1,   Box1Right         }, {Right,   Right,   Right,   Forward}},
    {{Factory1,   Box2Left          }, {Left,    Forward, Left,    Left,    Forward}},
    {{Factory1,   Box2Right         }, {Left,    Left,    Forward, Right,   Forward}},
    {{Factory1,   Box3Left          }, {Left,    Forward, Left,    Forward, Left,    Forward, Forward, Forward, Forward}},
    {{Factory1,   Box3Right         }, {Right,   Right,   Forward, Right,   Forward}},
    {{Factory1,   Box4Middle        }, {Left,    Forward, Left,    Forward, Left,    Forward, Forward}},
    {{Box2Left,   Start             }, {Left,    Left,    Forward, Right,   Forward, Forward}}
  };
};

#endif // MAPPING_H
