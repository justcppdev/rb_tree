#ifndef rb_tree_hpp
#define rb_tree_hpp

#include <iostream>
#include <memory>
#include <queue>
#include <sstream>

template< typename T >
class rb_tree_t
{
private:
	enum class color_t {
		black,
		red
	};
	
    struct node_t
    {
        std::shared_ptr<node_t> left = nullptr;
        std::shared_ptr<node_t> right = nullptr;
        std::shared_ptr<node_t> parent = nullptr;
        std::size_t count = 1;
        T key;
        color_t color;
        node_t( T aKey, color_t aColor ) : key{ aKey }, color{ aColor }
        {
            
        }
    };
    
    /**
     * @brief Returns referense on parent's link.
     *
     * If node is left child function returns reference on parent->left else returns reference on parent->right.
     *
     * @param node Ponter on node. Pointer must be nonnull.
     *
     * @return Referense on parent's link.
     */
    auto & parent_link( std::shared_ptr<node_t> node )
    {
        auto parent = node->parent;
        return parent->left == node ? parent->left : parent->right;
    }
    
    struct PrintParam
    {
        std::size_t x;
        std::size_t d;
    };
    
    enum PrintType {
        Root,
        LeftChild,
        RightChild
    };
    
    struct PrintResult
    {
        std::size_t x;
        std::size_t d;
        std::size_t length;
        PrintType type;
        std::size_t parent_j;
    };
    
    static std::size_t length( std::shared_ptr<node_t> node )
    {
        std::ostringstream stream;
        stream << node->count << ( node->color == color_t::red ? 'r' : 'b' ) << node->key;
        return stream.str().size();
    }
        
    void
    static print( std::shared_ptr<node_t> node,
                  std::vector<std::vector<PrintResult>> & table,
                  std::size_t level,
                  PrintParam & pair,
                  PrintResult & result,
                  PrintType type )
    {
        static std::size_t const MarginLeftChild = 2;
        static std::size_t const MarginRightChild = 2;
        static std::size_t const MarginNeighbor = 2;
        
        if( table.size() == level ) {
            table.push_back( {} );
        }
        
        if( table[ level ].size() != 0 ) {
            auto && neighbor = table[ level ].back();
            if( neighbor.x + pair.d - neighbor.d + MarginNeighbor + neighbor.length > pair.x ) {
                pair.x = neighbor.x + pair.d - neighbor.d + MarginNeighbor + neighbor.length;
            }
        }
        
        if( !node->left && !node->right ) {
            result.x = pair.x;
            result.d = pair.d;
            result.length = length( node );
            result.type = type;
            result.parent_j = level == 0 ? 0 : table[ level - 1 ].size();
            
            table[ level ].push_back( result );
        }
        else if ( !node->right ) {
            if( length( node->left ) + MarginLeftChild > pair.x ) {
                pair.d += length( node->left ) + MarginLeftChild - pair.x;
                pair.x = 0;
            }
            else {
                pair.x -= length( node->left ) + MarginLeftChild;
            }
            
            print( node->left,
                  table,
                  level + 1,
                  pair,
                  result,
                  LeftChild);
            
            result.x += result.length + MarginLeftChild;
            result.length = length( node );
            result.type = type;
            result.parent_j = level == 0 ? 0 : table[ level - 1 ].size();
            
            table[ level ].push_back( result );
        }
        else if ( !node->left ) {
            pair.x += length( node );
            pair.x += MarginRightChild;
            
            print( node->right,
                  table,
                  level + 1,
                  pair,
                  result,
                  RightChild );
            
            result.x -= length( node ) + MarginRightChild;
            result.length = length( node );
            result.type = type;
            result.parent_j = level == 0 ? 0 : table[ level - 1 ].size();
            
            table[ level ].push_back( result );
        }
        else {
            if( length( node->left ) + MarginLeftChild > pair.x ) {
                pair.d += length( node->left ) + MarginLeftChild - pair.x;
                pair.x = 0;
            }
            else {
                pair.x -= length( node->left ) + MarginLeftChild;
            }
            
            print( node->left,
                  table,
                  level + 1,
                  pair,
                  result,
                  LeftChild);
            
            auto left_x = result.x;
            auto left_length = result.length;
            auto left_d = result.d;
            
            pair.x = result.x + result.length + MarginLeftChild + length( node ) + MarginRightChild;
            pair.d = result.d;
            
            print( node->right,
                  table,
                  level + 1,
                  pair,
                  result,
                  RightChild );
            
            result.x = ( left_x + left_length + result.d - left_d + result.x - length( node ) ) / 2;
            result.length = length( node );
            result.type = type;
            result.parent_j = level == 0 ? 0 : table[ level - 1 ].size();
            
            table[ level ].push_back( result );
        }
    }
    
    static void print( std::ostream & stream, std::shared_ptr<node_t> node )
    {
        std::vector<std::vector<PrintResult>> table;
        
        PrintResult result = {0, 0, 0, Root, 0};
        PrintParam param = {0, 0};
        print( node, table, 0, param, result, Root );
        
        std::queue<std::shared_ptr<node_t>> queue;
        queue.push( node );
        
        for( std::size_t i = 0; i < table.size(); ++i ) {
            auto printed_symbols = 0;
            
            if( i != 0 ) {
                for( std::size_t j = 0; j < table[ i ].size(); ++j ) {
                    auto && pair = table[ i ][ j ];
                    if( pair.type == LeftChild ) {
                        auto offset = pair.x + result.d - pair.d + pair.length - printed_symbols + 1;
                        for( std::size_t k = 0; k < offset; ++k ) {
                            stream << ' ';
                        }
                        printed_symbols += offset;
                        
                        auto parent_pair = table[ i - 1 ][ pair.parent_j ];
                        
                        auto count =  parent_pair.x + result.d - parent_pair.d - printed_symbols - 1;
                        for( auto i = 0; i < count; ++i ) {
                            stream << '_';
                        }
                        printed_symbols += count;
                        
                        stream << '/';
                        printed_symbols += 1;
                    }
                    else if( pair.type == RightChild ) {
                        
                        auto parent_pair = table[ i - 1 ][ pair.parent_j ];
                        auto count =  parent_pair.x + result.d - parent_pair.d + parent_pair.length - printed_symbols;
                        for( auto i = 0; i < count; ++i ) {
                            stream << ' ';
                        }
                        printed_symbols += count;
                        
                        stream << '\\';
                        printed_symbols += 1;
                        
                        count =  pair.x + result.d - pair.d - 1 - printed_symbols;
                        for( auto i = 0; i < count; ++i ) {
                            stream << '_';
                        }
                        printed_symbols += count;
                    }
                }
                stream << '\n';
                printed_symbols = 0;
                
                for( std::size_t j = 0; j < table[ i ].size(); ++j ) {
                    auto && pair = table[ i ][ j ];
                    if( pair.type == LeftChild ) {
                        auto offset = pair.x + result.d - pair.d + pair.length - printed_symbols;
                        for( std::size_t k = 0; k < offset; ++k ) {
                            stream << ' ';
                        }
                        printed_symbols += offset;
                        stream << '/';
                        printed_symbols += 1;
                    }
                    else if ( pair.type == RightChild ) {
                        auto offset = pair.x + result.d - pair.d - printed_symbols - 1;
                        for( std::size_t k = 0; k < offset; ++k ) {
                            stream << ' ';
                        }
                        printed_symbols += offset;
                        stream << '\\';
                        printed_symbols += 1;
                    }
                }
                stream << '\n';
            }
            
            printed_symbols = 0;
            
            for( std::size_t j = 0; j < table[ i ].size(); ++j ) {
                auto node = queue.front();
                queue.pop();
                if( node->left ) {
                    queue.push( node->left );
                }
                if( node->right ) {
                    queue.push( node->right );
                }
                
                auto && pair = table[ i ][ j ];
                auto offset = pair.x + result.d - pair.d - printed_symbols;
                for( std::size_t k = 0; k < offset; ++k ) {
                    stream << ' ';
                }
                
                stream << node->count << ( node->color == color_t::red ? 'r' : 'b' ) << node->key;
                printed_symbols += offset + pair.length;
            }
            stream << '\n';
        }
        
        stream << std::endl;
    }
private:
    std::shared_ptr<node_t> root_ = nullptr;
    std::size_t size_ = 0;
    
    static color_t color( std::shared_ptr<node_t> node )
    {
        return node ? node->color : color_t::black;
    }
    
    static void color( std::shared_ptr<node_t> node, color_t color )
    {
        if( node ) {
            node->color = color;
        }
    }
    
    void insertFixUp( std::shared_ptr<node_t> node )
    {
        for( auto dad = node->parent; is_red( dad ) ; dad = node->parent ) {
            auto granddad = dad->parent;
            if( dad == granddad->left ) {
                auto uncle = granddad->right;
                if( is_red( uncle ) ) {
                    black( dad );
                    black( uncle );
                    red( granddad );
                
                    node = granddad;
                }
                else {
                    if( node == dad->right ) {
                        left_rotate( dad );
                        std::swap( node, dad );
                    }
                    black( dad );
                    red( granddad );
                    right_rotate( granddad );
                }
            }
            else {
                auto uncle = granddad->left;
                if( is_red( uncle ) ) {
                    black( dad );
                    black( uncle );
                    red( granddad );
                    
                    node = granddad;
                }
                else {
                    if( node == dad->left ) {
                        right_rotate( dad );
                        std::swap( node, dad );
                    }
                    black( dad );
                    red( granddad );
                    left_rotate( granddad );
                }
            }
        }
        
        black( root_ );
    }
    
    // x->right != nil
    void left_rotate( std::shared_ptr<node_t> x )
    {
        auto y = x->right;
        x->right = y->left;
        if( y->left ) {
            y->left->parent = x;
        }
        
        y->parent = x->parent;
        if( !x->parent ) {
            root_ = y;
        }
        else if( x == x->parent->left ) {
            x->parent->left = y;
        }
        else {
            x->parent->right = y;
        }
        
        left_link( y, x );
    }
    
    static void recount( std::shared_ptr<node_t> node )
    {
        std::size_t count = 1;
        if( node->left ) {
            count += node->left->count;
        }
        if( node->right ) {
            count += node->right->count;
        }
        
        node->count = count;
    }
    
    // y->left != nil
    void right_rotate( std::shared_ptr<node_t> y )
    {
        auto x = y->left;
        y->left = x->right;
        if( x->right ) {
            x->right->parent = y;
        }
        
        x->parent = y->parent;
        if( !y->parent ) {
            root_ = x;
        }
        else if( y == y->parent->left ) {
            y->parent->left = x;
        }
        else {
            y->parent->right = x;
        }
        
        right_link( x, y );
    }
    
    // oldnode != nil
    void transplant( std::shared_ptr<node_t> old_node, std::shared_ptr<node_t> new_node )
    {
        if( !old_node->parent ) {
            root_ = new_node;
        }
        else if ( old_node->parent->left == old_node ) {
            old_node->parent->left = new_node;
        }
        else {
            old_node->parent->right = new_node;
        }
        
        if( new_node ) {
            new_node->parent = old_node->parent;
        }
        
        old_node->parent = nullptr;
        
        for( auto it = new_node; it; it = it->parent ) {
            recount( it );
        }
    }
    
    
    auto minimum( std::shared_ptr<node_t> node )
    {
        while( node->left ) {
            node = node->left;
        }
        
        return node;
    }
    
    static void black( std::shared_ptr<node_t> node )
    {
        if( node ) {
            node->color = color_t::black;
        }
    }
    
    static void red( std::shared_ptr<node_t> node )
    {
        if( node ) {
            node->color = color_t::red;
        }
    }
    
    static bool is_black( std::shared_ptr<node_t> node )
    {
        return color( node ) == color_t::black;
    }
    
    static bool is_red( std::shared_ptr<node_t> node )
    {
        return color( node ) == color_t::red;
    }
    
    // node != nullptr
    void removeFixUp( std::shared_ptr<node_t> node )
    {
        while( node != root_ && is_black( node ) ) {
            auto p = node->parent;
            if( node == p->left ) {
                auto s = p->right;
                if( is_red( s ) ) {
                    black( s );
                    red( p );
                    left_rotate( p );
                    s = p->right;
                }
                if( is_black( s->left ) && is_black( s->right ) ) {
                    red( s );
                    node = p;
                }
                else {
                    if( is_red( s->left ) ) {
                        black( s->left );
                        red( s );
                        right_rotate( s );
                        s = p->right;
                    }
                    
                    color( s, color( p ) );
                    black( p );
                    black( s->right );
                    left_rotate( p );
                    node = root_;
                }
            }
            else {
                auto s = p->left;
                if( is_red( s ) ) {
                    black( s );
                    red( p );
                    right_rotate( p );
                    s = p->left;
                }
                    
                if( is_black( s->right ) && is_black( s->left ) ) {
                    red( s );
                    node = p;
                }
                else {
                    if( is_red( s->right ) ) {
                        black( s->right );
                        red( s );
                        left_rotate( s );
                        s = p->left;
                    }
                    
                    color( s, color( p ) );
                    black( p );
                    black( s->left );
                    right_rotate( p );
                    node = root_;
                }
            }
        }
        
        black( node );
    }
    
    static void left_link( std::shared_ptr<node_t> parent, std::shared_ptr<node_t> node )
    {
        parent->left = node;
        node->parent = parent;
        
        recount( node );
        recount( parent );
    }
    
    static void right_link( std::shared_ptr<node_t> parent, std::shared_ptr<node_t> node )
    {
        parent->right = node;
        node->parent = parent;
        
        for( auto it = parent; it; it = it->parent ) {
            recount( it );
        }
    }
    
    /**
     * Remove node from tree
     *
     * @param node Ponter on node. Pointer must be nonnull.
     */
    void remove( std::shared_ptr<node_t> node )
    {
        auto originalColor = color( node );
        std::shared_ptr<node_t> x = nullptr; // узел в котором может нарушиться свойство красно-черного дерева
        if( !node->left ) {
            x = node->right;
            transplant( node, node->right );
        }
        else if( !node->right ) {
            x = node->left;
            transplant( node, node->left );
        }
        else {
            auto m = minimum( node->right );
            originalColor = color( m );
            x = m->right;
            if( m->parent != node ) {
                transplant( m, m->right );
                right_link( m, node->right );
            }
            
            transplant( node, m );
            left_link( m, node->left );
            color( m, node->color );
        }
        
        if( x && originalColor == color_t::black ) {
            removeFixUp( x );
        }
        
        --size_;
    }
    
    auto find( T key ) -> std::shared_ptr<node_t>
    {
        auto node = root_;
        while( node ) {
            if( node->key == key ) {
                break;
            }
            else if ( key < node->key ) {
                node = node->left;
            }
            else {
                node = node->right;
            }
        }
        
        return node;
    }
    
    static std::size_t count( std::shared_ptr<node_t> node )
    {
        return node ? node->count : 0;
    }
    
public:
    void insert( T key );
    void remove( T key );
    void print( std::ostream & stream ) const;
    auto size() const -> std::size_t;
    auto representation() const -> std::string;
    void representation( std::shared_ptr<node_t> node, std::ostringstream & stream ) const
    {
        if( node->left ) {
            representation( node->left, stream );
        }
        
        stream << ( node->color == color_t::red ? "r" : "b" );
        stream << node->key;
        
        if( node->right ) {
            representation( node->right, stream );
        }
    }
    
    static std::shared_ptr<node_t> select( std::size_t n, std::shared_ptr<node_t> node )
    {
        if( !node ) {
            return nullptr;
        }
        
        auto rank = count( node->left ) + 1;
        if( rank == n ) {
            return node;
        }
        else if( n < rank ) {
            return select( n, node->left );
        }
        else {
            return select( n - rank, node->right );
        }
    }
    
    std::shared_ptr<T> select( std::size_t n ) const
    {
        if( n != 0 && n <= size_ ) {
            auto result = select( n, root_ );
            if( result ) {
                return std::make_shared<T>( result->key );
            }
        }
        
        return nullptr;
    }
};


//template< typename T >
//auto
//rb_tree_t<T>::select( std::size_t n ) const -> std::shared_ptr<T>
//{
//
//}

template< typename T >
auto & operator <<( std::ostream & stream, rb_tree_t< T > const & tree )
{
    tree.print( stream );
    
    return stream;
}

template< typename T >
void
rb_tree_t<T>::print( std::ostream & stream ) const
{
    if( root_ ) {
        print( stream, root_ );
    }
}

template< typename T >
void
rb_tree_t< T >::insert( T key )
{
    auto new_node = std::make_shared<node_t>( key, color_t::red );
    
    std::shared_ptr<node_t> parent;
    auto node = root_;
    while( node ) {
        parent = node;
        node = key < node->key ? node->left : node->right;
    }
    if( !parent ) {
        root_ = new_node;
    }
    else if ( key < parent->key ) {
        left_link( parent, new_node );
    }
    else {
        right_link( parent, new_node );
    }
    
    insertFixUp( new_node );
    ++size_;
}

template< typename T >
void
rb_tree_t< T >::remove( T key )
{
    auto node = find( key );
    if( node ) {
        remove( node );
    }
}

template< typename T >
auto rb_tree_t< T >::size() const -> std::size_t
{
    return size_;
}

template< typename T >
auto rb_tree_t< T >::representation() const -> std::string
{
    std::ostringstream stream;
    if( root_ ) {
        representation( root_, stream );
    }
    
    return stream.str();
}

#endif /* rb_tree_hpp */
