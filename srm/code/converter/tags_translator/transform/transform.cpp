/**
 * @file
 * @brief transform_t class source file
 * @authors Vorotnikov Andrey, Pavlov Ilya
 * @date 03.04.2021
 *
 * Contains transform_t class realisation to process tags with attribute "transform"
 */

#include <srm.h>

#include <list>
#include <sstream>
#include <cstdarg>

/**
 * Default constructor
 */
srm::transform_t::transform_t() noexcept {
}

static bool _getArgs(const std::string &attrStr, size_t pos, size_t *newPos, int n, ...) {
  size_t bracketPos = attrStr.find('(', pos);
  if (bracketPos == std::string::npos)
    return false;

  for (auto i = pos; i < bracketPos; ++i)
    if (!isspace(attrStr[i]))
      return false;
 
  pos = bracketPos + 1;
  bracketPos = attrStr.find(')', pos);
  if (bracketPos == std::string::npos)
    return false;

  for (auto i = pos; i < bracketPos; ++i)
    if (!isspace(attrStr[i]) && !isdigit(attrStr[i]) && attrStr[i] != '-' && attrStr[i] != '.')
      return false;

  *newPos = bracketPos;

  std::istringstream vals(attrStr.substr(pos, bracketPos - pos));

  va_list args;
  va_start(args, n);

  if (n == 1) {
    vals >> *va_arg(args, double*);
    if (vals.fail()) {
      va_end(args);
      return false;
    }
  }
  else if (n == 2) {
    double x, y;
    vals >> x;
    if (vals.fail()) {
      va_end(args);
      return false;
    }
    vals >> y;
    if (vals.fail()) {
      y = 0;
      *va_arg(args, double*) = x;
      *va_arg(args, double*) = y;
      va_end(args);
      return false;
    }
    *va_arg(args, double*) = x;
    *va_arg(args, double*) = y;
  }
  else if (n == 3) {
    double a, x, y;

    vals >> a;
    if (vals.fail()) {
      va_end(args);
      return false;
    }
    
    vals >> x;
    if (vals.fail())
      x = y = 0;
    else {
      vals >> y;
      if (vals.fail()) {
        va_end(args);
        return false;
      }
    }
    
    *va_arg(args, double*) = a;
    *va_arg(args, double*) = x;
    *va_arg(args, double*) = y;
  }
  else {
    while (n--)
      vals >> *va_arg(args, double*);

    if (vals.fail()) {
      va_end(args);
      return false;
    }
  }

  va_end(args);

  std::string check;
  vals >> check;
  return check == "";
}

/**
 * Constuctor from svg attribute
 * @param[in] trnsAttr string with svg transform attibute value
 */
srm::transform_t::transform_t(std::string transAttr) noexcept {

  for (size_t i = 0; i < transAttr.length(); ++i)
    if (transAttr[i] == ',')
      transAttr[i] = ' ';

  translator_t* trans = translator_t::GetPtr();
  std::list<transform_t> transforms;
  std::string type;

  for (size_t i = 0; i < transAttr.length(); ++i) {
    if (transAttr[i] == 'm') {
      type = transAttr.substr(i, 6);
      if (type == "matrix") {
        i += 6;
        double a, b, c, d, e, f;
        if (_getArgs(transAttr, i, &i, 6, &a, &b, &c, &d, &e, &f)) {
          transform_t matrixTransform;
          matrixTransform.SetMatrix(a, c, e, b, d, f);
          transforms.push_back(matrixTransform);
        }
        else {
          trans->WriteLog("Warning: invalid transform attribute");
          return;
        }
      }
      else {
        trans->WriteLog("Warning: invalid transform attribute");
        return;
      }
    }
    else if (transAttr[i] == 't') {
      type = transAttr.substr(i, 9);
      if (type == "translate") {
        i += 9;
        double x, y = 1;
        if (_getArgs(transAttr, i, &i, 2, &x, &y)) {
          transform_t translateTransform;
          translateTransform.SetMatrix(1, 0, x, 0, 1, y);
          transforms.push_back(translateTransform);
        }
        else if (y == 0) {
          transform_t translateTransform;
          translateTransform.SetMatrix(1, 0, x, 0, 1, y);
          transforms.push_front(translateTransform);
        }
        else {
          trans->WriteLog("Warning: invalid transform attribute");
          return;
        }
      }
      else {
        trans->WriteLog("Warning: invalid transform attribute");
        return;
      }
    }
    else if (transAttr[i] == 'r') {
      type = transAttr.substr(i, 6);
      if (type == "rotate") {
        i += 6;
        double angle, x, y;
        if (_getArgs(transAttr, i, &i, 3, &angle, &x, &y)) {
          transform_t translate;
          transform_t rotate;
          transform_t translateBack;
          angle = angle * srm::pi / 180.0;
          translate.SetMatrix(1, 0, x, 0, 1, y);
          rotate.SetMatrix(cos(angle), -sin(angle), 0, sin(angle), cos(angle), 0);
          translateBack.SetMatrix(1, 0, -x, 0, 1,- y);
          transforms.push_back(translate);
          transforms.push_back(rotate);
          transforms.push_back(translateBack);
        }
        else {
          trans->WriteLog("Warning: invalid transform attribute");
          return;
        }
      }
      else {
        trans->WriteLog("Warning: invalid transform attribute");
        return;
      }
    }
    else if (transAttr[i] == 's') {
      type = transAttr.substr(i, 5);
      i += 5;
      if (type == "scale") {
        double x, y = 1;
        if (_getArgs(transAttr, i, &i, 2, &x, &y)) {
          transform_t scaleTransform;
          scaleTransform.SetMatrix(x, 0, 0, 0, y, 0);
          transforms.push_back(scaleTransform);
        }
        else if (y == 0) {
          transform_t scaleTransform;
          scaleTransform.SetMatrix(x, 0, 0, 0, x, 0);
          transforms.push_back(scaleTransform);
        }
        else {
          trans->WriteLog("Warning: invalid transform attribute");
          return;
        }
      }
      else if (type == "skewX") {
        double angle;
        if (_getArgs(transAttr, i, &i, 1, &angle)) {
          transform_t skewXTransform;
          angle = angle * srm::pi / 180.0;
          skewXTransform.SetMatrix(1, tan(angle), 0, 0, 1, 0);
          transforms.push_back(skewXTransform);
        }
        else {
          trans->WriteLog("Warning: invalid transform attribute");
          return;
        }
      }
      else if (type == "skewY") {
        double angle;
        if (_getArgs(transAttr, i, &i, 1, &angle)) {
          transform_t skewYTransform;
          angle = angle * srm::pi / 180.0;
          skewYTransform.SetMatrix(1, 0, 0, tan(angle), 1, 0);
          transforms.push_back(skewYTransform);
        }
        else {
          trans->WriteLog("Warning: invalid transform attribute");
          return;
        }
      }
      else{
        trans->WriteLog("Warning: invalid transform attribute");
        return;
      }
    }
    else if (!isspace(transAttr[i])) {
      trans->WriteLog("Warning: invalid transform attribute");
      return;
    }
  }

  for (auto transform : transforms)
    *this *= transform;
}

/**
 * Set transformation matrix;
 * @param[in] m00,..m12 values is matrix [[m00 m01 m02] [m10 m11 m12]]
 */
void srm::transform_t::SetMatrix(double m00, double m01, double m02, double m10, double m11, double m12) noexcept {
  matrix[0][0] = m00;
  matrix[0][1] = m01;
  matrix[0][2] = m02;

  matrix[1][0] = m10;
  matrix[1][1] = m11;
  matrix[1][2] = m12;
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
  double tmp;

  tmp = primitive->start.x;
  primitive->start.x = matrix[0][0] * primitive->start.x + matrix[0][1] * primitive->start.y + matrix[0][2];
  primitive->start.y = matrix[1][0] * tmp + matrix[1][1] * primitive->start.y + matrix[1][2];

  for (auto &segment : *primitive) {
    tmp = segment.point.x;
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
