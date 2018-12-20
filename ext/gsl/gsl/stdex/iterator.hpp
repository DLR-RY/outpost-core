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

#ifndef STDEX_ITERATOR_H
#define STDEX_ITERATOR_H

#include <iterator>

namespace stdex
{

template< class C >
constexpr auto cbegin( const C& c ) -> decltype(std::begin(c)) {
  return std::begin(c);
}
template< class C >
constexpr auto cend( const C& c ) -> decltype(std::end(c)) {
  return std::end(c);
}

}

#endif // STDEX_ITERATOR_H
