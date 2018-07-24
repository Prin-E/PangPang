#pragma once

#include <memory>

namespace PangPang
{
class ResourceWin32;

typedef std::shared_ptr<const ResourceWin32> ResourcePtr;

class ResourceWin32
{
public:
	class FileNotFound {};
private:
	ResourceWin32(std::shared_ptr<void> ptr, size_t size);
public:
	~ResourceWin32();
public:
	static ResourcePtr Open(const char* path);
private:
	std::shared_ptr<void> holder;
public:
	void* ptr;
	size_t size;
};
}

