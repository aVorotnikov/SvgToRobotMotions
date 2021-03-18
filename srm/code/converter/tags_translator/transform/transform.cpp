/**
 * @file
 * @brief transform_t class source file
 * @authors Pavlov Ilya
 * @date 18.03.2021
 *
 * Contains transform_t class realisation to process tags with attribute "transform"
 */

#include <srm.h>

/**
 * Default constructor
 */
srm::transform_t::transform_t() noexcept {
}

/**
 * Constuctor from svg attribute
 * @param[in] string with svg transform attibute value
 */
srm::transform_t::transform_t(std::string trnsAttr) noexcept {
  // TODO: transform attribute parser
}

/**
 * Composition of transformations ~ *this = *this * transform
 * @param[in] transform right transformation
 */
void srm::transform_t::operator*=(transform_t transform) noexcept {
  double tmp[2][3] = { { matrix[0][0], matrix[0][1], matrix[0][2] },
                       { matrix[1][0], matrix[1][1], matrix[1][2]} };

  matrix[0][0] = tmp[0][0] * transform.matrix[0][0] + tmp[0][1] * transform.matrix[1][0];
  matrix[0][1] = tmp[0][0] * transform.matrix[0][1] + tmp[0][1] * transform.matrix[1][1];
  matrix[0][2] = tmp[0][0] * transform.matrix[0][2] + tmp[0][1] * transform.matrix[1][2] + tmp[0][2];

  matrix[1][0] = tmp[1][0] * transform.matrix[0][0] + tmp[1][1] * transform.matrix[1][0];
  matrix[1][1] = tmp[1][0] * transform.matrix[0][1] + tmp[1][1] * transform.matrix[1][1];
  matrix[1][2] = tmp[1][0] * transform.matrix[0][2] + tmp[1][1] * transform.matrix[1][2] + tmp[1][2];
} 

/**
  * Apply transformation to primitive
  * @param[in] primitive primitive to transform
  */
void srm::transform_t::Apply(srm::primitive_t *primitive) const noexcept {
  for (auto &segment : *primitive) {
    double tmp = segment.point.x;
    segment.point.x = matrix[0][0] * segment.point.x + matrix[0][1] * segment.point.y + matrix[0][2];
    segment.point.y = matrix[1][0] * tmp + matrix[1][1] * segment.point.y + matrix[1][2];
  }
}

/**
 * Set identity transformation
 */
void srm::transform_t::Clear() noexcept {
  matrix[0][0] = 1;
  matrix[0][1] = 0;
  matrix[0][2] = 0;
  matrix[1][0] = 0;
  matrix[1][1] = 1;
  matrix[1][2] = 0;
}
