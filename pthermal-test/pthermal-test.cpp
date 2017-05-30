/*
*  Application test
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
#pragma comment(lib, "pthermal.lib")
#include <printcoupon.h>
#include <ostream>
#include <iostream>
#include <ThermalPrinter.h>

int main(int argc, wchar_t * argv[])
{
   try
   {
      std::wstring szXMLPath = L"/projects/pthermal/xml-files/";
      std::wstring szJSONPath = L"/projects/pthermal/json-files/";
#if defined(_TESTER)
      //xps driver
      auto dwDriver = pthermal::printer::mode::WindowsSpool |
         pthermal::printer::mode::UseXPSPrinter;
      pthermal::LPThermalPrinter p ( new pthermal::ThermalPrinter );
      p->Build(szXMLPath + L"ped120820161823.xml", dwDriver ); 
      p->Build ( szXMLPath + L"270220160937.xml", dwDriver );
      p->Build ( szJSONPath + L"json_exemplo.json", dwDriver, pthermal::CouponType::couponpresale );
      p->Build ( szJSONPath + L"json_exemplo2.json", dwDriver, pthermal::CouponType::nocoupontax );
      p->Build ( szJSONPath + L"json_exemplo3.json", dwDriver, pthermal::CouponType::productlist );
#else
      PrintCoupon ( szXMLPath + L"ped120820161823.xml", pthermal::CouponType::coupon );
      PrintCoupon ( szJSONPath + L"json_exemplo.json", pthermal::CouponType::couponpresale );
      PrintCoupon ( szJSONPath + L"json_exemplo2.json", pthermal::CouponType::nocoupontax );
      PrintCoupon ( szJSONPath + L"json_exemplo3.json", pthermal::CouponType::productlist );
#endif
   }
   catch ( std::exception const & e )
   {
      std::cout << e.what ( ) << std::endl;
   }
	return 0;
}

