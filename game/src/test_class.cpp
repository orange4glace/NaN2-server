#include "test_class.h"

namespace nan2 {

  TestClass::TestClass(float x, float y) :
    vec_(x, y) {

  }

  const Vector2& TestClass::vec() const {
    return vec_;
  }

}