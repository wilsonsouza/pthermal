/*
*  Thermal Printer GUI Application
*
*  Build by wilson.souza
*  Default language used c++11
*
*  Copyright (c) 2016 PingoBox Software
*  Copyright (c) 2016 WR DevInfo
*
*  Created jun 26 2016
*  Last updated
*  Version 0.1-beta
*/
#pragma once
#pragma warning(disable:4172) //disable returning address of local variable or temporary
#include <std.mdiframewindow.hpp>

namespace pthermal
{
   namespace caption
   {
      static std::unicodestring const Name = "Thermal Printer Design";
      static std::unicodestring const Version = "Versão 1.0.0.Alfa";
   }
   //
   class PThermalFrameWindow: public std::MDIFrameWindow
   {
      Q_OBJECT
   public:
      PThermalFrameWindow ( QWidget * pOwner,
                            std::Application * pApplication,
                            std::unicodestring const strName = std::unicodestring ( ),
                            Qt::WindowFlags wfflags = 0 );
      virtual ~PThermalFrameWindow ( );
   protected:
      virtual bool const __fastcall OnCommand ( bool const & bChecked, std::Action * pAction );
      virtual bool const __fastcall OnCreate ( QWidget * pWnd );
      virtual bool const __fastcall OnClose ( QWidget * pWnd );
      virtual bool const __fastcall OnActivate ( QWidget * pWnd );
      virtual bool const __fastcall OnDeactivate ( QWidget * pWnd );
      virtual bool const __fastcall OnShowWindow ( QWidget * pWnd, unsigned int const & uMode );

   private:
      //abstract method to sql handle
      virtual std::shared_ptr<sql::Query> & __fastcall GetSingleQueryLanguageHandle ( ) 
      {
         return std::shared_ptr<sql::Query> ( );
      }
   };
   typedef std::shared_ptr<PThermalFrameWindow> LPPThermalFrameWindow;
}

