/**
 * @file
 * @brief Common definitions header file
 * @authors Vorotnikov Andrey
 * @date 07.03.2021
 *
 * Contains common definitions for converter
 */

#pragma once

#ifndef __DEFS_H_INLUDED
#define __DEFS_H_INLUDED

#include "../math/vector2.h"
#include "../math/vector3.h"

/** Project namespace */
namespace srm {
  /** 2 dimensional vector class for converter */
  using vec_t = math::vector2_t<>;

  /** 3 dimensional vector class for converter */
  using vec3_t = math::vector3_t<>;
}

#endif /* __DEFS_H_INLUDED */
