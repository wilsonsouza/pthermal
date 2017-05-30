/*
*  Thermal Printer GUI Application
*
*  Build by wilson.souza
*  Default language used c++11
*
*  Copyright (c) 2016 PingoBox Software
*  Copyright (c) 2016 WR DevInfo
*
*  Created jun 26 2016
*  Last updated
*  Version 0.1-beta
*/
#include <PThermalApp.h>
#include <PThermalFrameWindow.h>
#pragma comment(lib, "/projects/pthermal/debug/pthermal.lib")
using namespace pthermal;
//-----------------------------------------------------------------------------------------------//
PThermalApp::PThermalApp ( int & argc, char ** argv ) :std::Application ( argc, argv )
{
   std::wstring szCurDir = std::wstring ( L"", MAX_PATH + 1 );
   //
   GetCurrentDirectory ( szCurDir.length ( ), LPWSTR ( szCurDir.c_str ( ) ) );
   std::StringList strListPaths = std::StringList ( )
      << "/projects/pthermal/pthermal-gui/resources/"
      << "/projects/pthermal/debug/"
      << std::unicodestring::fromStdWString ( szCurDir ) + "/resources/"
      << std::unicodestring::fromStdWString ( szCurDir ) + "/table/"
      << std::unicodestring::fromStdWString ( szCurDir )
      << this->libraryPaths ( );
   this->setLibraryPaths ( strListPaths );
   /**/
   setApplicationName ( caption::Name );
   setApplicationDisplayName ( caption::Name );
   setApplicationVersion ( caption::Version );
   SetFrameWindow ( new PThermalFrameWindow ( NULL, this, caption::Name ) );
   GetFrameWindow ( )->setWindowIcon ( windowIcon ( ) );
}
//-----------------------------------------------------------------------------------------------//
int __fastcall PThermalApp::Execute ( )
{
   //running application
   return std::Application::Execute ( );
}
//-----------------------------------------------------------------------------------------------//
