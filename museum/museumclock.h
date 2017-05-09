#ifndef MUSEUMCLOCK_H
#define MUSEUMCLOCK_H

#include <iostream>

class MuseumClock {
private:
  int hour;
  int minute;
  int second;
public:
  MuseumClock();
  void updateClock();
  float getHourAngle() const;
  float getMinuteAngle() const;
  float getSecondAngle() const;
  std::string getTimeInString() const;
};
#endif //MUSEUMCLOCK_H
