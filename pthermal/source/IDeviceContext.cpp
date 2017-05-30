/*
*  Layer to Thermal Printer via WinSpool
*
*  Build by wilson.souza
*  Default language used c++11
*
*  Copyright (c) 2016 PingoBox Software
*  Copyright (c) 2016 WR DevInfo
*
*  Created jun 15 2016
*  Last updated
*  August 11 2016
*  Version 0.1-beta
*/
#include <IDeviceContext.h>
#include <GenerateTicketData.hpp>
#include <escpos.hpp>
#include <escbema.hpp>
#include <IWcharToChar.h>
using namespace pthermal;
//--------------------------------------------------------------------------------------------//
device::IDeviceContext::IDeviceContext ( pthermal::LPIUniversalSerialBusData const & pBus )
{
   m_pBus = pBus;
   m_cfe = NULL;
   m_queue = NULL;
   //
   m_thermal = m_pBus->ThermalPrinterID;
   m_thermal->Fonts = pthermal::LPFonts ( new Fonts );
   //initialize all handles
   m_thermal->hBitmap = NULL;
   m_thermal->hDC = NULL;
   m_thermal->hEvent = NULL;
   m_thermal->hPrinter = NULL;
   m_thermal->DocInfo = LPDocInfo ( new DOCINFO );
   m_DocInfo.cbSize = sizeof ( DOCINFO );
   //
   m_Data = LPITicketData ( new pthermal::ITicketData );
   //m_bmp = NULL;
   m_szDriver = std::wstring ( );
   m_szPrinterMode = std::wstring ( );
}
//--------------------------------------------------------------------------------------------//
device::IDeviceContext::~IDeviceContext ( )
{
   auto p = m_thermal;
   //
   for ( auto && f : p->Fonts.operator*( ) )
   {
      if ( f != NULL )
      {
         DeleteObject ( f );
         f = NULL;
      }
   }
   //
   //if ( m_bmp )
   //{
   //   free ( m_bmp );
   //   m_bmp = NULL;
   //}
   //
   if ( p->hBitmap )
   {
      DeleteObject ( p->hBitmap );
      p->hBitmap = NULL;
   }
   //
   if ( p->hEvent )
   {
      CloseHandle ( p->hEvent );
      p->hEvent = NULL;
   }
   //
   if ( p->hDC )
   {
      DeleteDC ( p->hDC );
      p->hDC = NULL;
   }
   //
   if ( p->hPrinter )
   {
      ClosePrinter ( p->hPrinter );
      p->hPrinter = NULL;
   }
}
//--------------------------------------------------------------------------------------------//
bool device::IDeviceContext::Create ( pthermal::LPXMLCFeData const & pcfe )
{
   LPCWSTR lpDriver = m_thermal->Driver.c_str ( );
   std::wstring szOldPrinter = m_thermal->Printer;
   //get default printer
   if ( m_thermal->Driver.empty ( ) )
   {
      DWORD dwNeed = MAX_PATH + 1;
      //
      m_thermal->Printer.reserve ( MAX_PATH + 1 );
      //
      if ( !GetDefaultPrinterW ( const_cast< LPWSTR >( m_thermal->Printer.c_str ( ) ), &dwNeed ) )
      {
         m_thermal->Printer.clear ( );
         m_thermal->Printer = szOldPrinter.c_str ( );
      }
   }
   //
   LPWSTR lpPrinter = const_cast< LPWSTR >( m_thermal->Printer.c_str ( ) );
   //
   if ( !OpenPrinter ( lpPrinter, &m_thermal->hPrinter, NULL ) )
   {
      throw std::exception ( "Unable to open printer port!" );
   }
   //
   long lBytes = DocumentProperties ( NULL,
                                      m_thermal->hPrinter,
                                      NULL,
                                      NULL,
                                      NULL,
                                      0 );
   long lDMOffset = ( lBytes / sizeof ( DEVMODE ) );
   //
   m_thermal->DeviceMode = pthermal::LPDeviceMode ( new DEVMODE[lDMOffset + 1] );
   //define document name
   if ( pcfe )
   {
      m_Data->szDocumentName = pcfe->m_infcfe.m_ide.nserieSAT + L"-" + pcfe->m_infcfe.m_ide.numeroCaixa;
      m_cfe = pcfe;
   }
   //
   DocumentProperties ( NULL,
                        m_thermal->hPrinter,
                        NULL,
                        m_thermal->DeviceMode.get ( ),
                        NULL,
                        DM_OUT_BUFFER );
   //
   //if ( !m_szDriver.empty ( ) )
   //{
   //   lpDriver = m_szDriver.c_str();
   //}
   //create device context
   if ( ( m_thermal->hDC = CreateDC ( lpDriver, lpPrinter, NULL, NULL ) ) == NULL )
   {
      Close ( );
      throw std::exception ( "Unable to create device context!" );
   }
   //
   if ( m_cfe )
   {
      if ( !m_cfe->szQuickResponseCodeFile.empty ( ) )
      {
         extern HMODULE g_hModule;
         m_hbmp = ( HBITMAP ) LoadImage ( g_hModule,
                                          m_cfe->szQuickResponseCodeFile.c_str ( ),
                                          IMAGE_BITMAP,
                                          0,
                                          0,
                                          LR_CREATEDIBSECTION | LR_LOADFROMFILE );
         if ( m_hbmp == NULL )
         {
            Close ( );
            throw std::exception ( "Unable to load Qrcode bitmap!" );
         }
      }
   }
   //
   //create font 8x24 and 8x30
   HFONT hFontA = CreateFont ( 24,
                               8,
                               0,
                               0,
                               FW_NORMAL,
                               FALSE,
                               FALSE,
                               0,
                               ANSI_CHARSET,
                               OUT_DEFAULT_PRECIS,
                               CLIP_DEFAULT_PRECIS,
                               DEFAULT_QUALITY,
                               DEFAULT_PITCH | FF_SWISS,
                               L"Courier" );
   //
   if ( hFontA == NULL )
   {
      Close ( );
      throw std::exception ( "Unable create font Courier24x8!" );
   }
   //
   HFONT hFontB = CreateFont ( 24,
                               8,
                               0,
                               0,
                               FW_BOLD,
                               FALSE,
                               FALSE,
                               0,
                               ANSI_CHARSET,
                               OUT_DEFAULT_PRECIS,
                               CLIP_DEFAULT_PRECIS,
                               DEFAULT_QUALITY,
                               DEFAULT_PITCH | FF_SWISS,
                               L"Courier" );
   if ( hFontB == NULL )
   {
      Close ( );
      throw std::exception ( "Unable create font Courier24x8!" );
   }
   //
   HFONT hFontC = CreateFont ( 30,
                               8,
                               0,
                               0,
                               FW_BOLD,
                               FALSE,
                               FALSE,
                               0,
                               ANSI_CHARSET,
                               OUT_DEFAULT_PRECIS,
                               CLIP_DEFAULT_PRECIS,
                               DEFAULT_QUALITY,
                               DEFAULT_PITCH | FF_SWISS,
                               L"Courier" );
   //
   if ( hFontC == NULL )
   {
      Close ( );
      throw std::exception ( "Unable create font Courier30x8!" );
   }
   //
   m_thermal->Fonts->push_back ( hFontA );
   m_thermal->Fonts->push_back ( hFontB );
   m_thermal->Fonts->push_back ( hFontC );
   return true;
}
//--------------------------------------------------------------------------------------------//
bool device::IDeviceContext::PreparePage ( )
{
   //default printer mode is RAW
   m_DocInfo.lpszDatatype = pthermal::printer::mode::output::RAW.c_str();
   m_DocInfo.lpszOutput = NULL;
   //
   if ( !m_szPrinterMode.empty() )
   {
      m_DocInfo.lpszDatatype = m_szPrinterMode.c_str();
   }
   //
   m_DocInfo.lpszDocName = m_Data->szDocumentName.c_str ( );
   //print mode set to xps
   if ( !m_szPrinterMode.empty ( ) )
   {
      std::wstring szFirstName = L"xps_doc" + m_Data->szDocumentName + L".oxps";
      //
      m_Data->szDocumentName = szFirstName.c_str ( );
      this->GetNextXPSDocumentName ( m_Data->szDocumentName );
      m_DocInfo.lpszOutput = m_Data->szDocumentName.c_str ( );
   }
   //reprocess ticket
   if ( m_cfe )
   {
      auto pTicket = pthermal::LPGenerateTicketData ( new pthermal::GenerateTicketData ( m_cfe, m_pBus ) );
      //set paper width
      SelectObject ( m_thermal->hDC, m_thermal->Fonts->at ( 0 ) );
      Update ( );
      pTicket->SetPaperWidth ( m_thermal->nColumns );
      m_queue = pTicket->Build ( );
   }
   return true;
}
//--------------------------------------------------------------------------------------------//
bool device::IDeviceContext::Print ( )
{
   auto p = m_thermal;
   //
   StartDoc ( p->hDC, &m_DocInfo );
   StartPage ( p->hDC );
   SetMapMode ( p->hDC, MM_TEXT );
   Update ( );
   //
   for ( auto && data: m_queue.operator*() )
   {
      if ( data->ulFlags & pthermal::DataType::Text )
      {
         for ( auto && next : data->Data.operator*( ) )
         {
            if ( next.second == escpos::Character::SelectCharacterFontA )
            {
               SelectObject ( p->hDC, p->Fonts->at ( 0 ) );
               Update ( );
               continue;
            }
            //
            if ( next.second == escpos::Character::EnableBold )
            {
               SelectObject ( p->hDC, p->Fonts->at ( 1 ) );
               Update ( );
               continue;
            }
            if ( next.second == escpos::Character::EnableEmphasized )
            {
               auto H = p->nFontHeight;
               SelectObject ( p->hDC, p->Fonts->at ( 2 ) );
               Update ( );
               p->nFontHeight += p->nFontHeight - H;
               continue;
            }
            //
            if ( next.second == escpos::Character::DisableEmphasized ||
                 next.second == escpos::Character::DisableBold)
            {
               SelectObject ( p->hDC, p->Fonts->at ( 0 ) );
               Update ( );
               continue;
            }
            //
            if ( next.second == escpos::Print::NextLine )
            {
               p->y += p->nOffset;
            }
            //Center [ ] = { ESC, 0x61, 0x01 };
            //Left [ ] = { ESC, 0x61, 0x00 };
            //Right [ ] = { ESC, 0x61, 0x02 };
            if ( next.second == escpos::Position::Center )
            {
               SIZE c;
               GetTextExtentPoint32 ( p->hDC, next.first.c_str ( ), next.first.length ( ), &c );
               c.cx += ( 0x7 * p->nFontWidth );
               p->x = ( ( p->nPageWidth - c.cx ) / 2 );
               //
               if ( next.first.size ( ) == 0 )
               {
                  continue;
               }
            }
            //
            if ( next.second == escpos::Position::Left )
            {
               p->x = 0;
            }
            //
            if ( next.second == escpos::Position::Right )
            {
               SIZE c;
               GetTextExtentPoint32 ( p->hDC, next.first.c_str ( ), next.first.length ( ), &c );
               c.cx += ( 0xA * p->nFontWidth );
               p->x = ( p->nPageWidth - c.cx );
               //
               if ( next.first.size ( ) == 0 )
               {
                  continue;
               }
            }
            //
            //
            if ( next.second == escpos::Character::EnableDoubleStrike ||
                 next.second == escpos::Character::DisableDoubleStrike )
            {
               continue;
            }
            //
            if ( next.first.find ( L"---------" ) != -1 )
            {
               next.first.resize ( p->nColumns * p->nFontWidth );
               std::fill ( next.first.begin ( ), next.first.end ( ), wchar_t ( '-' ) );
            }
            //
            if ( next.first.size ( ) > 0 )
            {
               TextOut ( p->hDC, p->x, p->y, next.first.c_str ( ), next.first.length ( ) );
            }
         }
      }
      else if ( data->ulFlags & pthermal::DataType::QuickResponseCode )
      {
         for ( auto && s : data->Data.operator*( ) )
         {
            if ( s.second == escpos::Print::NextLine )
            {
               p->y += p->nOffset;
            }
            //
            if ( s.first.size ( ) > 0 )
            {
               PrintQuickResponseCodeBitmap ( s );
            }
         }
      }
   }
   //
   EndPage ( p->hDC );
   EndDoc ( p->hDC );
   return false;
}
//--------------------------------------------------------------------------------------------//
void device::IDeviceContext::Close ( )
{
   //clear all handles
   this->~IDeviceContext ( );
}
//--------------------------------------------------------------------------------------------//
void device::IDeviceContext::SetDriverName ( std::wstring const & szDriver )
{
   m_szDriver = szDriver;
}
//--------------------------------------------------------------------------------------------//
void device::IDeviceContext::SetPrinterMode ( std::wstring const & szMode )
{
   m_szPrinterMode = szMode;
}
//--------------------------------------------------------------------------------------------//
void device::IDeviceContext::BuildBitmap ( )
{
   HDC hDC = m_thermal->hDC;
   unsigned uColors = 0;
   //
   GetObject ( m_hbmp, sizeof ( BITMAP ), &m_bmp );
   uColors = ( 1 << m_bmp.bmBitsPixel );
   //
   if ( uColors > 0x100 )
   {
      uColors = 0;
   }
   //
   size_t uOffset = sizeof ( BITMAPINFO ) + ( uColors * sizeof ( RGBQUAD ) );
   //m_bmp = LPBITMAPINFO ( malloc ( uOffset ) );
   //
   m_bmpinfo.bmiHeader.biSize = sizeof ( BITMAPINFOHEADER );
   m_bmpinfo.bmiHeader.biWidth = m_bmp.bmWidth;
   m_bmpinfo.bmiHeader.biHeight = m_bmp.bmHeight;
   m_bmpinfo.bmiHeader.biPlanes = 1;
   m_bmpinfo.bmiHeader.biBitCount = m_bmp.bmBitsPixel * m_bmp.bmPlanes;
   m_bmpinfo.bmiHeader.biCompression = BI_RGB;
   m_bmpinfo.bmiHeader.biSizeImage = m_bmp.bmWidthBytes * m_bmp.bmHeight;
   m_bmpinfo.bmiHeader.biXPelsPerMeter = 0;
   m_bmpinfo.bmiHeader.biYPelsPerMeter = 0;
   m_bmpinfo.bmiHeader.biClrUsed = 0;
   m_bmpinfo.bmiHeader.biClrImportant = 0;
   //
   if ( uColors <= 256 )
   {
      HDC     hMemDC = CreateCompatibleDC ( NULL );      // Creating an auxiliary device context
      HBITMAP hOldBitmap = ( HBITMAP ) SelectObject ( hMemDC, &m_bmp );  // Select 'hbit' in 'it'
      GetDIBColorTable ( hMemDC, 0, uColors, m_thermal->rgb );   // Obtaining the color table information

      // Now we pass this color information to "info"
      for ( auto i = 0U; i < uColors; ++i )
      {
         m_bmpinfo.bmiColors[i].rgbRed = m_thermal->rgb[i].rgbRed;
         m_bmpinfo.bmiColors[i].rgbGreen = m_thermal->rgb[i].rgbGreen;
         m_bmpinfo.bmiColors[i].rgbBlue = m_thermal->rgb[i].rgbBlue;
      }
      //
      SelectObject ( hMemDC, hOldBitmap );
      DeleteDC ( hMemDC );
   }
   //
}
//--------------------------------------------------------------------------------------------//
void device::IDeviceContext::PrintQuickResponseCodeBitmap (pthermal::FieldData const & p )
{
   long lW = m_bmp.bmWidth;
   long lH = m_bmp.bmHeight;
   //
   BuildBitmap ( );
   /*
   * resize bitmap if major what 296x296
   */
   if ( m_bmp.bmWidth > 0x128 || m_bmp.bmHeight > 0x128 )
   {
      lW = 0x128;
      lH = 0x128;
   }
   /*
   * Draw the bitmap
   */
   if ( p.second == escpos::Position::Center )
      m_thermal->x = ( ( m_thermal->nPageWidth - lW ) / 2 );
   //
   if ( p.second == escpos::Position::Left )
      m_thermal->x = 0;
   //
   if ( p.second == escpos::Position::Right )
      m_thermal->x = m_thermal->nPageWidth - lW;
   //
   StretchDIBits ( m_thermal->hDC,
                   m_thermal->x,
                   m_thermal->y,
                   lW,
                   lH,
                   0,
                   0,
                   m_bmp.bmWidth,
                   m_bmp.bmHeight,
                   m_bmp.bmBits,
                   &m_bmpinfo,
                   DIB_RGB_COLORS,
                   SRCCOPY );
}
//--------------------------------------------------------------------------------------------//
void device::IDeviceContext::Update ( )
{
   auto p = m_thermal;
   //
   GetTextMetrics ( p->hDC, p->lpMetric );
   p->nPageWidth = GetDeviceCaps ( p->hDC, PHYSICALWIDTH );
   p->nPageHeight = GetDeviceCaps ( p->hDC, PHYSICALHEIGHT );
   //
   p->nFontHeight = p->lpMetric->tmHeight;
   p->nFontWidth = int ( p->lpMetric->tmAveCharWidth );
   p->nColumns = int ( std::round ( p->nPageWidth / p->nFontWidth ) );
   p->nLines = int ( std::round ( p->nPageHeight / p->nFontHeight ) );
   p->x = 0;
   p->nOffset = p->nFontHeight;
   p->nColumns -= ( p->nColumns % 2 );
}
//--------------------------------------------------------------------------------------------//
void device::IDeviceContext::GetNextXPSDocumentName (std::wstring & szName)
{
   auto pos = 0;
   //
   while ( _waccess ( szName.c_str ( ), 0 ) != -1 )
   {
      auto s = std::wstring ( L"", MAX_PATH );
      //
      wsprintf ( const_cast< wchar_t * >( s.c_str ( ) ), L"xps_doc_prn-%03d.oxps\0", pos );
      szName = s.c_str ( );
      pos++;
   }
}
//--------------------------------------------------------------------------------------------//