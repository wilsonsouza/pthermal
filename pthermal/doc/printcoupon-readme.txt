	pthermal dynamic library interface for thermal printer

	Novos paramêtros adicionos para impressão de tickets.
	Função PrintCoupon:
	Imprime cupoms fiscais e não fiscais o padrão para impressão e cupom fiscal.
	bool __declspec(dllexport) __stdcall PrintCoupon ( std::wstring const & szXMLFileName,
        	                                           pthermal::CouponType const & uType = 
                	                                   pthermal::CouponType::coupon );

	Paramêtros:
	szXMLFileName - recebe o nome do arquivo a ser processado. Tipo XML ou JSON.
	uType         - definido tipo de impressão


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
	Definições:
	nocoupontax   - define impressão do cupom não fiscal.
	couponpresale - define impressão do cupom pre venda.
	productlist   - define impressão do cupom lista de produtos.
	coupon        - define impressão do cupom fiscal de venda

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
		seu código
		...
		PrintCoupon("../json-files/json_exemplo.json", pthermal::CouponType::produclist);
	     }
	     catch( std::exception const & e)
	     {
	        trata o erro gerado
	     }
             ....
             continua seu código
