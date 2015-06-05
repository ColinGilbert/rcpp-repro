#pragma once

#include <sstream>
#include <assert.h>
#include <memory>
#include <iostream>
#include <fstream>
#include <atomic>

#include <RuntimeObjectSystem/IObjectFactorySystem.h>
#include <RuntimeObjectSystem/ObjectInterface.h>
#include <RuntimeCompiler/AUArray.h>

#include "StdioLogSystem.h"
#include "DrawFunc.hpp"
#include "InitFunc.hpp"
#include "UpdateFunc.hpp"
#include "InterfaceIds.h"

namespace noob
{
	class app : public IObjectFactoryListener
	{

		public:
			app();
			virtual ~app();
			static app& get();

			bool init();

			uint32_t get_height() const { return static_cast<uint32_t>(window_height); }
			uint32_t get_width() const { return static_cast<uint32_t>(window_width); }

			void step();
			void pause();
			void resume();
		
			void update(double delta);
			
			void set_archive_dir(const std::string & filepath);
			void window_resize(uint32_t w, uint32_t h);

			virtual void OnConstructorsAdded();

		protected:
			static app* app_pointer;
			std::unique_ptr<std::string> prefix;
			std::atomic<bool> started;
			std::atomic<bool> rtcpp_success;
		
			uint64_t time;
			float window_width, window_height;
			
			ICompilerLogger* compiler_logger;
			IRuntimeObjectSystem* runtime_object_system;

			UpdateFunc* update_func;
			InitFunc* init_func;
			DrawFunc* draw_func;

			ObjectId draw_object_id;
			ObjectId init_object_id;
			ObjectId update_object_id;
			
			std::vector<std::tuple<std::string, IObject*, ObjectId>> rtcpp_modules;
	};
}
