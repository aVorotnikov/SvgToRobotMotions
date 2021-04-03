/**
 * @file
 * @brief transform_t class header file
 * @authors Vorotnikov Andrey, Pavlov Ilya
 * @date 03.04.2021
 *
 * Contains transform_t class description
 */

#pragma once

#ifndef __TRANSFORM_H_INCLUDED
#define __TRANSFORM_H_INCLUDED

/** \brief Project namespace */
namespace srm {
  /**
   * @brief transform_t class
   *
   * Class to process tags with attribute "transform"
   */
  class transform_t {
  private:
    double matrix[2][3] = { {1, 0, 0}, 
                            {0, 1, 0} };
  public:
    /**
     * Default constructor
     */
    transform_t() noexcept;

    /**
     * Constuctor from svg attribute
     * @param[in] trnsAttr string with svg transform attibute value
     */
    transform_t(std::string transAttr) noexcept;

    /**
     * Set transformation matrix;
     * @param[in] m00,..m12 values is matrix [[m00 m01 m02] [m10 m11 m12]] 
     */
    void SetMatrix(double m00, double m01, double m02, double m10, double m11, double m12) noexcept;

    /**
     * Composition of transformations ~ *this = *this * transform
     * @param[in] transform right transformation
     */
    void operator*=(transform_t transform) noexcept;

    /**
     * Apply transformation to primitive
     * @param[in] primitive primitive to transform
     */
    void Apply(srm::primitive_t *primitive) const noexcept;

    /**
     * Set identity transformation
     */
    void Clear() noexcept;
  };
}

#endif /* __TRANSFORM_H_INCLUDED */
