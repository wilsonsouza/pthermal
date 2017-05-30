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
#include <PThermalGUINew.h>
#include <qfiledialog.h>
#include <qdir.h>

using namespace pthermal;
//-----------------------------------------------------------------------------------------------//
PThermalGUINew::PThermalGUINew (QWidget * pOwner, std::unicodestring const & szName ):
std::MDIChildWindow (pOwner, szName )
{
   m_pXMLHandle = NULL;
   //initialize XML COM
   CoInitialize ( NULL );
}
//-----------------------------------------------------------------------------------------------//
PThermalGUINew::~PThermalGUINew ( )
{
}
//-----------------------------------------------------------------------------------------------//
bool const __fastcall PThermalGUINew::OnCreate ( MDIChildWindow * pWnd )
{
   try
   {
      this->AttachXMLFileName ( );
   }
   catch ( std::exception const & e )
   {
      QMessageBox::warning ( this, std::Errors::WARNING, e.what ( ) );
      Close ( );
   }
   return true;
}
//-----------------------------------------------------------------------------------------------//
bool const __fastcall PThermalGUINew::OnClose ( MDIChildWindow * pWnd )
{
   auto bSuccess = true;
   return bSuccess;
}
//-----------------------------------------------------------------------------------------------//
bool const __fastcall PThermalGUINew::OnActivate ( MDIChildWindow * pChildWnd )
{
   return true;
}
//-----------------------------------------------------------------------------------------------//
bool const __fastcall PThermalGUINew::OnDeactivate ( MDIChildWindow * pChildWnd )
{
   return true;
}
//-----------------------------------------------------------------------------------------------//
bool const __fastcall PThermalGUINew::OnShow ( MDIChildWindow * pChildWnd,
                                               unsigned int const & uMode )
{
   return true;
}
//-----------------------------------------------------------------------------------------------//
bool const __fastcall PThermalGUINew::OnContextMenuRequested ( QPoint const & p, 
                                                               MDIChildWindow * pChildWnd )
{
   return true;
}
//-----------------------------------------------------------------------------------------------//
bool const __fastcall PThermalGUINew::OnWindowStateChanged ( MDIChildWindow * pChildWnd,
                                                             Qt::WindowStates wsOld,
                                                             Qt::WindowStates wsNew )
{
   return true;
}
//-----------------------------------------------------------------------------------------------//
bool const __fastcall PThermalGUINew::OnNotify ( MDIChildWindow * pWnd, long lMsg, void const * pMsg )
{
   return true;
}
//-----------------------------------------------------------------------------------------------//
bool PThermalGUINew::AttachXMLFileName ( )
{
   std::shared_ptr<QFileDialog> pFileDlg ( new QFileDialog ( this, "Anexar XML" ) );
   //
   pFileDlg->setDirectory ( QDir::currentPath ( ) );
   pFileDlg->setNameFilter ( "XML File(s) (*.XML)" );
   pFileDlg->setViewMode ( QFileDialog::List );
   //
   if ( pFileDlg->exec ( ) == QDialog::Accepted )
   {
      connect ( pFileDlg.get ( ), &QFileDialog::selectFile, [&] ( std::unicodestring const & szSelected )
      {
         auto szFileName = szSelected.toStdWString ( );
         auto p = xml::LPExtensibleMarkupLanguage ( new xml::ExtensibleMarkupLanguage ( szFileName ) );
         m_pXMLHandle = p;
      } );
   }
   return true;
}
//-----------------------------------------------------------------------------------------------//