/************************************************************************************
*                                                                                   *
*   Copyright (c) 2014, 2015 Axel Menzel <info@axelmenzel.de>                       *
*                                                                                   *
*   This file is part of RTTR (Run Time Type Reflection)                            *
*   License: MIT License                                                            *
*                                                                                   *
*   Permission is hereby granted, free of charge, to any person obtaining           *
*   a copy of this software and associated documentation files (the "Software"),    *
*   to deal in the Software without restriction, including without limitation       *
*   the rights to use, copy, modify, merge, publish, distribute, sublicense,        *
*   and/or sell copies of the Software, and to permit persons to whom the           *
*   Software is furnished to do so, subject to the following conditions:            *
*                                                                                   *
*   The above copyright notice and this permission notice shall be included in      *
*   all copies or substantial portions of the Software.                             *
*                                                                                   *
*   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR      *
*   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,        *
*   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE     *
*   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER          *
*   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,   *
*   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE   *
*   SOFTWARE.                                                                       *
*                                                                                   *
*************************************************************************************/

#ifndef RTTR_WRAPPER_MAPPER_H_
#define RTTR_WRAPPER_MAPPER_H_

#include "rttr/base/core_prerequisites.h"

namespace rttr
{

//////////////////////////////////////////////////////////////////////////////////////

namespace detail { struct invalid_wrapper_type { }; }

/*!
 * The \ref wrapper_mapper class is an interface class to register custom wrapper types.
 *
 * A wrapper type is a class which encapsulate an instance of another type.
 * This are for instance smart pointer classes, e.g. `std::shared_ptr<T>` or `std::unique_ptr<T>`.
 * Out of the box, RTTR recognize following wrapper classes:
 * - \p `std::shared_ptr<T>` 
 * - \p `std::reference_wrapper<T>`
 * - \p `std::weak_ptr<T>`
 * - \p `std::unique_ptr<T>`
 *
 * Custom wrapper types
 * --------------------
 * In order to work with custom wrapper types, its required to specialize the class 
 * \ref rttr::wrapper_mapper<T> "wrapper_mapper<T>". Therefore you have to provide two typedefs:
 * 1. `typedef T::encapsulated_type wrapped_type;`
 * 2. `typedef T type`
 *
 * And two functions:
 * 1. `static wrapped_type get(const T& obj);`
 * 2. `static T create(wrapped_type& obj);`
 *
 * Following code example illustrates how to add a specialization:
 * \code{.cpp}
 *
 *  // custom_type.h
 *
 *  template<typename T>
 *  class custom_type
 *  {
 *  public:
 *      custom_type(T& obj) : m_data(std::addressof(obj)) {}
 *      T& get_data() { return *m_value; }
 *  private:
 *      T* m_data;
 *  };
 *
 *  // the best place for the specialization, is in the same header file like the type itself
 *  namespace rttr
 *  {
 *
 *  template<typename T>
 *  struct wrapper_mapper<custom_type<T>>
 *  {
 *      typedef decltype(std::declval<custom_type<T>>().get_data()) wrapped_type;
 *      typedef custom_type<T> type;
 *
 *      inline static wrapped_type get(const type& obj)
 *      {
 *         return obj.get_data();
 *      }
 *
 *      inline static type create(const wrapped_type& value)
 *      {
 *         return custom_type<T>(value);
 *      } 
 *  };
 *
 *  } // end namespace rttr
 * \endcode
 *
 * Remark that it is very important that the typedef for `wrapped_type` is the actual return type of the getter function.
 */
template<typename T>
struct wrapper_mapper
{
#ifndef DOXYGEN
    typedef detail::invalid_wrapper_type wrapped_type;
    typedef T type;
#else
    typedef wrapper_type::encapsulated_type wrapped_type;
    typedef T type;

    inline static wrapped_type get(const type& obj)
    {
        return obj.get();
    }

    inline static type create(const wrapped_type& value)
    {
        return type(value);
    } 
#endif
};

//////////////////////////////////////////////////////////////////////////////////////

} // end namespace rttr

#include "rttr/impl/wrapper_mapper_impl.h"

#endif // RTTR_WRAPPER_MAPPER_H_
