/**
 * @file
 * @brief Translator class header file
 * @authors Vorotnikov Andrey, Pavlov Ilya
 * @date 24.02.2021
 *
 * Contains main converter class description
 */

#pragma once

#ifndef __TRANSLATOR_H_INCLUDED
#define __TRANSLATOR_H_INCLUDED

#include <string>
#include "rapidxml.hpp"

/** \brief Project namespace */
namespace srm {
  /**
   * @brief Main converter class
   *
   * Contains method to parse svg file and create code
   */
  class translator_t {
  private:
    rapidxml::xml_document<> xmlTree;  ///< the root of xml DOM
    char *xmlString;                   ///< rapidxml needs this char string for its work

  public:
    /**
     * @defgroup converterFuncs Main functions to convert
     * @brief Tranlator functions to set functions to convert
     *
     * @{
     */

    /**
     * Set svg image to convert function function. Check file abd create tag tree
     * @param[in] svgFileName svg image file name
     */
    void SetSvg(const std::string &svgFileName);

    /**
     * Gen robot code from created tag tree
     * @param[in] codeFileName code file name
     * @see SetSvg
     */
    void GenCode(const std::string &codeFileName) const;

    /**@}*/

    /**
     * Translator class destructor
     */
    ~translator_t(void) noexcept;
  };
}

#endif /* __TRANSLATOR_H_INCLUDED */
