#include <Cocoa/Cocoa.h>

#include "ResourceCocoa.h"

namespace PangPang
{
	ResourceCocoa::ResourceCocoa(std::shared_ptr<void> ptr, size_t size)
	:	holder(ptr), ptr(ptr.get()), size(size)
	{
		
	}
	
	ResourceCocoa::~ResourceCocoa()
	{
	}
	
	ResourcePtr ResourceCocoa::Open(const char* path)
	{
		char *buf = new char[256];
		std::shared_ptr<void> dataptr(buf, [](char *p)->void{ delete[] p; });
		ResourceCocoa *res = new ResourceCocoa(dataptr, 256);
		return ResourcePtr(res);
	}
}