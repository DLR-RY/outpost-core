///////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2016 Vicente J. Botet Escriba. All rights reserved.
//
//
// This code is licensed under the MIT License (MIT).
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#ifndef STDEX_TYPE_TRAITS_H
#define STDEX_TYPE_TRAITS_H

#include <type_traits>

namespace stdex
{

template <bool B, class T=void>
using  enable_if_t = typename std::enable_if<B,T>::type;

template< class T >
using add_const_t    = typename std::add_const<T>::type;

template< class T >
using add_lvalue_reference_t = typename std::add_lvalue_reference<T>::type;

template< class T >
using add_pointer_t = typename std::add_pointer<T>::type;

template< bool B, class T, class F >
using conditional_t = typename std::conditional<B,T,F>::type;

template< class T >
using decay_t = typename std::decay<T>::type;

template< class T >
using remove_all_extents_t = typename std::remove_all_extents<T>::type;

template< class T >
using remove_cv_t       = typename std::remove_cv<T>::type;

template< class T >
using remove_const_t    = typename std::remove_const<T>::type;

template< class T >
using remove_reference_t = typename std::remove_reference<T>::type;

}

#endif // STDEX_TYPE_TRAITS_H
