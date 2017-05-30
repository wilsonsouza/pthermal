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
*
*  dllmain.cpp : Defines the entry point for the DLL application.
*/
#include <stdafx.h>
HMODULE g_hModule = NULL;
//-----------------------------------------------------------------------------------------------//
BOOL APIENTRY DllMain ( HMODULE hModule,
                        DWORD  ul_reason_for_call,
                        LPVOID lpReserved  )
{
   switch (ul_reason_for_call)
   {
   case DLL_PROCESS_ATTACH:
      //DisableThreadLibraryCalls(hModule);
      g_hModule = hModule;
      break;
   case DLL_THREAD_ATTACH:
   case DLL_THREAD_DETACH:
   case DLL_PROCESS_DETACH:
      break;
   }
   return TRUE;
}
//-----------------------------------------------------------------------------------------------//
