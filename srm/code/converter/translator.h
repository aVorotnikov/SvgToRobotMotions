/**
 * @file
 * @brief Translator class header file
 * @authors Vorotnikov Andrey
 * @date 23.02.2021
 *
 * Contains main converter class description
 */

#pragma once

#ifndef __TRANSLATOR_H_INCLUDED
#define __TRANSLATOR_H_INCLUDED

#include <string>

/* Project namespace */
namespace srm {
  /**
   * @brief main converter class
   *
   * Contains method to parse svg file and create code
   */
  class translator_t {
  private:
  public:
    /**
     * @defgroup converterFuncs Main functions to convert
     * @brief Tranlator functions to set functions to convert
     *
     * @{
     */

    /**
     * Get svg image function function. Check file abd create tag tree
     * @param[in] svgFileName svg image file name
     */
    void GetSvg(const std::string &svgFileName);

    /**
     * Gen robot code from created tag tree
     * @param[in] codeFileName code file name
     * @see GetSvg
     */
    void GenCode(const std::string &codeFileName) const;

    /**@}*/

    /**
     * Translator class destructor
     */
    ~translator_t(void);
  };
}

#endif /* __TRANSLATOR_H_INCLUDED */
