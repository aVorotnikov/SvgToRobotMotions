/**
 * @file
 * @brief tag_t class source file
 * @authors Vorotnikov Andrey, Pavlov Ilya
 * @date 19.03.2021
 *
 * Contains tag_t class realisation
 */

#include <srm.h>

/**
 * Constructor create tag from node
 * @param[in] node pointer to node
 */
srm::tag_t::tag_t(rapidxml::xml_node<> *node) noexcept {
  this->node = node;
  level = 0;
}
