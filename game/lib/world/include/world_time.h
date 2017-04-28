// world_time.h
#ifndef WORLD_TIME_H
#define WORLD_TIME_H

namespace nan2 {

  class Time {

  private:

    Time() {}

    // times will be updated every first of the update by World object.
    static float delta_time_;
    static float current_time_;
    static float fixed_delta_time_;
    static float current_fixed_time_;

  public:

    static void delta_time(float dt);
    static float delta_time();
    static void current_time(float t);
    static float current_time();
    static void fixed_delta_time(float t);
    static float fixed_delta_time();
    static void current_fixed_time(float t);
    static float current_fixed_time();

  };

};

#endif
