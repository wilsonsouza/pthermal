/*
*  Layer to load USB details
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
*/
#include <EnumeratorUniversalSerialBus.h>
#include <HKeyLocalMachine.h>
#include <HKeyLocalMachineSuccess.h>
#include <RegisterEnumeratorKey.h>
#include <RegisterEnumeratorValue.h>
#include <stdio.h>
using namespace reg;
//-------------------------------------------------------------------------------------------//
EnumeratorUniversalSerialBus::EnumeratorUniversalSerialBus():IEnumeratorUniversalSerialBus()
{
   m_pLocalMachine = std::shared_ptr<HKeyLocalMachine>(new HKeyLocalMachine(REG_KEY_NAME));
   m_pLocalMachine->Access();
   m_pDeviceQueue = std::shared_ptr<std::list<std::list<std::wstring>>>(
      new std::list<std::list<std::wstring>>());
   m_pKeyNames = std::shared_ptr<DebugMapper>(new DebugMapper());
}
//-------------------------------------------------------------------------------------------//
EnumeratorUniversalSerialBus::~EnumeratorUniversalSerialBus()
{
}
//-------------------------------------------------------------------------------------------//
std::shared_ptr<IRegisterEnumeratorKey> EnumeratorUniversalSerialBus::First()
{
   auto q = std::shared_ptr<RegisterEnumeratorKey>();
   //
   if(m_pLocalMachine->lSuccess == ERROR_SUCCESS)
   {
      q = std::shared_ptr<RegisterEnumeratorKey>(new RegisterEnumeratorKey(m_pLocalMachine->hKey));
      q->SetCurrentKey(REG_KEY_NAME);
      //
      q->Filter();
      q->Enumerator(L"VID");
   }
   else
   {
      throw std::exception(reg::ErrorMsg(__FUNCTION__, __LINE__, "Key not found!")->c_str());
   }
   //
   return q;
}
//-------------------------------------------------------------------------------------------//
std::shared_ptr<IRegisterEnumeratorKey> EnumeratorUniversalSerialBus::Next(
   std::shared_ptr<IRegisterEnumeratorKey> & q, std::wstring const & szNewKey)
{
   auto qs = std::shared_ptr<IRegisterEnumeratorKey>();
   auto qdev = std::shared_ptr<IRegisterEnumeratorKey>( );
   //
   if(q->size() > 0)
   {
      for(auto l = q->begin(); l != q->end(); l++)
      {
         std::wstring s = q->GetCurrentKey() + L"\\" + l.operator*().c_str();
         auto r = std::shared_ptr<HKeyLocalMachine>(new HKeyLocalMachine(s));
         r->Access();
         //
         if(r->lSuccess == ERROR_SUCCESS)
         {
            qs = std::shared_ptr<RegisterEnumeratorKey>(new RegisterEnumeratorKey(r->hKey));
            //
            qs->SetCurrentKey(s);
            qs->Filter();
            qs->Enumerator();
            //
            if(qs->size() > 0)
            {
               for(auto a = qs->begin(); a != qs->end(); a++)
               {
                  std::wstring param = a.operator*().c_str(); 
                  //
                  if(param == L"Device Parameters")
                  {
                     std::wstring dev = qs->GetCurrentKey() + L"\\" + param;
                     auto device = std::shared_ptr<HKeyLocalMachine>(new HKeyLocalMachine(dev));
                     device->Access();
                     //
                     if(device->lSuccess == ERROR_SUCCESS)
                     {
                        auto v = std::shared_ptr<RegisterEnumeratorValue>(new RegisterEnumeratorValue(*qs));
                        auto szCKey = qs->GetCurrentKey( );
                        //
                        v->Filter();
                        v->Enumerator();
                        m_pDeviceQueue->push_back(*v);
                        m_pKeyNames->operator[](szCKey.c_str()) = *v;
                        //m_pKeyNames->operator[](dev) = std::list<std::wstring>();
                        //
                        qdev = std::shared_ptr<RegisterEnumeratorKey>( new RegisterEnumeratorKey( device->hKey ) );
                        qdev->Filter( );
                        qdev->Enumerator( );
                        auto qvalue = std::shared_ptr<RegisterEnumeratorValue>( new RegisterEnumeratorValue( *qdev ) );
                        qvalue->Filter( );
                        qvalue->Enumerator( );
                        m_pDeviceQueue->push_back( *qvalue );
                        m_pKeyNames->operator[]( dev.c_str( ) ) = *qvalue;
                     }
                     else
                     {
                        throw std::exception(reg::ErrorMsg(__FUNCTION__, __LINE__, "Key not found!")->c_str());
                     }
                  }
                  else
                  {
                     std::wstring dev = qs->GetCurrentKey( ) + L"\\" + param;
                     auto device = std::shared_ptr<HKeyLocalMachine>( new HKeyLocalMachine( dev ) );
                     device->Access( );
                     //
                     if ( device->lSuccess == ERROR_SUCCESS )
                     {
                        auto v = std::shared_ptr<RegisterEnumeratorValue>( new RegisterEnumeratorValue( *qs ) );
                        auto szCKey = qs->GetCurrentKey( );
                        //
                        v->Filter( );
                        v->Enumerator( );
                        m_pDeviceQueue->push_back( *v );
                        m_pKeyNames->operator[]( szCKey.c_str( ) ) = *v;
                        //m_pKeyNames->operator[](dev) = std::list<std::wstring>();
                        //
                        qdev = std::shared_ptr<RegisterEnumeratorKey>( new RegisterEnumeratorKey( device->hKey ) );
                        qdev->Filter( );
                        qdev->Enumerator( );
                        qdev->SetCurrentKey( dev );
                        //
                        auto qvalue = std::shared_ptr<RegisterEnumeratorValue>( new RegisterEnumeratorValue( *qdev ) );
                        qvalue->Filter( );
                        qvalue->Enumerator( );
                        m_pDeviceQueue->push_back( *qvalue );
                        m_pKeyNames->operator[]( dev.c_str( ) ) = *qvalue;
                     }
                     else
                     {
                        throw std::exception( reg::ErrorMsg( __FUNCTION__, __LINE__, "Key not found!" )->c_str( ) );
                     }

                     Next(qdev, param);
                  }
               }
            }
         }
         else
         {
            break;
         }
      }
   }
   return qs;
}
//-------------------------------------------------------------------------------------------//
std::shared_ptr<std::list<std::list<std::wstring>>> EnumeratorUniversalSerialBus::GetDeviceParameters()
{
   auto f = First();
   Next(f);
   //
   auto q = m_pDeviceQueue->begin ( );
   for ( auto n = m_pKeyNames->begin ( ); n != m_pKeyNames->end ( ); n++, q++ )
   {
      if ( n->second.size ( ) == 0 )
      {
         m_pKeyNames->erase ( n );
         m_pDeviceQueue->erase ( q );
         n = m_pKeyNames->begin ( );
         q = m_pDeviceQueue->begin ( );
      }
   }
   return this->m_pDeviceQueue;
}
//-------------------------------------------------------------------------------------------//
std::shared_ptr<DebugMapper> EnumeratorUniversalSerialBus::GetKeyName()
{
   return this->m_pKeyNames;
}
//-------------------------------------------------------------------------------------------//
void EnumeratorUniversalSerialBus::Debug()
{
   auto q = m_pKeyNames;

   for(auto v = q->begin(); v != q->end(); v++)
   {
      auto n = v.operator*().second;
      //
      if(n.size() > 0 )
         wprintf( L"LastKeyName %s\n", v.operator*( ).first.c_str( ) );

      for(auto s = n.begin(); s != n.end(); s++)
      {
         wprintf(L"Properties: %s\n", s.operator*().c_str());
      }
   }
}
