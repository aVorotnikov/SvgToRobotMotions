/**
 * @file
 * @brief Coordinate system header file
 * @authors Vorotnikov Andrey, Pavlov Ilya
 * @date 05.05.2021
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
    double
      width,       ///< image width
      height;      ///< image height
    vec3_t
      boardStart,  ///< board angle point
      boardI,      ///< ort i of board (bottom from robot)
      boardJ;      ///< ort j of board (left from robot)

  public:

    /**
     * Default constructor
     */
    cs_t(void) noexcept;

    /**
     * Class constructor
     * @param[in] w image width
     * @param[in] h image height
     * @param[in] p1 first board angle
     * @param[in] p2 second board angle
     * @param[in] p3 third board angle
     */
    cs_t(double w, double h, vec3_t p1, vec3_t p2, vec3_t p3) noexcept;

  protected:
    /**
     * Set board plane function
     * @param[in] p1 first board angle
     * @param[in] p2 second board angle
     * @param[in] p3 third board angle
     */
    void SetPlane(vec3_t p1, vec3_t p2, vec3_t p3) noexcept;

  public:
    /**
     * Set svg image width and height function
     * @param[in] w image width
     * @param[in] h image height
     */
    void SetWH(double w, double h) noexcept;

    /**
     * Get svg image width function
     * @return svg width
     */
    double GetW(void) const noexcept;

    /**
     * Get svg image height function
     * @return svg height
     */
    double GetH(void) const noexcept;

    /**
     * Get board plane normal function
     * @return normal
     */
    vec3_t GetNormal(void) const noexcept;

    /**
     * Translate accuracy from robot cs to svg cs function
     * @param[in] roboAcc accuracy in robot coordinate system to coordinate
     * @return accuracy in svg coordinate system
     */
    double SvgToRobotAcc(double roboAcc) const noexcept;

    /**
     * Point translation from svg cs to robot cs relatively to main board angle function
     * @param[in] point point in svg cs to translate
     * @return point in robot cs
     */
    vec3_t SvgToRobotDelta(vec_t point) const noexcept;

    /**
     * Point translation from svg cs to global robot cs function
     * @param[in] point point in svg cs to translate
     * @return point in robot cs
     */
    vec3_t SvgToRobot(vec_t point) const noexcept;

    /**
     * Get ratio of canvas width to svg image width
     * @return ratio of canvas width to svg image width
     */
    double GetXScale();

    /**
     * Get ratio of canvas height to svg image height
     * @return ratio of canvas height to svg image height
     */
    double GetYScale();
  };
}

#endif /* __CS_H_INCLUDED */
