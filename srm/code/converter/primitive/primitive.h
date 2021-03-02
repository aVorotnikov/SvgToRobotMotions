/**
 * @file
 * @brief Primitives and support classes header file
 * @authors Vorotnikov Andrey
 * @date 02.03.2021
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
       * Generate code from to start by motion type
       * @param[in] start point to create code
       * @param[in] coordSys class to morph cs
       * @return string with code
       */
      virtual std::string GenCode(vec_t start, cs_t coordSys) const = 0;
    };

    /**
     * @brief Line segment motion class
     *
     * Line segment representation type to build code
     */
    class segment_t : public base_t {
    public:
      /**
       * Generate code from to start by motion type
       * @param[in] start point to create code
       * @param[in] coordSys class to morph cs
       * @return string with code
       */
      std::string GenCode(vec_t start, cs_t coordSys) const override;
    };

    /**
     * @brief Circular arc motion class
     *
     * Circular arc representation type to build code
     */
    class arc_t : public base_t {
    public:
      /**
       * Generate code from to start by motion type
       * @param[in] start point to create code
       * @param[in] coordSys class to morph cs
       * @return string with code
       */
      std::string GenCode(vec_t start, cs_t coordSys) const override;
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
    vec_t start;  ///< start point of primitive

    /**
     * Primitive destructor
     */
    ~primitive_t(void);
  };
}

#endif /* __PRIMITIVE_H_INCLUDED */
