#include <iostream>

#include <nan2/math/vector2.h>
#include <nan2/math/rect.h>

using namespace nan2;
using namespace std;

int main() {

  Vector2 vec;
  Rect rect(0, 0, 3, 5);

  Vector2* vp = &vec;
  Rect* rp = &rect;

  void* void_ptr = &vec;

  cout << vp << endl << void_ptr << endl;

}