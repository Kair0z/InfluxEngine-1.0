#pragma once
#include <functional>

namespace Influx
{
	struct Action
	{
		using FunctionList = std::vector<std::function<void()>>;
		FunctionList mFunctions;

		void AddListener(const std::function<void()>& f)
		{
			mFunctions.push_back(f);
		}

		void operator()()
		{
			for (const std::function<void()>& f : mFunctions) f();
		}
	};
}


