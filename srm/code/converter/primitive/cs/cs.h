/**
 * @file
 * @brief Coordinate system header file
 * @authors Vorotnikov Andrey, Pavlov Ilya
 * @date 07.03.2021
 *
 * Contains cs_t class definition to morph svg coordinate system to robor coordinate system
 */

#pragma once

#ifndef __CS_H_INCLUDED
#define __CS_H_INCLUDED

#include "../../defs.h"

 /** \brief Project namespace */
namespace srm {
  /**
   * @brief Coordinate system representation class
   *
   * Class to morph svg coordinate system to robot coordinate system
   */
  class cs_t {
  private:
    vec_t
      svgI = { 1,0 },    ///< svg i ort in robot cs
      svgJ = { 0, 1 },    ///< svg j ort in robot cs
      robotI = { 1, 0 },  ///< robot i ort in svg cs
      robotJ = { 0, 1 };  ///< robot j ort in svg cs

  public:
    /**
     * Class standart constructor
     */
    cs_t() {};

    /**
     * Class constructor
     * @param[in] roboI robot i ort in svg cs
     * @param[in] roboJ robot j ort in svg cs
     */
    cs_t(vec_t roboI, vec_t roboJ) noexcept;

    /**
     * Point translation from svg cs to robot cs function
     * @param[in] point point in svg cs to translate
     * @return string with code
     */
    vec_t SvgToRobot(vec_t point) const noexcept;

    /**
     * Point translation from robot cs to svg cs function
     * @param[in] point point in robot cs to translate
     * @return string with code
     */
    vec_t RobotToSvg(vec_t point) const noexcept;
  };
}

#endif /* __CS_H_INCLUDED */
