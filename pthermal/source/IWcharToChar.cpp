/*
*  Layer to Print in Thermal Printer
*
*  Build by wilson.souza
*  Default language used c++11
*
*  Copyright (c) 2016 PingoBox Software
*  Copyright (c) 2016 WR DevInfo
*
*  Created jun 12 2016
*  Last updated
*  Version 0.1-beta
*/
#include <IWcharToChar.h>
using namespace std;
//-----------------------------------------------------------------------------------------------//
IWcharToChar::IWcharToChar (std::basic_string<wchar_t> const & wstr ): 
std::basic_string<char> ( )
{
   m_out = std::string ( );
   //
   for ( auto s = wstr.begin ( ); s != wstr.end ( ); s++ )
   {
      int i;
      char c;
      //
      wctomb_s ( &i, &c, 1, *s );
      m_out += c;
   }
   //
   assign ( m_out );
}
//-----------------------------------------------------------------------------------------------//
IByteToWchar::IByteToWchar (std::basic_string<unsigned char> const & uchar ):
std::basic_string<wchar_t> ( )
{
   m_out = std::wstring ( );
   //
   for ( auto u = uchar.begin ( ); u != uchar.end ( ); u++ )
      m_out += wchar_t ( *u );
   //
   assign ( m_out );
}
//-----------------------------------------------------------------------------------------------//
IWcharToByte::IWcharToByte (std::basic_string<wchar_t> const & wchar ):
std::basic_string<unsigned char> ( )
{
   m_out = std::basic_string<unsigned char> ( );
   //
   for ( auto s = wchar.begin ( ); s != wchar.end ( ); s++ )
   {
      int i;
      char c;
      //
      wctomb_s ( &i, &c, 1, *s );
      m_out += unsigned char ( c );
   }
   //
   assign ( m_out );
}
//-----------------------------------------------------------------------------------------------//
