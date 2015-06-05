#include <iostream>

#include <RuntimeObjectSystem/ObjectInterfacePerModule.h>

#include "InitFunc.hpp"
#include "UserData.hpp"
#include "InterfaceIds.h"

class UserInitFunc : public TInterface<IID_INIT, InitFunc>
{
	public:
		virtual bool init()
		{
			std::cerr << "virtual init()";
			noob::data = std::unique_ptr<noob::user_data>(new noob::user_data());
			return true;
		}
};

REGISTERCLASS(UserInitFunc);
