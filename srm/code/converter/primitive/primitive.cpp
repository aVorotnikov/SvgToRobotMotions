/**
 * @file
 * @brief Primitives and support classes source file
 * @authors Vorotnikov Andrey, Pavlov Ilya
 * @date 07.03.2021
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
srm::motion::segment_t::segment_t(const int x, const int y) {
  point.x = x;
  point.y = y;
}

/**
  * Generate code for motion type
  * @param[in] coordSys class to morph cs
  * @return string with code
  */
std::string srm::motion::segment_t::GenCode(cs_t coordSys) const {
  std::string command = "LMOVE " + std::to_string(point.x) + ", " + std::to_string(point.y) + "\n";
  return command;
}

/**
  * Generate code for motion type
  * @param[in] coordSys class to morph cs
  * @return string with code
  */
std::string srm::motion::arc_t::GenCode(cs_t coordSys) const {
  std::string command = "C1MOVE " + std::to_string(point1.x) + ", " + std::to_string(point1.y) + "\n" +
    "C2MOVE " + std::to_string(point1.x) + ", " + std::to_string(point1.y) + "\n";
  return command;
}

 /**
  *
  */
std::ostream & srm::operator<<(std::ostream &out, const primitive_t &primitive) {
  out << "\tJAPPRO " << std::to_string(primitive.start.x) << " " << std::to_string(primitive.start.y) << ", 500\n";
  out << "\tDRAW ,,-500\n";

  for (auto base : primitive) {
    out << "\t" << base->GenCode(primitive.coordSys);
  }
  out << "\tDRAW ,,500\n";

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
