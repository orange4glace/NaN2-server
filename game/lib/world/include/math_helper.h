// math_helper.h
#ifndef MATH_HELPER_H_
#define MATH_HELPER_H_

#include <nan2/math/vector2.h>

namespace nan2 {

  class MathHelper {

  private:

    MathHelper();
    static MathHelper* instance_;

    Vector2 vec_normal_256_[256];


  public:

    static const MathHelper& instance();

    const Vector2& normal_dir_256(unsigned char x) const;

  };

};

#endif