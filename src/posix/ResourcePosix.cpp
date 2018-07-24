#include <memory>

#include <sstream>
#include <cstdio>
#include <cstdlib>

#include "ResourcePosix.h"

ResourcePosix::ResourcePosix(std::shared_ptr<void> ptr, size_t size)
	:	holder(ptr), ptr(ptr.get()), size(size)
{

}

ResourcePosix::~ResourcePosix()
{
}

ResourcePtr ResourcePosix::Open(const char* path)
{
	std::stringstream ss;
	ss << "./res/" << path;
	FILE *fp = std::fopen(ss.str().data(), "r");
	if(!fp)
	{
		throw FileNotFound();
	}
	
	std::fseek(fp, 0, SEEK_END);
	size_t size = std::ftell(fp);
	std::fseek(fp, 0, SEEK_SET);
	
	char *buf = new char[size];
	size_t nread = std::fread(buf, size, 1, fp);
	if(nread != 1)
		::abort(); // FIXME
	
	std::fclose(fp);
	
	std::shared_ptr<void> dataptr(buf, [](char *p)->void{ delete[] p; });
	ResourcePosix* res = new ResourcePosix(dataptr, size);
	return ResourcePtr(res);
}

