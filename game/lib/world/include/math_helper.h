// math_helper.h
#ifndef MATH_HELPER_H_
#define MATH_HELPER_H_

#include <nan2/math/vector2.h>

namespace nan2 {

  class MathHelper {

  private:

    MathHelper();
    static MathHelper* instance_;

    Vector2 vec_normal_252_[252];
    float   normal_angle_252_[252] = {0.0f, 1.42857f, 2.85714f, 4.28571f, 5.71429f, 7.14286f, 8.57143f, 10.0f, 11.42857f, 12.85714f, 14.28571f, 15.71429f, 17.14286f, 18.57143f, 20.0f, 21.42857f, 22.85714f, 24.28571f, 25.71429f, 27.14286f, 28.57143f, 30.0f, 31.42857f, 32.85714f, 34.28571f, 35.71429f, 37.14286f, 38.57143f, 40.0f, 41.42857f, 42.85714f, 44.28571f, 45.71429f, 47.14286f, 48.57143f, 50.0f, 51.42857f, 52.85714f, 54.28571f, 55.71429f, 57.14286f, 58.57143f, 60.0f, 61.42857f, 62.85714f, 64.28571f, 65.71429f, 67.14286f, 68.57143f, 70.0f, 71.42857f, 72.85714f, 74.28571f, 75.71429f, 77.14286f, 78.57143f, 80.0f, 81.42857f, 82.85714f, 84.28571f, 85.71429f, 87.14286f, 88.57143f, 90.0f, 91.42857f, 92.85714f, 94.28571f, 95.71429f, 97.14286f, 98.57143f, 100.0f, 101.42857f, 102.85714f, 104.28571f, 105.71429f, 107.14286f, 108.57143f, 110.0f, 111.42857f, 112.85714f, 114.28571f, 115.71429f, 117.14286f, 118.57143f, 120.0f, 121.42857f, 122.85714f, 124.28571f, 125.71429f, 127.14286f, 128.57143f, 130.0f, 131.42857f, 132.85714f, 134.28571f, 135.71429f, 137.14286f, 138.57143f, 140.0f, 141.42857f, 142.85714f, 144.28571f, 145.71429f, 147.14286f, 148.57143f, 150.0f, 151.42857f, 152.85714f, 154.28571f, 155.71429f, 157.14286f, 158.57143f, 160.0f, 161.42857f, 162.85714f, 164.28571f, 165.71429f, 167.14286f, 168.57143f, 170.0f, 171.42857f, 172.85714f, 174.28571f, 175.71429f, 177.14286f, 178.57143f, 180.0f, 181.42857f, 182.85714f, 184.28571f, 185.71429f, 187.14286f, 188.57143f, 190.0f, 191.42857f, 192.85714f, 194.28571f, 195.71429f, 197.14286f, 198.57143f, 200.0f, 201.42857f, 202.85714f, 204.28571f, 205.71429f, 207.14286f, 208.57143f, 210.0f, 211.42857f, 212.85714f, 214.28571f, 215.71429f, 217.14286f, 218.57143f, 220.0f, 221.42857f, 222.85714f, 224.28571f, 225.71429f, 227.14286f, 228.57143f, 230.0f, 231.42857f, 232.85714f, 234.28571f, 235.71429f, 237.14286f, 238.57143f, 240.0f, 241.42857f, 242.85714f, 244.28571f, 245.71429f, 247.14286f, 248.57143f, 250.0f, 251.42857f, 252.85714f, 254.28571f, 255.71429f, 257.14286f, 258.57143f, 260.0f, 261.42857f, 262.85714f, 264.28571f, 265.71429f, 267.14286f, 268.57143f, 270.0f, 271.42857f, 272.85714f, 274.28571f, 275.71429f, 277.14286f, 278.57143f, 280.0f, 281.42857f, 282.85714f, 284.28571f, 285.71429f, 287.14286f, 288.57143f, 290.0f, 291.42857f, 292.85714f, 294.28571f, 295.71429f, 297.14286f, 298.57143f, 300.0f, 301.42857f, 302.85714f, 304.28571f, 305.71429f, 307.14286f, 308.57143f, 310.0f, 311.42857f, 312.85714f, 314.28571f, 315.71429f, 317.14286f, 318.57143f, 320.0f, 321.42857f, 322.85714f, 324.28571f, 325.71429f, 327.14286f, 328.57143f, 330.0f, 331.42857f, 332.85714f, 334.28571f, 335.71429f, 337.14286f, 338.57143f, 340.0f, 341.42857f, 342.85714f, 344.28571f, 345.71429f, 347.14286f, 348.57143f, 350.0f, 351.42857f, 352.85714f, 354.28571f, 355.71429f, 357.14286f, 358.57143f };

  public:

    static const MathHelper& instance();

    const Vector2& normal_dir_252(unsigned char x) const;
    float normal_angle_252(unsigned char x) const;
    Vector2 RotateVector2(const Vector2& vec, float rad) const;
    float DegToRad(float deg) const;

  };

};

#endif