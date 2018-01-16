#ifndef PARTITION_HPP
#define PARTITION_HPP

template <typename _Iterator>
struct range_t
{
    _Iterator begin;
    typename _Iterator::difference_type size;
};

template <typename _Iterator>
struct parition_t
{
    range_t<_Iterator> less;
    range_t<_Iterator> equal;
    range_t<_Iterator> great;
};

#endif
