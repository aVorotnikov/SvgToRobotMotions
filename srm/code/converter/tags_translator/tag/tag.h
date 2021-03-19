/**
 * @file
 * @brief tag_t class header file
 * @authors Vorotnikov Andrey, Pavlov Ilya
 * @date 19.03.2021
 *
 * Contains description of tag_t class
 */

#pragma once

#ifndef __TAG_H_INCLUDED
#define __TAG_H_INCLUDED

#include "../../rapidxml.hpp"

/** \brief Project namespace */
namespace srm {
  /**
   * @brief Class for xml tag
   * Class for xml tag (extension of rapidxml::xml_node)
   */
  class tag_t {
  public:
    unsigned int level = 0;           ///< level by g tags in svg DOM
    rapidxml::xml_node<> *node;       ///< pointer to node in svg DOM

    /**
     * Constructor create tag from node
     * @param[in] node pointer to node
     */
    tag_t(rapidxml::xml_node<> *node) noexcept;
  };
}

#endif /* __TAG_H_INCLUDED */
