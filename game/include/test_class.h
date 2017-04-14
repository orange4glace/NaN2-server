#ifndef _NAN2_TEST_CLASS_H_
#define _NAN2_TEST_CLASS_H_

#include <nan2/math/vector2.h>

namespace nan2 {

  class TestClass {

  private:
    Vector2 vec_;

  public:
    TestClass(float x, float y);
    const Vector2& vec() const;

  };

}

#endif