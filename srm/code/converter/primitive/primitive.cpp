/**
 * @file
 * @brief Primitives and support classes source file
 * @authors Vorotnikov Andrey
 * @date 02.03.2021
 *
 * Contains definition motions class (motion_t, segment_t, arc_t) and primitive class
 */

#include <srm.h>

/**
 * Generate code from to start by motion type
 * @param[in] start point to create code
 * @return string with code
 */
std::string srm::motion::segment_t::GenCode(srm::vec_t start, cs_t coordSys) const {
  return "";
}

/**
 * Generate code from to start by motion type
 * @param[in] start point to create code
 * @return string with code
 */
std::string srm::motion::arc_t::GenCode(srm::vec_t start, cs_t coordSys) const {
  return "";
}

/**
 * Primitive destructor
 */
srm::primitive_t::~primitive_t(void) {
  for (auto motion : *this) {
    delete motion;
    motion = nullptr;
  }
}
