// math_helper.cpp
#include "math_helper.h"

namespace nan2 {
  
  const MathHelper& MathHelper::instance() {
    if (instance_ == nullptr) {
      instance_ = new MathHelper();
      float angle = 0;
      float dangle = 360 / 256.0f;
      for (int i = 0; i < 256; i ++) {
        Vector2 nv = Vector2::FromAngle(angle);
        (*instance_).vec_normal_256_[i] = nv;
        angle += dangle;
      }
    }
  }

  const Vector2& MathHelper::normal_dir_256(unsigned char x) const {
    return vec_normal_256_[x];
  }

}