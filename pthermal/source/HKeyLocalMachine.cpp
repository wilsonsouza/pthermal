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
#include <HKeyLocalMachine.h>
#include <HKeyLocalMachineSuccess.h>
using namespace reg;
//-------------------------------------------------------------------------------------------//
HKeyLocalMachine::HKeyLocalMachine(std::wstring const & szKeyName):IHKeyLocalMachine(szKeyName)
{
   m_szKeyName = szKeyName.c_str();
}
//-------------------------------------------------------------------------------------------//
HKeyLocalMachine::~HKeyLocalMachine()
{
   if(hKey)
      RegCloseKey(hKey);
}
//-------------------------------------------------------------------------------------------//
void HKeyLocalMachine::Access(DWORD dwAccessFlags)
{
   this->lSuccess = RegOpenKeyEx( HKEY_LOCAL_MACHINE,
                                  m_szKeyName.c_str( ),
                                  0,
                                  dwAccessFlags,
                                  &hKey );
   return;
}
