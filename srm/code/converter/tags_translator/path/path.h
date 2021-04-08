/**
 * @file
 * @brief Path parsing class header file
 * @authors Chevykalov Grigory
 * @date 08.04.2021
 *
 * Contains path parsing class description
 */

#pragma once

#ifndef __PATH_H_INCLUDED
#define __PATH_H_INCLUDED

#include <srm.h>

#include <list>
#include <string>
#include <vector>

/** \brief Project namespace */
namespace srm {
  /**
   * @brief Analyzer states class
   *
   * Contains possible states of analyzer
   */
  enum class state_t {
    start,
    comma,
    number,
    command,
    error
  };
  /**
   * @brief Path parsing class
   *
   * Contains method to parse path
   */
  class path_t {
  private:
    std::list<srm::primitive_t *> *primitives;  ///< pointer to the currently filling list of primitives
    srm::primitive_t *primitive;                ///< pointer to the currently filling primitive
    srm::vec_t last;                            ///< last significant point
    srm::vec_t checkPoint;                      ///< last control point of the last Bezier curve
    char lastCommand;                           ///< previous command
    state_t state;                              ///< the current state of the analyzer
    srm::transform_t transformCompos;           ///< composition of all transformations

    /**
     * Selecting a set of numbers from a string of command
     * @param[in] str string of command
     * @return Vector of numbers contained in the string
     * @warning Numbers are considered until the first invalid entry 
     */
    std::vector<double> GetNums(const char **str) noexcept;

    /**
     * @defgroup pathCommandParse Path command parse functions
     * @brief Functions to parse commands from path
     *
     * @{
     */

    /**
     * Processes the moveto command with absolute coordinates as arguments ("M")
     * @param[in] nums vector of numeric command arguments
     */
    void PathMAbs(const std::vector<double> &nums, const rapidxml::xml_node<>* tag) noexcept;

    /**
     * Processes the moveto command with relative coordinates as arguments ("m")
     * @param[in] nums vector of numeric command arguments
     */
    void PathMRel(const std::vector<double> &nums, const rapidxml::xml_node<>* tag) noexcept;

    /**
     * Processes the lineto command with absolute coordinates as arguments ("L")
     * @param[in] nums vector of numeric command arguments
     */
    void PathLAbs(const std::vector<double> &nums) noexcept;

    /**
     * Processes the lineto command with relative coordinates as arguments ("l")
     * @param[in] nums vector of numeric command arguments
     */
    void PathLRel(const std::vector<double> &nums) noexcept;

    /**
     * Processes the horizontal line command with absolute coordinates as arguments ("H")
     * @param[in] nums vector of numeric command arguments
     */
    void PathHAbs(const std::vector<double> &nums) noexcept;

    /**
     * Processes the horizontal line command with relative coordinates as arguments ("h")
     * @param[in] nums vector of numeric command arguments
     */
    void PathHRel(const std::vector<double> &nums) noexcept;

    /**
     * Processes the vertical line command with absolute coordinates as arguments ("V")
     * @param[in] nums vector of numeric command arguments
     */
    void PathVAbs(const std::vector<double> &nums) noexcept;

    /**
     * Processes the vertical line command with relative coordinates as arguments ("v")
     * @param[in] nums vector of numeric command arguments
     */
    void PathVRel(const std::vector<double> &nums) noexcept;

    /**
     * Processes the subpath close command ("Z" or "z")
     * @param[in] nums vector of numeric command arguments
     * @warning If the vector of arguments is not empty, then there is an error in the path record
     */
    void PathZ(const std::vector<double> &nums) noexcept;

    /**
     * Processes the cubic bezier curve command with absolute coordinates as arguments ("C")
     * @param[in] nums vector of numeric command arguments
     */
    void PathCAbs(const std::vector<double> &nums) noexcept;

    /**
     * Processes the cubic bezier curve command with relative coordinates as arguments ("c")
     * @param[in] nums vector of numeric command arguments
     */
    void PathCRel(const std::vector<double> &nums) noexcept;

    /**
     * Processes the quadratic bezier curve command with absolute coordinates as arguments ("Q")
     * @param[in] nums vector of numeric command arguments
     */
    void PathQAbs(const std::vector<double> &nums) noexcept;

    /**
     * Processes the quadratic bezier curve command with relative coordinates as arguments ("q")
     * @param[in] nums vector of numeric command arguments
     */
    void PathQRel(const std::vector<double> &nums) noexcept;

    /**
     * Processes the smooth cubic bezier curve command with absolute coordinates as arguments ("S")
     * @param[in] nums vector of numeric command arguments
     */
    void PathSAbs(const std::vector<double> &nums) noexcept;

    /**
     * Processes the smooth cubic bezier curve command with relative coordinates as arguments ("s")
     * @param[in] nums vector of numeric command arguments
     */
    void PathSRel(const std::vector<double> &nums) noexcept;

    /**
     * Processes the smooth quadratic bezier curve command with absolute coordinates as arguments ("T")
     * @param[in] nums vector of numeric command arguments
     */
    void PathTAbs(const std::vector<double> &nums) noexcept;

    /**
     * Processes the smooth quadratic bezier curve command with relative coordinates as arguments ("t")
     * @param[in] nums vector of numeric command arguments
     */
    void PathTRel(const std::vector<double> &nums) noexcept;

    /**
     * Processes the elliptical arc command with absolute coordinates as arguments ("A")
     * @param[in] nums vector of numeric command arguments
     */
    void PathAAbs(const std::vector<double> &nums) noexcept;

    /**
     * Processes the elliptical arc command with relative coordinates as arguments ("a")
     * @param[in] nums vector of numeric command arguments
     */
    void PathARel(const std::vector<double> &nums) noexcept;

    /**@}*/

  public:
    /**
     * Constructor for path_t
     * @param[in] ps pointer to a list of primitives
     * @warning pointer mustn't be nullptr
     */
    path_t(std::list<srm::primitive_t *> *ps, srm::transform_t transform);

    /**
     * Main path parsing function
     * @param[in] tag pointer to path node in xml DOM
     */
    void ParsePath(const rapidxml::xml_node<> *tag) noexcept;
  };
}

#endif /* __PATH_H_INCLUDED */