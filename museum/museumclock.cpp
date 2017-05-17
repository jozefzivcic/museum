#include "museumclock.h"

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <sstream>

using namespace std;

MuseumClock::MuseumClock(): hour(0), minute(0), second(0) {}

void MuseumClock::updateClock() {
  time_t t = time(0);
  struct tm* now = localtime(&t);
  hour = now->tm_hour % 12;
  minute = now->tm_min;
  second = now->tm_sec;
}

float MuseumClock::getHourAngle() const {
  return static_cast<float>(glm::radians(-30.0f * hour - 0.1 * minute));
}

float MuseumClock::getMinuteAngle() const {
  return static_cast<float>(glm::radians(-6.0f * minute - 0.1 * second));
}

float MuseumClock::getSecondAngle() const {
  return static_cast<float>(glm::radians(-6.0f * second));
}

string MuseumClock::getTimeInString() const {
  stringstream s;
  s << hour << ":" << minute << ":" << second << endl;
  return s.str();
}
