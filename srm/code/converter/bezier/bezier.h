/**
 * @file
 * @brief Bezier building class header file
 * @authors Vorotnikov Andrey
 * @date 23.02.2021
 *
 * Contains description of class to build Bezier spline
 */

#pragma once

#ifndef __BEZIER_H_INCLUDED
#define __BEZIER_H_INCLUDED

#include <vector>
#include "../defs.h"

/* Project namespace */
namespace srm {
  /**
   * @brief Class to build bezier spline
   * @see vector_2
   *
   * Bezier building class based on std::vector
   */
  class build_bezier_t : public std::vector<vec_t> {
  public:
    /**
     * Evaluate point with parameter
     * @param[in] t parameter from 0 to 1 to evaluate point of Bezier spline
     * @return point corresponding to parameter
     */
    vec_t EvaluatePoint(double t) const;

    /**
     * Sampling by N line segments
     * @param[in] N number of line segments
     * @return Bezier point vector with N+1 length
     * @watning N mustn't be 0
     */
    std::vector<vec_t> Sampling(unsigned N) const;
  };
}

#endif /* __BEZIER_H_INCLUDED */
