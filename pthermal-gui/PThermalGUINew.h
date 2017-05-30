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
#include <std.mdichildwindow.hpp>
#include <ExtensibleMarkupLanguage.hpp>

namespace pthermal
{
   class PThermalGUINew :  public std::MDIChildWindow
   {
   public:
      PThermalGUINew ( QWidget * pOwner, std::unicodestring const & szName );
      virtual ~PThermalGUINew ( );
      //
      virtual bool const __fastcall OnCreate ( MDIChildWindow * pWnd );
      virtual bool const __fastcall OnClose ( MDIChildWindow * pWnd );
      virtual bool const __fastcall OnActivate ( MDIChildWindow * pChildWnd );
      virtual bool const __fastcall OnDeactivate ( MDIChildWindow * pChildWnd );
      virtual bool const __fastcall OnShow ( MDIChildWindow * pChildWnd,
                                             unsigned int const & uMode );
      virtual bool const __fastcall OnContextMenuRequested ( QPoint const & p, MDIChildWindow * pChildWnd );
      virtual bool const __fastcall OnWindowStateChanged ( MDIChildWindow * pChildWnd,
                                                           Qt::WindowStates wsOld, Qt::WindowStates wsNew );
      virtual bool const __fastcall OnNotify ( MDIChildWindow * pWnd, long lMsg, void const * pMsg );
   protected:
      virtual bool AttachXMLFileName ( );
   protected:
      xml::LPExtensibleMarkupLanguage m_pXMLHandle;
   };
};
