#ifndef PCH_H
#define PCH_H

#include <iostream>
#include <Windows.h>

#define SG_STYLE( style )   "\033[" style "m"
#define SG_STYLE_RESET      SG_STYLE( "0" )
#define STYLE_ERROR         SG_STYLE( "31" )
#define STYLE_SUCCESS       SG_STYLE( "32" )

#define PRINT_ERROR( message )   { std::cout << STYLE_ERROR   message "\n" SG_STYLE_RESET; }
#define RETURN_ERROR( message )   { std::cout << STYLE_ERROR   message "\n" SG_STYLE_RESET; return 1; }
#define PRINT_SUCCESS( message ) { std::cout << STYLE_SUCCESS message "\n" SG_STYLE_RESET; }

#endif