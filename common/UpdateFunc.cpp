#include <iostream>

#include <RuntimeObjectSystem/ObjectInterfacePerModule.h>

#include "UserData.hpp"
#include "UpdateFunc.hpp"
#include "InterfaceIds.h"

class UserUpdateFunc : public TInterface<IID_UPDATE, UpdateFunc>
{
	public:
		virtual void update(double delta)
		{
			std::cerr << "virtual update()";
		}
};

REGISTERCLASS(UserUpdateFunc);
