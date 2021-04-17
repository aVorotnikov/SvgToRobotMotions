/**
 * @file
 * @brief Fill class header file
 * @authors Pavlov Ilya
 * @date 17.04.2021
 *
 * Contains description of class for filling primitives
 */

#pragma once

#ifndef __FILL_H_INCLUDED
#define __FILL_H_INCLUDED

#include <srm.h>

#include <ostream>

 /** \brief Project namespace */
namespace srm {
  /**
   * @brief Class for filling primitives
   */
  class fill_t {
  public:
    /**
     * Gen and print code for filling primitive
     * @param[in] out output stream
     * @param[in] primitive for filling
     * @warning primitive can be changed
     */
    void FillPrim(std::ostream &out, srm::primitive_t *primitive);
  };
}

#endif /* __FILL_H_INCLUDED */
