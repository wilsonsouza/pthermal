/*
*  Simple function to print coupon
*
*  Build by wilson.souza
*  Default language used c++11
*
*  Copyright (c) 2016 PingoBox Software
*  Copyright (c) 2016 WR DevInfo
*
*  Created july 01 2016
*  Last updated
*  August 10 2016
*  Version 0.1-beta
*/
#pragma once
#include <string>
//for static load
namespace pthermal
{
   enum class CouponType : unsigned
   {
      nocoupontax = 0x100,
      couponpresale,
      productlist,
      coupon
   };
}
//__cdecl turn compatible with windows standard call
//add second parameter to identify that coupon must to print
bool __declspec(dllexport) __stdcall PrintCoupon ( std::wstring const & szXMLFileName,
                                                   pthermal::CouponType const & uType = 
                                                   pthermal::CouponType::coupon );
//for dynamic load
//name to dynamic load 
//?PrintCoupon@@YG_NABV?$basic_string@_WU?$char_traits@_W@std@@V?$allocator@_W@2@@std@@@Z
typedef bool ( __stdcall *LPFNPrintCoupon )( std::wstring const & lpszXMLFileName,
                                             pthermal::CouponType const & uType );
