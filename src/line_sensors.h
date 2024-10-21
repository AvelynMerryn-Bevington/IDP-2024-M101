#ifndef LINE_SENSORS_H
#define LINE_SENSORS_H

class LineSensors
{
public:
  enum Background
  {
    Black,
    White
  };

  enum Location
  {
    BackLeft,
    BackRight,
    FrontLeft,
    FrontRight
  };

  LineSensors();
  Background Read(Location loc);
};

#endif // LINE_SENSORS_H
