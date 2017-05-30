/*
*  Layer to Generate Ticket
*
*  Build by wilson.souza
*  Default language used c++11
*
*  Copyright (c) 2016 PingoBox Software
*  Copyright (c) 2016 WR DevInfo
*
*  Created jun 15 2016
*  Last updated
*  August 11 2016
*  Version 0.1-beta
*/
#include <GenerateTicketData.hpp>
#include <IWcharToChar.h>
#include <escpos.hpp>
using namespace escpos;
using namespace pthermal;
//-----------------------------------------------------------------------------------------------//
pthermal::GenerateTicketData::GenerateTicketData ( pthermal::LPXMLCFeData const & pcfe,
                                                   pthermal::LPIUniversalSerialBusData const & pBus ) :
                                                   IGenerateTicketData ( pcfe, pBus )
{
   if ( m_pBus )
   {
      m_ulPrinterID = m_pBus->m_userial.ulPrinterID;
   }
   //
   m_nPageWidth = 0x30;
   m_pay = std::vector<std::pair<int, std::wstring>>
   {
      std::pair<unsigned, std::wstring> ( 1, L"Dinheiro" ),
      std::pair<unsigned, std::wstring> ( 2, L"Cheque" ),
      std::pair<unsigned, std::wstring> ( 3, L"Crédito" ),
      std::pair<unsigned, std::wstring> ( 4, L"Debito" ),
      std::pair<unsigned, std::wstring> ( 99, L"Outros" )
   };
}
//-----------------------------------------------------------------------------------------------//
pthermal::LPITicketDataField pthermal::GenerateTicketData::GetHeader ( )
{
   LPITicketDataField p = std::make_shared<ITicketDataField> ( this->Header );
   //
   std::wstring szName = m_pcfe->m_infcfe.m_emit.xNome;
   //
   if ( szName.size ( ) > GetPaperWidth ( ) )
      szName = szName.substr ( 0, GetPaperWidth ( ) - 1 );
   //
   p->ulFlags = DataType::Text | FieldId::Header;
   p->Data->push_back ( FieldData ( *replicate ( '-', GetPaperWidth ( ) ), Position::Left ) );
   p->Data->push_back ( FieldData ( L"", Character::SelectCharacterFontA ) );
   p->Data->push_back ( FieldData ( L"", Character::EnableEmphasized ) );
   p->Data->push_back ( FieldData ( L"", Print::NextLine ) );
   p->Data->push_back ( FieldData ( szName, Position::Center ) );
   p->Data->push_back ( FieldData ( L"", Print::NextLine ) );
   p->Data->push_back ( FieldData ( L"", Character::SelectCharacterFontA ) );
   p->Data->push_back ( FieldData ( L"", Character::DisableEmphasized ) );
   p->Data->push_back ( FieldData ( m_pcfe->m_infcfe.m_emit.CNPJ, Position::Center ) );
   p->Data->push_back ( FieldData ( L"", Print::NextLine ) );
   p->Data->push_back ( FieldData ( m_pcfe->m_infcfe.m_emit.m_enderemit.xLgr + L" - " +
      m_pcfe->m_infcfe.m_emit.m_enderemit.nro, Position::Center ) );
   p->Data->push_back ( FieldData ( L"", Print::NextLine ) );
   p->Data->push_back ( FieldData ( m_pcfe->m_infcfe.m_emit.m_enderemit.xBairro + L" " +
      m_pcfe->m_infcfe.m_emit.m_enderemit.xMun + L" ", Position::Center ) );
   return p;
}
//-----------------------------------------------------------------------------------------------//
pthermal::LPITicketDataField pthermal::GenerateTicketData::GetPDV ( )
{
   LPITicketDataField p = std::make_shared<ITicketDataField> ( this->PDV );
   //
   std::wstring szDate = m_pcfe->m_infcfe.m_ide.dEmi;
   std::wstring szTime = m_pcfe->m_infcfe.m_ide.hEmi;
   //
   p->ulFlags = DataType::Text | FieldId::PDV;
   //
   std::wstring szDataR = std::wstring ( L"", GetPaperWidth ( ) );
   //
   if ( !szDate.empty ( ) )
   {
      szDataR = szDate.substr ( 6, 2 ) + L"/";
      szDataR += szDate.substr ( 4, 2 ) + L"/";
      szDataR += szDate.substr ( 0, 4 ) + L" ";
      szDataR += szTime.substr ( 4, 2 ) + L":" + szTime.substr ( 2, 2 );
   }
   else
   {
      SYSTEMTIME st;
      //
      GetLocalTime ( &st );
      swprintf ( const_cast< wchar_t * >( szDataR.c_str ( ) ),
                 szDataR.length (),
                 L"%02d/%02d/%04d %02d:%02d:%02d\0",
                 st.wDay, st.wMonth, st.wYear, st.wHour, st.wMinute, st.wSecond );
   }
   //
   p->Data->push_back ( FieldData ( L"", Print::NextLine ) );
   p->Data->push_back ( FieldData ( szDataR.c_str(), Position::Center ) );
   p->Data->push_back ( FieldData ( L"", Print::NextLine ) );
   p->Data->push_back ( FieldData ( *replicate ( '-', GetPaperWidth ( ) ), Position::Left ) );
   return p;
}
//-----------------------------------------------------------------------------------------------//
pthermal::LPITicketDataField pthermal::GenerateTicketData::GetExtract ( )
{
   LPITicketDataField p = std::make_shared<ITicketDataField> ( this->Extract );
   //
   p->ulFlags = DataType::Text | FieldId::Extract;
   p->Data->push_back ( FieldData ( L"", Character::SelectCharacterFontA ) );
   p->Data->push_back ( FieldData ( L"", Character::EnableEmphasized ) );
   p->Data->push_back ( FieldData ( L"", Print::NextLine ) );
   p->Data->push_back ( FieldData ( L" Extrato N " + m_pcfe->m_infcfe.m_ide.cNF, Position::Center ) );
   p->Data->push_back ( FieldData ( L"", Print::NextLine ) );
   p->Data->push_back ( FieldData ( L"", Character::SelectCharacterFontA ) );
   p->Data->push_back ( FieldData ( L"", Character::DisableEmphasized ) );
   p->Data->push_back ( FieldData ( *replicate ( '-', GetPaperWidth ( ) ), Position::Left ) );
   //
   return p;
}
//-----------------------------------------------------------------------------------------------//
pthermal::LPITicketDataField pthermal::GenerateTicketData::GetListOfProductHeader ( )
{
   LPITicketDataField p = std::make_shared<ITicketDataField> ( this->ListOfProductHeader );
   //
   p->ulFlags = DataType::Text | FieldId::ListOfProductHeader;
   std::wstring szComp = std::wstring ( L"", GetPaperWidth ( ) );
   std::wstring szCF = std::wstring ( L"", GetPaperWidth ( ) );
   //
   swprintf ( const_cast< wchar_t * >( szCF.c_str ( ) ),
              szCF.length (),
              L"%-6s %-13s\0",
              L"#", L"DESCRIÇÃO" );
   swprintf ( const_cast<wchar_t *>( szComp.c_str ( ) ),
              szComp.length (),
              L"%-7s %-3s %-7s %-2s %-6s %-7s\0",
              L"QNT", L"UN", L"VLR.UN", L"ST", L"ALIQ", L"VLR TOT" );
   //
   p->Data->push_back ( FieldData ( L"", Print::NextLine ) );
   p->Data->push_back ( FieldData ( szCF.c_str(), Position::Left ) );
   p->Data->push_back ( FieldData ( szComp.c_str ( ), Position::Right ) );
   p->Data->push_back ( FieldData ( L"", Print::NextLine ) );
   p->Data->push_back ( FieldData ( *replicate ( '-', GetPaperWidth ( ) ), Position::Left ) );
   return p;
}
//-----------------------------------------------------------------------------------------------//
pthermal::LPITicketDataField pthermal::GenerateTicketData::GetProductItems ( )
{
   unsigned uItems = m_pcfe->m_infcfe.m_det.nItem;
   LPITicketDataField p = std::make_shared<ITicketDataField> ( this->ProductItems );
   std::vector<pthermal::CFe::infCFe::det::prod> items = m_pcfe->m_infcfe.m_det.m_ItemsList;
   int nCount = 1;
   //
   //fill array with space
   p->ulFlags = DataType::Text | FieldId::ProductItems;
   p->Data->push_back ( FieldData ( L"", Print::NextLine ) );
   //
   for ( auto i = items.begin ( ); i != items.end ( ); i++, nCount++ )
   {
      auto qtd = i->qCom;
      auto uni = i->uCom;
      auto vlr = i->vUnCom;
      auto st = i->indRegra;
      auto vg = i->vProd;
      //
      std::wstring szT = std::wstring ( L"", GetPaperWidth ( ) );
      swprintf ( const_cast<wchar_t *>( szT.c_str ( ) ), 
                 szT.length (),
                 L"%-6d %-40s\0", 
                 nCount, i->xProd.c_str ( ) );
      //
      p->Data->push_back ( FieldData ( szT.c_str(), Position::Left ) );
      p->Data->push_back ( FieldData ( L"", Print::NextLine ) );
      //
      std::wstring szRow = std::wstring ( L"", GetPaperWidth ( ) );
      auto imp = m_pcfe->m_infcfe.m_det.m_imposto.vItem12741;
      auto alig_vlr = std::atol ( std::WideChar ( vlr ).c_str ( ) );
      auto alig_imp = std::atol ( std::WideChar ( imp ).c_str ( ) );
      auto alig_per = 0L;
      //calcule tributtes value
      if ( alig_imp > 0 && alig_vlr > 0 )
      {
         alig_per = ( alig_vlr / alig_imp );
      }
      //
      //%7s %3s %7s %2s %6s %8s
      szT = std::to_wstring ( alig_per ).c_str ( );
      swprintf ( const_cast<wchar_t *>( szRow.c_str ( ) ),
                 szRow.length (),
                 L"%-7s %-3s %-7s %-2s %-6s %12s\0",
                 qtd.c_str ( ),
                 uni.c_str ( ),
                 vlr.c_str ( ),
                 st.c_str ( ),
                 szT.c_str(),
                 vg.c_str ( ) );
      //
      p->Data->push_back ( FieldData ( szRow.c_str(), Position::Right ) );
      p->Data->push_back ( FieldData ( L"", Print::NextLine ) );
   }
   return p;
}
//-----------------------------------------------------------------------------------------------//
pthermal::LPITicketDataField pthermal::GenerateTicketData::GetTotal ( )
{
   LPITicketDataField p = std::make_shared<ITicketDataField> ( this->Total );
   //
   p->ulFlags = DataType::Text | FieldId::Total;
   std::wstring szTax = L"Taxa de serviço";
   //
   p->Data->push_back ( FieldData ( L"", Print::NextLine ) );
   p->Data->push_back ( FieldData ( szTax.c_str(), Position::Left ) );
   p->Data->push_back ( FieldData ( L"0.00", Position::Right ) );
   p->Data->push_back ( FieldData ( L"", Print::NextLine ) );
   //
   std::wstring szSub = L"Subtotal";
   p->Data->push_back ( FieldData ( szSub.c_str(), Position::Left ) );
   p->Data->push_back ( FieldData ( m_pcfe->m_infcfe.m_total.vCFe, Position::Right ) );
   p->Data->push_back ( FieldData ( L"", Print::NextLine ) );
   //
   std::wstring szDesc = L"Desconto";
   p->Data->push_back ( FieldData ( szDesc.c_str(), Position::Left ) );
   p->Data->push_back ( FieldData ( m_pcfe->m_infcfe.m_total.m_icmstot.vDesc, Position::Right ) );
   p->Data->push_back ( FieldData ( L"", Print::NextLine ) );

   p->Data->push_back ( FieldData ( *replicate ( '-', GetPaperWidth ( ) ), Position::Left ) );
   p->Data->push_back ( FieldData ( L"", Print::NextLine ) );
   //
   p->Data->push_back ( FieldData ( L"", Character::SelectCharacterFontA ) );
   p->Data->push_back ( FieldData ( L"", Character::EnableEmphasized ) );
   //
   std::wstring szTotal = L"TOTAL";
   p->Data->push_back ( FieldData ( szTotal.c_str(), Position::Left ) );
   p->Data->push_back ( FieldData ( m_pcfe->m_infcfe.m_total.vCFe, Position::Right ) );
   p->Data->push_back ( FieldData ( L"", Print::NextLine ) );
   p->Data->push_back ( FieldData ( L"", Character::SelectCharacterFontA ) );
   p->Data->push_back ( FieldData ( L"", Character::DisableEmphasized ) );
   p->Data->push_back ( FieldData ( *replicate ( '-', GetPaperWidth ( ) ), Position::Left ) );
   //
   return p;
}
//-----------------------------------------------------------------------------------------------//
pthermal::LPITicketDataField pthermal::GenerateTicketData::GetTypePayment ( )
{
   LPITicketDataField p = std::make_shared<ITicketDataField> ( this->TypePayment );
   //
   p->ulFlags = DataType::Text | FieldId::TypePayment;
   //
   for ( auto && mp: m_pcfe->m_infcfe.m_pgto.m_paytypes )
   {
      for ( auto && tp: m_pay )
      {
         if ( tp.first == unsigned ( std::stoi ( mp.cMP ) ) )
         {
            p->Data->push_back ( FieldData ( L"", Print::NextLine ) );
            p->Data->push_back ( FieldData ( tp.second.c_str(), Position::Left ) );
            p->Data->push_back ( FieldData ( mp.vMP, Position::Right ) );
         }
      }
   }
   //
   std::wstring szPay = L"TROCO";
   p->Data->push_back ( FieldData ( L"", Print::NextLine ) );
   p->Data->push_back ( FieldData ( szPay.c_str(), Position::Left ) );
   p->Data->push_back ( FieldData ( m_pcfe->m_infcfe.m_pgto.vTroco.c_str(), Position::Right ) );
   p->Data->push_back ( FieldData ( L"", Print::NextLine ) );
   p->Data->push_back ( FieldData ( *replicate ( 0x20, GetPaperWidth ( ) ), Position::Left ) );
   return p;
}
//-----------------------------------------------------------------------------------------------//
pthermal::LPITicketDataField pthermal::GenerateTicketData::GetCustomer ( )
{
   LPITicketDataField p = std::make_shared<ITicketDataField> ( this->Customer );
   //
   p->ulFlags = DataType::Text | FieldId::Customer;
   //
   p->Data->push_back ( FieldData ( L"", Print::NextLine ) );
   p->Data->push_back ( FieldData ( *replicate ( '-', GetPaperWidth ( ) ), Position::Left ) );
   p->Data->push_back ( FieldData ( L"", Print::NextLine ) );
   p->Data->push_back ( FieldData ( L"", Character::EnableBold ) );
   p->Data->push_back ( FieldData ( L"Dados do Cliente", Position::Center ) );
   p->Data->push_back ( FieldData ( L"", Print::NextLine ) );
   p->Data->push_back ( FieldData ( L"", Character::DisableBold ) );
   p->Data->push_back ( FieldData ( *replicate ( '-', GetPaperWidth ( ) ), Position::Left ) );
   //
   p->Data->push_back ( FieldData ( L"", Print::NextLine ) );
   p->Data->push_back ( FieldData ( L"Consumidor:", Position::Left ) );
   p->Data->push_back ( FieldData ( L"", Print::NextLine ) );
   p->Data->push_back ( FieldData ( m_pcfe->m_infcfe.m_dest.xNome + L", " +
      m_pcfe->m_infcfe.m_dest.cpf, Position::Left ) );
   p->Data->push_back ( FieldData ( L"", Print::NextLine ) );
   //
   p->Data->push_back ( FieldData ( m_pcfe->m_infcfe.m_entrega.xLgr + L", " + 
      m_pcfe->m_infcfe.m_entrega.nro , Position::Left ) );
   p->Data->push_back ( FieldData ( L"", Print::NextLine ) );
   p->Data->push_back ( FieldData ( m_pcfe->m_infcfe.m_entrega.xBairro + L", " +
      m_pcfe->m_infcfe.m_entrega.xMun, Position::Left ) );

   return p;
}
//-----------------------------------------------------------------------------------------------//
pthermal::LPITicketDataField pthermal::GenerateTicketData::GetCustomerDescription ( )
{
   LPITicketDataField p = std::make_shared<ITicketDataField> ( this->CustomerDescription );
   //
   p->ulFlags = DataType::Text | FieldId::CustomerDescription;
   //
   p->Data->push_back ( FieldData ( L"", Print::NextLine ) );
   p->Data->push_back ( FieldData ( *replicate ( '-', GetPaperWidth ( ) ), Position::Left ) );
   p->Data->push_back ( FieldData ( L"", Print::NextLine ) );
   p->Data->push_back ( FieldData ( L"Observações do contribuinte", Position::Center ) );
   p->Data->push_back ( FieldData ( L"", Print::NextLine ) );
   p->Data->push_back ( FieldData ( *replicate ( '-', GetPaperWidth ( ) ), Position::Left ) );
   return p;
}
//-----------------------------------------------------------------------------------------------//
pthermal::LPITicketDataField pthermal::GenerateTicketData::GetValueTributes ( )
{
   LPITicketDataField p = std::make_shared<ITicketDataField> ( this->ValueTributes );
   //
   p->ulFlags = DataType::Text | FieldId::ValueTributes;
   //
   p->Data->push_back ( FieldData ( L"", Print::NextLine ) );
   p->Data->push_back ( FieldData ( m_pcfe->m_infcfe.m_infadic.infCpl, Position::Center ) );
   return p;
}
//-----------------------------------------------------------------------------------------------//
pthermal::LPITicketDataField pthermal::GenerateTicketData::GetSatDateTime ( )
{
   LPITicketDataField p = std::make_shared<ITicketDataField> ( this->SatDateTime );
   //
   p->ulFlags = DataType::Text | FieldId::SatDateTime;
   //
   p->Data->push_back ( FieldData ( L"", Print::NextLine ) );
   p->Data->push_back ( FieldData ( *replicate ( '-', GetPaperWidth ( ) ), Position::Left ) );
   p->Data->push_back ( FieldData ( L"", Print::NextLine ) );
   p->Data->push_back ( FieldData ( L"", Character::SelectCharacterFontA ) );
   p->Data->push_back ( FieldData ( L"", Character::EnableEmphasized ) );
   p->Data->push_back ( FieldData ( L"SAT N. " + m_pcfe->m_infcfe.m_ide.nserieSAT, Position::Center ) );
   p->Data->push_back ( FieldData ( L"", Character::SelectCharacterFontA ) );
   p->Data->push_back ( FieldData ( L"", Character::DisableEmphasized ) );
   p->Data->push_back ( FieldData ( L"", Print::NextLine ) );
   p->Data->push_back ( FieldData ( *replicate ( '-', GetPaperWidth ( ) ), Position::Left ) );
   //
   std::wstring szDate = m_pcfe->m_infcfe.m_ide.dEmi;
   std::wstring szTime = m_pcfe->m_infcfe.m_ide.hEmi;
   //
   std::wstring szDataR = std::wstring ( L"", GetPaperWidth ( ) );
   //
   if ( !szDate.empty ( ) && !szTime.empty ( ) )
   {
      szDataR = szDate.substr ( 6, 2 ) + L"/";
      szDataR += szDate.substr ( 4, 2 ) + L"/";
      szDataR += szDate.substr ( 0, 4 ) + L" ";
      szDataR += szTime.substr ( 4, 2 ) + L":" + szTime.substr ( 2, 2 );
   }
   else
   {
      SYSTEMTIME st;
      //
      GetLocalTime ( &st );
      swprintf ( const_cast< wchar_t * >( szDataR.c_str ( ) ),
                 szDataR.length (),
                 L"%02d/%02d/%04d %02d:%02d:%02d\0",
                 st.wDay, st.wMonth, st.wYear, st.wHour, st.wMinute, st.wSecond );
   }
   //
   std::wstring szKeyAccess = m_pcfe->m_infcfe.Id;
   //
   if ( szKeyAccess.find ( L"CFe" ) != -1 )
   {
      szKeyAccess = m_pcfe->m_infcfe.Id.substr ( 4, szKeyAccess.length ( ) );
   }
   //
   p->Data->push_back ( FieldData ( L"", Print::NextLine ) );
   p->Data->push_back ( FieldData ( szDataR.c_str(), Position::Center ) );
   p->Data->push_back ( FieldData ( L"", Print::NextLine ) );
   p->Data->push_back ( FieldData ( L"Chave:" + szKeyAccess, Position::Center ) );
   return p;
}
//-----------------------------------------------------------------------------------------------//
pthermal::LPITicketDataField pthermal::GenerateTicketData::GetQuickResponseCode ( )
{
   LPITicketDataField p = std::make_shared<ITicketDataField> ( this->QuickResponseCode );
   auto szData = m_pcfe->m_infcfe.Id + L"|" +
      m_pcfe->m_infcfe.m_pgto.m_mp.vMP + L"|" +
      m_pcfe->m_infcfe.m_ide.assinaturaQRCODE;
   //
   p->ulFlags = DataType::QuickResponseCode | FieldId::QuickResponseCode;
   //
   p->Data->push_back ( FieldData ( L"", Print::NextLine ) );
   p->Data->push_back ( FieldData ( L"", Print::NextLine ) );
   p->Data->push_back ( FieldData ( szData.c_str(), Position::Center ) );
   //
   return p;
}
//-----------------------------------------------------------------------------------------------//
pthermal::LPITicketDataField pthermal::GenerateTicketData::GetComments ( )
{
   LPITicketDataField p = std::make_shared<ITicketDataField> ( this->Comments );
   //
   p->ulFlags = DataType::Text | FieldId::Comments;
   p->Data->push_back ( FieldData ( L"", Print::NextLine ) );
   return p;
}
//-----------------------------------------------------------------------------------------------//
pthermal::LPITicketDataFieldQueue pthermal::GenerateTicketData::Build ( )
{
   LPITicketDataFieldQueue p = LPITicketDataFieldQueue ( new std::vector<LPITicketDataField> ( ) );
   //
   p->push_back ( this->GetHeader ( ) );
   p->push_back ( this->GetPDV ( ) );
   p->push_back ( this->GetExtract ( ) );
   p->push_back ( this->GetListOfProductHeader ( ) );
   p->push_back ( this->GetProductItems ( ) );
   p->push_back ( this->GetTotal ( ) );
   p->push_back ( this->GetTypePayment ( ) );
   p->push_back ( this->GetCustomer ( ) );
   p->push_back ( this->GetCustomerDescription ( ) );
   p->push_back ( this->GetValueTributes ( ) );
   p->push_back ( this->GetSatDateTime ( ) );
   p->push_back ( this->GetQuickResponseCode ( ) );
   p->push_back ( this->GetComments ( ) );
   //
   return p;
}
//-----------------------------------------------------------------------------------------------//
pthermal::LPWideString const pthermal::GenerateTicketData::replicate ( unsigned char c, size_t uSize )
{
   std::wstring szFmt = std::wstring ( L"", uSize );
   std::fill ( szFmt.begin ( ), szFmt.end ( ), wchar_t ( c ) );
   return std::make_shared<std::wstring> ( szFmt );
}
//-----------------------------------------------------------------------------------------------//
void pthermal::GenerateTicketData::SetPaperWidth ( unsigned uPaperWidth )
{
   m_nPageWidth = uPaperWidth;
}
//-----------------------------------------------------------------------------------------------//
std::size_t pthermal::GenerateTicketData::GetPaperWidth ( )
{
   return m_nPageWidth;
}
//-----------------------------------------------------------------------------------------------//