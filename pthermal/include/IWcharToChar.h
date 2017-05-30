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
#pragma once
#include <string>
#include <wchar.h>

namespace std
{
   class IWcharToChar: public std::basic_string<char>
   {
   protected:
      std::basic_string<char> m_out;
   public:
      explicit IWcharToChar ( std::basic_string<wchar_t> const & wstr );
      virtual ~IWcharToChar ( ) {}
   };

   class IByteToWchar: public std::basic_string<wchar_t>
   {
   protected:
      std::basic_string<wchar_t> m_out;
   public:
      IByteToWchar ( std::basic_string<unsigned char> const & uchar );
      virtual ~IByteToWchar ( ) {}
   };
   //
   class IWcharToByte: public std::basic_string<unsigned char>
   {
   protected:
      std::basic_string<unsigned char> m_out;
   public:
      IWcharToByte ( std::basic_string<wchar_t> const & wchar );
      virtual ~IWcharToByte ( ) {}
   };
   //
   typedef IWcharToChar WideChar;
   typedef IByteToWchar ByteWide;
   typedef IWcharToByte WideByte;
};

