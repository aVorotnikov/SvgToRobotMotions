/**
 * @file
 * @brief Path parsing class source file
 * @authors Chevykalov Grigory
 * @date 10.03.2021
 *
 * Contains path parsing class realisation
 */

#include <srm.h>

#include <cctype>

/**
 * Constructor for path_t
 * @param[in] ps pointer to a list of primitives
 * @param[in] acc accuracy of approximation of Bezier curves and elliptical arcs
 * @warning accuracy must be greater than 0, otherwise will set to 1
 * @warning pointer mustn't be nullptr
 */
srm::path_t::path_t(std::list<srm::primitive_t *> *ps, double acc) {
  if(ps == nullptr)
    throw std::exception("Incorrect pointer");

  if (acc > 0)
    accuracy = acc;
  else
    accuracy = 1;

  primitives = ps;
  primitive = nullptr;
  last = srm::vec_t(0,0);
  checkPoint = srm::vec_t(0, 0);
  lastCommand = '\0';
  state = srm::state_t::start;
}

/**
 * Selecting a set of numbers from a string of command
 * @param[in] str string of command
 * @return Vector of numbers contained in the string
 * @warning Numbers are considered until the first invalid entry
 */
std::vector<double> srm::path_t::GetNums(const char **strPtr) noexcept {
  const char *str = *strPtr + 1;
  std::vector<double> nums;

  // process to end of line or first letter
  while (*str != '\0' && !isalpha(static_cast<unsigned char>(*str))) {
    // skip space
    if (isspace(static_cast<unsigned char>(*str))) {
      str++;
      continue;
    }
    // process number
    if (isdigit(static_cast<unsigned char>(*str)) || *str == '.' || *str == '-' || *str == '+') {
      char *end = NULL;
      double number = strtod(str, &end);

      // the sequence is not a number
      if (str == end) {
        state = srm::state_t::error;
        break;
      }

      str = end;
      state = srm::state_t::number;
      nums.push_back(number);
    }
    // process the comma
    else if (*str == ',') {
      // wrong comma position
      if (state != srm::state_t::number) {
        state = srm::state_t::error;
        break;
      }

      state = srm::state_t::comma;
      str++;
    }
    // non-convertible character reached
    else {
      state = srm::state_t::error;
      break;
    }
  }

  *strPtr = str;
  return nums;
}

/**
 * Processes the moveto command with absolute coordinates as arguments ("M")
 * @param[in] nums vector of numeric command arguments
 */
void srm::path_t::PathMAbs(const std::vector<double> &nums) noexcept {
  // add the previous primitive to the list
  if (primitive != nullptr)
    if (primitive->size() > 0)
      primitives->push_back(primitive);
    else
      delete primitive;

  // create new primitive
  primitive = new srm::primitive_t;

  size_t counter = 0;
  size_t size = nums.size();
  // set the start of the primitive
  if (size > 1) {
    last = srm::vec_t(nums[counter], nums[counter + 1]);
    counter += 2;
    primitive->start = last;
  }
  else {
    state = srm::state_t::error;
    return;
  }

  // add a sequence of line segments to a primitive
  for (; size - counter > 1; counter += 2) {
    srm::vec_t cur(nums[counter], nums[counter + 1]);
    if ((cur - last).Len2() != 0) {
      last = cur;
      srm::motion::segment_t *p = new srm::motion::segment_t(last.x, last.y);
      primitive->push_back(p);
    }
  }

  // handle the wrong number of arguments
  if (size - counter > 0) {
    state = srm::state_t::error;
    return;
  }
}

/**
 * Processes the moveto command with relative coordinates as arguments ("m")
 * @param[in] nums vector of numeric command arguments
 */
void srm::path_t::PathMRel(const std::vector<double> &nums) noexcept {
  // add the previous primitive to the list
  if (primitive != nullptr)
    if (primitive->size() > 0)
      primitives->push_back(primitive);
    else
      delete primitive;

  // create new primitive
  primitive = new srm::primitive_t;

  size_t counter = 0;
  size_t size = nums.size();
  // set the start of the primitive
  if (size > 1) {
    last += srm::vec_t(nums[counter], nums[counter + 1]);
    counter += 2;
    primitive->start = last;
  }
  else {
    state = srm::state_t::error;
    return;
  }

  // add a sequence of line segments to a primitive
  for (; size - counter > 1; counter += 2) {
    srm::vec_t delta(nums[counter], nums[counter + 1]);
    if (delta.Len2() != 0) {
      last += delta;
      srm::motion::segment_t *p = new srm::motion::segment_t(last.x, last.y);
      primitive->push_back(p);
    }
  }

  // handle the wrong number of arguments
  if (size - counter > 0) {
    state = srm::state_t::error;
    return;
  }
}

/**
 * Processes the lineto command with absolute coordinates as arguments ("L")
 * @param[in] nums vector of numeric command arguments
 */
void srm::path_t::PathLAbs(const std::vector<double> &nums) noexcept {
  size_t counter = 0;
  size_t size = nums.size();

  // add a sequence of line segments to a primitive
  for (; size - counter > 1; counter += 2) {
    srm::vec_t cur(nums[counter], nums[counter + 1]);
    if ((cur - last).Len2() != 0) {
      last = cur;
      srm::motion::segment_t *p = new srm::motion::segment_t(last.x, last.y);
      primitive->push_back(p);
    }
  }

  // handle the wrong number of arguments
  if (size - counter > 0) {
    state = srm::state_t::error;
    return;
  }
}

/**
 * Processes the lineto command with relative coordinates as arguments ("l")
 * @param[in] nums vector of numeric command arguments
 */
void srm::path_t::PathLRel(const std::vector<double> &nums) noexcept {
  size_t counter = 0;
  size_t size = nums.size();

  // add a sequence of line segments to a primitive
  for (; size - counter > 1; counter += 2) {
    srm::vec_t delta(nums[counter], nums[counter + 1]);
    if (delta.Len2() != 0) {
      last += delta;
      srm::motion::segment_t *p = new srm::motion::segment_t(last.x, last.y);
      primitive->push_back(p);
    }
  }

  // handle the wrong number of arguments
  if (size - counter > 0) {
    state = srm::state_t::error;
    return;
  }
}

/**
 * Processes the horizontal line command with absolute coordinates as arguments ("H")
 * @param[in] nums vector of numeric command arguments
 */
void srm::path_t::PathHAbs(const std::vector<double> &nums) noexcept {
  size_t counter = 0;
  size_t size = nums.size();

  // handle the wrong number of arguments
  if(size == 0) {
    state = srm::state_t::error;
    return;
  }

  // add a sequence of line segments to a primitive
  for (; size > counter; counter++) {
    if (nums[counter] != last.x) {
      last.x = nums[counter];
      srm::motion::segment_t *p = new srm::motion::segment_t(last.x, last.y);
      primitive->push_back(p);
    }
  }
}

/**
 * Processes the horizontal line command with relative coordinates as arguments ("h")
 * @param[in] nums vector of numeric command arguments
 */
void srm::path_t::PathHRel(const std::vector<double> &nums) noexcept {
  size_t counter = 0;
  size_t size = nums.size();

  // handle the wrong number of arguments
  if (size == 0) {
    state = srm::state_t::error;
    return;
  }

  // add a sequence of line segments to a primitive
  for (; size > counter; counter++) {
    if (nums[counter] != 0) {
      last.x += nums[counter];
      srm::motion::segment_t *p = new srm::motion::segment_t(last.x, last.y);
      primitive->push_back(p);
    }
  }
}

/**
 * Processes the vertical line command with absolute coordinates as arguments ("V")
 * @param[in] nums vector of numeric command arguments
 */
void srm::path_t::PathVAbs(const std::vector<double> &nums) noexcept {
  size_t counter = 0;
  size_t size = nums.size();

  // handle the wrong number of arguments
  if (size == 0) {
    state = srm::state_t::error;
    return;
  }

  // add a sequence of line segments to a primitive
  for (; size > counter; counter++) {
    if (nums[counter] != last.y) {
      last.y = nums[counter];
      srm::motion::segment_t *p = new srm::motion::segment_t(last.x, last.y);
      primitive->push_back(p);
    }
  }
}

/**
 * Processes the vertical line command with relative coordinates as arguments ("v")
 * @param[in] nums vector of numeric command arguments
 */
void srm::path_t::PathVRel(const std::vector<double> &nums) noexcept {
  size_t counter = 0;
  size_t size = nums.size();

  // handle the wrong number of arguments
  if (size == 0) {
    state = srm::state_t::error;
    return;
  }

  // add a sequence of line segments to a primitive
  for (; size > counter; counter++) {
    if (nums[counter] != 0) {
      last.y += nums[counter];
      srm::motion::segment_t *p = new srm::motion::segment_t(last.x, last.y);
      primitive->push_back(p);
    }
  }
}

/**
 * Processes the subpath close command ("Z" or "z")
 * @param[in] nums vector of numeric command arguments
 * @warning If the vector of arguments is not empty, then there is an error in the path record
 */
void srm::path_t::PathZ(const std::vector<double> &nums) noexcept {
  // add the subpath close line segments to a primitive
  if ((last - primitive->start).Len2() != 0) {
    last = primitive->start;
    srm::motion::segment_t *p = new srm::motion::segment_t(last.x, last.y);
    primitive->push_back(p);
  }

  // handle the wrong number of arguments
  if (nums.size() != 0) {
    state = srm::state_t::error;
    return;
  }
}

/**
 * Processes the cubic bezier curve command with absolute coordinates as arguments ("C")
 * @param[in] nums vector of numeric command arguments
 */
void srm::path_t::PathCAbs(const std::vector<double> &nums) noexcept {
  size_t counter = 0;
  size_t size = nums.size();

  for (; size - counter > 5; counter += 6) {
    build_bezier_t bezier;
    // starting point
    bezier.push_back(last);
    // first check point
    checkPoint = srm::vec_t(nums[counter], nums[counter + 1]);
    bezier.push_back(checkPoint);
    // second check point
    checkPoint = srm::vec_t(nums[counter + 2], nums[counter + 3]);
    bezier.push_back(checkPoint);
    // end point
    last = srm::vec_t(nums[counter + 4], nums[counter + 5]);
    bezier.push_back(last);
    if ((bezier[0] - bezier[1]).Len2() != 0 || (bezier[1] - bezier[2]).Len2() != 0 || (bezier[2] - bezier[3]).Len2() != 0) {
      std::vector<vec_t> res;
      res = bezier.Sampling(accuracy);
      // add a sequence of line segments to a primitive
      for (auto& r : res) {
        srm::motion::segment_t *p = new srm::motion::segment_t(r.x, r.y);
        primitive->push_back(p);
      }
    }
  }

  // handle the wrong number of arguments
  if (size - counter > 0) {
    state = srm::state_t::error;
    return;
  }
}

/**
 * Processes the cubic bezier curve command with relative coordinates as arguments ("c")
 * @param[in] nums vector of numeric command arguments
 */
void srm::path_t::PathCRel(const std::vector<double> &nums) noexcept {
  size_t counter = 0;
  size_t size = nums.size();

  for (; size - counter > 5; counter += 6) {
    build_bezier_t bezier;
    // starting point
    bezier.push_back(last);
    // first check point
    checkPoint = last + srm::vec_t(nums[counter], nums[counter + 1]);
    bezier.push_back(checkPoint);
    // second check point
    checkPoint = last + srm::vec_t(nums[counter + 2], nums[counter + 3]);
    bezier.push_back(checkPoint);
    // end point
    last += srm::vec_t(nums[counter + 4], nums[counter + 5]);
    bezier.push_back(last);
    if ((bezier[0] - bezier[1]).Len2() != 0 || (bezier[1] - bezier[2]).Len2() != 0 || (bezier[2] - bezier[3]).Len2() != 0) {
      std::vector<vec_t> res;
      res = bezier.Sampling(accuracy);
      // add a sequence of line segments to a primitive
      for (auto& r : res) {
        srm::motion::segment_t *p = new srm::motion::segment_t(r.x, r.y);
        primitive->push_back(p);
      }
    }
  }

  // handle the wrong number of arguments
  if (size - counter > 0) {
    state = srm::state_t::error;
    return;
  }
}

/**
 * Processes the quadratic bezier curve command with absolute coordinates as arguments ("Q")
 * @param[in] nums vector of numeric command arguments
 */
void srm::path_t::PathQAbs(const std::vector<double> &nums) noexcept {
  size_t counter = 0;
  size_t size = nums.size();

  for (; size - counter > 3; counter += 4) {
    build_bezier_t bezier;
    // starting point
    bezier.push_back(last);
    // check point
    checkPoint = srm::vec_t(nums[counter], nums[counter + 1]);
    bezier.push_back(checkPoint);
    // end point
    last = srm::vec_t(nums[counter + 2], nums[counter + 3]);
    bezier.push_back(last);
    if ((bezier[0] - bezier[1]).Len2() != 0 || (bezier[1] - bezier[2]).Len2() != 0) {
      std::vector<vec_t> res;
      res = bezier.Sampling(accuracy);
      // add a sequence of line segments to a primitive
      for (auto& r : res) {
        srm::motion::segment_t *p = new srm::motion::segment_t(r.x, r.y);
        primitive->push_back(p);
      }
    }
  }
  
  // handle the wrong number of arguments
  if (size - counter > 0) {
    state = srm::state_t::error;
    return;
  }
}

/**
 * Processes the quadratic bezier curve command with relative coordinates as arguments ("q")
 * @param[in] nums vector of numeric command arguments
 */
void srm::path_t::PathQRel(const std::vector<double> &nums) noexcept {
  size_t counter = 0;
  size_t size = nums.size();

  for (; size - counter > 3; counter += 4) {
    build_bezier_t bezier;
    // starting point
    bezier.push_back(last);
    // check point
    checkPoint = last + srm::vec_t(nums[counter], nums[counter + 1]);
    bezier.push_back(checkPoint);
    // end point
    last += srm::vec_t(nums[counter + 2], nums[counter + 3]);
    bezier.push_back(last);
    if ((bezier[0] - bezier[1]).Len2() != 0 || (bezier[1] - bezier[2]).Len2() != 0) {
      std::vector<vec_t> res;
      res = bezier.Sampling(accuracy);
      // add a sequence of line segments to a primitive
      for (auto& r : res) {
        srm::motion::segment_t *p = new srm::motion::segment_t(r.x, r.y);
        primitive->push_back(p);
      }
    }
  }

  // handle the wrong number of arguments
  if (size - counter > 0) {
    state = srm::state_t::error;
    return;
  }
}

/**
 * Processes the smooth cubic bezier curve command with absolute coordinates as arguments ("S")
 * @param[in] nums vector of numeric command arguments
 */
void srm::path_t::PathSAbs(const std::vector<double> &nums) noexcept {
  size_t counter = 0;
  size_t size = nums.size();

  for (; size - counter > 3; counter += 4) {
    build_bezier_t bezier;
    // starting point
    bezier.push_back(last);
    // first check point
    if (lastCommand == 's' || lastCommand == 'S' || lastCommand == 'c' || lastCommand == 'C')
      checkPoint = last * 2 - checkPoint;
    else {
      checkPoint = last;
      lastCommand = 'S';
    }
    bezier.push_back(checkPoint);
    // second check point
    checkPoint = srm::vec_t(nums[counter], nums[counter + 1]);
    bezier.push_back(checkPoint);
    // end point
    last = srm::vec_t(nums[counter + 2], nums[counter + 3]);
    bezier.push_back(last);
    if ((bezier[0] - bezier[1]).Len2() != 0 || (bezier[1] - bezier[2]).Len2() != 0 || (bezier[2] - bezier[3]).Len2() != 0) {
      std::vector<vec_t> res;
      res = bezier.Sampling(accuracy);
      // add a sequence of line segments to a primitive
      for (auto& r : res) {
        srm::motion::segment_t *p = new srm::motion::segment_t(r.x, r.y);
        primitive->push_back(p);
      }
    }
  }

  // handle the wrong number of arguments
  if (size - counter > 0) {
    state = srm::state_t::error;
    return;
  }
}

/**
 * Processes the smooth cubic bezier curve command with relative coordinates as arguments ("s")
 * @param[in] nums vector of numeric command arguments
 */
void srm::path_t::PathSRel(const std::vector<double> &nums) noexcept {
  size_t counter = 0;
  size_t size = nums.size();

  for (; size - counter > 3; counter += 4) {
    build_bezier_t bezier;
    // starting point
    bezier.push_back(last);
    // first check point
    if (lastCommand == 's' || lastCommand == 'S' || lastCommand == 'c' || lastCommand == 'C')
      checkPoint = last * 2 - checkPoint;
    else {
      checkPoint = last;
      lastCommand = 's';
    }
    bezier.push_back(checkPoint);
    // second check point
    checkPoint = last + srm::vec_t(nums[counter], nums[counter + 1]);
    bezier.push_back(checkPoint);
    // end point
    last += srm::vec_t(nums[counter + 2], nums[counter + 3]);
    bezier.push_back(last);
    if ((bezier[0] - bezier[1]).Len2() != 0 || (bezier[1] - bezier[2]).Len2() != 0 || (bezier[2] - bezier[3]).Len2() != 0) {
      std::vector<vec_t> res;
      res = bezier.Sampling(accuracy);
      // add a sequence of line segments to a primitive
      for (auto& r : res) {
        srm::motion::segment_t *p = new srm::motion::segment_t(r.x, r.y);
        primitive->push_back(p);
      }
    }
  }

  // handle the wrong number of arguments
  if (size - counter > 0) {
    state = srm::state_t::error;
    return;
  }
}

/**
 * Processes the smooth quadratic bezier curve command with absolute coordinates as arguments ("T")
 * @param[in] nums vector of numeric command arguments
 */
void srm::path_t::PathTAbs(const std::vector<double> &nums) noexcept {
  size_t counter = 0;
  size_t size = nums.size();

  for (; size - counter > 1; counter += 2) {
    build_bezier_t bezier;
    // starting point
    bezier.push_back(last);
    // check point
    if (lastCommand == 't' || lastCommand == 'T' || lastCommand == 'q' || lastCommand == 'Q')
      checkPoint = last * 2 - checkPoint;
    else {
      checkPoint = last;
      lastCommand = 'T';
    }
    bezier.push_back(checkPoint);
    // end point
    last = srm::vec_t(nums[counter], nums[counter + 1]);
    bezier.push_back(last);
    if ((bezier[0] - bezier[1]).Len2() != 0 || (bezier[1] - bezier[2]).Len2() != 0) {
      std::vector<vec_t> res;
      res = bezier.Sampling(accuracy);
      // add a sequence of line segments to a primitive
      for (auto& r : res) {
        srm::motion::segment_t *p = new srm::motion::segment_t(r.x, r.y);
        primitive->push_back(p);
      }
    }
  }
 
  // handle the wrong number of arguments
  if (size - counter > 0) {
    state = srm::state_t::error;
    return;
  }
}

/**
 * Processes the smooth quadratic bezier curve command with relative coordinates as arguments ("t")
 * @param[in] nums vector of numeric command arguments
 */
void srm::path_t::PathTRel(const std::vector<double> &nums) noexcept {
  size_t counter = 0;
  size_t size = nums.size();

  for (; size - counter > 1; counter += 2) {
    build_bezier_t bezier;
    // starting point
    bezier.push_back(last);
    // check point
    if (lastCommand == 't' || lastCommand == 'T' || lastCommand == 'q' || lastCommand == 'Q')
      checkPoint = last * 2 - checkPoint;
    else {
      checkPoint = last;
      lastCommand = 't';
    }
    bezier.push_back(checkPoint);
    // end point
    last += srm::vec_t(nums[counter], nums[counter + 1]);
    bezier.push_back(last);
    if ((bezier[0] - bezier[1]).Len2() != 0 || (bezier[1] - bezier[2]).Len2() != 0) {
      std::vector<vec_t> res;
      res = bezier.Sampling(accuracy);
      // add a sequence of line segments to a primitive
      for (auto& r : res) {
        srm::motion::segment_t *p = new srm::motion::segment_t(r.x, r.y);
        primitive->push_back(p);
      }
    }
  }

  // handle the wrong number of arguments
  if (size - counter > 0) {
    state = srm::state_t::error;
    return;
  }
}

/**
 * Processes the elliptical arc command with absolute coordinates as arguments ("A")
 * @param[in] nums vector of numeric command arguments
 */
void PathAAbs(const std::vector<double> &nums) noexcept {
  //TODO
}

/**
 * Processes the elliptical arc command with relative coordinates as arguments ("a")
 * @param[in] nums vector of numeric command arguments
 */
void PathARel(const std::vector<double> &nums) noexcept {
  //TODO
}

/**
 * Main path parsing function
 * @param[in] tag pointer to path node in xml DOM
 */
void srm::path_t::ParsePath(const rapidxml::xml_node<> *tag) noexcept {
  const char *attr = tag->first_attribute("d")->value();
  char command;

  while (*attr != '\0') {
    // skip space
    if (isspace(static_cast<unsigned char>(*attr))) {
      attr++;
      continue;
    }
    // command handling
    if (isalpha(static_cast<unsigned char>(*attr))) {
      command = *attr;
      if (state == srm::state_t::start) {
        // wrong start of path string
        if (command != 'm' && command != 'M') {
          state = srm::state_t::error;
          break;
        }
      }

      std::vector<double> nums;
      nums = this->GetNums(&attr);
      switch (command) {
        case 'M':
          this->PathMAbs(nums);
          break;
        case 'm':
          this->PathMRel(nums);
          break;
        case 'L':
          this->PathLAbs(nums);
          break;
        case 'l':
          this->PathLRel(nums);
          break;
        case 'H':
          this->PathHAbs(nums);
          break;
        case 'h':
          this->PathHRel(nums);
          break;
        case 'V':
          this->PathVAbs(nums);
          break;
        case 'v':
          this->PathVRel(nums);
          break;
        case 'Z':
        case 'z':
          this->PathZ(nums);
          break;
        case 'C':
          this->PathCAbs(nums);
          break;
        case 'c':
          this->PathCRel(nums);
          break;
        case 'Q':
          this->PathQAbs(nums);
          break;
        case 'q':
          this->PathQRel(nums);
          break;
        case 'S':
          this->PathSAbs(nums);
          break;
        case 's':
          this->PathSRel(nums);
          break;
        case 'T':
          this->PathTAbs(nums);
          break;
        case 't':
          this->PathTRel(nums);
          break;
        default:
          state = srm::state_t::error;
          break;
      }

      // discard the part after the first error
      if (state == srm::state_t::error)
        break;

      state = srm::state_t::command;
      lastCommand = command;
    }
    // unhandled character
    else {
      state = srm::state_t::error;
      break;
    }
  }
  // add the last primitive
  if(primitive != nullptr)
    if (primitive->size() > 0)
      primitives->push_back(primitive);
    else
      delete primitive;
}