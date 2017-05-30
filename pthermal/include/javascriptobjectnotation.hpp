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
#pragma once
#pragma warning(disable:4275)
#pragma warning(disable:4251)
#include <pthermal.hpp>

namespace json
{
   class JavascriptObjectNotation: public pthermal::jsondata
   {
   public:
      JavascriptObjectNotation ( ) = default;
      explicit JavascriptObjectNotation ( std::wstring const & wszFileName );
      virtual ~JavascriptObjectNotation ( );
      //
      virtual bool Load ( std::wstring const & wszFileName );
      virtual bool Parse ( );
      virtual bool Process ( );
   protected:
      virtual std::vector<std::pair<std::wstring, std::wstring>>::iterator & 
         find ( std::wstring const & key, 
         std::vector<std::pair<std::wstring, std::wstring>>::iterator & it );
   protected:
      std::wstring m_wszFileName = std::wstring();
      std::shared_ptr<std::wifstream> m_fhandle;
      std::vector<std::pair<std::wstring, std::wstring>> m_stack;
   };
   typedef std::shared_ptr<json::JavascriptObjectNotation> LPJavascriptObjectNotation;
}