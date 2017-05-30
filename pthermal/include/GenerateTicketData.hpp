/*
*  Layer to Generate Ticket
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
#pragma once
#pragma warning(disable:4275)
#pragma warning(disable:4251)
#include <IGenerateTicketData.h>

namespace pthermal
{
   class PTHERMAL_API GenerateTicketData : IGenerateTicketData
   {
   protected:
      std::vector<std::pair<int, std::wstring>> m_pay;
   public:
      GenerateTicketData ( pthermal::LPXMLCFeData const & pcfe,
                           pthermal::LPIUniversalSerialBusData const & pBus);
      virtual ~GenerateTicketData ( ) { }
      //
      virtual LPITicketDataField GetHeader ( );
      virtual LPITicketDataField GetPDV ( );
      virtual LPITicketDataField GetExtract ( );
      virtual LPITicketDataField GetListOfProductHeader ( );
      virtual LPITicketDataField GetProductItems ( );
      virtual LPITicketDataField GetTotal ( );
      virtual LPITicketDataField GetTypePayment ( );
      virtual LPITicketDataField GetCustomer ( );
      virtual LPITicketDataField GetCustomerDescription ( );
      virtual LPITicketDataField GetValueTributes ( );
      virtual LPITicketDataField GetSatDateTime ( );
      virtual LPITicketDataField GetQuickResponseCode ( );
      virtual LPITicketDataField GetComments ( );
      virtual LPITicketDataFieldQueue Build ( );
      virtual void SetPaperWidth(unsigned uPaperWidth );
      virtual std::size_t GetPaperWidth ( );
   protected:
      virtual pthermal::LPWideString const replicate ( unsigned char c, size_t uSize );
   };
   //
   typedef std::shared_ptr<GenerateTicketData> LPGenerateTicketData;
}