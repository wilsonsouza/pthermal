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
#include <PThermalFrameWindow.h>
#include <PThermalApp.h>
using namespace pthermal;
//-----------------------------------------------------------------------------------------------//
PThermalFrameWindow::PThermalFrameWindow ( QWidget * pOwner,
                                           std::Application * pApplication,
                                           std::unicodestring const strName,
                                           Qt::WindowFlags wfflags ) :
                                           std::MDIFrameWindow ( pOwner, pApplication, strName, wfflags )
{

}
//-----------------------------------------------------------------------------------------------//
PThermalFrameWindow::~PThermalFrameWindow ( )
{ }
//-----------------------------------------------------------------------------------------------//
bool const __fastcall PThermalFrameWindow::OnCreate ( QWidget * pWnd )
{
   std::shared_ptr<std::Popup> const & pp = *this;
   /* mrnu mount */
   pp->operator+( new std::FileItems ( ) ).
      operator+( new std::EditItems ( ) ).
      operator+( new std::UserItems ( ) ).
      operator+( new std::StyleItems ( ) ).
      operator+( new std::WindowItems ( ) ).
      operator+( new std::HelpWRDeveloperInfoItems ( ) );

   /**/
   CreateMenuBar ( *pp, new std::Action ( this ) );
   CreateToolBar ( *pp );
   statusBar ( );
   /**/
   QStringList strListMenuItems = QStringList ( )
      << std::Captions::File::NEW
      << std::Captions::File::OPEN
      << std::Captions::Tools::PREFERENCES;
   EnableMenuItem ( strListMenuItems, true );
   /**/
   operator[]( std::Captions::File::NEW )->setShortcut ( QKeySequence ( "Alt+N" ) );
   operator[]( std::Captions::File::OPEN )->setShortcut ( QKeySequence ( "Alt+O" ) );
   /**/
   return std::MDIFrameWindow::OnCreate ( pWnd );
}
//-----------------------------------------------------------------------------------------------//
bool const __fastcall PThermalFrameWindow::OnClose ( QWidget * pWnd )
{
   return std::MDIFrameWindow::OnClose ( pWnd );
}
//-----------------------------------------------------------------------------------------------//
bool const __fastcall PThermalFrameWindow::OnActivate ( QWidget * pWnd )
{
   return true;
}
//-----------------------------------------------------------------------------------------------//
bool const __fastcall PThermalFrameWindow::OnDeactivate ( QWidget * pWnd )
{
   return true;
}
//-----------------------------------------------------------------------------------------------//
bool const __fastcall PThermalFrameWindow::OnShowWindow ( QWidget * pWnd, unsigned int const & uMode )
{
   return true;
}
//-----------------------------------------------------------------------------------------------//
bool const __fastcall PThermalFrameWindow::OnCommand ( bool const & bChecked, std::Action * pAction )
{
   std::unicodestring szName = pAction->objectName ( );
   //
   if ( szName == std::Captions::File::EXIT )
   {
      this->MainApplication->exit ( );
   }
   return std::MDIFrameWindow::OnCommand ( bChecked, pAction );
}
