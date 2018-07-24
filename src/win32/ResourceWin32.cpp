#include "Common.h"
#include "resource1.h"
#include "ResourceWin32.h"
#include "zip.h"
#include "unzip.h"

namespace PangPang
{
ResourceWin32::ResourceWin32(std::shared_ptr<void> ptr, size_t size)
	:	holder(ptr), ptr(ptr.get()), size(size)
{

}

ResourceWin32::~ResourceWin32()
{
}

ResourcePtr ResourceWin32::Open(const char* path)
{
	HINSTANCE hInstance = GetModuleHandle(0);

	HRSRC hrsrc = FindResource(hInstance, MAKEINTRESOURCE(IDR_FILESYSTEM), TEXT("ZIP"));
	HANDLE hglob = LoadResource(hInstance, hrsrc);

	void* zipbuf = LockResource(hglob);
	size_t size = SizeofResource(hInstance, hrsrc);

	HZIP hz = OpenZip(zipbuf, size, 0);


	int len = MultiByteToWideChar(CP_ACP, 0, path, strlen(path) + 1, 0, 0);

	std::shared_ptr<wchar_t>wpath (new wchar_t[len + 1], [](wchar_t* p)->void{delete[] p;});

	MultiByteToWideChar(CP_ACP, 0, path, strlen(path) + 1, wpath.get(), len + 1);

	int i;	ZIPENTRY ze;
	FindZipItem(hz, wpath.get(), true, &i, &ze);

	std::shared_ptr<void> file(new char[ze.unc_size], [](void* p)->void{delete[] p;});
	if(UnzipItem(hz, i, file.get(), ze.unc_size) != ZR_OK)
		throw FileNotFound();

	CloseZip(hz);

	// 오류 점검 필요함

	ResourceWin32* res = new ResourceWin32(file, ze.unc_size);

	return ResourcePtr(res);
}
}

