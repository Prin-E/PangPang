#pragma once

#include <memory>

class ResourcePosix;

typedef std::shared_ptr<const ResourcePosix> ResourcePtr;

class ResourcePosix
{
public:
	class FileNotFound {};
private:
	ResourcePosix(std::shared_ptr<void> ptr, size_t size);
public:
	~ResourcePosix();
public:
	static ResourcePtr Open(const char* path);
private:
	std::shared_ptr<void> holder;
public:
	void* ptr;
	size_t size;
};

