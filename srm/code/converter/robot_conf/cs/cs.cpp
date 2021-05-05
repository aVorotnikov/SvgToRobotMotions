/**
 * @file
 * @brief Coordinate system source file
 * @authors Vorotnikov Andrey, Pavlov Ilya
 * @date 05.05.2021
 *
 * Contains cs_t class realisation to morph svg coordinate system to robor coordinate system
 */

#include <srm.h>

/**
 * Default constructor
 */
srm::cs_t::cs_t(void) noexcept : width(0), height(0) {
}

/**
 * Class constructor
 * @param[in] w image width
 * @param[in] h image height
 * @param[in] p1 first board angle
 * @param[in] p2 second board angle
 * @param[in] p3 third board angle
 */
srm::cs_t::cs_t(double w, double h, vec3_t p1, vec3_t p2, vec3_t p3) noexcept :
  width(w), height(h), boardStart(p1), boardI(p2 - p1), boardJ(p3 - p1) {
}

/**
 * Set svg image width and height function
 * @param[in] w image width
 * @param[in] h image height
 */
void srm::cs_t::SetWH(double w, double h) noexcept {
  width = w;
  height = h;
}

/**
 * Set board plane function
 * @param[in] p1 first board angle
 * @param[in] p2 second board angle
 * @param[in] p3 third board angle
 */
void srm::cs_t::SetPlane(vec3_t p1, vec3_t p2, vec3_t p3) noexcept {
  boardStart = p1;
  boardI = p2 - p1;
  boardJ = p3 - p1;
}

/**
 * Get svg image width function
 * @return svg width
 */
double srm::cs_t::GetW(void) const noexcept {
  return width;
}

/**
 * Get svg image height function
 * @return svg height
 */
double srm::cs_t::GetH(void) const noexcept {
  return height;
}

/**
 * Get board plane normal function
 * @return normal
 */
srm::vec3_t srm::cs_t::GetNormal(void) const noexcept {
  return boardI.Cross(boardJ).Norm();
}

/**
 * Translate accuracy from robot cs to svg cs function
 * @param[in] roboAcc accuracy in robot coordinate system to coordinate
 * @return accuracy in svg coordinate system
 */
double srm::cs_t::SvgToRobotAcc(double roboAcc) const noexcept {
  double
    accCoefX = width / boardI.Len(),
    accCoefY = height / boardJ.Len();
  if (accCoefX > accCoefY)
    return roboAcc * accCoefY;
  return roboAcc * accCoefX;
}

/**
 * Point translation from svg cs to robot cs relatively to main board angle function
 * @param[in] point point in svg cs to translate
 * @return point in robot cs
 */
srm::vec3_t srm::cs_t::SvgToRobotDelta(vec_t point) const noexcept {
  return boardI * point.x / width + boardJ * point.y / height;
}

/**
 * Point translation from svg cs to global robot cs function
 * @param[in] point point in svg cs to translate
 * @return point in robot cs
 */
srm::vec3_t srm::cs_t::SvgToRobot(vec_t point) const noexcept {
  return boardStart + SvgToRobotDelta(point);
}

/**
 * Get ratio of canvas width to svg image width
 * @return ratio of canvas width to svg image width
 */
double srm::cs_t::GetXScale() {
  return boardI.Len() / width;
}

/**
 * Get ratio of canvas height to svg image height
 * @return ratio of canvas height to svg image height
 */
double srm::cs_t::GetYScale() {
  return boardJ.Len() / height;
}
