/**
 * @file
 * @brief Precompiled header
 * @authors Vorotnikov Andrey, Pavlov Ilya
 * @date 15.03.2021
 *
 * Contains includes of header file to accelerate compilation
 */

#pragma once

#ifndef __SRM_H_INCLUDED
#define __SRM_H_INCLUDED

#include "math/vector2.h"
#include "math/vector3.h"

#include "converter/defs.h"
#include "converter/translator.h"
#include "converter/rapidxml.hpp"
#include "converter/primitive/primitive.h"
#include "converter/split_primitives/split_prims.h"
#include "converter/tags_translator/tag/tag.h"
#include "converter/tags_translator/tags_translator.h"
#include "converter/tags_translator/path/bezier/bezier.h"
#include "converter/tags_translator/ellipse/ellipse.h"
#include "converter/tags_translator/transform/transform.h"
#include "converter/tags_translator/path/path.h"
#include "converter/robot_conf/robot_conf.h"
#include "converter/robot_conf/cs/cs.h"
#include "converter/split_primitives/split_prims.h"
#include "converter/fill/fill.h"

#endif /* __SRM_H_INCLUDED */
