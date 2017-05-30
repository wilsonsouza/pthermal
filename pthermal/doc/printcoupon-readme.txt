	pthermal dynamic library interface for thermal printer

	Novos param�tros adicionos para impress�o de tickets.
	Fun��o PrintCoupon:
	Imprime cupoms fiscais e n�o fiscais o padr�o para impress�o e cupom fiscal.
	bool __declspec(dllexport) __stdcall PrintCoupon ( std::wstring const & szXMLFileName,
        	                                           pthermal::CouponType const & uType = 
                	                                   pthermal::CouponType::coupon );

	Param�tros:
	szXMLFileName - recebe o nome do arquivo a ser processado. Tipo XML ou JSON.
	uType         - definido tipo de impress�o


	Class enumeradora:
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
	Defini��es:
	nocoupontax   - define impress�o do cupom n�o fiscal.
	couponpresale - define impress�o do cupom pre venda.
	productlist   - define impress�o do cupom lista de produtos.
	coupon        - define impress�o do cupom fiscal de venda

	Exemplo:
	void main()
	{
	   try
	   {
	      return PrintCoupon("../json-files/json_exemplo.json", pthermal::CouponType::produclist);
	   }
	   catch( std::exception const & e)
	   {
	      std::cout << w.what() << std::endl;
	   }
	}

	ou
		seu c�digo
		...
		PrintCoupon("../json-files/json_exemplo.json", pthermal::CouponType::produclist);
	     }
	     catch( std::exception const & e)
	     {
	        trata o erro gerado
	     }
             ....
             continua seu c�digo
