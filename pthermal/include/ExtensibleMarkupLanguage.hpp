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
*  Version 0.1-beta
*/
#pragma once
#pragma warning(disable:4275)
#pragma warning(disable:4251)

#include <pthermal.hpp>
#include <exception>
#include <msxml6.h>
#include <list>
#include <map>
#include <unordered_map>

namespace xml
{
   struct ExtensibleMarkupLanguageData : public pthermal::CFe
   {
      ExtensibleMarkupLanguageData ( ) = default;
   };
   //
   static std::wstring const WSZ_CFE = L"CFe";
   static std::wstring const WSZ_INFCFE = L"infCFe";
   static std::wstring const WSZ_IDE = L"ide";
   static std::wstring const WSZ_EMIT = L"emit";
   static std::wstring const WSZ_DET = L"det";
   static std::wstring const WSZ_TOTAL = L"total";
   static std::wstring const WSZ_PGTO = L"pgto";
   static std::wstring const WSZ_INFADIC = L"infAdic";
   static std::wstring const WSZ_SIGNATURE = L"Signature";
   static std::wstring const WSZ_NULL = std::wstring();
   //
   static std::wstring const XMLTagNames[] =
   {
      WSZ_CFE,
      WSZ_DET,
      WSZ_EMIT,
      WSZ_IDE,
      WSZ_INFADIC,
      WSZ_INFCFE,
      WSZ_PGTO,
      WSZ_SIGNATURE,
      WSZ_TOTAL,
      WSZ_NULL
   };
   //
   namespace INFCFE
   {
      static std::wstring const Id = L"Id";
      static std::wstring const Version = L"versao";
      static std::wstring const VersionDataEnt = L"versaoDadosEnt";
      static std::wstring const VersionSB = L"versaoSB";
   };
   //
   class PTHERMAL_API ExtensibleMarkupLanguage :
      public ExtensibleMarkupLanguageData
   {
   protected:
      IXMLDOMDocument * m_pDoc;
      IXMLDOMElement * m_pElement;
      std::wstring m_wszFileName;
      std::wstring m_wszOldNodeName;
      IXMLDOMParseError * m_pError;
   public:
      explicit ExtensibleMarkupLanguage(std::wstring const & szFileName);
      virtual ~ExtensibleMarkupLanguage();
      //
      virtual void Load();
      virtual void Parse();
      virtual void Debug();
   protected:
      virtual void First(IXMLDOMNodeList * pNodeList);
      virtual void Next(IXMLDOMNode * pNode);
      //
      virtual VARIANT Alloc(std::wstring const & wszData);
      virtual void Throw(HRESULT lSuccess);
      virtual void SetVar(std::wstring & wszData, IXMLDOMNode * pNode, std::wstring const & wszName);
      virtual std::shared_ptr<std::wstring> GetAttr(IXMLDOMNode * pNode);
   };
   //
   typedef std::shared_ptr<ExtensibleMarkupLanguage> LPExtensibleMarkupLanguage;
};