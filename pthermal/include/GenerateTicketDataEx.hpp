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

#include <GenerateTicketData.hpp>

namespace pthermal
{
   class PTHERMAL_API GenerateTicketDataEx: public GenerateTicketData
   {
   public:
      explicit GenerateTicketDataEx ( pthermal::LPJsonData const & pData,
                                      pthermal::LPIUniversalSerialBusData const & pBus );
      virtual ~GenerateTicketDataEx ( ) { }
      virtual LPITicketDataFieldQueue Build ( );
   protected:
      virtual void NoCouponTax ( );
      virtual void ProductList ( );
      virtual void CouponPreSale ( );
      virtual std::shared_ptr<std::wstring> GetDateTime ( );
      virtual std::shared_ptr<std::wstring> Upper ( std::wstring const & str );
   protected:
      pthermal::LPJsonData m_data;
      GenerateTicketDataEx::TicketType m_type;
      pthermal::LPITicketDataFieldQueue m_queue;
   };
   typedef std::shared_ptr<pthermal::GenerateTicketDataEx> LPGenerateTicketDataEx;
}