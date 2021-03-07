/**
 * @file
 * @brief Primitives and support classes header file
 * @authors Vorotnikov Andrey, Pavlov Ilya
 * @date 07.03.2021
 *
 * Contains declaration motions class (motion_t, segment_t, arc_t) and primitive class
 */

#pragma once

#ifndef __PRIMITIVE_H_INCLUDED
#define __PRIMITIVE_H_INCLUDED

#include <vector>
#include "../defs.h"
#include "cs/cs.h"

/** \brief Project namespace */
namespace srm {
  /** \brief Motion types namespace */
  namespace motion {
    /**
     * @brief Basic robot motion class
     *
     * Contains virtual function, that build robot motions code
     */
    class base_t {
    public:
      /**
       * Generate code for motion type
       * @param[in] coordSys class to morph cs
       * @return string with code
       */
      virtual std::string GenCode(cs_t coordSys) const = 0;
    };

    /**
     * @brief Line segment motion class
     *
     * Line segment representation type to build code
     */
    class segment_t : public base_t {
    public:
      vec_t point;  ///< point to which robot moves in a straight line

      /**
       * Constructor for segment_t
       * @param[in] x x coordinate of point to which robot moves in a straight line
       * @param[in] y y coordinate of point to which robot moves in a straight line
       */
      segment_t(const int x, const int y);

      /**
       * Generate code for motion type
       * @param[in] coordSys class to morph cs
       * @return string with code
       */
      std::string GenCode(cs_t coordSys) const override;
    };

    /**
     * @brief Circular arc motion class
     *
     * Circular arc representation type to build code
     */
    class arc_t : public base_t {
    public:
      vec_t
        point1,  ///< the first point to building arc
        point2;  ///< the second point to building arc

      /**
       * Generate code for motion type
       * @param[in] coordSys class to morph cs
       * @return string with code
       */
      std::string GenCode(cs_t coordSys) const override;
    };
  }

  /**
   * @brief Primitive representation class
   * @see srm::motion::base_t
   *
   * Sequence of simple lines to build code
   */
  class primitive_t : public std::vector<motion::base_t *> {
  public:
    vec_t start;    ///< start point of primitive
    cs_t coordSys;  ///< for transforming coordinates from svg system to robot system

    /**
     * Generate code and write it to output stream 
     * @param[in] output variable
     * @param[in] primitive
     * @return ostream variable
     */
    friend std::ostream & operator<<(std::ostream &out, const primitive_t &primitive);

    /**
     * Primitive destructor
     */
    ~primitive_t(void);
  };

  std::ostream& operator<<(std::ostream& out, const primitive_t& primitive);

}

#endif /* __PRIMITIVE_H_INCLUDED */
