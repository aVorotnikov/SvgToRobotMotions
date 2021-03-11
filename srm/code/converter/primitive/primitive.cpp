/**
 * @file
 * @brief Primitives and support classes source file
 * @authors Vorotnikov Andrey, Pavlov Ilya, Chevykalov Grigory
 * @date 11.03.2021
 *
 * Contains definition motions class (motion_t, segment_t, arc_t) and primitive class
 */

#include <srm.h>
#include <sstream>

 /**
 * Constructor for segment_t
 * @param[in] x x coordinate of point to which robot moves in a straight line
 * @param[in] y y coordinate of point to which robot moves in a straight line
 */
srm::motion::segment_t::segment_t(const double x, const double y) {
  point.x = x;
  point.y = y;
}

/**
 * Generate code for motion type
 * @param[in] coordSys class to morph cs
 * @return string with code
 */
std::string srm::motion::segment_t::GenCode(cs_t coordSys) const {
  vec3_t delta = coordSys.SvgToRobotDelta(point);
  return "LMOVE SHIFT (p1 BY "+
    std::to_string(delta.x) + ", " +
    std::to_string(delta.y) + ", " +
    std::to_string(delta.z) + ")\n";
}

/**
 * Constructor for arc_t
 * @param[in] c1x x coordinate of the first point
 * @param[in] c1y y coordinate of the first point
 * @param[in] c2x x coordinate of the second point
 * @param[in] c2y y coordinate of the second point
 */
srm::motion::arc_t::arc_t(const double c1x, const double c1y, const double c2x, const double c2y) {
  point1.x = c1x;
  point1.y = c1y;
  point2.x = c2x;
  point2.y = c2y;
}

/**
 * Generate code for motion type
 * @param[in] coordSys class to morph cs
 * @return string with code
 */
std::string srm::motion::arc_t::GenCode(cs_t coordSys) const {
  vec3_t
    delta1 = coordSys.SvgToRobotDelta(point1),
    delta2 = coordSys.SvgToRobotDelta(point2);
  return "C1MOVE SHIFT (p1 BY " +
    std::to_string(delta1.x) + ", " +
    std::to_string(delta1.y) + ", " +
    std::to_string(delta1.z) + ")\n" +
    "C2MOVE SHIFT (p1 BY " +
    std::to_string(delta2.x) + ", " +
    std::to_string(delta2.y) + ", " +
    std::to_string(delta2.z) + ")\n";
}

/**
 * Generate code and write it to output stream
 * @param[in] out output variable
 * @param[in] primitive primitive to output
 * @return ostream variable
 */
std::ostream & srm::operator<<(std::ostream &out, const primitive_t &primitive) {
  vec3_t delta = translator_t::GetPtr()->roboConf.SvgToRobotDelta(primitive.start);
  out << "\tLAPPRO SHIFT (p1 BY " +
    std::to_string(delta.x) + ", " +
    std::to_string(delta.y) + ", " +
    std::to_string(delta.z) + "), 500\n";
  out << "\tLMOVE SHIFT (p1 BY " +
    std::to_string(delta.x) + ", " +
    std::to_string(delta.y) + ", " +
    std::to_string(delta.z) + ")\n";;

  for (auto base : primitive)
    out << "\t" << base->GenCode(translator_t::GetPtr()->roboConf);

  out << "\tLDERAPT SHIFT (p1 BY " +
    std::to_string(delta.x) + ", " +
    std::to_string(delta.y) + ", " +
    std::to_string(delta.z) + "), 500\n";

  return out;
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
