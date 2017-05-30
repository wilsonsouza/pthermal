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
#pragma once
#pragma warning(disable:4275)
#pragma warning(disable:4251)

#include <pthermal.hpp>
#include <string>
#include <memory>

namespace reg
{
   static std::wstring const REG_KEY_NAME = L"SYSTEM\\CurrentControlSet\\Enum\\USB";
      //L"SYSTEM\\ControlSet001\\Enum\\USB";
   //-------------------------------------------------------------------------------------------//
   enum
   {
      MAX_KEY_LENGTH =  0x00ff,
      MAX_VALUE_NAME = 0x3fff
   };
   //-------------------------------------------------------------------------------------------//
   static std::shared_ptr<std::string> ErrorMsg(std::string szFunc, long double lLine, char const * pMsg)
   {
      auto out = std::shared_ptr<std::string>(new std::string());
      //
      *out += szFunc + std::string(", line:") + std::to_string(lLine);
      *out += std::string(", ") + pMsg;
      //
      return out;
   }
   //-------------------------------------------------------------------------------------------//
   struct HKeyLocalMachineSuccess
   {
      HKEY hKey;
      DWORD lSuccess;
      //
      explicit HKeyLocalMachineSuccess( )
      {
         hKey = 0;
         lSuccess = ERROR;
      }
      explicit HKeyLocalMachineSuccess(HKEY hKey, DWORD lSuccess)
      {
         this->hKey = hKey;
         this->lSuccess = lSuccess;
      }
   };
};