/**
 * @file
 * @brief Translator class header file
 * @authors Vorotnikov Andrey, Pavlov Ilya
 * @date 13.03.2021
 *
 * Contains main converter class description
 */

#pragma once

#ifndef __TRANSLATOR_H_INCLUDED
#define __TRANSLATOR_H_INCLUDED

#include <string>
#include <ostream>
#include "rapidxml.hpp"
#include "robot_conf/robot_conf.h"

/** \brief Project namespace */
namespace srm {
  /**
   * @brief Main converter class
   *
   * Contains method to parse svg file and create code
   */
  class translator_t {
  private:
    static translator_t singleToneVar;  ///< tranlator singletone variable

    rapidxml::xml_document<> xmlTree;   ///< the root of xml DOM
    char *xmlString;                    ///< rapidxml needs this char string for its work

    std::ostream *logStream;            ///< stream to make logs

    /**
     * Private constructor for single tone
     */
    translator_t(void) noexcept;

  public:
    robot_conf_t roboConf;              ///< robot configuration

    /**
     * Get 'translator_t' class single tone variable
     * @return pointer to single tone variable
     */
    static translator_t * GetPtr(void) noexcept;

    /**
     * Write string in log stream function
     * @param[in] str string to write
     */
    void WriteLog(const std::string &str) noexcept;

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
