// updatable.h
#ifndef UPDATABLE_H
#define UPDATABLE_H

#include "entity.h"
#include "../world_time.h"

namespace nan2 {

  class Updatable : public Entity {

  private:
    static int next_internal_id_;

    int internal_id_;
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

    int internal_id() const;

    virtual void Destroy();
    virtual void Update() {};
    virtual void FixedUpdate() {};
    int update_order() const;
    Updatable& set_is_on_stage(bool v);
    bool is_on_stage() const;

  };
  
}

#endif
