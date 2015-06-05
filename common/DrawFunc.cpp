#include <iostream>

#include "UserData.hpp"
#include "DrawFunc.hpp"
#include "InterfaceIds.h"

#include <RuntimeObjectSystem/ObjectInterfacePerModule.h>
#include <RuntimeObjectSystem/IObject.h>

class UserDrawFunc : public TInterface<IID_DRAW, DrawFunc>
{
	virtual void draw(float width, float height, double delta)
	{
		std::cout << "virtual draw() width = " <<  width << ", height = " << height << ", delta = " << delta;

	}
};

REGISTERCLASS(UserDrawFunc);
