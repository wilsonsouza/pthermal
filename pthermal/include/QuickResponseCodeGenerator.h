/*
*  Layer to generator of Quick Response Code (QRCode)
*
*  Build by wilson.souza
*  Default language used c++11
*
*  Copyright (c) 2016 PingoBox Software
*  Copyright (c) 2016 WR DevInfo
*
*  Created jun 3 2016
*  Last updated
*  Version 0.1-beta
*/
#pragma once
#pragma warning(disable:4275)
#pragma warning(disable:4251)

#include <pthermal.hpp>

namespace qr
{
   class PTHERMAL_API QuickResponseCodeGenerator
   {
   public:
      pthermal::LPWideString m_wszFileName;
   public:
      QuickResponseCodeGenerator ( ) { }
      std::basic_string<wchar_t> const GetFileName ( );
      virtual bool WINAPI Build ( std::basic_string<wchar_t> const & wszData,
                                  int nVersion = 1,
                                  unsigned uLevel = 3,
                                  unsigned uEncode = 2,
                                  unsigned uPrescaler = 8 );
      virtual bool WINAPI Build ( std::string const & szData,
                                  pthermal::LPImageBits & pImage,
                                  int nVersion = 1,
                                  unsigned uLevel = 3,
                                  unsigned uEncode = 2,
                                  unsigned uPrescaler = 8 );
   };
   //
   typedef std::shared_ptr<QuickResponseCodeGenerator> LPQuickResponseCodeGenerator;
};