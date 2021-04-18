/**
 * @file
 * @brief Fill primitive header file
 * @authors Pavlov Ilya
 * @date 18.04.2021
 *
 * Contains declaration of function for filling primitives
 */

#pragma once

#ifndef __FILL_H_INCLUDED
#define __FILL_H_INCLUDED

#include <srm.h>

#include <ostream>

 /** \brief Project namespace */
namespace srm {
    /**
     * Gen and print code for filling primitive
     * @param[in] out output stream
     * @param[in] primitive for filling
     * @warning primitive can be changed
     */
    void FillPrimitive(std::ostream &out, const srm::primitive_t &primitive);
}

#endif /* __FILL_H_INCLUDED */
