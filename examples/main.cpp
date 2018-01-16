#include <iostream>
#include <sstream>

#include "rb_tree.hpp"

int main(int argc, const char * argv[]) 
{
    
    rb_tree_t<int> tree;
    for( std::string line; std::getline( std::cin, line ); ) {
        std::istringstream stream{ line };
        char op;
        if( stream >> op ) {
            if( op == '+' ) {
                if( int number; stream >> number ) {
                    tree.insert( number );
                    std::cout << tree << std::endl;
                }
            }
            else if ( op == '-' ) {
                if( int number; stream >> number ) {
                    tree.remove( number );
                    std::cout << tree << std::endl;
                }
            }
            else {
                break;
            }
        }
    }
    
    return 0;
}

