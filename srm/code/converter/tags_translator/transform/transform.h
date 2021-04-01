/**
 * @file
 * @brief transform_t class header file
 * @authors Vorotnikov Andrey, Pavlov Ilya
 * @date 19.03.2021
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
    transform_t(std::string trnsAttr) noexcept;

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
