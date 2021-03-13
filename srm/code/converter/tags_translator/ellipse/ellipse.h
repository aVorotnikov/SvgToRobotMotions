/**
 * @file
 * @brief ellipse and arcs sampling functions header file
 * @authors Vorotnikov Andrey
 * @date 13.03.2021
 *
 * Contains ellipse and arcs sampling functions definition
 */

#pragma once

#ifndef __ELLIPSE_H_INCLUDED
#define __ELLIPSE_H_INCLUDED

#include <vector>
#include "../../defs.h"

/** \brief Project namespace */
namespace srm {
  /**
   * Ellipse sampling function
   * @param[in] center ellipse center point
   * @param[in] radiuses 2 ellipse radiuses by x and y
   * @param[in] accuracy sampling accuracy
   * @return point vector with sampling
   */
  std::vector<vec_t> EllipseSampling(vec_t center, vec_t radiuses, double accuracy);
}

#endif /* __ELLIPSE_H_INCLUDED */
