#include <chrono>
#include <iostream>
#include <cstring>

using namespace std::chrono;

class Timer
{
  public:
    
    void start() {
      if(_started) {std::cout<<"Timer already started!"<<std::endl;}
      _startTime = system_clock::now();
      _started = true;
    }

    void stop() {
      _endTime = system_clock::now();
      _started = false;
    }
    
    bool isStarted() const { return _started; }

    float elapsedMilliseconds() {
      time_point<system_clock> endTime = (_started) ? system_clock::now() : _endTime;
      return duration_cast<milliseconds>(endTime - _startTime).count();
    }

    float elapsedSeconds() {
      return elapsedMilliseconds()/1000.0;
    }

  private:
    time_point<system_clock> _startTime;
    time_point<system_clock> _endTime;
    bool _started = false;
    
    std::string timeToString(system_clock::time_point &t)
    {
        std::time_t time = system_clock::to_time_t(t);
        std::string time_str = std::ctime(&time);
        time_str.resize(time_str.size() - 1);
        return time_str;
    }
};
