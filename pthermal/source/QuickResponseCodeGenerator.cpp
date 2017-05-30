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
#include <QuickResponseCodeGenerator.h>
#include <QuickResponseCode.h>
#include <IWcharToChar.h>

using namespace qr;
//-----------------------------------------------------------------------------------------------//
bool WINAPI QuickResponseCodeGenerator::Build ( std::basic_string<wchar_t> const & wszData,
                                                int nVersion,
                                                unsigned uLevel,
                                                unsigned uEncode,
                                                unsigned uPrescaler )
{
   auto pQuickC = LPQuickResponseCode ( new QuickResponseCode ( ) );
   auto pQuickI = LPQuickResponseCodeDetails ( new QuickResponseCodeDetails ( ) );
   //
   pQuickI->nVersion = nVersion;
   pQuickI->qLevel = uLevel;
   pQuickI->qMode = uEncode;
   pQuickI->uPrescaler = uPrescaler;
   //
   auto pImage = pthermal::LPImageBits ( new pthermal::ImageBits );
   pQuickC->SetQuickResponseCodeParameters ( pQuickI );
   pQuickC->Build ( std::make_shared<std::string> ( std::IWcharToChar ( wszData ).data ( ) ),
                    pImage,
                    false );
   this->m_wszFileName = std::make_shared<std::wstring> ( pQuickC->GetFileName ( ) );
   return true;
}
//-----------------------------------------------------------------------------------------------//
bool WINAPI QuickResponseCodeGenerator::Build ( std::string const & szData,
                                                pthermal::LPImageBits & pImage,
                                                int nVersion,
                                                unsigned uLevel,
                                                unsigned uEncode,
                                                unsigned uPrescaler )
{
   auto pQuickC = LPQuickResponseCode ( new QuickResponseCode ( ) );
   auto pQuickI = LPQuickResponseCodeDetails ( new QuickResponseCodeDetails ( ) );
   //
   pQuickI->nVersion = nVersion;
   pQuickI->qLevel = uLevel;
   pQuickI->qMode = uEncode;
   pQuickI->uPrescaler = uPrescaler;
   //
   pQuickC->SetQuickResponseCodeParameters ( pQuickI );
   pQuickC->Build ( std::make_shared<std::string> ( szData ), pImage, true );
   return true;
}
//-----------------------------------------------------------------------------------------------//
std::basic_string<wchar_t> const QuickResponseCodeGenerator::GetFileName ( )
{
   return m_wszFileName->c_str();
}
//-----------------------------------------------------------------------------------------------//