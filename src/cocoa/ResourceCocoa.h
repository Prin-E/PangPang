#pragma once

#include "Common.h"

namespace PangPang
{
	class ResourceCocoa;
	
	typedef std::shared_ptr<const ResourceCocoa> ResourcePtr;
	
	class ResourceCocoa
	{
	private:
		ResourceCocoa(std::shared_ptr<void> ptr, size_t size);
	public:
		~ResourceCocoa();
	public:
		static ResourcePtr Open(const char* path);
	private:
		std::shared_ptr<void> holder;
	public:
		void* ptr;
		size_t size;
	};
}

