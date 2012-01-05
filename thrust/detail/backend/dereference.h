/*
 *  Copyright 2008-2011 NVIDIA Corporation
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */


/*! \file dereference.h
 *  \brief Overloads for everything dereferenceable by dereference.h
 */

#pragma once

#include <thrust/detail/type_traits.h>
#include <thrust/iterator/iterator_traits.h>
#include <thrust/detail/type_traits/is_metafunction_defined.h>
#include <iterator> // for std::output_iterator_tag below

namespace thrust
{

// forward declarations of iterator types

template<typename Element, typename Tag, typename Reference, typename Derived>
  class pointer;

template<typename T>
  class device_ptr;

namespace system
{

namespace cpp
{

template<typename T>
  class pointer;

} // end cpp

namespace omp
{

template<typename T>
  class pointer;

} // end omp

namespace tbb
{

template<typename T>
  class pointer;

} // end tbb

namespace cuda
{

template<typename T>
  class pointer;

} // end cuda

} // end system

namespace detail
{

template<typename Pointer>
  class normal_iterator;

template<typename Iterator, typename Tag>
  class tagged_iterator;

} // end detail


template<typename Value,
         typename Incrementable,
         typename Space>
  class constant_iterator;


template <class UnaryFunc, class Iterator, class Reference, class Value>
  class transform_iterator;

template<typename Incrementable,
         typename Space,
         typename Traversal,
         typename Difference>
  class counting_iterator;

template<typename Space>
  class discard_iterator;

template <typename IteratorTuple>
  class zip_iterator;

template <typename BidirectionalIterator>
  class reverse_iterator;

template <typename ElementIterator, typename IndexIterator>
  class permutation_iterator;

namespace detail
{

namespace backend
{

template <typename> struct dereference_result;

// in general, assume that dereference behaves as on host
// if the iterator is not a pure output iterator, return its reference
// otherwise, return a copy of the iterator, which enables
// types like std::back_insert_iterator to work
template<typename Iterator>
  struct dereference_result
    : thrust::detail::eval_if<
        thrust::detail::is_convertible<
          typename thrust::iterator_traits<Iterator>::iterator_category,
          std::output_iterator_tag
        >::value,
        thrust::detail::identity_<Iterator>,
        thrust::iterator_reference<Iterator>
      >
{};

// for non output iterators
template<typename Iterator>
  inline __host__ __device__
    typename dereference_result<Iterator>::type
      dereference(Iterator iter,
                  typename thrust::detail::disable_if<
                    thrust::detail::is_convertible<
                      typename thrust::iterator_traits<Iterator>::iterator_category,
                      std::output_iterator_tag
                    >::value
                  >::type * = 0)
{
  return *iter;
} // dereference

// for non output iterators
template<typename Iterator, typename IndexType>
  inline __host__ __device__
    typename dereference_result<Iterator>::type
      dereference(Iterator iter, IndexType n,
                  typename thrust::detail::disable_if<
                    thrust::detail::is_convertible<
                      typename thrust::iterator_traits<Iterator>::iterator_category,
                      std::output_iterator_tag
                    >::value
                  >::type * = 0)
{
  return iter[n];
} // dereference

// for pure output iterators
template<typename Iterator>
  inline __host__ __device__
    typename dereference_result<Iterator>::type
      dereference(Iterator iter,
                  typename thrust::detail::enable_if<
                    thrust::detail::is_convertible<
                      typename thrust::iterator_traits<Iterator>::iterator_category,
                      std::output_iterator_tag
                    >::value
                  >::type * = 0)
{
  return iter;
} // dereference

// for pure output iterators
template<typename Iterator, typename IndexType>
  inline __host__ __device__
    typename dereference_result<Iterator>::type
      dereference(Iterator iter, IndexType n,
                  typename thrust::detail::enable_if<
                    thrust::detail::is_convertible<
                      typename thrust::iterator_traits<Iterator>::iterator_category,
                      std::output_iterator_tag
                    >::value
                  >::type * = 0)
{
  return iter + n;
} // dereference


// pointer prototypes
template<typename Element, typename Tag, typename Reference, typename Derived>
  inline __host__ __device__
    typename dereference_result< thrust::pointer<Element,Tag,Reference,Derived> >::type
      dereference(thrust::pointer<Element,Tag,Reference,Derived> ptr);

template<typename Element, typename Tag, typename Reference, typename Derived, typename IndexType>
  inline __host__ __device__
    typename dereference_result< thrust::pointer<Element,Tag,Reference,Derived> >::type
      dereference(thrust::pointer<Element,Tag,Reference,Derived> ptr, IndexType n);


// device_ptr prototypes
template<typename T>
  inline __host__ __device__
    typename dereference_result< device_ptr<T> >::type
      dereference(device_ptr<T> ptr);

template<typename T, typename IndexType>
  inline __host__ __device__
    typename dereference_result< device_ptr<T> >::type
      dereference(thrust::device_ptr<T> ptr, IndexType n);



// normal_iterator
template<typename Pointer>
  inline __host__ __device__
    typename dereference_result< normal_iterator<Pointer> >::type
      dereference(const normal_iterator<Pointer> &iter);

template<typename Pointer, typename IndexType>
  inline __host__ __device__
    typename dereference_result< normal_iterator<Pointer> >::type
      dereference(const normal_iterator<Pointer> &iter, IndexType n);


// tagged_iterator
template<typename Iterator, typename Tag>
  inline __host__ __device__
    typename dereference_result< tagged_iterator< Iterator, Tag > >::type
      dereference(const tagged_iterator< Iterator, Tag > &iter);

template<typename Iterator, typename Tag, typename IndexType>
  inline __host__ __device__
    typename dereference_result< tagged_iterator< Iterator, Tag > >::type
      dereference(const tagged_iterator< Iterator, Tag > &iter, IndexType n);



// transform_iterator prototypes
template<typename UnaryFunc, typename Iterator, typename Reference, typename Value>
  inline __host__ __device__
    typename dereference_result< thrust::transform_iterator<UnaryFunc,Iterator,Reference,Value> >::type
      dereference(const thrust::transform_iterator<UnaryFunc,Iterator,Reference,Value> &iter);

template<typename UnaryFunc, typename Iterator, typename Reference, typename Value, typename IndexType>
  inline __host__ __device__
    typename dereference_result< thrust::transform_iterator<UnaryFunc,Iterator,Reference,Value> >::type
      dereference(const thrust::transform_iterator<UnaryFunc,Iterator,Reference,Value> &iter, IndexType n);



// counting_iterator prototypes
template<typename Incrementable, typename Space, typename Traversal, typename Difference>
  inline __host__ __device__
    typename dereference_result< thrust::counting_iterator<Incrementable,Space,Traversal,Difference> >::type
      dereference(const thrust::counting_iterator<Incrementable,Space,Traversal,Difference> &iter);

template<typename Incrementable, typename Space, typename Traversal, typename Difference, typename IndexType>
  inline __host__ __device__
    typename dereference_result< thrust::counting_iterator<Incrementable,Space,Traversal,Difference> >::type
      dereference(const thrust::counting_iterator<Incrementable,Space,Traversal,Difference> &iter, IndexType n);


// constant_iterator prototypes
template<typename Value, typename Incrementable, typename Space>
  inline __host__ __device__
    typename dereference_result< thrust::constant_iterator<Value,Incrementable,Space> >::type
      dereference(const thrust::constant_iterator<Value,Incrementable,Space> &iter);

template<typename Value, typename Incrementable, typename Space, typename IndexType>
  inline __host__ __device__
    typename dereference_result< thrust::constant_iterator<Value,Incrementable,Space> >::type
      dereference(const thrust::constant_iterator<Value,Incrementable,Space> &iter, IndexType n);


// discard_iterator prototypes
template<typename Space>
  inline __host__ __device__
    typename dereference_result< thrust::discard_iterator<Space> >::type
      dereference(const thrust::discard_iterator<Space> &iter);

template<typename Space, typename IndexType>
  inline __host__ __device__
    typename dereference_result< thrust::discard_iterator<Space> >::type
      dereference(const thrust::discard_iterator<Space> &iter, IndexType n);


// zip_iterator prototypes
template<typename IteratorTuple>
  inline __host__ __device__
    typename dereference_result< thrust::zip_iterator<IteratorTuple> >::type
      dereference(const thrust::zip_iterator<IteratorTuple> &iter);

template<typename IteratorTuple, typename IndexType>
  inline __host__ __device__
    typename dereference_result< thrust::zip_iterator<IteratorTuple> >::type
      dereference(const thrust::zip_iterator<IteratorTuple> &iter, IndexType n);


// reverse_iterator prototypes
template<typename BidirectionalIterator>
  inline __host__ __device__
    typename dereference_result< thrust::reverse_iterator<BidirectionalIterator> >::type
      dereference(const thrust::reverse_iterator<BidirectionalIterator> &iter);

template<typename BidirectionalIterator, typename IndexType>
  inline __host__ __device__
    typename dereference_result< thrust::reverse_iterator<BidirectionalIterator> >::type
      dereference(const thrust::reverse_iterator<BidirectionalIterator> &iter, IndexType n);


// permutation_iterator prototypes
template<typename ElementIterator, typename IndexIterator>
  inline __host__ __device__
    typename dereference_result< thrust::permutation_iterator<ElementIterator, IndexIterator> >::type
      dereference(const thrust::permutation_iterator<ElementIterator, IndexIterator> &iter);

template<typename ElementIterator, typename IndexIterator, typename IndexType>
  inline __host__ __device__
    typename dereference_result< thrust::permutation_iterator<ElementIterator, IndexIterator> >::type
      dereference(const thrust::permutation_iterator<ElementIterator, IndexIterator> &iter, IndexType n);


// cpp::pointer prototypes
template<typename T>
  inline __host__ __device__
    typename dereference_result< thrust::system::cpp::pointer<T> >::type
      dereference(thrust::system::cpp::pointer<T> ptr);

template<typename T, typename IndexType>
  inline __host__ __device__
    typename dereference_result< thrust::system::cpp::pointer<T> >::type
      dereference(thrust::system::cpp::pointer<T> ptr, IndexType n);


// omp::pointer prototypes
template<typename T>
  inline __host__ __device__
    typename dereference_result< thrust::system::omp::pointer<T> >::type
      dereference(thrust::system::omp::pointer<T> ptr);

template<typename T, typename IndexType>
  inline __host__ __device__
    typename dereference_result< thrust::system::omp::pointer<T> >::type
      dereference(thrust::system::omp::pointer<T> ptr, IndexType n);


// tbb::pointer prototypes
template<typename T>
  inline __host__ __device__
    typename dereference_result< thrust::system::tbb::pointer<T> >::type
      dereference(thrust::system::tbb::pointer<T> ptr);

template<typename T, typename IndexType>
  inline __host__ __device__
    typename dereference_result< thrust::system::tbb::pointer<T> >::type
      dereference(thrust::system::tbb::pointer<T> ptr, IndexType n);


// cuda::pointer prototypes
template<typename T>
  inline __host__ __device__
    typename dereference_result< thrust::system::cuda::pointer<T> >::type
      dereference(thrust::system::cuda::pointer<T> ptr);

template<typename T, typename IndexType>
  inline __host__ __device__
    typename dereference_result< thrust::system::cuda::pointer<T> >::type
      dereference(thrust::system::cuda::pointer<T> ptr, IndexType n);


} // end backend

} // end detail

} // end thrust

