/**
 * @file
 * @brief 3d vector class header file
 * @authors Vorotnikov Andrey
 * @date 07.03.2021
 *
 * Contains template 3 dimensional vector mathematician class
 */

#pragma once

#ifndef __VECTOR3_H_INCLUDED
#define __VECTOR3_H_INCLUDED

#include <cmath>

/** \brief Math module namespace */
namespace math {
  /**
   * @brief 3 dimensional vector template class
   * @warning default vector type is double
   */
  template <typename scalar_t = double>
    class vector3_t {
    public:
      scalar_t
        x,  ///< x vector coordinate
        y,  ///< y vector coordinate
        z;  ///< z vector coordinate

      /**
       * @defgroup vec3Constructors Constructors of vector class
       * @brief 3 dimensional vector class constructors module
       *
       * @{
       */

      /**
       * Default constructor
       */
      vector3_t(void) noexcept : x(0), y(0), z(0) {
      }

      /**
       * Constructor for 2 numbers
       * @param[in] x0 x coordinate
       * @param[in] y0 y coordinate
       * @param[in] z0 z coordinate
       */
      vector3_t(scalar_t x0, scalar_t y0, scalar_t z0) noexcept : x(x0), y(y0), z(z0) {
      }

      /**
       * Copy constructor
       * @param[in] v vector to copy
       */
      vector3_t(const vector3_t &v) noexcept : x(v.x), y(v.y), z(v.z) {
      }

      /**@}*/

      /**
       * @defgroup vec3Math Math operation on 3d vector
       * @brief 3 dimensional vector class operations module
       *
       * @{
       */

      /**
       * = operator function
       * @param[in] v vector to copy
       * @return each self
       */
      vector3_t & operator=(const vector3_t &v) noexcept {
        x = v.x;
        y = v.y;
        z = v.z;
        return *this;
      }

      /**
       * - operator function
       * @return unary minus result
       */
      vector3_t operator-(void) const noexcept {
        return vector3_t(-x, -y, -z);
      }

      /**
       * + operator function
       * @param[in] v vector to add
       * @return sum of vectors
       */
      vector3_t operator+(const vector3_t &v) const noexcept {
        return vector3_t(x + v.x, y + v.y, z + v.z);
      }

      /**
       * += operator function
       * @param[in] v vector to add
       * @return each self
       */
      vector3_t & operator+=(const vector3_t &v) noexcept {
        x += v.x;
        y += v.y;
        z += v.z;
        return *this;
      }

      /**
       * - operator function
       * @param[in] v vector to dif
       * @return difference of vectors
       */
      vector3_t operator-(const vector3_t &v) const noexcept {
        return vector3_t(x - v.x, y - v.y, z - v.z);
      }

      /**
       * -= operator function
       * @param[in] v vector to dif
       * @return each self
       */
      vector3_t & operator-=(const vector3_t &v) noexcept {
        x -= v.x;
        y -= v.y;
        z -= v.z;
        return *this;
      }

      /**
       * * operator function
       * @param[in] a number to mul
       * @return multiplicated vector
       */
      vector3_t operator*(scalar_t a) const noexcept {
        return vector3_t(x * a, y * a, z * a);
      }

      /**
       * *= operator function
       * @param[in] a number to mul
       * @return each self
       */
      vector3_t & operator*=(scalar_t a) noexcept {
        x *= a;
        y *= a;
        z *= a;
        return *this;
      }

      /**
       * / operator function
       * @param[in] a number to div
       * @return divided vector
       */
      vector3_t operator/(scalar_t a) const noexcept {
        return vector3_t(x / a, y / a, z / a);
      }

      /**
       * /= operator function
       * @param[in] a number to div
       * @return each self
       */
      vector3_t & operator/=(scalar_t a) noexcept {
        x /= a;
        y /= a;
        z /= a;
        return *this;
      }

      /**
       * Dot multipling of 2 vectors function
       * @param[in] v vector to mul
       * @return dot multipling iof 2 vectors
       */
      scalar_t Dot(const vector3_t &v) const noexcept {
        return x * v.x + y * v.y + z * v.z;
      }

      /**
       * Cross multipling of 2 vectors function
       * @param[in] v vector to mul
       * @return cross multipling iof 2 vectors
       */
      vector3_t Cross(const vector3_t &v) const noexcept {
        return vector3_t(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
      }

      /**
       * Length of vector function
       * @return vector length
       */
      scalar_t Len(void) const noexcept {
        return sqrt(x * x + y * y + z * z);
      }

      /**
       * Squared length of vector function
       * @return square of vector length
       */
      scalar_t Len2(void) const noexcept {
        return x * x + y * y + z * z;
      }

      /**
       * Vector normalization
       * @return normalized vector
       */
      vector3_t Norm(void) const noexcept {
        scalar_t len = Len();
        if (len == 0)
          return vector3_t();
        return *this / len;
      }

      /**
       * Make vector normalized
       * @return each self
       */
      vector3_t & Normalize(void) noexcept {
        scalar_t len = Len();
        if (len == 0)
          return *this;
        *this /= len;
        return *this;
      }

      /**@}*/
    };
}

#endif /* __VECTOR3_H_INCLUDED */
