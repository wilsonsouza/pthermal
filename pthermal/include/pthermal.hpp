/*
*  Layer to Thermal Printer Controller
*
*  Build by wilson.souza
*  Default language used c++11
*
*  Copyright (c) 2016 PingoBox Software
*  Copyright (c) 2016 WR DevInfo
*
*  Created jun 2 2016
*  Last updated
*  Version 0.1-beta
*
*  Description:
*  The following ifdef block is the standard way of creating macros which make exporting
*  from a DLL simpler. All files within this DLL are compiled with the PTHERMAL_EXPORTS
*  symbol defined on the command line. This symbol should not be defined on any project
*  that uses this DLL. This way any other project whose source files include this file see
*  PTHERMAL_API functions as being imported from a DLL, whereas this DLL sees symbols
*  defined with this macro as being exported.
*/
#pragma once
#ifdef PTHERMAL_EXPORTS
#define PTHERMAL_API __declspec(dllexport)
#else
#define PTHERMAL_API __declspec(dllimport)
#endif

#include <Windows.h>
#include <memory>
#include <string>
#include <wchar.h>
#include <list>
#include <vector>
#include <IThermalPrinterDefs.hpp>

namespace pthermal
{
   //HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet
   static std::wstring KEY_0005 = L"SYSTEM\\CurrentControlSet\\Enum\\USB\\VID_23B8&PID_0005";
   static std::wstring KEY_0003 = L"SYSTEM\\CurrentControlSet\\Enum\\USB\\VID_0B1B&PID_0003";
   static std::wstring KEY_0202 = L"SYSTEM\\CurrentControlSet\\Enum\\USB\\VID_04B8&PID_0202";
   static std::wstring KDP_0003 = L"SYSTEM\\CurrentControlSet\\Enum\\USB\\VID_0B1B&PID_0003\\5&33079e5b&0&1\\Device Parameters";
   static std::wstring KDP_0005 = L"SYSTEM\\CurrentControlSet\\Enum\\USB\\VID_23B8&PID_0005\\0\\Device Parameters";
   //
   struct THERMAL_PRINTER_ID
   {
      unsigned long ulVID;
      unsigned long ulPID;
      unsigned long ulPort;
   };
   typedef std::shared_ptr<THERMAL_PRINTER_ID> LPTHERMALPRINTERID;
   //
   namespace daruma_dr800
   {
      static THERMAL_PRINTER_ID ID = 
      { 
         0x23b8, 
         0x0005, 
         4 
      };
   };
   //
   namespace epson_tm20
   {
      static THERMAL_PRINTER_ID ID = 
      { 
         0x04b8, 
         0x0202, 
         2 
      };
   };
   //
   namespace bematech_mp400_th
   {
      static THERMAL_PRINTER_ID ID = 
      { 
         0x0b1b, 
         0x0003, 
         6 
      };
   };
   //
   namespace xps_windows_printer
   {
      static THERMAL_PRINTER_ID ID =
      {
         0xff,
         0xff,
         0
      };
   }
   //
   namespace pdf_windows_printer
   {
      static THERMAL_PRINTER_ID ID =
      {
         0x100,
         0x100,
         0
      };
   }
   //--------------------------------------------------------------------------------------------//
   typedef std::shared_ptr<DOCINFO> LPDocInfo;
   typedef std::shared_ptr<std::vector<HFONT>> LPFonts;
   typedef std::vector<HFONT> Fonts;
   typedef std::shared_ptr<DEVMODE> LPDeviceMode;
   struct IThermalPrinterNames
   {
      std::wstring Printer;
      std::wstring Driver;
      LPTHERMALPRINTERID ThermalPrinterID;
      HANDLE hPrinter;
      HANDLE hEvent;
      HDC hDC;
      HBITMAP hBitmap;
      BITMAP bitmap;
      LPFonts Fonts;
      LPBITMAPINFO lpBitmap;
      TEXTMETRIC lpMetric[1];
      LPDocInfo DocInfo;
      int x;
      int y;
      int nColumns;
      int nLines;
      int nFontHeight;
      int nFontWidth;
      int nPageWidth;
      int nPageHeight;
      int nOffset;
      LPDeviceMode DeviceMode;
      RGBQUAD rgb[0x100];
      //
      IThermalPrinterNames ( ) = default;
   };
   typedef std::shared_ptr<IThermalPrinterNames> LPIThermalPrinterNames;
   typedef std::pair<std::wstring, std::basic_string<unsigned char>> FieldData;
   typedef std::shared_ptr<std::vector<FieldData>> LPFieldData;
   //--------------------------------------------------------------------------------------------//
   struct ITicketDataField
   {
      LPFieldData Data;
      unsigned long ulFlags;

      ITicketDataField ( )
      {
         ulFlags = 0;
         Data = LPFieldData ( new std::vector<FieldData> );
      }
   };
   typedef std::shared_ptr<ITicketDataField> LPITicketDataField;
   //--------------------------------------------------------------------------------------------//
   struct ITicketData
   {
      ITicketDataField Header;
      ITicketDataField PDV;
      ITicketDataField Extract;
      ITicketDataField ListOfProductHeader;
      ITicketDataField ProductItems;
      ITicketDataField Total;
      ITicketDataField TypePayment;
      ITicketDataField Customer;
      ITicketDataField CustomerDescription;
      ITicketDataField ValueTributes;
      ITicketDataField SatDateTime;
      ITicketDataField QuickResponseCode;
      ITicketDataField Comments;
      //
      std::wstring szDocumentName;
      std::wstring szFileNameOut; //when use WINSPOOL driver with XPS format
      ITicketData ( ) = default;
   };
   typedef std::shared_ptr<ITicketData> LPITicketData;
   typedef std::shared_ptr<std::vector<LPITicketDataField>> LPITicketDataFieldQueue;
   //--------------------------------------------------------------------------------------------//
   struct ImageBits
   {
      std::basic_string<BYTE> cbSource;
      std::basic_string<BYTE> cbDest;
      std::shared_ptr<BYTE> cbData;
      std::basic_string<BYTE> cbQuickResponseCode;
      std::basic_string<BYTE> cbBuffer;
      BITMAPFILEHEADER bmpFile;
      BITMAPINFOHEADER bmpInfo;
      BITMAP bmp;
      unsigned uQRCodeWidth = 0;
      unsigned uVersion = 0;
      std::wstring wszBitmap;
   };
   typedef std::shared_ptr<ImageBits> LPImageBits;
   //--------------------------------------------------------------------------------------------//
   struct IUniversalSerialBusData
   {
      unsigned long Capabilites = 0;
      std::wstring ClassGUID;
      std::wstring CompatibleIDs;
      unsigned long ConfigFlags = 0;
      std::wstring ContainerID;
      std::wstring DeviceDesc;
      std::wstring Driver;
      std::wstring FriendlyName;
      std::wstring HardwareID;
      std::wstring LocationInformation;
      std::wstring Mfg;
      std::wstring ParentIdPrefix;
      std::wstring Service;
      std::wstring UpperFilters;
      std::wstring LowerFilters;
      //
      struct DeviceParameters
      {
         unsigned long CODType;
         std::wstring DeviceAddressCache;
         unsigned long DeviceSelectiveSuspended;
         unsigned long ScansBeforeOutOfRange;
         unsigned long SCOMaxChannels;
         unsigned StoreLinkKeyCODMasks;
         std::wstring SymbolicLinkName;
         std::wstring SymbolicName;
         unsigned long AllowIdleIrpnD3;
         unsigned long EnhancedPowerManagementEnabled;
         unsigned long ExtPropDescSemaphore;
         unsigned long SelectiveSuspendEnabled;
         unsigned long DeviceSelectiveSuspend;
         std::wstring PortName;
         unsigned long IsPrinter;
         std::wstring DeviceObjectName;
         std::wstring DriverUserInterfaceGuid;
         std::wstring VPortName;
      } m_device;
      //
      struct Properties
      {
         unsigned long Reserved;
         void * Buffer;
         Properties ( )
         {
            Reserved = 0L;
            Buffer = nullptr;
         }
      } m_prop;
      //
      struct VirtualPrinterID
      {
         unsigned long ulVID;
         unsigned long ulPID;
         unsigned long ulPrinterMode;
         unsigned long ulPrinterID;
         //
         explicit VirtualPrinterID ( ) 
         {
            ulPID = ulVID = ulPrinterID = ulPrinterMode = 0;
         }
         explicit VirtualPrinterID ( unsigned long ulVID, unsigned long ulPID )
         {
            this->ulVID = ulVID;
            this->ulPID = ulPID;
         }
      }m_userial;
      //
      struct ThermalPrinterStatus
      {
         bool bIsReady;
         bool bIsNoPaper;
      }m_printer_status;
      //
      LPIThermalPrinterNames ThermalPrinterID;
      LPITicketDataFieldQueue DataFieldQueue;
      LPImageBits m_pImage;
   };
   typedef std::shared_ptr<IUniversalSerialBusData> LPIUniversalSerialBusData;
   //--------------------------------------------------------------------------------------------//
   struct CFe
   {
      unsigned uPPos = 0;
      std::wstring szQuickResponseCodeFile;

      struct infCFe
      {
         std::wstring Id;
         std::wstring Version;
         std::wstring VersionDataEntry;
         std::wstring VersionSB;
         //
         struct ide
         {
            std::wstring cUF;
            std::wstring cNF;
            std::wstring mod;
            std::wstring nserieSAT;
            std::wstring nCFe;
            std::wstring dEmi;
            std::wstring hEmi;
            std::wstring cDV;
            std::wstring tpAmb;
            std::wstring CNPJ;
            std::wstring signAC;
            std::wstring assinaturaQRCODE;
            std::wstring numeroCaixa;
         } m_ide;
         //
         struct emit
         {
            std::wstring CNPJ;
            std::wstring xNome;
            //
            struct enderEmit
            {
               std::wstring xLgr;
               std::wstring nro;
               std::wstring xCpl;
               std::wstring xBairro;
               std::wstring xMun;
               std::wstring CEP;
            } m_enderemit;
            //
            std::wstring IE;
            std::wstring IM;
            std::wstring cRegTrib;
            std::wstring cRegTribISSQN;
            std::wstring indRatISSQN;
            std::wstring dest;
         } m_emit;
         //
         struct dest
         {
            std::wstring cpf;
            std::wstring xNome;
         } m_dest;
         //
         struct entrega
         {
            std::wstring xLgr;
            std::wstring nro;
            std::wstring xBairro;
            std::wstring xMun;
            std::wstring uf;
         } m_entrega;
         //
         struct det
         {
            unsigned nItem = 0;
            //
            struct prod
            {
               std::wstring cProd;
               std::wstring xProd;
               std::wstring NCM;
               std::wstring CFOP;
               std::wstring uCom;
               std::wstring qCom;
               std::wstring vUnCom;
               std::wstring vProd;
               std::wstring indRegra;
               std::wstring vItem;
            } m_prod;
            std::vector<prod> m_ItemsList;
            //
            struct imposto
            {
               std::wstring vItem12741;
               //
               struct ICMS
               {
                  struct ICMS40
                  {
                     std::wstring Orig;
                     std::wstring CST;
                  } m_icms40;
               } m_icms;
               //
               struct PIS
               {
                  struct PISNT
                  {
                     std::wstring CST;
                  } m_pisnt;
               }m_pis;
               //
               struct CONFINS
               {
                  struct CONFINSNT
                  {
                     std::wstring CST;
                  }m_confinsnt;
               }m_confins;
            }m_imposto;
         } m_det;
         //
         struct total
         {
            struct ICMSTot
            {
               std::wstring vICMS;
               std::wstring vProd;
               std::wstring vDesc;
               std::wstring vPIS;
               std::wstring vCONFINS;
               std::wstring vPISST;
               std::wstring vCONFISST;
               std::wstring vOutro;
            } m_icmstot;
            //
            std::wstring vCFe;
         } m_total;
         //
         struct pgto
         {
            struct MP
            {
               std::wstring cMP;
               std::wstring vMP;
            } m_mp;
            //
            std::vector<pgto::MP> m_paytypes;
            std::wstring vTroco;
         } m_pgto;
         //
         struct infAdic
         {
            std::wstring infCpl;
            //
            struct obsFisco
            {
               std::wstring xCampo;
               std::wstring xTexto;
            } m_obsfisco;
         } m_infadic;
      } m_infcfe;
      //
      struct Signature
      {
         std::wstring xmlns;
         //
         struct SignedInfo
         {
            struct CanonicalizationMethod
            {
               std::wstring Algorithm;
            } m_CanonicalizationMethod;
            //
            struct SignatureMethod
            {
               std::wstring Algorithm;
            } m_SignatureMethod;
            //
            struct Reference
            {
               std::wstring URI;
               //
               struct Transforms
               {
                  struct Transform
                  {
                     std::wstring Algorithm[1];
                  } m_transform;
               } m_transforms;
               //
               struct DigestMethod
               {
                  std::wstring Algorithm;
               } m_digestmethod;
               //
               std::wstring DigestValue;
            } m_reference;
            //
            std::wstring SignatureValue;
            //
            struct KeyInfo
            {
               struct X509Data
               {
                  std::wstring X509Certificate;
               } m_x509data;
            } m_keyinfo;
         } m_signedinfo;
      } m_signature;
   };
   typedef std::shared_ptr<CFe> LPXMLCFeData;
   //--------------------------------------------------------------------------------------------//
   // structure to support of json format file
   enum class TicketType : unsigned
   {
      nocoupontax = 0x100,
      couponpresale,
      productlist,
      coupon
   };

   struct jsondata
   {
      std::wstring company_name;
      //
      struct customer
      {
         std::wstring cnpj;
         std::wstring cpf;
         std::wstring email;
         std::wstring name;
      } m_customer;
      //
      int discount;
      std::wstring discount_type;
      int increment;
      //
      struct payments
      {
         int installment;
         std::wstring payment_method;
         double value_paid = 0;
         double value_received = 0;
      };
      //
      std::vector<payments> m_payments_queue;
      //
      struct products
      {
         std::wstring name;
         struct options
         {
            std::wstring reserved;
         };
         std::vector<options> m_options_queue;
         double price = 0;
         std::wstring product_variable;
         long qty = 0;
      };
      //
      std::vector<products> m_products_queue;
      //
      std::wstring promotional_message;
      std::wstring seller_name;
      std::wstring user_name;
      //
      pthermal::TicketType utype;
   };
   typedef std::shared_ptr<jsondata> LPJsonData;
   //--------------------------------------------------------------------------------------------//
   typedef std::shared_ptr<std::basic_string<wchar_t>> LPWideString;
   typedef std::shared_ptr<std::basic_string<char>> LPAnsiString;
   typedef std::shared_ptr<std::basic_string<unsigned char>> LPByteString;
   typedef std::shared_ptr<unsigned char> LPByte;
   //--------------------------------------------------------------------------------------------//
   namespace printer
   {
      namespace mode
      {
         static unsigned long const VirtualUniversalSerialBus = 0x1L;
         static unsigned long const WindowsDriver = 0x2L;
         static unsigned long const DynamicLibaryPrinter = 0x4L;
         static unsigned long const WindowsSpool = 0x8L;
         static unsigned long const UseXPSPrinter = 0x10L;
         //
         namespace output
         {
            static std::wstring const RAW = L"RAW";
            static std::wstring const XPS = L"XPS";
            static std::wstring const PDF = L"PDF";
         }
      }
      //--------------------------------------------------------------------------------------------//
      namespace models
      {
         static std::wstring const WinSpool = L"WINSPOOL";

         namespace epson
         {
            static std::wstring const Driver = L"EPSON TM-T20 Receipt5";
            static std::wstring const Printer = L"EPSON TM-T20 Receipt5";
            static std::wstring const AlternativePrinterName = L"EPSON TM-T20 Receipt";
         }
         //
         namespace bematech
         {
            static std::wstring const Driver = L"MP-4200 TH";
            static std::wstring const Printer = L"MP-4200 TH";
         }
         //
         namespace daruma
         {
            static std::wstring const Driver = L"Daruma DR700 Spooler";
            static std::wstring const Printer = L"Daruma DR700 Spooler";
         }
         //
         namespace windows_xps
         {
            static std::wstring const Driver = L"Microsoft XPS Document Writer v4";
            static std::wstring const Printer = L"Microsoft XPS Document Writer";
         }
         //
         namespace windows_pdf
         {
            static std::wstring const Driver = L"Microsoft Print to PDF";
            static std::wstring const Printer = L"Microsoft Print to PDF";
         }
      }
      //--------------------------------------------------------------------------------------------//
      namespace code
      {
         namespace id
         {
            static unsigned const Bematech_MP400_TH = 0x0b1b;
            static unsigned const Epson_TM20 = 0x04b8;
            static unsigned const Daruma_DR800 = 0x23b8;
            static unsigned const XPSPrinter = 0xffff;
         }
         //
         namespace index
         {
            static unsigned const Bematech_MP400_TH = 0;
            static unsigned const Epson_TM20 = 1;
            static unsigned const Daruma_DR800 = 2;
            static unsigned const XPSPrinter = 3;
         }
         //
         static pthermal::THERMAL_PRINTER_ID * g_pThermalListId[] =
         {
            &pthermal::bematech_mp400_th::ID,
            &pthermal::epson_tm20::ID,
            &pthermal::daruma_dr800::ID,
            &pthermal::xps_windows_printer::ID
         };
         //--------------------------------------------------------------------------------------------//
         static LPIThermalPrinterNames const GetPrinterModel (
            pthermal::IUniversalSerialBusData::VirtualPrinterID * pID )
         {
            IThermalPrinterNames pNames[] =
            {
               {
                  printer::models::bematech::Printer,
                  printer::models::bematech::Driver,
                  //g_pThermalListId [ index::Bematech_MP400_TH ]
                  std::make_shared<THERMAL_PRINTER_ID> ( pthermal::bematech_mp400_th::ID )
               },
               {
                  printer::models::epson::Printer,
                  printer::models::epson::Driver,
                  //g_pThermalListId [ index::Daruma_DR800 ]
                  std::make_shared<THERMAL_PRINTER_ID> ( pthermal::epson_tm20::ID )
               },
               {
                  printer::models::daruma::Printer,
                  printer::models::daruma::Driver,
                  //g_pThermalListId [ index::Epson_TM20 ]
                  std::make_shared<THERMAL_PRINTER_ID> ( pthermal::daruma_dr800::ID )
               },
               {
                  printer::models::windows_xps::Printer,
                  printer::models::windows_xps::Driver,
                  std::make_shared<THERMAL_PRINTER_ID> ( pthermal::xps_windows_printer::ID )
               }
            };
            //
            for ( auto i = 0; i < sizeof ( pNames ) / sizeof ( *pNames ); i++ )
            {
               auto p = pNames + i;
               //
               if ( p->ThermalPrinterID->ulPID == pID->ulPID &&
                    p->ThermalPrinterID->ulVID == pID->ulVID )
               {
                  return std::make_shared<IThermalPrinterNames> ( *p );
               }
            }
            return std::make_shared<IThermalPrinterNames> ( IThermalPrinterNames
            {
               std::wstring ( ),
               std::wstring ( ),
               NULL,
               NULL
            } );
         }
      }
   }
   //--------------------------------------------------------------------------------------------//
   static bool WINAPI IsVendor ( LPIUniversalSerialBusData const & pBus, 
                                 THERMAL_PRINTER_ID const * pID )
   {
      return ( pBus->m_userial.ulPID == pID->ulPID && pBus->m_userial.ulVID == pID->ulVID );
   }
   //--------------------------------------------------------------------------------------------//
   static bool WINAPI IsVendor ( pthermal::IUniversalSerialBusData::VirtualPrinterID const * pFrom,
                                 pthermal::IUniversalSerialBusData::VirtualPrinterID const * pTo )
   {
      return ( pFrom->ulPID == pTo->ulPID && pFrom->ulVID == pTo->ulVID );
   }
};
//--------------------------------------------------------------------------------------------//
namespace std
{
   typedef basic_string<unsigned char> ustring; //stringbyte
};
