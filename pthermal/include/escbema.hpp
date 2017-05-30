/*
*  Layer to Bematech Printer Escbema
*
*  Build by wilson.souza
*  Default language used c++11
*
*  Copyright (c) 2016 PingoBox Software
*  Copyright (c) 2016 WR DevInfo
*
*  Created jun 9 2016
*  Last updated
*  Version 0.1-beta
*/
#pragma once
#include <pthermal.hpp>

namespace escbema
{
   static const BYTE ESC = 0x1b;
   static const BYTE GS = 0x1d;
   //last byte define mode escpos para bematech
   //bits 0              1 2  3               4       5       6  7
   //   0 Font C 14x24   U U  0 EmphasizedOff 0 DHOff 0 DWOff U  0 UnderlineOff
   //   1 Font D 10.5x24      1 EmphasizeOn   1 DHOn  1 DWOn  U  1 UnderlineOn
   static const BYTE SelectPrintMode [ ] = { ESC, 0x21, 0x0 };
   static const BYTE PrinterEnable [ ] = { ESC, 0x3d, ( 1 << 0 ) };
   static const BYTE PrinterDisable [ ] = { ESC, 0x3d, 0 };

   static const BYTE Left [ ] = { ESC, 'a', '0' };
   static const BYTE Center [ ] = { ESC, 'a', '1' };
   static const BYTE Right [ ] = { ESC, 'a', '2' };

   static const BYTE InitializePrinterToItsDefaultSettings [ ] = { ESC, '@' };
   static const BYTE CarriageReturn [ ] = { 0xd };
   //image format
   static const BYTE RasterNormalImage [ ] = { GS,0x76,0x30,0x30 }; // Set raster image normal size
   static const BYTE RasterDoubleWidthImage [ ] = { GS,0x76,0x30,0x31 }; // Set raster image double width
   static const BYTE RasterDoubleHeightImage [ ] = { GS,0x76,0x30,0x32 }; // Set raster image double height
   static const BYTE RasterQuadrupleImage [ ] = { GS,0x76,0x30,0x33 }; // Set raster image quadruple
                                                        //
   static const BYTE SetPrinterOperatingModeEscBema [ ] = { GS,0xf9,0x30 };
   static const BYTE SetPrinterOperatingModeEscPos [ ] = { GS,0xf9,0x31 };
   static const BYTE GetPrinterCurrentCommandSet [ ] = { GS,0xf9,0x43,0x00 };
   static const BYTE SelectPrinterOperatingModeOfOperationTemporarilyEscBema [ ] = { GS,0xf9,0x20,0x30 };
   static const BYTE SelectPrinterOperationModeOfOperationTemporarilyEscPos [ ] = { GS,0xf9,0x20,0x31 };
   static const BYTE PerformFullPaperCut [ ] = { ESC,0x69 };
   static const BYTE EnablePanelKeys [ ] = { ESC,0x79,0x31 };
   static const BYTE DisablePanelKeys [ ] = { ESC,0x79,0x30 };
   static const BYTE EnableDumpMode [ ] = { ESC,0x78 };
   static const BYTE EnableAutomaticLineFeed [ ] = { ESC,0x7a,0x31 };
   static const BYTE DisableAutomaticLineFeed [ ] = { ESC,0x7a,0x30 };
   static const BYTE SetPrintModeNormal [ ] = { ESC,0xf9,0x2d,0x30 };
   static const BYTE SetPrintModeHighQuality [ ] = { ESC,0xf9,0x31 };
   static const BYTE SetPrintModeHighSpeed [ ] = { ESC,0xf9,0x31 };
   static const BYTE SetPaperWidth48 [ ] = { ESC,0xf9,0x21,0x00 };
   static const BYTE SetPaperWidth72 [ ] = { ESC,0xf9,0x21,0x01 };
   static const BYTE SetPaperWidth76 [ ] = { ESC,0xf9,0x21,0x03 };
   static const BYTE SetPrinterLanguageEnglish [ ] = { GS,0xfa,0x30 };
   static const BYTE SetPrinterLanguagePortuguese [ ] = { GS,0xfa,0x31 };
   static const BYTE SetPrinterLanguageSpanish [ ] = { ESC,0xfa,0x32 };
   static const BYTE SetPrinterLanguageGerman [ ] = { ESC,0xfa,0x33 };
   static const BYTE GetPrinterInformationProductCode [ ] = { GS,0xf9,0x27,0x30 };
   static const BYTE GetPrinterInformationSerialNumber [ ] = { GS,0xf9,0x27,0x31 };
   static const BYTE GetPrinterInformationManufacturingData [ ] = { GS,0xf9,0x27,0x32 };
   static const BYTE GetPrinterInformationFirmwareVersion [ ] = { GS,0xf9,0x27,0x33 };
   static const BYTE GetPrinterInformationInterfaceType [ ] = { GS,0xf9,0x27,0x38 };
   static const BYTE PrinterReset [ ] = { GS,0xf8,0x46 };
   static const BYTE SetPageSizeInLines [ ] = { ESC,0x43,0xff };
   static const BYTE FormOnePage [ ] = { 0x0c };
   static const BYTE FeedOneLine [ ] = { 0x0a };
   static const BYTE HorizontalTab [ ] = { 0x09 };
   static const BYTE SetRightMargin [ ] = { ESC,0x51 };
   static const BYTE SetLeftMargin [ ] = { ESC,0x6c };
   static const BYTE EnableUnderline [ ] = { ESC,0x2d,0x30 };
   static const BYTE DisableUnderline [ ] = { ESC,0x2d,0x31 };
   static const BYTE EnableItalic [ ] = { ESC,0x34 };
   static const BYTE DisableItalic [ ] = { ESC,0x35 };
   static const BYTE EnableEmphasized [ ] = { ESC,0x45 };
   static const BYTE DisableEmphasized [ ] = { ESC,0x46 };
   static const BYTE EnableCondensed [ ] = { ESC,0x0f };
   static const BYTE DisableCondensed [ ] = { ESC,0x50 };
   static const BYTE PrinterStatusEnquiry [ ] = { 0x05 };
   static const BYTE PrinterExtendedStatusEnquiry [ ] = { GS,0xf8,0x31 };
   static const BYTE ClearBuffer [ ] = { 0x02 };
};