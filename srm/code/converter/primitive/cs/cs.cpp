/**
 * @file
 * @brief Coordinate system source file
 * @authors Vorotnikov Andrey
 * @date 03.03.2021
 *
 * Contains cs_t class realisation to morph svg coordinate system to robor coordinate system
 */

#include <srm.h>

/**
 * Class constructor
 * @param[in] roboI robot i ort in svg cs
 * @param[in] roboJ robot j ort in svg cs
 */
srm::cs_t::cs_t(vec_t roboI, vec_t roboJ) noexcept : robotI(roboI), robotJ(roboJ) {
}

/**
 * Point translation from svg cs to robot cs function
 * @param[in] point point in svg cs to translate
 * @return string with code
 */
srm::vec_t srm::cs_t::SvgToRobot(vec_t point) const noexcept {
  return point;
}

/**
 * Point translation from robot cs to svg cs function
 * @param[in] point point in robot cs to translate
 * @return string with code
 */
srm::vec_t srm::cs_t::RobotToSvg(vec_t point) const noexcept {
  return point;
}
