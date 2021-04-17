/**
 * @file
 * @brief Primitives and support classes header file
 * @authors Vorotnikov Andrey, Pavlov Ilya, Chevykalov Grigory
 * @date 17.03.2021
 *
 * Contains declaration motions class (motion_t, segment_t, arc_t) and primitive class
 */

#pragma once

#ifndef __PRIMITIVE_H_INCLUDED
#define __PRIMITIVE_H_INCLUDED

#include <vector>
#include "../defs.h"
#include "../robot_conf/cs/cs.h"

/** \brief Project namespace */
namespace srm {
  /**
   * @brief Line segment motion class
   *
   * Line segment representation type to build code
   */
  class segment_t {
  public:
    vec_t point;  ///< point to which robot moves in a straight line

    /**
     * Constructor for segment_t
     * @param[in] x x coordinate of point to which robot moves in a straight line
     * @param[in] y y coordinate of point to which robot moves in a straight line
     */
    segment_t(const double x, const double y);

    /**
     * Generate code for motion type
     * @param[in] coordSys class to morph cs
     * @return string with code
     */
    std::string GenCode(cs_t coordSys) const;
  };

  /**
   * @brief Primitive representation class
   * @see srm::motion::base_t
   *
   * Sequence of simple lines to build code
   */
  class primitive_t : public std::vector<segment_t> {
  public:
    vec_t start;    ///< start point of primitive

    /**
     * Generate code and write it to output stream
     * @param[in] out output variable
     * @param[in] primitive primitive to output
     * @return ostream variable
     */
    friend std::ostream & operator<<(std::ostream &out, const primitive_t &primitive);

    bool fill = false;
  };

  /**
   * Generate code and write it to output stream
   * @param[in] out output variable
   * @param[in] primitive primitive to output
   * @return ostream variable
   */
  std::ostream & operator<<(std::ostream& out, const primitive_t& primitive);

}

#endif /* __PRIMITIVE_H_INCLUDED */
