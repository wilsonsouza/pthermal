/*
*  Layer to XML encode and decode
*
*  Build by wilson.souza
*  Default language used c++11
*
*  Copyright (c) 2016 PingoBox Software
*  Copyright (c) 2016 WR DevInfo
*
*  Created jun 1 2016
*  Last updated
*  August 16 2016 by wilson.souza
*  Version 0.1-beta
*
*  Description: access msxml dynamic library for process
*/
#include <ExtensibleMarkupLanguage.hpp>
#include <strsafe.h>
#include <IWcharToChar.h>
using namespace xml;
using namespace pthermal;
//-----------------------------------------------------------------------------------------------//
ExtensibleMarkupLanguage::ExtensibleMarkupLanguage ( std::wstring const & wszFileName ) :
ExtensibleMarkupLanguageData ( ), m_wszFileName ( wszFileName.c_str ( ) )
{
   HRESULT lSuccess = CoCreateInstance ( __uuidof( DOMDocument60 ),
                                         NULL,
                                         CLSCTX_ALL,
                                         IID_PPV_ARGS ( &m_pDoc ) );

   if ( SUCCEEDED ( lSuccess ) )
   {
      // these methods should not fail so don't inspect result
      m_pDoc->put_async ( VARIANT_FALSE );
      m_pDoc->put_validateOnParse ( VARIANT_FALSE );
      m_pDoc->put_resolveExternals ( VARIANT_FALSE );
      m_pDoc->put_preserveWhiteSpace ( VARIANT_TRUE );
   }
   else
   {
      Throw ( lSuccess );
   }
   //
   m_pElement = NULL;
   this->uPPos = 0;
}
//-----------------------------------------------------------------------------------------------//
ExtensibleMarkupLanguage::~ExtensibleMarkupLanguage ( )
{
   if ( m_pElement )
   {
      m_pElement->Release ( );
      m_pElement = NULL;
   }
   //
   if ( m_pDoc )
   {
      m_pDoc->Release ( );
      m_pDoc = NULL;
   }
   //
   CoUninitialize ( );
}
//-----------------------------------------------------------------------------------------------//
VARIANT ExtensibleMarkupLanguage::Alloc ( std::wstring const & wszData )
{
   BSTR bsOut = SysAllocString ( wszData.c_str ( ) );
   VARIANT vOut;
   //
   if ( !bsOut )
   {
      throw std::exception ( "OLESTR out of memory!" );
   }
   //
   V_VT ( &vOut ) = VT_BSTR;
   V_BSTR ( &vOut ) = bsOut;
   //
   return vOut;
}
//-----------------------------------------------------------------------------------------------//
void ExtensibleMarkupLanguage::First ( IXMLDOMNodeList * pNodeList )
{
   long lNodeCount = 0L;
   HRESULT lSuccess = S_OK;
   //
   if ( FAILED ( lSuccess = pNodeList->get_length ( &lNodeCount ) ) )
   {
      Throw ( lSuccess );
   }
   //
   for ( auto i = 0L; i < lNodeCount; i++ )
   {
      BSTR bszNodeName = BSTR ( );
      IXMLDOMNode * pNode = NULL;
      //
      if ( FAILED ( lSuccess = pNodeList->get_item ( i, &pNode ) ) )
      {
         Throw ( lSuccess );
      }
      //
      if ( FAILED ( lSuccess = pNode->get_nodeName ( &bszNodeName ) ) )
      {
         pNode->Release ( );
         Throw ( lSuccess );
      }
      //
      if ( bszNodeName == xml::WSZ_INFCFE )
      {
         IXMLDOMNamedNodeMap * pNameNodeMap = NULL;
         long lNameNodeMap = 0L;
         BSTR bszNodeName = BSTR ( );
         DOMNodeType dntNodeType;
         //
         if ( FAILED ( lSuccess = pNode->get_nodeType ( &dntNodeType ) ) )
         {
            pNode->Release ( );
            Throw ( lSuccess );
         }
         //
         if ( dntNodeType == NODE_ELEMENT )
         {
            auto bszValue = BSTR ( );
            IXMLDOMNode * pNodeKeep = NULL;
            //
            if ( FAILED ( lSuccess = pNode->get_attributes ( &pNameNodeMap ) ) )
            {
               pNode->Release ( );
               Throw ( lSuccess );
            }
            //
            std::wstring waszNames[] =
            {
               INFCFE::Id,
               INFCFE::Version,
               INFCFE::VersionDataEnt,
               INFCFE::VersionSB,
               std::wstring ( )
            };
            //
            for ( auto n = 0; waszNames[n].size ( ) > 0; n++ )
            {
               auto bszName = BSTR ( waszNames[n].c_str ( ) );
               //
               pNameNodeMap->getNamedItem ( bszName, &pNodeKeep );
               //
               if ( !pNodeKeep )
                  continue;
               //
               pNodeKeep->get_text ( &bszValue );
               //
               if ( bszName == waszNames[0] )
                  m_infcfe.Id = bszValue;
               else if ( bszName == waszNames[1] )
                  m_infcfe.Version = bszValue;
               else if ( bszName == waszNames[2] )
                  m_infcfe.VersionDataEntry = bszValue;
               else if ( bszName == waszNames[3] )
                  m_infcfe.VersionSB = bszValue;
               //
               pNodeKeep->Release ( );
            }
         }
         //free OLE object buffer
         pNameNodeMap->Release ( );
      }
      else if ( std::wstring ( bszNodeName ) == xml::WSZ_SIGNATURE )
      {
         //
         auto wszValue = GetAttr ( pNode );
         //
         if ( wszValue->length ( ) > 0 )
         {
            m_signature.xmlns = wszValue->c_str ( );
         }
      }

      //jump next DOM node list
      Next ( pNode );
      pNode->Release ( );
   }
}
//-----------------------------------------------------------------------------------------------//
std::shared_ptr<std::wstring> ExtensibleMarkupLanguage::GetAttr ( IXMLDOMNode * pNode )
{
   IXMLDOMNamedNodeMap * pNameNodeMap = NULL;
   long lCount = 0L;
   std::wstring wszOut = std::wstring ( );
   //
   pNode->get_attributes ( &pNameNodeMap );
   pNameNodeMap->get_length ( &lCount );
   //
   for ( auto i = 0L; i < lCount; i++ )
   {
      IXMLDOMNode * pNextNode = NULL;
      BSTR bszNameDet = BSTR ( );
      BSTR bszValue = BSTR ( );
      //
      pNameNodeMap->get_item ( i, &pNextNode );
      pNextNode->get_baseName ( &bszNameDet );
      pNextNode->get_text ( &bszValue );
      //
      wszOut = std::wstring ( bszValue );
      pNextNode->Release ( );
   }
   //
   pNameNodeMap->Release ( );
   return std::make_shared<std::wstring> ( wszOut );
}
//-----------------------------------------------------------------------------------------------//
void ExtensibleMarkupLanguage::Next ( IXMLDOMNode * pNode )
{
   IXMLDOMNodeList * pNodeList = NULL;
   long lNodeListCount = 0L;
   HRESULT lSuccess = S_OK;
   //
   if ( FAILED ( lSuccess = pNode->get_childNodes ( &pNodeList ) ) )
   {
      Throw ( lSuccess );
   }
   //
   if ( FAILED ( lSuccess = pNodeList->get_length ( &lNodeListCount ) ) )
   {
      pNodeList->Release ( );
      Throw ( lSuccess );
   }
   //
   for ( auto i = 0L; i < lNodeListCount; i++ )
   {
      DOMNodeType dntNodeType;
      IXMLDOMNode * pNextNode = NULL;
      BSTR bszNodeName = BSTR ( );
      VARIANT_BOOL bMore;
      //
      pNodeList->get_item ( i, &pNextNode );
      pNextNode->get_nodeType ( &dntNodeType );
      pNextNode->get_nodeName ( &bszNodeName );
      //
      if ( dntNodeType == NODE_TEXT )
      {
         //process #text value
         SetVar ( m_infcfe.m_ide.cUF, pNextNode, L"cUF" );
         SetVar ( m_infcfe.m_ide.cNF, pNextNode, L"cNF" );
         SetVar ( m_infcfe.m_ide.mod, pNextNode, L"mod" );
         SetVar ( m_infcfe.m_ide.nserieSAT, pNextNode, L"nserieSAT" );
         SetVar ( m_infcfe.m_ide.nCFe, pNextNode, L"nCfe" );
         SetVar ( m_infcfe.m_ide.dEmi, pNextNode, L"dEmi" );
         SetVar ( m_infcfe.m_ide.hEmi, pNextNode, L"hEmi" );
         SetVar ( m_infcfe.m_ide.cDV, pNextNode, L"cDV" );
         SetVar ( m_infcfe.m_ide.tpAmb, pNextNode, L"tpAmb" );
         SetVar ( m_infcfe.m_ide.CNPJ, pNextNode, L"CNPJ" );
         SetVar ( m_infcfe.m_ide.signAC, pNextNode, L"signAC" );
         SetVar ( m_infcfe.m_ide.assinaturaQRCODE, pNextNode, L"assinaturaQRCODE" );
         SetVar ( m_infcfe.m_ide.numeroCaixa, pNextNode, L"numeroCaixa" );
         SetVar ( m_infcfe.m_emit.CNPJ, pNextNode, L"CNPJ" );
         SetVar ( m_infcfe.m_emit.xNome, pNextNode, L"xNome" );
         SetVar ( m_infcfe.m_emit.m_enderemit.xLgr, pNextNode, L"xLgr" );
         SetVar ( m_infcfe.m_emit.m_enderemit.CEP, pNextNode, L"CEP" );
         SetVar ( m_infcfe.m_emit.m_enderemit.nro, pNextNode, L"nro" );
         SetVar ( m_infcfe.m_emit.m_enderemit.xBairro, pNextNode, L"xBairro" );
         SetVar ( m_infcfe.m_emit.m_enderemit.xCpl, pNextNode, L"xCpl" );
         SetVar ( m_infcfe.m_emit.m_enderemit.xMun, pNextNode, L"xMun" );
         SetVar ( m_infcfe.m_emit.IE, pNextNode, L"IE" );
         SetVar ( m_infcfe.m_emit.IM, pNextNode, L"IM" );
         SetVar ( m_infcfe.m_emit.cRegTrib, pNextNode, L"cRegTrib" );
         SetVar ( m_infcfe.m_emit.cRegTribISSQN, pNextNode, L"cRegTribISSQN" );
         SetVar ( m_infcfe.m_emit.indRatISSQN, pNextNode, L"indRatISSQN" );
         SetVar ( m_infcfe.m_emit.dest, pNextNode, L"dest" );
         //
         SetVar ( m_infcfe.m_dest.cpf, pNextNode, L"CPF" );
         SetVar ( m_infcfe.m_dest.xNome, pNextNode, L"xNome" );
         SetVar ( m_infcfe.m_entrega.xLgr, pNextNode, L"xLgr" );
         SetVar ( m_infcfe.m_entrega.nro, pNextNode, L"nro" );
         SetVar ( m_infcfe.m_entrega.xBairro, pNextNode, L"xBairro" );
         SetVar ( m_infcfe.m_entrega.xMun, pNextNode, L"xMun" );
         SetVar ( m_infcfe.m_entrega.uf, pNextNode, L"UF" );
         //read all product

         if ( uPPos > 0 )
         {
            auto pp = &m_infcfe.m_det.m_prod;
            //
            SetVar ( pp->cProd, pNextNode, L"cProd" );
            SetVar ( pp->xProd, pNextNode, L"xProd" );
            SetVar ( pp->NCM, pNextNode, L"NCM" );
            SetVar ( pp->CFOP, pNextNode, L"CFOP" );
            SetVar ( pp->uCom, pNextNode, L"uCom" );
            SetVar ( pp->qCom, pNextNode, L"qCom" );
            SetVar ( pp->vItem, pNextNode, L"vItem" );
            SetVar ( pp->vUnCom, pNextNode, L"vUnCom" );
            SetVar ( pp->vProd, pNextNode, L"vProd" );
            SetVar ( pp->indRegra, pNextNode, L"indRegra" );
            //last node_element on prod 
            //modified of vItem to vProd
            if ( this->m_wszOldNodeName == L"indRegra" )
            {
               m_infcfe.m_det.m_ItemsList.push_back ( *pp );
               uPPos--;
            }
         }
         //
         SetVar ( m_infcfe.m_det.m_imposto.vItem12741, pNextNode, L"vItem12741" );
         SetVar ( m_infcfe.m_det.m_imposto.m_icms.m_icms40.CST, pNextNode, L"CST" );
         SetVar ( m_infcfe.m_det.m_imposto.m_icms.m_icms40.Orig, pNextNode, L"Orig" );
         SetVar ( m_infcfe.m_det.m_imposto.m_pis.m_pisnt.CST, pNextNode, L"CST" );
         SetVar ( m_infcfe.m_det.m_imposto.m_confins.m_confinsnt.CST, pNextNode, L"CST" );
         //
         SetVar ( m_infcfe.m_total.vCFe, pNextNode, L"vCFe" );
         SetVar ( m_infcfe.m_total.m_icmstot.vCONFINS, pNextNode, L"vCONFINS" );
         SetVar ( m_infcfe.m_total.m_icmstot.vCONFISST, pNextNode, L"vCONFISST" );
         SetVar ( m_infcfe.m_total.m_icmstot.vDesc, pNextNode, L"vDesc" );
         SetVar ( m_infcfe.m_total.m_icmstot.vICMS, pNextNode, L"vICMS" );
         SetVar ( m_infcfe.m_total.m_icmstot.vOutro, pNextNode, L"vOutro" );
         SetVar ( m_infcfe.m_total.m_icmstot.vPIS, pNextNode, L"vPIS" );
         SetVar ( m_infcfe.m_total.m_icmstot.vPISST, pNextNode, L"vPISST" );
         SetVar ( m_infcfe.m_total.m_icmstot.vProd, pNextNode, L"vProd" );
         //
         auto pcmp = &m_infcfe.m_pgto.m_mp;
         //
         SetVar ( pcmp->cMP, pNextNode, L"cMP" );
         SetVar ( pcmp->vMP, pNextNode, L"vMP" );
         //
         if ( pcmp->cMP.size ( ) > 0 && pcmp->vMP.size ( ) > 0 )
         {
            m_infcfe.m_pgto.m_paytypes.push_back ( *pcmp );
            m_infcfe.m_pgto.m_mp.cMP = std::wstring ( );
            m_infcfe.m_pgto.m_mp.vMP = std::wstring ( );
         }

         SetVar ( m_infcfe.m_pgto.vTroco, pNextNode, L"vTroco" );
         SetVar ( m_infcfe.m_infadic.infCpl, pNextNode, L"infCpl" );
         SetVar ( m_signature.m_signedinfo.m_reference.DigestValue, pNextNode, L"DigestValue" );
         SetVar ( m_signature.m_signedinfo.SignatureValue, pNextNode, L"SignatureValue" );
         SetVar ( m_signature.m_signedinfo.m_keyinfo.m_x509data.X509Certificate,
                  pNextNode, L"X509Certificate" );
      }
      else if ( dntNodeType == NODE_ATTRIBUTE )
      {
      }
      else if ( dntNodeType == NODE_ELEMENT )
      {
         //save old node position
         m_wszOldNodeName = bszNodeName;

         if ( bszNodeName == WSZ_DET )
         {
            auto wszValue = GetAttr ( pNextNode );
            //
            if ( wszValue->length ( ) > 0 )
            {
               m_infcfe.m_det.nItem = unsigned int ( std::stoi ( wszValue->c_str ( ) ) );
               this->uPPos = m_infcfe.m_det.nItem;
            }
         }
         //
         if ( std::wstring ( bszNodeName ) == L"obsFisco" )
         {
            m_infcfe.m_infadic.m_obsfisco.xCampo = GetAttr ( pNextNode )->c_str ( );
         }
         //<CanonicalizationMethod Algorithm="http://www.w3.org/TR/2001/REC-xml-c14n-20010315" xmlns="http://www.w3.org/2000/09/xmldsig#"></CanonicalizationMethod>
         if ( std::wstring ( bszNodeName ) == L"CanonicalizationMethod" )
         {
            m_signature.m_signedinfo.m_CanonicalizationMethod.Algorithm =
               GetAttr ( pNextNode )->c_str ( );
         }
         //
         if ( std::wstring ( bszNodeName ) == L"SignatureMethod" )
         {
            m_signature.m_signedinfo.m_SignatureMethod.Algorithm =
               GetAttr ( pNextNode )->c_str ( );
         }
         //
         if ( std::wstring ( bszNodeName ) == L"Reference" )
         {
            m_signature.m_signedinfo.m_reference.URI = GetAttr ( pNextNode )->c_str ( );
         }
         //
         if ( std::wstring ( bszNodeName ) == L"Transform" )
         {
            static int nfirst = 0;
            m_signature.m_signedinfo.m_reference.m_transforms.m_transform.Algorithm[nfirst] =
               GetAttr ( pNextNode )->c_str ( );
         }
         //
         if ( std::wstring ( bszNodeName ) == L"DigestMethod" )
         {
            m_signature.m_signedinfo.m_reference.m_digestmethod.Algorithm =
               GetAttr ( pNextNode )->c_str ( );
         }
      }
      //
      pNextNode->hasChildNodes ( &bMore );
      if ( bMore == VARIANT_TRUE )
      {
         Next ( pNextNode );
      }
   }
   //
   pNodeList->Release ( );
}
//-----------------------------------------------------------------------------------------------//
void ExtensibleMarkupLanguage::Throw ( HRESULT lSuccess )
{
   std::wstring wszData = L"XMLDOMErrorCode: " + std::to_wstring ( long double ( lSuccess ) ) + L".";
   throw std::exception ( std::IWcharToChar ( wszData ).c_str ( ) );
}
//-----------------------------------------------------------------------------------------------//
void ExtensibleMarkupLanguage::Parse ( )
{
   IXMLDOMNodeList * pNodeList = NULL;
   HRESULT lSuccess = S_OK;
   //
   try
   {
      //get elements
      if ( FAILED ( lSuccess = m_pDoc->get_documentElement ( &m_pElement ) ) )
      {
         Throw ( lSuccess );
      }
      //get child nodes
      if ( FAILED ( lSuccess = m_pElement->get_childNodes ( &pNodeList ) ) )
      {
         Throw ( lSuccess );
      }
      //
      First ( pNodeList );
      pNodeList->Release ( );
   }
   catch ( std::exception const & e )
   {
      pNodeList->Release ( );
      throw e;
   }
}
//-----------------------------------------------------------------------------------------------//
void ExtensibleMarkupLanguage::Load ( )
{
   HRESULT lSuccess = S_FALSE;
   VARIANT_BOOL vbSuccess;
   VARIANT vFileName;
   //
   VariantInit ( &vFileName );
   //
   try
   {
      vFileName = Alloc ( m_wszFileName );
      //
      if ( ( this->m_pDoc->load ( vFileName, &vbSuccess ), vbSuccess != VARIANT_TRUE ) )
      {
         std::string s = std::IWcharToChar ( m_wszFileName ).c_str ( );
         auto out = std::string ( "Error to load DOM from " + s + "!" );
         //
         VariantClear ( &vFileName );
         throw std::exception ( out.c_str ( ) );
      }
   }
   catch ( std::exception const & e )
   {
      throw e;
   }
   //
   VariantClear ( &vFileName );
}
//-----------------------------------------------------------------------------------------------//
void ExtensibleMarkupLanguage::SetVar ( std::wstring & wszData,
                                        IXMLDOMNode * pNode, std::wstring const & wszName )
{
   BSTR bszValue = BSTR ( );
   //
   if ( m_wszOldNodeName == wszName )
   {
      pNode->get_text ( &bszValue );
      wszData = std::wstring ( bszValue ).c_str ( );
   }
}
//-----------------------------------------------------------------------------------------------//
void ExtensibleMarkupLanguage::Debug ( )
{
#if defined(_DEBUG)
   wprintf ( L"\nConteudo do XML %s\n", this->m_wszFileName.c_str ( ) );
   //
   wprintf ( L"" );
   //
   wprintf ( L"\n\n" );
#endif
}
//-----------------------------------------------------------------------------------------------//