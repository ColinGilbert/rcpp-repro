#include "Application.hpp"

#include <RuntimeCompiler/AUArray.h>
#include <RuntimeCompiler/BuildTool.h>
#include <RuntimeCompiler/ICompilerLogger.h>
#include <RuntimeCompiler/FileChangeNotifier.h>
#include <RuntimeObjectSystem/IObjectFactorySystem.h>
#include <RuntimeObjectSystem/ObjectFactorySystem/ObjectFactorySystem.h>
#include <RuntimeObjectSystem/RuntimeObjectSystem.h>
#include <RuntimeObjectSystem/IObject.h>

noob::app* noob::app::app_pointer = nullptr;

noob::app::app() : compiler_logger(0), runtime_object_system(0), init_func(0), update_func(0), draw_func(0)
{
	app_pointer = this;
	timespec timeNow;
	clock_gettime(CLOCK_MONOTONIC, &timeNow);
	time = timeNow.tv_sec * 1000000000ull + timeNow.tv_nsec;
	prefix = std::unique_ptr<std::string>(new std::string("."));
	rtcpp_modules.push_back(std::make_tuple("UserUpdateFunc", update_func, update_object_id));
	rtcpp_modules.push_back(std::make_tuple("UserDrawFunc", draw_func, draw_object_id));
	rtcpp_modules.push_back(std::make_tuple("UserInitFunc", init_func, init_object_id));
}

void noob::app::OnConstructorsAdded()
{
	// This could have resulted in a change of object pointer, so release old and get new one.
	for (auto a : rtcpp_modules)
	{
		if (std::get<1>(a))
		{
			IObject* pObj = runtime_object_system->GetObjectFactorySystem()->GetObject(std::get<2>(a));
			pObj->GetInterface(&std::get<1>(a));
			if( 0 == std::get<1>(a))
			{
				delete pObj;
				compiler_logger->LogError( "Error - no updateable interface found");
			}
		}
	}
}

noob::app::~app()
{
	std::cout <<  "~application()";
	app_pointer = nullptr;

	if( runtime_object_system )
	{
		// clean temp object files
		runtime_object_system->CleanObjectFiles();
	}
	if( runtime_object_system && runtime_object_system->GetObjectFactorySystem() )
	{
		runtime_object_system->GetObjectFactorySystem()->RemoveListener(this);

		for (auto a : rtcpp_modules)
		{
			// delete object via correct interface
			IObject* pObj = runtime_object_system->GetObjectFactorySystem()->GetObject(std::get<2>(a));
			delete pObj;
		}
	}

	delete runtime_object_system;
	delete compiler_logger;
}


noob::app& noob::app::get()
{
	std::cout <<  "application::get()";
	assert(app_pointer && "application not created!");
	return *app_pointer;
}


void noob::app::step()
{
	timespec timeNow;
	clock_gettime(CLOCK_MONOTONIC, &timeNow);
	uint64_t uNowNano = timeNow.tv_sec * 1000000000ull + timeNow.tv_nsec;
	double delta = (uNowNano - time) * 0.000000001f;
	time = uNowNano;
}


bool noob::app::init()
{
	std::cout << "init()";
	// Initialise the RuntimeObjectSystem
	runtime_object_system = new RuntimeObjectSystem;
	compiler_logger = new StdioLogSystem();
	if( !runtime_object_system->Initialise(compiler_logger, 0) )
	{
		runtime_object_system = 0;
		return false;
	}
	runtime_object_system->GetObjectFactorySystem()->AddListener(this);

	for (auto a : rtcpp_modules)
	{
		// construct first object
		IObjectConstructor* pCtor = runtime_object_system->GetObjectFactorySystem()->GetConstructor(std::get<0>(a).c_str());
		if(pCtor)
		{
			IObject* pObj = pCtor->Construct();

			pObj->GetInterface(&std::get<1>(a));

			if(0 == std::get<1>(a))
			{
				std::string message = "Error - no updateable interface found for ";
				message += std::get<0>(a);
				// delete pObj;
				std::cout << message;
				return false;
			}

			std::get<2>(a) = pObj->GetObjectId();
			{
				std::string name = std::get<0>(a);
				std::stringstream ss;
				ss << "[RTCPP] - object valid? " << std::get<2>(a).IsValid();
				std::cout << ss;

			}
		}
	}
	std::cout << "done init()";
	return true;
}

void noob::app::update(double delta)
{
	if (rtcpp_success == true)
	{
		//check status of any compile
		if(runtime_object_system->GetIsCompiledComplete())
		{
			// load module when compile complete
			runtime_object_system->LoadCompiledModule();
		}

		if(!runtime_object_system->GetIsCompiling())
		{
			runtime_object_system->GetFileChangeNotifier()->Update(delta);
			if (!started)
			{
				std::cout << "calling virtual init()";
				started = init_func->init();
			}
			else 
			{
				std::cout << "calling virtual update()";
				update_func->update(delta);
				std::cout << "calling virtual draw()";
				draw_func->draw(window_width, window_height, delta);
			}
		}
	}
}


void noob::app::set_archive_dir(const std::string& filepath)
{

	{
		std::stringstream ss;
		ss << "setting archive dir(\"" << filepath << "\")";
		std::cout << ss;

	}	

	prefix = std::unique_ptr<std::string>(new std::string(filepath));

	{
		std::stringstream ss;
		ss << "archive dir = " << *prefix;
		std::cout << ss;
	}
}

void noob::app::window_resize(uint32_t w, uint32_t h)
{
	window_width = w;
	window_height = h;
	{
		std::stringstream ss;
		ss << "window_resize(" << window_width << ", " << window_height << ")";
		std::cout << ss;
	}

	if (window_height == 0) 
	{
		window_height = 1;
	}
}
