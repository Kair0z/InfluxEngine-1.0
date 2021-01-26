#pragma once

namespace Influx
{
	template <class Service>
	class ServiceLocator
	{
	public:
		ServiceLocator();

		inline static void Load(sPtr<Service> service) { s_pService = service; }
		inline static sPtr<Service> Get() { return s_pService; }

	private:
		static sPtr<Service> s_pService;
	};

	template <class Service>
	sPtr<Service> ServiceLocator<Service>::s_pService{};


	template<class Service>
	inline ServiceLocator<Service>::ServiceLocator()
	{
		
	}
}


