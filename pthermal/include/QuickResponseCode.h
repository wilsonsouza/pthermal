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
#include <IQuickResponseCode.h>

namespace qr
{
   class QuickResponseCode : public IQuickResponseCode
   {
   protected:
      HANDLE m_fHandle;
   public:
      QuickResponseCode ( );
      virtual ~QuickResponseCode ( );
      //
      virtual void SetFileOutput ( std::wstring const & wszFileName );
      virtual void SetQuickResponseCodeParameters ( LPQuickResponseCodeDetails const & pQuickI );
      virtual bool Build ( std::shared_ptr<std::string> const & szData,
                           pthermal::LPImageBits & pImage,
                           bool bOutFile = false );
      virtual std::basic_string<wchar_t> const & GetFileName ( );
   protected:
      virtual void CreateQuickResponseCodeHeader ( );
      virtual void WriteQuickResponseCodeData ( std::shared_ptr<BYTE> const & pData, UINT uBytes );
   };
   //
   typedef std::shared_ptr<QuickResponseCode> LPQuickResponseCode;
}