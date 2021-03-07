/**
 * @file
 * @brief 2d vector class header file
 * @authors Vorotnikov Andrey
 * @date 07.03.2021
 *
 * Contains template 2 dimensional vector mathematician class
 */

#pragma once

#ifndef __VECTOR2_H_INCLUDED
#define __VECTOR2_H_INCLUDED

#include <cmath>

/** \brief Math module namespace */
namespace math {
  /**
   * @brief 2 dimensional vector template class
   * @warning default vector type is double
   */
  template <typename scalar_t = double>
    class vector2_t {
    public:
      scalar_t
        x,  ///< x vector coordinate
        y;  ///< y vector coordinate

      /**
       * @defgroup vec2Constructors Constructors of 2d vector class
       * @brief 2 dimensional vector class constructors module
       *
       * @{
       */

      /**
       * Default constructor
       */
      vector2_t(void) noexcept : x(0), y(0) {
      }

      /**
       * Constructor for 2 numbers
       * @param[in] x0 x coordinate
       * @param[in] y0 y coordinate
       */
      vector2_t(scalar_t x0, scalar_t y0) noexcept : x(x0), y(y0) {
      }

      /**
       * Copy constructor
       * @param[in] v vector to copy
       */
      vector2_t(const vector2_t &v) noexcept : x(v.x), y(v.y) {
      }

      /**@}*/

      /**
       * @defgroup vec2Math Math operation on 2d vector
       * @brief 2 dimensional vector class operations module
       *
       * @{
       */

      /**
       * = operator function
       * @param[in] v vector to copy
       * @return each self
       */
      vector2_t & operator=(const vector2_t &v) noexcept {
        x = v.x;
        y = v.y;
        return *this;
      }

      /**
       * - operator function
       * @return unary minus result
       */
      vector2_t operator-(void) const noexcept {
        return vector2_t(-x, -y);
      }

      /**
       * + operator function
       * @param[in] v vector to add
       * @return sum of vectors
       */
      vector2_t operator+(const vector2_t &v) const noexcept {
        return vector2_t(x + v.x, y + v.y);
      }

      /**
       * += operator function
       * @param[in] v vector to add
       * @return each self
       */
      vector2_t & operator+=(const vector2_t &v) noexcept {
        x += v.x;
        y += v.y;
        return *this;
      }

      /**
       * - operator function
       * @param[in] v vector to dif
       * @return difference of vectors
       */
      vector2_t operator-(const vector2_t &v) const noexcept {
        return vector2_t(x - v.x, y - v.y);
      }

      /**
       * -= operator function
       * @param[in] v vector to dif
       * @return each self
       */
      vector2_t & operator-=(const vector2_t &v) noexcept {
        x -= v.x;
        y -= v.y;
        return *this;
      }

      /**
       * * operator function
       * @param[in] a number to mul
       * @return multiplicated vector
       */
      vector2_t operator*(scalar_t a) const noexcept {
        return vector2_t(x * a, y * a);
      }

      /**
       * *= operator function
       * @param[in] a number to mul
       * @return each self
       */
      vector2_t & operator*=(scalar_t a) noexcept {
        x *= a;
        y *= a;
        return *this;
      }

      /**
       * / operator function
       * @param[in] a number to div
       * @return divided vector
       */
      vector2_t operator/(scalar_t a) const noexcept {
        return vector2_t(x / a, y / a);
      }

      /**
       * /= operator function
       * @param[in] a number to div
       * @return each self
       */
      vector2_t & operator/=(scalar_t a) noexcept {
        x /= a;
        y /= a;
        return *this;
      }

      /**
       * Dot multipling of 2 vectors function
       * @param[in] v vector to mul
       * @return dot multipling iof 2 vectors
       */
      scalar_t Dot(const vector2_t &v) const noexcept {
        return x * v.x + y * v.y;
      }

      /**
       * Cross multipling of 2 vectors function
       * @param[in] v vector to mul
       * @return cross multipling iof 2 vectors
       */
      scalar_t Cross(const vector2_t &v) const noexcept {
        return x * v.y - y * v.x;
      }

      /**
       * Length of vector function
       * @return vector length
       */
      scalar_t Len(void) const noexcept {
        return sqrt(x * x + y * y);
      }

      /**
       * Squared length of vector function
       * @return square of vector length
       */
      scalar_t Len2(void) const noexcept {
        return x * x + y * y;
      }

      /**
       * Vector normalization
       * @return normalized vector
       */
      vector2_t Norm(void) const noexcept {
        scalar_t len = Len();
        if (len == 0)
          return vector2_t();
        return *this / len;
      }

      /**
       * Make vector normalized
       * @return each self
       */
      vector2_t & Normalize(void) noexcept {
        scalar_t len = Len();
        if (len == 0)
          return *this;
        *this /= len;
        return *this;
      }

      /**
       * Rotate alpha angle function
       * @param[in] alpha angle to rotate
       * @return cross multipling iof 2 vectors
       */
      vector2_t Cross(scalar_t alpha) const noexcept {
        scalar_t si = sin(alpha), co = cos(alpha);
        return vector2_t(x * co - y * si, x * si + y * co);
      }

      /**@}*/
    };
}

#endif /* __VECTOR2_H_INCLUDED */
