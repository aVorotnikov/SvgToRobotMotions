/**
 * @file
 * @brief Split primitive header file
 * @authors Vorotnikov Andrey
 * @date 17.03.2021
 *
 * Contains function definition to split primitives by points out of svg cs
 */

#pragma once

#ifndef __SPLIT_PRIMS_H_INCLUDED
#define __SPLIT_PRIMS_H_INCLUDED

#include <list>
#include "../primitive/primitive.h"

/** \brief Project namespace */
namespace srm {
  /**
   * Split primitives in list function
   * @param[out] prims primitive to split
   */
  void SplitPrimitives(std::list<primitive_t *> *prims);
}

#endif /* __SPLIT_PRIMS_H_INCLUDED */
