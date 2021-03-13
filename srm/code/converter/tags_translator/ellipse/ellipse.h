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

  /**
   * Ellipse arc from 'path' tag sampling function
   * @param[in] p1 first arc point
   * @param[in] p2 second arc point
   * @param[in] radiuses 2 ellipse radiuses by x and y
   * @param[in] fA flag for arc angle size
   * @param[in] fS flag for angle delta
   * @param[in] phi angle
   * @param[in] accuracy sampling accuracy
   * @return point vector with sampling
   */
  std::vector<vec_t> EllipseArcSampling(vec_t p1, vec_t p2, vec_t radiuses, bool fA, bool fS, double phi, double accuracy);
}

#endif /* __ELLIPSE_H_INCLUDED */
