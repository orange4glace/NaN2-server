// from ./test
#include "test.h"

// from ./include
#include "test_class.h"

// Inlcude nan2 lib directly
#include <nan2/math/aabb.h>
#include <nan2/math/rect.h>
#include <nan2/world/entity/character.h>

// std
#include <iostream>

using namespace nan2;

int main() {
  TestClass tc(100, 200);
  std::cout << tc.vec().x() << std::endl;

  AABB aabb(Rect(0, 0, 20, 30));
  std::cout << aabb.rect().size().x() << std::endl;

  std::cout << test_h_var << std::endl;
}