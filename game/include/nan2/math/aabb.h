// aabb.h
#ifndef AABB_H
#define AABB_H

#include "rect.h"

#include <math.h>

namespace nan2 {

  class AABB {

  private:

    Rect rect_;

    static float GetRayIntersectionFractionOfFirstRay(const Vector2& oA, const Vector2& eA, const Vector2& oB, const Vector2& eB);
    static float GetRayIntersectionFraction(const Rect& b, const Vector2& o, const Vector2& dir);

  public:

    AABB(const Rect& rect);

    AABB& operator=(const AABB& r);

    const Rect& rect() const;
    Rect& rect();
    void set_rect(const Rect& r);

    // Swept AABB collision detection for fast objects.
    static float SweptAABB(const AABB& r1, const Vector2& r1v, const AABB& r2, const Vector2& r2v, bool& out_collided);
    
    // Simple AABB collsion detection for regular speed objects.
    static Vector2 SimpleAABB(const AABB& r1, const AABB& r2, bool& out_collided);

    friend std::ostream& operator<<(std::ostream& os, const AABB& v);

  };

};

#endif