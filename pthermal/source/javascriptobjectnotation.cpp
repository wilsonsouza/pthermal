/*
*  Layer to JSON decode
*
*  Build by wilson.souza
*  Default language used c++11
*
*  Copyright (c) 2016 PingoBox Software
*  Copyright (c) 2016 WR DevInfo
*
*  Created august 4 2016
*  Last updated
*  Version 0.1-beta
*/
#include <javascriptobjectnotation.hpp>
#include <fstream>
#include <IWcharToChar.h>

namespace std
{
   //-----------------------------------------------------------------------------------------------//
   static std::wstring const & trim ( std::wstring & out )
   {
      std::wstring data = out.c_str ( );
      int n = 0;
      //
      out.clear ( );
      out.reserve ( data.length ( ) );
      //
      for ( auto && s : data )
      {
         if ( s == 0x20 )
            n++;
         else
         {
            out += &data[n];
            break;
         }
      }
      return out;
   }
   //-----------------------------------------------------------------------------------------------//
}
namespace json
{
   //-----------------------------------------------------------------------------------------------//
   JavascriptObjectNotation::JavascriptObjectNotation ( std::wstring const & wszFileName ) :
      pthermal::jsondata ( )
   {
      this->m_wszFileName = wszFileName.c_str ( );
      this->m_fhandle = std::make_shared<std::wifstream> ( std::wifstream ( wszFileName ) );
   }
   //-----------------------------------------------------------------------------------------------//
   JavascriptObjectNotation::~JavascriptObjectNotation ( )
   {
      if ( m_fhandle->is_open ( ) )
      {
         m_fhandle->close ( );
      }
   }
   //-----------------------------------------------------------------------------------------------//
   bool JavascriptObjectNotation::Load ( std::wstring const & wszFileName )
   {
      if ( wszFileName.size ( ) > 0 )
      {
         m_wszFileName = wszFileName;
      }
      //
      m_fhandle->close ( );
      m_fhandle->open ( m_wszFileName );

      if ( !m_fhandle->is_open ( ) )
      {
         std::string s 
         { 
            "Error to load JSON file " + 
            std::string { std::IWcharToChar ( wszFileName ) } + "!" 
         };
         throw std::exception ( s.c_str ( ) );
      }
      //
      return true;
   }
   //-----------------------------------------------------------------------------------------------//
   bool JavascriptObjectNotation::Parse ( )
   {
      std::wstring doc;
      std::vector<std::wstring> queue;
      std::wstring temp;
      //
      while ( std::getline ( *m_fhandle, doc ) )
      {
         queue.push_back ( doc.c_str() );
      }
      //clear
      doc.clear ( );
      //
      for ( auto && p : queue )
      {
         for ( auto && s : p )
         {
            switch ( s )
            {
            case 123:
               continue;
            case 125:
               continue;
            case 91:
               continue;
            case 93:
               continue;
            case 34:
               continue;
            }
            //
            temp += s;
         }
         //
         if ( !temp.empty ( ) )
         {
            auto n = temp.find ( 58 );
            //
            if ( n != std::wstring::npos )
            {
               auto first = temp.substr ( 0, n );
               auto second = temp.substr ( n + 1, temp.length ( ) - ( n + 1 ) );
               auto rs = second.substr ( 0, second.length ( ) );
               //   
               if ( second[second.length ( ) - 1] == L',' )
                  rs = second.substr ( 0, second.length ( ) - 1 );
               //
               auto pair = std::pair<std::wstring, std::wstring> ( std::trim ( first ).c_str(),
                                                                   std::trim ( rs ).c_str() );
               m_stack.push_back ( pair );
            }
            //
            temp = std::wstring ( );
         }
      }
      //
      queue.clear ( );
      //
      try
      {
         auto & it = m_stack.begin ( );
         this->company_name = find ( L"company_name", it )->second.c_str();
         this->m_customer.cnpj = find ( L"cnpj", it )->second.c_str();
         this->m_customer.cpf = find ( L"cpf", it )->second.c_str();
         this->m_customer.email = find ( L"email", it )->second.c_str();
         this->m_customer.name = find ( L"name", it )->second.c_str();
         this->discount = _wtoi ( find ( L"discount", it )->second.c_str() );
         this->discount_type = find ( L"discount_type", it )->second.c_str();
         this->increment = _wtoi ( find ( L"increment", it )->second.c_str ( ) );
         //
         if ( ( it = find ( L"payments", it ) ) == m_stack.end ( ) )
         {
            throw std::exception ( "key payments is not found!" );
         }
         //
         while (it->first != L"products" )
         {
            pthermal::jsondata::payments p;
            //
            p.installment = _wtoi ( find ( L"installment", it )->second.c_str ( ) );
            p.payment_method = find ( L"payment_method", it )->second.c_str();
            p.value_paid = _wtof ( find ( L"value_paid", it )->second.c_str ( ) );
            p.value_received = _wtof ( find(L"value_received", it)->second.c_str ( ) );
            this->m_payments_queue.push_back ( p );
            it++;
         }
         //
         if ( ( it = find ( L"products", it ) ) == m_stack.end ( ) )
         {
            throw std::exception ( "key products is not found!" );
         }
         //
         while ( it->first != L"promotional_message" )
         {
            pthermal::jsondata::products p;
            //
            p.name = find ( L"name", it )->second.c_str();
            p.price = _wtof ( find ( L"price", it )->second.c_str ( ) );
            p.qty = _wtol ( find ( L"qty", it )->second.c_str ( ) );
            //p.product_variable = find ( L"product_variable", it )->second;
            this->m_products_queue.push_back ( p );
            it++;
         }
         //
         this->promotional_message = find ( L"promotional_message", it )->second.c_str();
         this->seller_name = find ( L"seller_name", it )->second.c_str();
         this->user_name = find ( L"user_name", it )->second.c_str();
      }
      catch ( std::exception const & e )
      {
         throw e;
      }
      return true;
   }
   //-----------------------------------------------------------------------------------------------//
   bool JavascriptObjectNotation::Process ( )
   {
      Load ( std::wstring ( ) );
      Parse ( );
      return true;
   }
   //-----------------------------------------------------------------------------------------------//
   std::vector<std::pair<std::wstring, std::wstring>>::iterator &
      JavascriptObjectNotation::find ( std::wstring const & key,
      std::vector<std::pair<std::wstring, std::wstring>>::iterator & it )
   {
      std::vector<std::pair<std::wstring, std::wstring>>::iterator & p = m_stack.end ( );
      //
      for (; it != m_stack.end ( ); it++ )
      {
         if ( it->first.find ( key ) != std::wstring::npos )
         {
            return it;
         }
      }
      return p;
   }
   //-----------------------------------------------------------------------------------------------//
}