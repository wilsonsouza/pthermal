/*
*  Layer to Epson Printer EscPos
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

namespace escpos
{
   static const BYTE ESC = 0x1b;
   static const BYTE GS = 0x1d;
   static const BYTE FS = 0x1c;
   //symbol FF represent parameter to insert
   namespace Print
   {
      static const BYTE LineFeed [ ] = { 0x0a };
      static const BYTE CarriageReturn [ ] = { 0x0d };
      static const BYTE ReturnToStandardMode_in_PageMode [ ] = { 0x0c };
      static const BYTE Data_in_PageMode [ ] = { ESC, 0x0c };
      static const BYTE FeedPaper [ ] = { ESC, 0x4a, 0xFF };
      static const BYTE FeedLines [ ] = { ESC, 0x64, 0xFF };
      static const BYTE NextLine [ ] = { 0x0d, 0x0a };
   }
   //
   namespace Position
   {
      static const BYTE HorizontalTab [ ] = { 0x09 };
      static const BYTE SelectPrintDirectionPageMode_LeftRight [ ] = { ESC, 0x54, 0x00 };
      static const BYTE SelectPrintDirectionPageMode_BottomTop [ ] = { ESC, 0x54, 0x01 };
      static const BYTE SelectPrintDirectionPageMode_RightLeft [ ] = { ESC, 0x54, 0x02 };
      static const BYTE SelectPrintDirectionPageMode_TopBottom [ ] = { ESC, 0x54, 0x03 };
      //
      static const BYTE Center [ ] = { ESC, 0x61, 0x01 };
      static const BYTE Left [ ] = { ESC, 0x61, 0x00 };
      static const BYTE Right [ ] = { ESC, 0x61, 0x02 };
   }
   //
   namespace LineSpacing
   {
      static const BYTE SelectDefaultLineSpacing [ ] = { ESC, 0x32 };
      static const BYTE SetLineSpacing [ ] = { ESC, 0x33 }; //set calcule type 3.75mm (30/203 inch)
   }
   //
   namespace MechanismControl
   {
      static const BYTE PartialCut [ ] = { ESC, 'i' };
      static const BYTE PartialAutoCutter [ ] = { ESC, 'm' };
      static const BYTE SelectCutMode_FullFontA [ ] = { GS, 0x56, 0x00 };
      static const BYTE SelectCutMode_PartialFontA [ ] = { GS, 0x56, 0x01 };
   }
   //
   namespace RealtimeStatus
   {
      static unsigned printerstatus = 1;
      static unsigned offlinecause = 2;
      static unsigned errorcause = 3;
      static unsigned continuouspaperdetector = 4;
      static unsigned presenterpaperdetector = 5;
   }
   //
   namespace Miscellaneous
   {
      static const BYTE SendRealtimeRequestPrinter_RestartPrintingLine [ ] = { 0x10, 0x05, 0x01 };
      static const BYTE SendRealtimeRequestPrinter_ClearingReceiveBuffers [ ] = { 0x10, 0x05, 0x02 };
      static const BYTE InitializePrinter [ ] = { ESC, 0x40 };
      static const BYTE SelectPeripheralDevice [ ] = { ESC, 0x3d, 0x01 };
      //
      static const BYTE ExecuteTestPrint_Basic_PrinterStatus [ ] = { GS, 0x28, 0x41, 0x02, 0x00, 0x02 };
      static const BYTE ExecuteTestPrint_RollPaper_RollingPattern [ ] = { GS, 0x28, 0x41, 0x01, 0x03 };
      static const BYTE ExecuteTestPrint_Basicsheet_Hexadecimal_Dump [ ] = { GS, 0x28, 0x41, 0x00, 0x01 };
      //
      static const BYTE TransmitPrinterID_Name [ ] = { ESC, 0x49, 0x42 };
      static const BYTE TransmitPrinterID_Model [ ] = { ESC, 0x49, 0x43 };
      static const BYTE TransmitPrinterID_SerialNumber [ ] = { ESC, 0x49, 0x44 };
   }
   //
   namespace Character
   {
      static const BYTE SelectCharacterFontA [ ] = { ESC, 'M', '0' }; //font_a 12x24
      static const BYTE SelectCharacterFontB [ ] = { ESC, 'M', '1' }; //font_b  9x17
      static const BYTE SetectInternational_LatinAmerica [ ] = { ESC, 0x52, 0x0c }; //latim america
                                                                                     //
      static const BYTE EnableUnderline [ ] = { ESC, 0x2d, 0x00 };
      static const BYTE DisableUnderline [ ] = { ESC, 0x2d, 0x01 };
      static const BYTE EnableEmphasized [ ] = { ESC, 0x45, 0x01 };
      static const BYTE DisableEmphasized [ ] = { ESC, 0x45, 0x00 };
      static const BYTE EnableDoubleStrike [ ] = { ESC, 0x47, 0x01 };
      static const BYTE DisableDoubleStrike [ ] = { ESC, 0x47, 0x00 };
      //only to compatible with windows fonts
      static const BYTE EnableBold[] = { ESC, 0xff, 0xff };
      static const BYTE DisableBold[] = { ESC, 0xff, 0xfe };

      //bematech 
      //Font A 14x24
      //Font B 10.5x24
      static const BYTE SelectCharacterFont_A [ ] = { ESC, 0x4d, 0x00 };
      static const BYTE SelectCharacterFont_B [ ] = { ESC, 0x4d, 0x01 };
      //
      static const BYTE SelectCharacterTableMode_PC850_Multilingual [ ] = { ESC, 0x74, 0x02 };
      static const BYTE SelectCharacterTableMode_PC860_Portuguese [ ] = { ESC, 0x74, 0x03 };
      //
      static const BYTE EnableSmoothing [ ] = { GS, 0x62, 0x01 };
      static const BYTE DisableSmoothing [ ] = { GS, 0x62, 0x00 };
   }
   //
   namespace PanelButtons
   {
      static const BYTE EnablePanelKeys [ ] = { ESC, 'c', '5', '0' };
      static const BYTE DisablePanelKeys [ ] = { ESC, 'c', '5', '1' };
   }
   //Quick response code table commands
   namespace QRCode
   {
      static const BYTE SelectsQRCodeModel [ ] = { GS, 0x28, 0x6B, 0x04, 0x00, 0x31, 0x41, 0x32, 0x00 };
      static const BYTE SetsTheModuleSizeOfQrCode [ ] = { GS, 0x28, 0x6B, 0x03, 0x00, 0x31, 0x43, 0x03 };
      static const BYTE SelectTheErrorCorrectionLevel [ ] = { GS, 0x28, 0x6B, 0x03, 0x00, 0x31, 0x45, 0x31 };
      static const BYTE StoreTheDataSymbolStorageArea [ ] = { GS, 0x28, 0x6b, 0xFF, 0xFF, 0x31, 0x50, 0x30 };
      static const BYTE PrintTheSymbolDataSymbolStorageArea [ ] = { GS, 0x28, 0x6b, 0x03, 0x00, 0x31, 0x51, 0x30 };
      static const BYTE TransmitSizeInformationSymbolDataSymbolStorageArea [ ] =
      { GS, 0x28, 0x6b, 0x03, 0x00, 0x31, 0x52, 0x30 };
   }
   //status commands
   namespace StatusCommand
   {
      static const BYTE PrinterStatus [ ] = { 0x10, 0x04, 0x01 };
      static const BYTE OfflineStatus [ ] = { 0x10, 0x04, 0x02 };
      static const BYTE ErrorStatus [ ] = { 0x10, 0x04, 0x03 };
      static const BYTE RollPaperSensorStatus [ ] = { 0x10, 0x04, 0x04 };
      static const BYTE EnableAutomaticStatusBack [ ] = { GS, 0x61, 0x02 };
      static const BYTE DisableAutomaticStatusBack [ ] = { GS, 0x61, 0x00 };
   }
   //
   namespace Image
   {
      static const BYTE DefineNVBitImage [ ] = { FS, 'q' };
      static const BYTE PrintNVBitImage [ ] = { FS, 'p' };
   }
   //
   static const BYTE PrinterEnable [ ] = { ESC, '=', (1 << 0) };
   static const BYTE PrinterDisable [ ] = { ESC, '=', 0 };

};