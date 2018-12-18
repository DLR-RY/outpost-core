///////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2016 Vicente J. Botet Escriba. All rights reserved.
//
// Some part of this file have been adapted from libc++ <algoritm> file
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#ifndef STDEX_ALGORIHM_H
#define STDEX_ALGORIHM_H

#include <type_traits>
#include <iterator>
#include <algorithm>

namespace stdex
{


// The following code has been adapted from libc++ <algoritm> file
// BEGIN   libc++
namespace detail {
template <class _T1, class _T2 = _T1>
struct equal_to
{
     bool operator()(const _T1& _x, const _T1& _y) const {return _x == _y;}
     bool operator()(const _T1& _x, const _T2& _y) const {return _x == _y;}
     bool operator()(const _T2& _x, const _T1& _y) const {return _x == _y;}
     bool operator()(const _T2& _x, const _T2& _y) const {return _x == _y;}
};

template <class _T1>
struct equal_to<_T1, _T1>
{
  // fixme this should work for C++11
  GSL_CXX14_CONSTEXPR
    bool operator()(const _T1& _x, const _T1& _y) const {return _x == _y;}
};

template <class _T1>
struct equal_to<const _T1, _T1>
{
  // fixme this should work for C++11
  GSL_CXX14_CONSTEXPR
    bool operator()(const _T1& _x, const _T1& _y) const {return _x == _y;}
};

template <class _T1>
struct equal_to<_T1, const _T1>
{
  // fixme this should work for C++11
  GSL_CXX14_CONSTEXPR
    bool operator()(const _T1& _x, const _T1& _y) const {return _x == _y;}
};

template <class _BinaryPredicate, class _InputIterator1, class _InputIterator2>
inline
bool
equal(_InputIterator1 _first1, _InputIterator1 _last1,
        _InputIterator2 _first2, _InputIterator2 _last2, _BinaryPredicate _pred,
        std::input_iterator_tag, std::input_iterator_tag )
{
    for (; _first1 != _last1 && _first2 != _last2; ++_first1, (void) ++_first2)
        if (!_pred(*_first1, *_first2))
            return false;
    return _first1 == _last1 && _first2 == _last2;
}

template <class _BinaryPredicate, class _RandomAccessIterator1, class _RandomAccessIterator2>
inline
bool
equal(_RandomAccessIterator1 _first1, _RandomAccessIterator1 _last1,
        _RandomAccessIterator2 _first2, _RandomAccessIterator2 _last2, _BinaryPredicate _pred,
        std::random_access_iterator_tag, std::random_access_iterator_tag )
{
    if ( std::distance(_first1, _last1) != std::distance(_first2, _last2))
        return false;
    return std::equal<_RandomAccessIterator1, _RandomAccessIterator2,
                        typename std::add_lvalue_reference<_BinaryPredicate>::type>
                       (_first1, _last1, _first2, _pred );
}

}

template <class _InputIterator1, class _InputIterator2, class _BinaryPredicate>
inline
bool
equal(_InputIterator1 _first1, _InputIterator1 _last1,
      _InputIterator2 _first2, _InputIterator2 _last2, _BinaryPredicate _pred )
{
    return stdex::detail::equal<typename std::add_lvalue_reference<_BinaryPredicate>::type>
       (_first1, _last1, _first2, _last2, _pred,
        typename std::iterator_traits<_InputIterator1>::iterator_category(),
        typename std::iterator_traits<_InputIterator2>::iterator_category());
}

template <class _InputIterator1, class _InputIterator2>
inline
bool
equal(_InputIterator1 _first1, _InputIterator1 _last1,
      _InputIterator2 _first2, _InputIterator2 _last2)
{
    typedef typename std::iterator_traits<_InputIterator1>::value_type _v1;
    typedef typename std::iterator_traits<_InputIterator2>::value_type _v2;
    return stdex::detail::equal(_first1, _last1, _first2, _last2, stdex::detail::equal_to<_v1, _v2>(),
        typename std::iterator_traits<_InputIterator1>::iterator_category(),
        typename std::iterator_traits<_InputIterator2>::iterator_category());
}
// END   libc++

}

#endif // STDEX_ALGORIHM_H
