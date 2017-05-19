// updatable.h
#ifndef UPDATABLE_H
#define UPDATABLE_H

#include "entity.h"
#include "../world_time.h"

namespace nan2 {

  class Updatable : public Entity {

  private:
    bool is_on_stage_;

  protected:

    int update_order_;
    // time = MAX(0, time - dt)
    inline void DecreaseByDT(int& time) {
      time -= Time::delta_time();
      if (time < 0) time = 0;
    }

  public:

    Updatable(World* world);

    virtual void Destroy();
    virtual void Update() {};
    virtual void FixedUpdate() {};
    int update_order() const;
    Updatable& set_is_on_stage(bool v);
    bool is_on_stage() const;

  };
  
}

#endif
