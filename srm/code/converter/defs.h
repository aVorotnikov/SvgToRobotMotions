/**
 * @file
 * @brief Common definitions header file
 * @authors Vorotnikov Andrey
 * @date 23.02.2021
 *
 * Contains common definitions for converter
 */

#pragma once

#ifndef __DEFS_H_INLUDED
#define __DEFS_H_INLUDED

#include "../math/vector.h"

/** Project namespace */
namespace srm {
  /** 2 dimensional vector class for converter */
  using vec_t = math::vector2_t<>;
}

#endif /* __DEFS_H_INLUDED */
