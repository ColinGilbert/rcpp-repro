#pragma once
#include <memory>

namespace noob
{
	struct user_data
	{
	};

	static std::unique_ptr<noob::user_data> data;
}
