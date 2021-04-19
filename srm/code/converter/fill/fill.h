/**
 * @file
 * @brief Fill primitive header file
 * @authors Pavlov Ilya
 * @date 19.04.2021
 *
 * Contains declaration of function for filling primitives
 */

#pragma once

#ifndef __FILL_H_INCLUDED
#define __FILL_H_INCLUDED

#include <srm.h>

#include <ostream>

 /** \brief Project namespace */
namespace srm {
  /**
   * Check if tag must be filled
   * @param[in] tag tag for checking
   * @return true if must, false if not
   */
  bool IsFill(const rapidxml::xml_node<>* tag) noexcept;

  /**
   * Gen and print code for filling primitive
   * @param[in] out output stream
   * @param[in] primitive for filling
   */
  void FillPrimitive(std::ostream &out, const srm::primitive_t &primitive) noexcept;
}

#endif /* __FILL_H_INCLUDED */
