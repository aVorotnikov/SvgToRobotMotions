/**
 * @file
 * @brief 2d matrices operations header file
 * @authors Vorotnikov Andrey
 * @date 12.03.2021
 *
 * Contains math operations with 2d matrices
 */

#pragma once

#ifndef __MATR2_H_INCLUDED
#define __MATR2_H_INCLUDED

#include <utility>
#include "vector2.h"

/** \brief Math module namespace */
namespace math {
  /**
   * Matrix determinant evaluation function
   * @param[in] a first coulumn
   * @param[in] b second coulumn
   * @return determinant
   */
  template <typename scalar_t = double>
    scalar_t det(const vector2_t<scalar_t> &a, const vector2_t<scalar_t> &b) {
      return a.x * b.y - b.x * a.y;
    }

  /**
   * Symmetrical matrix eigenvalue evaluation function
   * @param[in] a (1, 1) element
   * @param[in] b (1, 2) & (2, 1) element
   * @param[in] c (2, 2) element
   * @return 2 eigenvalues
   */
  template <typename scalar_t = double>
    std::pair<scalar_t, scalar_t> eigVal(scalar_t a, scalar_t b, scalar_t c) {
      scalar_t
        axe = (a + c) / 2,
        deltaAC = a - c,
        delta = sqrt(deltaAC * deltaAC + 4 * b * b) / 2;
      return std::pair<scalar_t, scalar_t>(axe - delta, axe + delta);
    }

  /**
   * Symmetrical matrix eigenvector evaluation by eigenvalue and first row function
   * @param[in] a (1, 1) element
   * @param[in] b (1, 2) & (2, 1) element
   * @param[in] lambda eigenvalue
   * @return normalized eigenvector
   */
  template <typename scalar_t = double>
    vector2_t<scalar_t> eigVec(scalar_t a, scalar_t b, scalar_t lambda) {
      double aLambda = lambda - a;
      if (abs(b) < abs(aLambda))
        return vector2_t<scalar_t>(b / aLambda, 1);
      return vector2_t<scalar_t>(1, aLambda / b).Norm();
    }
}

#endif /* __MATR2_H_INCLUDED */
