/*
*  Thermal Printer GUI Application
*
*  Build by wilson.souza
*  Default language used c++11
*
*  Copyright (c) 2016 PingoBox Software
*  Copyright (c) 2016 WR DevInfo
*
*  Created jun 24 2016
*  Last updated
*  Version 0.1-beta
*/
#include <PThermalApp.h>
using namespace pthermal;
//-----------------------------------------------------------------------------------------------//
int main(int argc, char *argv[])
{
   return( new PThermalApp ( argc, argv ) )->Execute ( );
}
