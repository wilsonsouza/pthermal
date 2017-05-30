/*
*  Layer to Thermal Printer via WinSpool
*
*  Build by wilson.souza
*  Default language used c++11
*
*  Copyright (c) 2016 PingoBox Software
*  Copyright (c) 2016 WR DevInfo
*
*  Created jun 15 2016
*  Last updated
*  Version 0.1-beta
*/
#pragma once
#pragma warning(disable:4275)
#pragma warning(disable:4251)

#include <IDeviceContext.h>

namespace device
{
   class PTHERMAL_API IDeviceContextEx : public IDeviceContext
   {
   public:
      explicit IDeviceContextEx ( pthermal::LPIUniversalSerialBusData const & pBus );
      virtual ~IDeviceContextEx ( ) { }
      virtual bool Create ( pthermal::LPJsonData const & pData );
      virtual bool PreparePage ( );
   protected:
      pthermal::LPJsonData m_json;
   };
   typedef std::shared_ptr<IDeviceContextEx> LPIDeviceContextEx;
}