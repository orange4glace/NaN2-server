// aabb.cpp
#include "../include/aabb.h"

#include <math.h>

#include <iostream>

namespace nan2 {

  AABB::AABB(const Rect& rect) :
  	rect_(rect) {
  }

  AABB& AABB::operator=(const AABB& ab) {
  	if (this == &ab) return *this;
  	rect_ = ab.rect();
  	return *this;
  }

  const Rect& AABB::rect() const {
  	return rect_;
  }

  Rect& AABB::rect() {
  	return rect_;
  }

  void AABB::set_rect(const Rect& r) {
  	rect_ = r;
  }

  float AABB::SweptAABB(const AABB& src, const Vector2& srcDelta, const AABB& tar, const Vector2& tarDelta, bool& out_collided) {
    out_collided = false;
    Rect minkowski = tar.rect().MinkowskiDifference(src.rect());
    Vector2 relative_delta = srcDelta - tarDelta;

    float h = AABB::GetRayIntersectionFraction(minkowski, Vector2::ZERO, relative_delta);
    if (h < INFINITY) {
      out_collided = true;
      return h;
    }
    else return 1;
  }

  Vector2 AABB::SimpleAABB(const AABB& src, const AABB& tar, bool& out_collided) {
    Rect minkowski = tar.rect().MinkowskiDifference(src.rect());
    if (!minkowski.ContainsPoint(Vector2::ZERO)) {
      out_collided = false;
      return Vector2::ZERO;
    }
    out_collided = true;
    return minkowski.ClosestPointOnBoundsToPoint(Vector2::ZERO);
  }

  float AABB::GetRayIntersectionFractionOfFirstRay(const Vector2& oA, const Vector2& eA, const Vector2& oB, const Vector2& eB) {
    Vector2 r = eA - oA;
    Vector2 s = eB - oB;
    Vector2 oAoB = oB - oA;

    float numerator = oAoB.cross(r);
    float denominator = r.cross(s);

    if (numerator == 0 && denominator == 0) {
      // Colinear
      return INFINITY;
    }
    if (denominator == 0) {
      // parallel
      return INFINITY;
    }

    float u = numerator / denominator;
    float t = oAoB.cross(s) / denominator;

    if ((t >= 0) && (t <= 1) && (u >= 0) && (u <= 1)) return t;
    return INFINITY;
  }

  float AABB::GetRayIntersectionFraction(const Rect& b, const Vector2& o, const Vector2& dir) {
    Vector2 e = o + dir;
    Vector2 b_ll = b.center() - b.size() / 2;
    Vector2 b_ur = b.center() + b.size() / 2;

    float minv = AABB::GetRayIntersectionFractionOfFirstRay(o, e, Vector2(b_ll.x(), b_ll.y()), Vector2(b_ll.x(), b_ur.y()));
    float x;
    x = AABB::GetRayIntersectionFractionOfFirstRay(o, e, Vector2(b_ll.x(), b_ur.y()), Vector2(b_ur.x(), b_ur.y()));
    if (x < minv) minv = x;
    x = AABB::GetRayIntersectionFractionOfFirstRay(o, e, Vector2(b_ur.x(), b_ur.y()), Vector2(b_ur.x(), b_ll.y()));
    if (x < minv) minv = x;
    x = AABB::GetRayIntersectionFractionOfFirstRay(o, e, Vector2(b_ur.x(), b_ll.y()), Vector2(b_ll.x(), b_ll.y()));
    if (x < minv) minv = x;
    return minv;
  }

  std::ostream& operator<<(std::ostream& os, const AABB& aabb) {
      os << "[AABB] " << aabb.rect_;
      return os;
  }
}