/**
 * @file
 * @brief Primitives and support classes source file
 * @authors Vorotnikov Andrey, Pavlov Ilya, Chevykalov Grigory
 * @date 05.05.2021
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
srm::segment_t::segment_t(const double x, const double y) {
  point.x = x;
  point.y = y;
}

/**
 * Generate code for motion type
 * @param[in] coordSys class to morph cs
 * @return string with code
 */
std::string srm::segment_t::GenCode(cs_t coordSys) const {
  double scaleX = translator_t::GetPtr()->roboConf.GetXScale();
  double scaleY = translator_t::GetPtr()->roboConf.GetYScale();
  return "LMOVE frm + SHIFT (P BY " +
    std::to_string(point.x * scaleX) + ", " +
    std::to_string(point.y * scaleY) + ", 0)\n";
}

/**
 * Generate code and write it to output stream
 * @param[in] out output variable
 * @param[in] primitive primitive to output
 * @return ostream variable
 */
std::ostream & srm::operator<<(std::ostream &out, const primitive_t &primitive) {
  double scaleX = translator_t::GetPtr()->roboConf.GetXScale();
  double scaleY = translator_t::GetPtr()->roboConf.GetYScale();

  out << "\tLAPPRO frm + SHIFT (P BY " +
    std::to_string(primitive.start.x * scaleX) + ", " +
    std::to_string(primitive.start.y * scaleY) + ", 0), " << std::to_string(translator_t::GetPtr()->roboConf.GetDepDist()) << "\n";

  out << "\tLMOVE frm + SHIFT (P BY " +
    std::to_string(primitive.start.x * scaleX) + ", " +
    std::to_string(primitive.start.y * scaleY) + ", 0)\n";

  for (auto base : primitive) {
    out << "\t" << base.GenCode(translator_t::GetPtr()->roboConf);
  }

  out << "\tLDEPART " << std::to_string(translator_t::GetPtr()->roboConf.GetDepDist()) << "\n";
 
  return out;
}
