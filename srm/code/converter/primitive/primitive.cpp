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
 * Generate code and write it to output stream
 * @param[in] out output variable
 * @param[in] primitive primitive to output
 * @return ostream variable
 */
std::ostream & srm::operator<<(std::ostream &out, const primitive_t &primitive) {
  vec3_t delta = primitive.coordSys.SvgToRobotDelta(primitive.start);
  out << "\tLAPPRO SHIFT (p1 BY " +
    std::to_string(delta.x) + ", " +
    std::to_string(delta.y) + ", " +
    std::to_string(delta.z) + "), 500\n";
  out << "\tLMOVE SHIFT (p1 BY " +
    std::to_string(delta.x) + ", " +
    std::to_string(delta.y) + ", " +
    std::to_string(delta.z) + ")\n";;

  for (auto base : primitive)
    out << "\t" << base.GenCode(primitive.coordSys);

  out << "\tLDERAPT SHIFT (p1 BY " +
    std::to_string(delta.x) + ", " +
    std::to_string(delta.y) + ", " +
    std::to_string(delta.z) + "), 500\n";

  return out;
}
