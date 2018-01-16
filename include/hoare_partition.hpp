#ifndef HOARE_PARTITION_HPP
#define HOARE_PARTITION_HPP

#include <algorithm>
#include <utility>

#include "partition.hpp"

template <typename _BidirectionalIterator, typename T>
auto hoare_partition( _BidirectionalIterator first, _BidirectionalIterator last, T pivot ) 
    -> parition_t<_BidirectionalIterator>
{
    if( first == last ) {
        return {{first, 0}, {first, 0}, {first, 0}};
    }
    
    auto i = first;
    auto k = first;
    auto j = last;

    typename _BidirectionalIterator::difference_type less_count = 0;
    typename _BidirectionalIterator::difference_type equal_count = 0;
    typename _BidirectionalIterator::difference_type great_count = 0;
    
    while( i != j ) {
        while( i != j && !(pivot < *i) ) {
            if( *i < pivot ) {
                std::iter_swap( i , k++ );
                ++less_count;
            }
            else {
                ++equal_count;
            }
            ++i;
        }
        
        while( i != j ) {
            auto tmp = j;
            
            if( pivot < *--tmp ) {
                j = tmp;
                ++great_count;
            }
            else {
                break;
            }
        }
        
        if( i != j ) {
            std::iter_swap( i, --j );
            if( *i < pivot ) {
                std::iter_swap( i, k );
                ++k;
                ++less_count;
            }
            else {
                ++equal_count;
            }
            
            ++great_count;
            ++i;
        }
    }
    
    return {{first, less_count}, {k, equal_count}, {i, great_count}};
}

#endif
