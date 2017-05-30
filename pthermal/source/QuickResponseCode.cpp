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
#include <qrencode.h>
#include <QuickResponseCode.h>
#include <ostream>
#include <IWcharToChar.h>
#include <algorithm>
//
using namespace qr;
//-----------------------------------------------------------------------------------------------//
QuickResponseCode::QuickResponseCode ( ) :IQuickResponseCode ( )
{
   this->m_pbDestData = NULL;
   this->m_pbRGBData = NULL;
   this->m_pbSourceData = NULL;
   this->m_fHandle = INVALID_HANDLE_VALUE;
   this->m_pQRC = NULL;
   this->m_pQuickI = LPQuickResponseCodeDetails ( new QuickResponseCodeDetails );
   //
   this->m_pQuickI->nVersion = 1;
   this->m_pQuickI->qLevel = unsigned ( QR_ECLEVEL_H );
   this->m_pQuickI->qMode = unsigned ( QR_MODE_8 );
   this->m_pQuickI->rgb.rgbBlue = 0;
   this->m_pQuickI->rgb.rgbGreen = 0;
   this->m_pQuickI->rgb.rgbRed = 0; //0xff;
   this->m_pQuickI->uPrescaler = 8;
}
//-----------------------------------------------------------------------------------------------//
QuickResponseCode::~QuickResponseCode ( )
{
   if ( m_pQRC )
      QRcode_free ( m_pQRC );
}
//-----------------------------------------------------------------------------------------------//
bool QuickResponseCode::Build ( std::shared_ptr<std::string> const & szData,
                                pthermal::LPImageBits & pImage,
                                bool bOutFile )
{
   //check pointer
   if ( m_pQRC )
   {
      QRcode_free ( m_pQRC );
      m_pQRC = NULL;
   }
   //
   m_pQRC = ( QRcode * ) QRcode_encodeString ( szData->c_str ( ),
                                               m_pQuickI->nVersion,
                                               QRecLevel ( m_pQuickI->qLevel ),
                                               QRencodeMode ( m_pQuickI->qMode ),
                                               1 );
   //
   try
   {
      if ( m_pQRC )
      {
         unsigned int uWidth = m_pQRC->width;
         unsigned int uWidthAdjusted = uWidth * m_pQuickI->uPrescaler * 3;
         unsigned uPrescaler = m_pQuickI->uPrescaler;
         //
         if ( uWidthAdjusted % 4 )
         {
            uWidthAdjusted = ( uWidthAdjusted / 4 + 1 ) * 4;
         }
         //
         unsigned int uDataBytes = uWidthAdjusted * uWidth * m_pQuickI->uPrescaler;
         //
         this->m_pbRGBData = std::shared_ptr<BYTE> ( new BYTE [ uDataBytes ] );
         memset ( m_pbRGBData.operator->( ), 0xff, uDataBytes );
         //fill bitmap file header
         m_pQuickI->bmFile.bfType = 0x4d42;
         m_pQuickI->bmFile.bfSize = sizeof ( BITMAPFILEHEADER ) + sizeof ( BITMAPINFOHEADER ) + uDataBytes;
         m_pQuickI->bmFile.bfReserved1 = 0;
         m_pQuickI->bmFile.bfReserved2 = 0;
         m_pQuickI->bmFile.bfOffBits = sizeof ( BITMAPFILEHEADER ) + sizeof ( BITMAPINFOHEADER );
         //
         m_pQuickI->bmInfo.biSize = sizeof ( BITMAPINFOHEADER );
         m_pQuickI->bmInfo.biWidth = uWidth * uPrescaler;
         m_pQuickI->bmInfo.biHeight = int ( uWidth * uPrescaler );
         m_pQuickI->bmInfo.biPlanes = 1;
         m_pQuickI->bmInfo.biBitCount = 0x18;
         m_pQuickI->bmInfo.biCompression = BI_RGB;
         m_pQuickI->bmInfo.biSizeImage = 0;
         m_pQuickI->bmInfo.biXPelsPerMeter = 0;
         m_pQuickI->bmInfo.biYPelsPerMeter = 0;
         m_pQuickI->bmInfo.biClrUsed = 0;
         m_pQuickI->bmInfo.biClrImportant = 0;
         // Convert QrCode bits to bmp pixels
         m_pbSourceData = m_pQRC->data;
         //
         for ( auto y = 0U; y < uWidth; y++ )
         {
            m_pbDestData = m_pbRGBData.operator->( ) + uWidthAdjusted * y * m_pQuickI->uPrescaler;

            for ( auto x = 0U; x < uWidth; x++ )
            {
               if ( *m_pbSourceData & 1 )
               {
                  for ( auto l = 0U; l < m_pQuickI->uPrescaler; l++ )
                  {
                     for ( auto n = 0U; n < m_pQuickI->uPrescaler; n++ )
                     {
                        *( m_pbDestData + n * 3 + uWidthAdjusted * l ) = m_pQuickI->rgb.rgbBlue;
                        *( m_pbDestData + 1 + n * 3 + uWidthAdjusted * l ) = m_pQuickI->rgb.rgbGreen;
                        *( m_pbDestData + 2 + n * 3 + uWidthAdjusted * l ) = m_pQuickI->rgb.rgbRed;
                     }
                  }
               }
               m_pbDestData += 3 * m_pQuickI->uPrescaler;
               m_pbSourceData++;
            }
         }
         //output file in bitmap format
         if ( !bOutFile )
         {
            this->CreateQuickResponseCodeHeader ( );
            this->WriteQuickResponseCodeData ( m_pbRGBData, uDataBytes );
         }
         else
         {
            pImage->uQRCodeWidth = long ( m_pQRC->width );
            pImage->bmpFile = m_pQuickI->bmFile;
            pImage->bmpInfo = m_pQuickI->bmInfo;
            pImage->cbData = m_pbRGBData; //pixels
            pImage->cbQuickResponseCode = std::ustring ( m_pQRC->data, m_pQRC->width );
            pImage->cbSource = std::ustring ( m_pbSourceData );
            pImage->cbDest = std::ustring ( m_pbDestData );
            pImage->uVersion = m_pQRC->version;
            return true;
         }
      }
      else
      {
         throw std::exception ( "Unable to create qrcode image!" );
      }
   }
   catch ( std::exception const & e )
   {
      throw e;
   }
   return true;
}
//-----------------------------------------------------------------------------------------------//
void QuickResponseCode::CreateQuickResponseCodeHeader ( )
{
   std::string szFile = std::IWcharToChar ( m_wszFileNameOut ).c_str ( );
   std::string aszMsg [ ] =
   {
      std::string ( "Cann´t create temporary qrcode file " + szFile + "!" ),
      std::string ( "Cann´t write qrcode image header of file " + szFile + "!" ),
      std::string ( "Cann´t write qrcode image final information of file " + szFile + "!" )
   };
   //
   if ( m_wszFileNameOut.empty ( ) )
   {
      auto wszTempDir = std::wstring ( L"", MAX_PATH );
      auto uUnique = 0x00ff00ff;
      TCHAR wacTempName[MAX_PATH ];
      //
      GetCurrentDirectory ( wszTempDir.length ( ), LPWSTR ( wszTempDir.c_str ( ) ) );
      GetTempFileName ( wszTempDir.c_str ( ), L"QRC", uUnique, wacTempName );
      m_wszFileNameOut = std::wstring(wacTempName ).replace(std::wstring(wacTempName ).length() - 3, 3, L"bmp" );
   }
   //
   m_fHandle = CreateFile ( m_wszFileNameOut.c_str(),
                            GENERIC_READ | GENERIC_WRITE,
                            0,
                            NULL,
                            CREATE_ALWAYS,
                            FILE_ATTRIBUTE_NORMAL,
                            NULL );
   //
   if ( m_fHandle == INVALID_HANDLE_VALUE )
   {
      throw std::exception ( aszMsg [ 0 ].c_str ( ) );
   }
   //
   DWORD dwBytes;
   //
   if ( WriteFile ( m_fHandle, &m_pQuickI->bmFile, 
                    sizeof ( m_pQuickI->bmFile ), 
                    &dwBytes, 
                    NULL ) == FALSE )
   {
      throw std::exception ( aszMsg [ 1 ].c_str ( ) );
   }
   //
   if ( WriteFile ( m_fHandle, 
                    &m_pQuickI->bmInfo, 
                    sizeof ( m_pQuickI->bmInfo ), 
                    &dwBytes, 
                    NULL ) == FALSE )
   {
      throw std::exception ( aszMsg [ 2 ].c_str ( ) );
   }
}
//-----------------------------------------------------------------------------------------------//
void QuickResponseCode::WriteQuickResponseCodeData ( std::shared_ptr<BYTE> const & pData, UINT uBytes )
{
   DWORD dwOffset = 0;

   if ( WriteFile ( m_fHandle, m_pbRGBData.get( ), uBytes, &dwOffset, NULL ) == FALSE )
   {
      std::string s = std::string
      {
         "Data write failure on file " +
         std::WideChar ( m_wszFileNameOut ) + "!"
      };
      //
      std::exception ( s.c_str ( ) );
   }
   //
   CloseHandle ( m_fHandle );
   m_fHandle = INVALID_HANDLE_VALUE;
}
//-----------------------------------------------------------------------------------------------//
void QuickResponseCode::SetFileOutput ( std::wstring const & wszFileName )
{
   this->m_wszFileNameOut = wszFileName.c_str ( );
}
//-----------------------------------------------------------------------------------------------//
void QuickResponseCode::SetQuickResponseCodeParameters ( LPQuickResponseCodeDetails const & pQuickI )
{
   if ( pQuickI )
   {
      this->m_pQuickI->nVersion = pQuickI->nVersion;
      this->m_pQuickI->qLevel = pQuickI->qLevel;
      this->m_pQuickI->qMode = pQuickI->qMode;
   }
   else
      throw std::exception ( "Invalid parameters!" );
}
//-----------------------------------------------------------------------------------------------//
std::basic_string<wchar_t> const & QuickResponseCode::GetFileName ( )
{
   return this->m_wszFileNameOut;
}
//-----------------------------------------------------------------------------------------------//
