#include "Common.h"
#include "Texture.h"

#include "Resource.h"
#include "Utility.h"

namespace PangPang
{
unsigned char Texture::nullImage[nullImageSize * nullImageSize * nullbpp] = {0, };

Texture::Texture(void) : textureID(0), bufferID(0), imageData(0)
{
	this->InitNullTexture();
	this->SetImageInMemory(nullImage, Size(nullImageSize, nullImageSize), nullbpp);
}

Texture::Texture(const char *path) : textureID(0), bufferID(0), imageData(0) {
	this->InitNullTexture();
	this->SetImageInFile(path);
}

Texture::Texture(unsigned char *image, Size size, int bpp) : textureID(0), bufferID(0), imageData(0) {
	this->InitNullTexture();
	this->SetImageInMemory(image, size, bpp);
}

void Texture::InitNullTexture() {
	static bool isInitialized = false;

	if(!isInitialized)
	{
		// 텍스처 기본값 설정
		bool pattern = false;
		for(int i = 0; i < nullImageSize; i++) {
			if(i % 16 == 0) {
				if(pattern) pattern = 0;
				else pattern = 1;
			}
			for(int j = 0; j < nullImageSize; j++) {
				if(j % 16 == 0) {
					if(pattern) pattern = 0;
					else pattern = 1;
				}
				nullImage[i*(nullImageSize*4)+j*4] = 0;
				nullImage[i*(nullImageSize*4)+j*4+1] = pattern ? 255 : 0;
				nullImage[i*(nullImageSize*4)+j*4+2] = pattern ? 255 : 0;
				nullImage[i*(nullImageSize*4)+j*4+3] = 128;
			}
		}
		isInitialized = true;
	}
}

// 이미지 설정
void Texture::SetImageInFile(const char *path) {
	ResourcePtr r = Resource::Open(path);

	unsigned char* ptr = (unsigned char *)r->ptr;

	const TGAFileHeader* header = (const TGAFileHeader *)ptr;
	Size size(header->width, header->height);
	int bpp = header->pixelDepth / 8;

	this->SetImageInMemory(ptr + sizeof(TGAFileHeader), size, bpp);
}

void Texture::SetImageInMemory(unsigned char *image, Size size, int bpp) {
	int format;
	int internalFormat;

	// 이미지가 NULL 이면 디폴트 텍스처로 설정한다.
	if(image == NULL) {
		std::fprintf(stderr, "image가 NULL 입니다!\n");
		std::fprintf(stderr, "디폴트 텍스처로 설정합니다.\n");

		size.width = nullImageSize;
		size.height = nullImageSize;
		bpp = nullbpp;

		image = nullImage;
	}

	// 이미지 포맷 설정
	if(bpp == 1) {
		format = GL_LUMINANCE;
		internalFormat = format;
		imageFormat = BlackAndWhiteFormat;
	}
	else if(bpp == 2) {
		format = GL_LUMINANCE_ALPHA;
		internalFormat = GL_LUMINANCE_ALPHA;
		imageFormat = BlackWhiteAndAlphaFormat;
	}
	else if(bpp == 3) {
		internalFormat = GL_RGB;
		format = GL_BGR;
		imageFormat = BGRFormat;
	}
	else if(bpp == 4) {
		internalFormat = GL_RGBA;
		format = GL_BGRA;
		imageFormat = BGRAFormat;
	}

	// 이미지 영역 설정
	imageRect = Rect(0, 0, size.width, size.height);
	
	unsigned int major, minor;
	GetOpenGLVersion(major, minor);
	
	// OpenGL 2.1+
	if((major == 2 && minor >= 1) || major >= 3) {
		// 버퍼 생성
		if(bufferID == 0)
			glGenBuffers(1, &bufferID);
		glBindBuffer(GL_PIXEL_UNPACK_BUFFER, bufferID);
		glBufferData(GL_PIXEL_UNPACK_BUFFER, size.width*size.height*bpp, image, GL_STREAM_DRAW);
	}
	// OpenGL 1.1
	else {
		// 이미지 데이터가 있으면 지워준다.
		if(imageData != NULL) {
			delete imageData;
			imageData = NULL;
		}
		imageData = new unsigned char[size.width * size.height * bpp];
		std::memcpy(imageData, image, size.width * size.height * bpp * sizeof(unsigned char));
	}

	// 텍스처 생성
	glEnable(GL_TEXTURE_2D);

	if(textureID == 0)
		glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	if((major == 2 && minor >= 1) || major >= 3) // OpenGL 2.1+
		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, size.width, size.height, 0, format, GL_UNSIGNED_BYTE, 0);
	else // OpenGL 1.1
		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, size.width, size.height, 0, format, GL_UNSIGNED_BYTE, imageData);

	GLenum error = glGetError();
	if(error != GL_NO_ERROR) {
		std::fprintf(stderr, "이미지 로딩 중 에러 발생\n");
		switch(error) {
		case GL_INVALID_ENUM:
			std::fprintf(stderr, "에러 : GL_INVALID_ENUM\n");
			break;
		case GL_INVALID_VALUE:
			std::fprintf(stderr, "에러 : GL_INVALID_VALUE\n");
			break;
		case GL_INVALID_OPERATION:
			std::fprintf(stderr, "에러 : GL_INVALID_OPERATION\n");
			break;
		case GL_STACK_OVERFLOW:
			std::fprintf(stderr, "에러 : GL_STACK_OVERFLOW\n");
			break;
		case GL_STACK_UNDERFLOW:
			std::fprintf(stderr, "에러 : GL_STACK_UNDERFLOW\n");
			break;
		case GL_OUT_OF_MEMORY:
			std::fprintf(stderr, "에러 : GL_OUT_OF_MEMORY\n");
			break;
		}
	}

	glDisable(GL_TEXTURE_2D);
}

void* Texture::Lock() {
	void *ptr;

	// 초기화가 되어 있는지 점검
	if(textureID == 0) {
		std::fprintf(stderr, "텍스처가 초기화가 되지 않은 상태에서 Lock()을 호출했습니다!\n");
		return NULL;
	}
	unsigned int major, minor;
	GetOpenGLVersion(major, minor);

	GLenum error = glGetError();
	
	// OpenGL 2.1+
	if((major == 2 && minor >= 1) || major >= 3) {
		if(bufferID == 0) {
			std::fprintf(stderr, "텍스처가 초기화가 되지 않은 상태에서 Lock()을 호출했습니다!\n");
			return NULL;
		}
		glBindBuffer(GL_PIXEL_UNPACK_BUFFER, bufferID);
		ptr = glMapBuffer(GL_PIXEL_UNPACK_BUFFER, GL_READ_WRITE);
	}
	// OpenGL 1.1
	else {
		ptr = (void *)imageData;
	}

	error = glGetError();
	if(error != GL_NO_ERROR) {
		std::fprintf(stderr, "텍스터 Lock 에러 발생\n");
		switch(error) {
		case GL_INVALID_ENUM:
			std::fprintf(stderr, "에러 : GL_INVALID_ENUM\n");
			break;
		case GL_INVALID_VALUE:
			std::fprintf(stderr, "에러 : GL_INVALID_VALUE\n");
			break;
		case GL_INVALID_OPERATION:
			std::fprintf(stderr, "에러 : GL_INVALID_OPERATION\n");
			break;
		case GL_STACK_OVERFLOW:
			std::fprintf(stderr, "에러 : GL_STACK_OVERFLOW\n");
			break;
		case GL_STACK_UNDERFLOW:
			std::fprintf(stderr, "에러 : GL_STACK_UNDERFLOW\n");
			break;
		case GL_OUT_OF_MEMORY:
			std::fprintf(stderr, "에러 : GL_OUT_OF_MEMORY\n");
			break;
		}
	}

	return ptr;
}

void Texture::Unlock() {
	// 초기화가 되어있는지 점검
	if(textureID == 0) {
		std::fprintf(stderr, "텍스처가 초기화가 되지 않은 상태에서 Unlock()을 호출했습니다!\n");
		return;
	}
	
	unsigned int major, minor;
	GetOpenGLVersion(major, minor);
	
	// OpenGL 2.1+
	if(((major == 2 && minor >= 1) || major >= 3) && !bufferID) {
		std::fprintf(stderr, "텍스처가 초기화가 되지 않은 상태에서 Unlock()을 호출했습니다!\n");
		return;
	}

	unsigned int format = 0;
	switch(imageFormat) {
	case BlackAndWhiteFormat:
		format = GL_LUMINANCE;
		break;
	case BlackWhiteAndAlphaFormat:
		format = GL_LUMINANCE_ALPHA;
		break;
	case RGBFormat:
		format = GL_RGB;
		break;
	case BGRFormat:
		format = GL_BGR;
		break;
	case RGBAFormat:
		format = GL_RGBA;
		break;
	case BGRAFormat:
		format = GL_BGRA;
		break;
	default:
		// 아무것도 안함
		return;
	}

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textureID);
		
	// OpenGL 2.1+
	if((major == 2 && minor >= 1) || major >= 3) {
		glBindBuffer(GL_PIXEL_UNPACK_BUFFER, bufferID);
		glUnmapBuffer(GL_PIXEL_UNPACK_BUFFER);

		glTexSubImage2D(GL_TEXTURE_2D, 0, imageRect.origin.x, imageRect.origin.y, 
			imageRect.size.width, imageRect.size.height, format, GL_UNSIGNED_BYTE, 0);

		glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
	}
	else {
		glTexSubImage2D(GL_TEXTURE_2D, 0, imageRect.origin.x, imageRect.origin.y, 
			imageRect.size.width, imageRect.size.height, format, GL_UNSIGNED_BYTE, imageData);
	}

	glDisable(GL_TEXTURE_2D);
}

void Texture::BindTexture() {
	if(textureID != 0)
		glBindTexture(GL_TEXTURE_2D, textureID);
}

// 메모리 해제
Texture::~Texture(void)
{
	glDeleteTextures(1, &textureID);
	// OpenGL 2.1+
	unsigned int major, minor;
	GetOpenGLVersion(major, minor);
	
	// OpenGL 2.1+
	if((major == 2 && minor >= 1) || major >= 3)
		glDeleteBuffers(1, &bufferID);
	// OpenGL 1.1
	else if(imageData != NULL) {
		delete imageData;
		imageData = NULL;
	}
}
}

