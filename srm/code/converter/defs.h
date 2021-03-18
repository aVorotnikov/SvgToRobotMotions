/**
 * @file
 * @brief Common definitions header file
 * @authors Vorotnikov Andrey
 * @date 13.03.2021
 *
 * Contains common definitions for converter
 */

#pragma once

#ifndef __DEFS_H_INLUDED
#define __DEFS_H_INLUDED

#include "../math/vector2.h"
#include "../math/vector3.h"
#include "../math/matr2.h"

/** Project namespace */
namespace srm {
  /** 2 dimensional vector class for converter */
  using vec_t = math::vector2_t<>;

  /** 3 dimensional vector class for converter */
  using vec3_t = math::vector3_t<>;

  /** pi number */
  constexpr double pi = 3.14159265358979323846;
}

#endif /* __DEFS_H_INLUDED */
