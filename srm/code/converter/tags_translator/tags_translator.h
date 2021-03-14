/**
 * @file
 * @brief header file with tagsToPrimitives function declaration
 * @authors Vorotnikov Andrey, Pavlov Ilya
 * @date 14.03.2021
 *
 * Contains tagsToPrimitives function declaration
 */

#pragma once

#ifndef __TAGS_TRANSLATOR_H_INCLUDED
#define __TAGS_TRANSLATOR_H_INCLUDED

#include <srm.h>
#include <list>

/** \brief Project namespace */
namespace srm {
  /**
   * Transform svg tags to primitives
   * @param[in] tags the list of tags in DOM
   * @param[out] primitives the list of primitive representations of tags
   */
  void TagsToPrimitives(const std::list<rapidxml::xml_node<>*>& tags, std::list<srm::primitive_t*>* primitives) noexcept;
}

#endif /* __TAGS_TRANSLATOR_H_INCLUDED */
