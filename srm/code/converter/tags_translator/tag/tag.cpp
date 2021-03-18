/**
 * @file
 * @brief tag_t class source file
 * @authors Pavlov Ilya
 * @date 14.03.2021
 *
 * Contains tag_t class realisation
 */

#include <srm.h>

/**
 * Transform svg tags to primitives
 * @param[in] tags the list of tags in DOM
 * @param[out] primitives the list of primitive representations of tags
 */
srm::tag_t::tag_t(rapidxml::xml_node<> *node) noexcept {
  this->node = node;
  level = 0;
}
