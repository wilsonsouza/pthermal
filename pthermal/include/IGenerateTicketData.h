/*
*  Layer to Generate Ticket
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
#pragma warning(disable:4275)
#pragma warning(disable:4251)
#include <pthermal.hpp>
#include <list>

namespace pthermal
{
   namespace FieldId
   {
      static unsigned long const Header = 1;
      static unsigned long const PDV = 2;
      static unsigned long const Extract = 4;
      static unsigned long const ListOfProductHeader = 8;
      static unsigned long const ProductItems = 0x10;
      static unsigned long const Total = 0x20;
      static unsigned long const TypePayment = 0x40;
      static unsigned long const Customer = 0x80;
      static unsigned long const CustomerDescription = 0x100;
      static unsigned long const ValueTributes = 0x200;
      static unsigned long const SatDateTime = 0x400;
      static unsigned long const QuickResponseCode = 0x800;
      static unsigned long const Comments = 0x1000;
   }
   //
   namespace DataType
   {
      static unsigned long const Text = 0x2000;
      static unsigned long const QuickResponseCode = 0x4000;
      static unsigned long const ArrayOfText = 0x8000;
   }
   //
   class PTHERMAL_API IGenerateTicketData : public ITicketData
   {
   protected:
      std::basic_string<wchar_t> m_wszBuffer;
      pthermal::LPIUniversalSerialBusData m_pBus;
      LPXMLCFeData m_pcfe;
      unsigned long m_ulPrinterID;
      size_t m_nPageWidth;
   public:
      explicit IGenerateTicketData ( LPXMLCFeData const & pcfe,
                                     pthermal::LPIUniversalSerialBusData const & pBus ) 
      {
         m_pBus = pBus;
         m_pcfe = pcfe;
      }
      virtual ~IGenerateTicketData ( ) { }
      //
      virtual LPITicketDataField GetHeader ( ) = 0;
      virtual LPITicketDataField GetPDV ( ) = 0;
      virtual LPITicketDataField GetExtract ( ) = 0;
      virtual LPITicketDataField GetListOfProductHeader ( ) = 0;
      virtual LPITicketDataField GetProductItems ( ) = 0;
      virtual LPITicketDataField GetTotal ( ) = 0;
      virtual LPITicketDataField GetTypePayment ( ) = 0;
      virtual LPITicketDataField GetCustomer ( ) = 0;
      virtual LPITicketDataField GetCustomerDescription ( ) = 0;
      virtual LPITicketDataField GetValueTributes ( ) = 0;
      virtual LPITicketDataField GetSatDateTime ( ) = 0;
      virtual LPITicketDataField GetQuickResponseCode ( ) = 0;
      virtual LPITicketDataField GetComments ( ) = 0;
      virtual LPITicketDataFieldQueue Build ( ) = 0;
   protected:
      virtual pthermal::LPWideString const replicate ( unsigned char c, size_t uSize ) = 0;
   };
}