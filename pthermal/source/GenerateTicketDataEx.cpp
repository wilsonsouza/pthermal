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
#include <GenerateTicketDataEx.hpp>
#include <IWcharToChar.h>
#include <escpos.hpp>
#include <time.h>
#include <regex>

using namespace escpos;
using namespace pthermal;
//-----------------------------------------------------------------------------------------------//
GenerateTicketDataEx::GenerateTicketDataEx ( LPJsonData const & pData,
                                             LPIUniversalSerialBusData const & pBus ) :
                                             GenerateTicketData ( NULL, pBus )
{
   m_data = pData;
   m_queue = LPITicketDataFieldQueue ( new std::vector<LPITicketDataField> );
}
//-----------------------------------------------------------------------------------------------//
void GenerateTicketDataEx::CouponPreSale ( )
{
   LPITicketDataField p = LPITicketDataField ( new ITicketDataField );
   //title
   auto Caption = [&] ( )->void
   {
      p->ulFlags = DataType::Text;
      p->Data->push_back ( FieldData ( L"", Print::NextLine ) );
      p->Data->push_back ( FieldData ( L"", Character::EnableEmphasized ) );
      p->Data->push_back ( FieldData ( m_data->company_name, Position::Center ) );
      p->Data->push_back ( FieldData ( L"", Character::DisableEmphasized ) );
      p->Data->push_back ( FieldData ( L"", Print::NextLine ) );
      p->Data->push_back ( FieldData ( *replicate ( '-', GetPaperWidth ( ) ), Position::Left ) );
   };
   //date time
   auto DateTime = [&] ( )->void
   {
      p->Data->push_back ( FieldData ( L"", Print::NextLine ) );
      p->Data->push_back ( FieldData ( GetDateTime ( )->c_str ( ), Position::Center ) );
      p->Data->push_back ( FieldData ( L"", Print::NextLine ) );
      //
      p->Data->push_back ( FieldData ( *replicate ( '-', GetPaperWidth ( ) ), Position::Left ) );
   };
   //coupon title
   auto CouponTitle = [&] ( )->void
   {
      p->Data->push_back ( FieldData ( L"", Print::NextLine ) );
      p->Data->push_back ( FieldData ( L"", Character::EnableEmphasized ) );
      p->Data->push_back ( FieldData ( L"PRE VENDA", Position::Center ) );
      p->Data->push_back ( FieldData ( L"", Character::DisableEmphasized ) );
      p->Data->push_back ( FieldData ( L"", Print::NextLine ) );
      p->Data->push_back ( FieldData ( *replicate ( '-', GetPaperWidth ( ) ), Position::Left ) );
   };
   //order and request order
   auto Order = [&] ( )->void
   {
      p->Data->push_back ( FieldData ( L"", Print::NextLine ) );
      p->Data->push_back ( FieldData ( L"", Character::EnableBold ) );
      p->Data->push_back ( FieldData ( L"Pedido:", Position::Center ) );
      p->Data->push_back ( FieldData ( L"", Character::DisableBold ) );
      p->Data->push_back ( FieldData ( L"", Print::NextLine ) );
      p->Data->push_back ( FieldData ( L"Número do pedido:", Position::Center ) );
      p->Data->push_back ( FieldData ( L"", Print::NextLine ) );
      p->Data->push_back ( FieldData ( *replicate ( '-', GetPaperWidth ( ) ), Position::Left ) );
   };
   //product queue
   auto ProductQueue = [&] ( )->void
   {
      double ultotal = 0;

      p->Data->push_back ( FieldData ( L"", Print::NextLine ) );

      for ( auto && i : m_data->m_products_queue )
      {
         p->Data->push_back ( FieldData ( i.name, Position::Left ) );
         p->Data->push_back ( FieldData ( L"", Print::NextLine ) );
         //
         std::wstring price = std::wstring ( L"", GetPaperWidth() );
         swprintf ( const_cast< wchar_t * >( price.c_str ( ) ),
                    price.length ( ),
                    L"R$ %.2f\0",
                    float ( i.price * double ( i.qty ) ) );

         p->Data->push_back ( FieldData ( price.c_str ( ), Position::Left ) );
         p->Data->push_back ( FieldData ( std::to_wstring ( i.qty ), Position::Center ) );
         //
         std::wstring value = std::wstring ( L"", GetPaperWidth() );
         swprintf ( const_cast< wchar_t * >( value.c_str ( ) ),
                    value.length ( ),
                    L"R$ %.2f\0",
                    float ( i.price ) );
         p->Data->push_back ( FieldData ( value.c_str ( ), Position::Right ) );
         //
         ultotal += ( i.price * double ( i.qty ) );
         p->Data->push_back ( FieldData ( L"", Print::NextLine ) );
         p->Data->push_back ( FieldData ( L"", Print::NextLine ) );
      }
      //
      p->Data->push_back ( FieldData ( *replicate ( '-', GetPaperWidth ( ) ), Position::Left ) );
      p->Data->push_back ( FieldData ( L"", Print::NextLine ) );
      p->Data->push_back ( FieldData ( L"", Character::EnableEmphasized ) );
      p->Data->push_back ( FieldData ( L"TOTAIS", Position::Left ) );
      p->Data->push_back ( FieldData ( L"", Character::DisableEmphasized ) );
      p->Data->push_back ( FieldData ( L"", Print::NextLine ) );
      //
      p->Data->push_back ( FieldData ( L"SUB-TOTAL", Position::Left ) );
      //
      std::wstring total = std::wstring ( L"", GetPaperWidth ( ) );
      swprintf ( const_cast< wchar_t * >( total.c_str ( ) ),
                 total.length ( ),
                 L"R$ %.2f\0",
                 float ( ultotal ) );

      p->Data->push_back ( FieldData ( total.c_str ( ), Position::Right ) );
      p->Data->push_back ( FieldData ( L"", Print::NextLine ) );
      //service tax
      if ( m_data->increment > 0 )
      {
         p->Data->push_back ( FieldData ( L"TAXA DE SERVIÇO", Position::Left ) );
         p->Data->push_back ( FieldData ( std::to_wstring ( m_data->increment ), Position::Right ) );
         p->Data->push_back ( FieldData ( L"", Print::NextLine ) );
      }
      //discount
      if ( m_data->discount > 0 )
      {
         p->Data->push_back ( FieldData ( L"DESCONTO", Position::Left ) );
         p->Data->push_back ( FieldData ( std::to_wstring ( m_data->discount ), Position::Right ) );
         p->Data->push_back ( FieldData ( L"", Print::NextLine ) );
      }
      //
      p->Data->push_back ( FieldData ( L"TOTAL", Position::Left ) );
      p->Data->push_back ( FieldData ( L"", Character::EnableBold ) );
      p->Data->push_back ( FieldData ( total.c_str ( ), Position::Right ) );
      p->Data->push_back ( FieldData ( L"", Character::DisableBold ) );
      p->Data->push_back ( FieldData ( L"", Print::NextLine ) );
   };
   //
   auto Comments = [&] ( )->void
   {
      //
      if ( m_data->promotional_message.size ( ) > 0 )
      {
         p->Data->push_back ( FieldData ( *replicate ( '-', GetPaperWidth ( ) ), Position::Left ) );
         p->Data->push_back ( FieldData ( m_data->promotional_message, Position::Left ) );
         p->Data->push_back ( FieldData ( L"", Print::NextLine ) );
      }
      //
      p->Data->push_back ( FieldData ( *replicate ( '-', GetPaperWidth ( ) ), Position::Left ) );
      p->Data->push_back ( FieldData ( L"", Print::NextLine ) );
      p->Data->push_back ( FieldData ( L"www.pingobox.com.br", Position::Center ) );
      p->Data->push_back ( FieldData ( L"", Print::NextLine ) );
      p->Data->push_back ( FieldData ( L"", Print::NextLine ) );
   };
   //
   Caption ( );
   DateTime ( );
   CouponTitle ( );
   Order ( );
   ProductQueue ( );
   Comments ( );
   //add on queue
   m_queue->push_back ( p );
}
//-----------------------------------------------------------------------------------------------//
void GenerateTicketDataEx::NoCouponTax ( )
{
   LPITicketDataField p = LPITicketDataField ( new ITicketDataField );
   //title
   auto Caption = [&] ( )->void
   {
      p->ulFlags = DataType::Text;
      p->Data->push_back ( FieldData ( L"", Print::NextLine ) );
      p->Data->push_back ( FieldData ( L"", Character::EnableEmphasized ) );
      p->Data->push_back ( FieldData ( m_data->company_name, Position::Center ) );
      p->Data->push_back ( FieldData ( L"", Character::DisableEmphasized ) );
      p->Data->push_back ( FieldData ( L"", Print::NextLine ) );
      p->Data->push_back ( FieldData ( *replicate ( '-', GetPaperWidth ( ) ), Position::Left ) );
   };
   //date time
   auto DateTime = [&] ( )->void
   {
      p->Data->push_back ( FieldData ( L"", Print::NextLine ) );
      p->Data->push_back ( FieldData ( GetDateTime ( )->c_str ( ), Position::Center ) );
      p->Data->push_back ( FieldData ( L"", Print::NextLine ) );
      //
      p->Data->push_back ( FieldData ( *replicate ( '-', GetPaperWidth ( ) ), Position::Left ) );
   };
   //owner
   auto Owner = [&] ( )->void
   {
      if ( m_data->m_customer.name.size ( ) > 0 )
      {
         p->Data->push_back ( FieldData ( L"", Print::NextLine ) );
         p->Data->push_back ( FieldData ( m_data->m_customer.name, Position::Center ) );
      }
      //if cpf or cnpj
      if ( m_data->m_customer.cnpj.size ( ) > 0 )
      {
         p->Data->push_back ( FieldData ( L"", Print::NextLine ) );
         p->Data->push_back ( FieldData ( m_data->m_customer.cnpj, Position::Center ) );
      }
      else
      {
         p->Data->push_back ( FieldData ( L"", Print::NextLine ) );
         p->Data->push_back ( FieldData ( m_data->m_customer.cpf, Position::Center ) );
      }
      //
      if ( m_data->m_customer.email.size ( ) > 0 )
      {
         p->Data->push_back ( FieldData ( L"", Print::NextLine ) );
         p->Data->push_back ( FieldData ( m_data->m_customer.email, Position::Center ) );
         p->Data->push_back ( FieldData ( L"", Print::NextLine ) );
      }
   };
   //coupon title
   auto CouponTitle = [&] ( )->void
   {
      p->Data->push_back ( FieldData ( *replicate ( '-', GetPaperWidth ( ) ), Position::Left ) );
      p->Data->push_back ( FieldData ( L"", Print::NextLine ) );
      p->Data->push_back ( FieldData ( L"", Character::EnableEmphasized ) );
      p->Data->push_back ( FieldData ( L"CUPON NÃO FISCAL", Position::Center ) );
      p->Data->push_back ( FieldData ( L"", Character::DisableEmphasized ) );
      p->Data->push_back ( FieldData ( L"", Print::NextLine ) );
      p->Data->push_back ( FieldData ( *replicate ( '-', GetPaperWidth ( ) ), Position::Left ) );
   };
   //order and request order
   auto Order = [&] ( )->void
   {
      p->Data->push_back ( FieldData ( L"", Print::NextLine ) );
      p->Data->push_back ( FieldData ( L"", Character::EnableBold ) );
      p->Data->push_back ( FieldData ( L"Pedido:", Position::Center ) );
      p->Data->push_back ( FieldData ( L"", Character::DisableBold ) );
      p->Data->push_back ( FieldData ( L"", Print::NextLine ) );
      p->Data->push_back ( FieldData ( L"Número do pedido:", Position::Center ) );
      p->Data->push_back ( FieldData ( L"", Print::NextLine ) );
      p->Data->push_back ( FieldData ( *replicate ( '-', GetPaperWidth ( ) ), Position::Left ) );
   };
   //product queue
   auto ProductQueue = [&] ( )->void
   {
      double ultotal = 0;

      p->Data->push_back ( FieldData ( L"", Print::NextLine ) );

      for ( auto && i : m_data->m_products_queue )
      {
         p->Data->push_back ( FieldData ( i.name, Position::Left ) );
         p->Data->push_back ( FieldData ( L"", Print::NextLine ) );
         //
         std::wstring price = std::wstring ( L"", GetPaperWidth ( ) );
         swprintf ( const_cast< wchar_t * >( price.c_str ( ) ),
                    price.length ( ),
                    L"R$ %.2f\0",
                    float ( i.price * double ( i.qty ) ) );
         p->Data->push_back ( FieldData ( price.c_str ( ), Position::Left ) );
         p->Data->push_back ( FieldData ( std::to_wstring ( i.qty ), Position::Center ) );
         //
         std::wstring value = std::wstring ( L"", GetPaperWidth ( ) );
         swprintf ( const_cast< wchar_t * >( value.c_str ( ) ),
                    value.length ( ),
                    L"R$ %.2f\0",
                    ( i.price ) );
         p->Data->push_back ( FieldData ( value.c_str ( ), Position::Right ) );
         //
         ultotal += ( i.price * double ( i.qty ) );
         p->Data->push_back ( FieldData ( L"", Print::NextLine ) );
         p->Data->push_back ( FieldData ( L"", Print::NextLine ) );
      }
      //
      std::wstring total = std::wstring ( L"", GetPaperWidth ( ) );
      swprintf ( const_cast< wchar_t * >( total.c_str ( ) ),
                 total.length ( ),
                 L"R$ %.2f\0",
                 ultotal );
      p->Data->push_back ( FieldData ( *replicate ( '-', GetPaperWidth ( ) ), Position::Left ) );
      p->Data->push_back ( FieldData ( L"", Print::NextLine ) );
      p->Data->push_back ( FieldData ( L"", Character::EnableBold ) );
      p->Data->push_back ( FieldData ( L"TOTAIS", Position::Left ) );
      p->Data->push_back ( FieldData ( L"", Character::DisableBold ) );
      p->Data->push_back ( FieldData ( L"", Print::NextLine ) );
      //
      p->Data->push_back ( FieldData ( L"SUB-TOTAL", Position::Left ) );
      p->Data->push_back ( FieldData ( total.c_str ( ), Position::Right ) );
      p->Data->push_back ( FieldData ( L"", Print::NextLine ) );
      //service tax
      if ( m_data->increment > 0 )
      {
         p->Data->push_back ( FieldData ( L"TAXA DE SERVIÇO", Position::Left ) );
         p->Data->push_back ( FieldData ( std::to_wstring ( m_data->increment ), Position::Right ) );
         p->Data->push_back ( FieldData ( L"", Print::NextLine ) );
      }
      //discount
      if ( m_data->discount > 0 )
      {
         p->Data->push_back ( FieldData ( L"DESCONTO", Position::Left ) );
         p->Data->push_back ( FieldData ( std::to_wstring ( m_data->discount ), Position::Right ) );
         p->Data->push_back ( FieldData ( L"", Print::NextLine ) );
      }
      //
      p->Data->push_back ( FieldData ( L"TOTAL", Position::Left ) );
      p->Data->push_back ( FieldData ( L"", Character::EnableBold ) );
      p->Data->push_back ( FieldData ( total.c_str ( ), Position::Right ) );
      p->Data->push_back ( FieldData ( L"", Character::DisableBold ) );
      p->Data->push_back ( FieldData ( L"", Print::NextLine ) );
      p->Data->push_back ( FieldData ( *replicate ( '-', GetPaperWidth ( ) ), Position::Left ) );
   };
   //
   auto Payments = [&] ( )->void
   {
      double ulpaid = 0;
      double ulreceived = 0;

      p->Data->push_back ( FieldData ( L"", Print::NextLine ) );
      p->Data->push_back ( FieldData ( L"", Character::EnableBold ) );
      p->Data->push_back ( FieldData ( L"PAGAMENTOS", Position::Left ) );
      p->Data->push_back ( FieldData ( L"", Character::DisableBold ) );
      p->Data->push_back ( FieldData ( L"", Print::NextLine ) );
      //
      for ( auto && i : m_data->m_payments_queue )
      {
         auto r = [&] ( )->std::pair<int, std::wstring> const
         {
            for ( auto && t : m_pay )
            {
               if ( t.first == i.installment )
                  return t;
            }
            return std::pair<int, std::wstring> ( 0, L"" );
         }( );
         //
         ulpaid += i.value_paid;
         ulreceived += i.value_received;
         //
         std::wstring value_paid = std::wstring ( L"", GetPaperWidth ( ) );
         swprintf ( const_cast< wchar_t * >( value_paid.c_str ( ) ),
                    value_paid.length ( ),
                    L"R$ %.2f\0",
                    i.value_paid );
         std::wstring moneytype = this->Upper ( r.second.c_str ( ) )->c_str ( );

         p->Data->push_back ( FieldData ( moneytype.c_str(), Position::Left ) );
         p->Data->push_back ( FieldData ( value_paid.c_str ( ), Position::Right ) );
         p->Data->push_back ( FieldData ( L"", Print::NextLine ) );
      }
      //
      std::wstring paid = std::wstring ( L"", GetPaperWidth ( ) );
      std::wstring troc = std::wstring ( L"", GetPaperWidth ( ) );
      //
      swprintf ( const_cast< wchar_t * >( paid.c_str ( ) ),
                 paid.length ( ),
                 L"R$ %.2f\0",
                 ulpaid );
      swprintf ( const_cast< wchar_t * >( troc.c_str ( ) ),
                 troc.length ( ),
                 L"R$ %.2f\0",
                 double ( ulreceived - ulpaid ) );
      //
      p->Data->push_back ( FieldData ( L"TOTAL PAGO", Position::Left ) );
      p->Data->push_back ( FieldData ( paid.c_str ( ), Position::Right ) );
      p->Data->push_back ( FieldData ( L"", Print::NextLine ) );
      p->Data->push_back ( FieldData ( L"TROCO", Position::Left ) );
      p->Data->push_back ( FieldData ( L"", Character::EnableBold ) );
      p->Data->push_back ( FieldData ( troc.c_str ( ), Position::Right ) );
      p->Data->push_back ( FieldData ( L"", Character::DisableBold ) );
      p->Data->push_back ( FieldData ( L"", Print::NextLine ) );
      //
      if ( m_data->promotional_message.size ( ) > 0 )
      {
         p->Data->push_back ( FieldData ( *replicate ( '-', GetPaperWidth ( ) ), Position::Left ) );
         p->Data->push_back ( FieldData ( L"", Print::NextLine ) );
         p->Data->push_back ( FieldData ( m_data->promotional_message, Position::Left ) );
         p->Data->push_back ( FieldData ( L"", Print::NextLine ) );
      }
      //
      p->Data->push_back ( FieldData ( *replicate ( '-', GetPaperWidth ( ) ), Position::Left ) );
      p->Data->push_back ( FieldData ( L"", Print::NextLine ) );
      p->Data->push_back ( FieldData ( L"www.pingobox.com.br", Position::Center ) );
      p->Data->push_back ( FieldData ( L"", Print::NextLine ) );
      p->Data->push_back ( FieldData ( L"", Print::NextLine ) );
   };
   //
   Caption ( );
   DateTime ( );
   Owner ( );
   CouponTitle ( );
   Order ( );
   ProductQueue ( );
   Payments ( );
   //add on queue
   m_queue->push_back ( p );
}
//-----------------------------------------------------------------------------------------------//
void GenerateTicketDataEx::ProductList ( )
{
   LPITicketDataField p = LPITicketDataField ( new ITicketDataField );
   //title
   auto Caption = [&] ( )->void
   {
      p->ulFlags = DataType::Text;
      p->Data->push_back ( FieldData ( L"", Print::NextLine ) );
      p->Data->push_back ( FieldData ( L"", Character::EnableEmphasized ) );
      p->Data->push_back ( FieldData ( m_data->company_name, Position::Center ) );
      p->Data->push_back ( FieldData ( L"", Character::DisableEmphasized ) );
      p->Data->push_back ( FieldData ( L"", Print::NextLine ) );
      p->Data->push_back ( FieldData ( *replicate ( '-', GetPaperWidth ( ) ), Position::Left ) );
   };
   //date time
   auto DateTime = [&] ( )->void
   {
      p->Data->push_back ( FieldData ( L"", Print::NextLine ) );
      p->Data->push_back ( FieldData ( GetDateTime ( )->c_str ( ), Position::Center ) );
      p->Data->push_back ( FieldData ( L"", Print::NextLine ) );
      p->Data->push_back ( FieldData ( *replicate ( '-', GetPaperWidth ( ) ), Position::Left ) );
   };
   //coupon title
   auto CouponTitle = [&] ( )->void
   {
      p->Data->push_back ( FieldData ( L"", Print::NextLine ) );
      p->Data->push_back ( FieldData ( L"", Character::EnableEmphasized ) );
      p->Data->push_back ( FieldData ( L"LISTA DE PRODUTOS", Position::Center ) );
      p->Data->push_back ( FieldData ( L"", Character::DisableEmphasized ) );
      p->Data->push_back ( FieldData ( L"", Print::NextLine ) );
      p->Data->push_back ( FieldData ( *replicate ( '-', GetPaperWidth ( ) ), Position::Left ) );
   };
   //order and request order
   auto Order = [&] ( )->void
   {
      p->Data->push_back ( FieldData ( L"", Print::NextLine ) );
      p->Data->push_back ( FieldData ( L"", Character::EnableBold ) );
      p->Data->push_back ( FieldData ( L"Pedido:", Position::Center ) );
      p->Data->push_back ( FieldData ( L"", Character::DisableBold ) );
      p->Data->push_back ( FieldData ( L"", Print::NextLine ) );
      p->Data->push_back ( FieldData ( L"Número do pedido:", Position::Center ) );
      p->Data->push_back ( FieldData ( L"", Print::NextLine ) );
      p->Data->push_back ( FieldData ( *replicate ( '-', GetPaperWidth ( ) ), Position::Left ) );
   };
   //product queue
   auto ProductQueue = [&] ( )->void
   {
      p->Data->push_back ( FieldData ( L"", Print::NextLine ) );

      for ( auto && i : m_data->m_products_queue )
      {
         p->Data->push_back ( FieldData ( i.name, Position::Left ) );
         p->Data->push_back ( FieldData ( L"", Print::NextLine ) );
         //
         std::wstring price = std::wstring ( L"", GetPaperWidth ( ) );
         swprintf ( const_cast< wchar_t * >( price.c_str ( ) ),
                    price.length ( ),
                    L"R$ %.2f\0",
                    double ( i.price * double ( i.qty ) ) );
         p->Data->push_back ( FieldData ( price.c_str ( ), Position::Left ) );
         p->Data->push_back ( FieldData ( std::to_wstring ( i.qty ), Position::Center ) );
         //
         std::wstring value = std::wstring ( L"", GetPaperWidth ( ) );
         swprintf ( const_cast< wchar_t * >( value.c_str ( ) ),
                    value.length ( ),
                    L"R$ %.2f\0",
                    i.price );
         p->Data->push_back ( FieldData ( value.c_str ( ), Position::Right ) );
         p->Data->push_back ( FieldData ( L"", Print::NextLine ) );
         p->Data->push_back ( FieldData ( L"", Print::NextLine ) );
      }
   };
   //
   auto Comments = [&] ( )->void
   {
      p->Data->push_back ( FieldData ( *replicate ( '-', GetPaperWidth ( ) ), Position::Left ) );
      p->Data->push_back ( FieldData ( L"", Print::NextLine ) );
      p->Data->push_back ( FieldData ( L"", Print::NextLine ) );
      p->Data->push_back ( FieldData ( L"www.pingobox.com.br", Position::Center ) );
      p->Data->push_back ( FieldData ( L"", Print::NextLine ) );
      p->Data->push_back ( FieldData ( L"", Print::NextLine ) );
   };
   //
   Caption ( );
   DateTime ( );
   CouponTitle ( );
   Order ( );
   ProductQueue ( );
   Comments ( );
   //add on queue
   m_queue->push_back ( p );
}
//-----------------------------------------------------------------------------------------------//
LPITicketDataFieldQueue GenerateTicketDataEx::Build ( )
{
   m_queue->clear ( );
   //
   if ( m_data->utype == pthermal::TicketType::couponpresale )
      this->CouponPreSale ( );
   else if ( m_data->utype == pthermal::TicketType::nocoupontax )
      this->NoCouponTax ( );
   else if ( m_data->utype == pthermal::TicketType::productlist )
      this->ProductList ( );
   else
      throw std::exception ( "Undefined coupon type!" );

   return m_queue;
}
//-----------------------------------------------------------------------------------------------//
std::shared_ptr<std::wstring> GenerateTicketDataEx::GetDateTime ( )
{
   SYSTEMTIME p;
   auto fmt = std::make_shared<std::wstring> ( std::wstring ( L"", GetPaperWidth ( ) ) );
   //
   GetLocalTime ( &p );
   swprintf ( const_cast< wchar_t * >( fmt->c_str ( ) ),
              fmt->length ( ),
              L"%02d/%02d/%04d - %02d:%02d:%02d\0",
              p.wDay, p.wMonth, p.wYear, p.wHour, p.wMinute, p.wSecond );
   return fmt;
}
//-----------------------------------------------------------------------------------------------//
std::shared_ptr<std::wstring> GenerateTicketDataEx::Upper ( std::wstring const & str )
{
   auto& f = std::use_facet<std::ctype<wchar_t>> ( std::locale ( ) );
   auto fmt = std::make_shared<std::wstring> ( std::wstring ( str ) );
   //
   for ( auto c = fmt->begin ( ); c != fmt->end (); c++)
      *c = f.toupper ( *c );
   //
   return fmt;
}
//-----------------------------------------------------------------------------------------------//