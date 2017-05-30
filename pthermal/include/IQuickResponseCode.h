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
#include <pthermal.hpp>
#include <memory>
#include <string>

namespace qr
{
   struct QuickResponseCodeData
   {
      unsigned uPrescaler;
      RGBQUAD  rgb;
      BITMAPFILEHEADER bmFile;
      BITMAPINFOHEADER bmInfo;
   };
   //
   struct QuickResponseCodeDetails: public QuickResponseCodeData
   {
      unsigned qMode;
      unsigned qLevel;
      int nVersion;
   };
   //
   typedef std::shared_ptr<QuickResponseCodeDetails> LPQuickResponseCodeDetails;
   //
   class IQuickResponseCode
   {
   public:
      std::wstring m_wszFileNameOut;
      std::shared_ptr<BYTE> m_pbRGBData;
      BYTE * m_pbSourceData;
      BYTE * m_pbDestData;
      LPQuickResponseCodeDetails m_pQuickI;
      QRcode * m_pQRC;
   public:
      IQuickResponseCode() {}
      virtual ~IQuickResponseCode() {}
      //
      virtual void SetFileOutput(std::wstring const & wszFileName) = 0;
      virtual void SetQuickResponseCodeParameters(LPQuickResponseCodeDetails const & pQuickI) = 0;
      virtual bool Build ( std::shared_ptr<std::string> const & szData,
                           pthermal::LPImageBits & pImage,
                           bool bOutFile ) = 0;
   };
}
