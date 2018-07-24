#include "Common.h"

#include <boost/thread/once.hpp>

#include "Collidable.h"
#include "Drawable.h"

namespace PangPang
{

bool TestCollision(Collidable* lhs, Collidable* rhs)
{
	Rect lrect = lhs->GetBoundingRect();
	Rect rrect = rhs->GetBoundingRect();

#if defined(WIN32)
#ifdef max
#undef max
#endif
#ifdef min
#undef min
#endif
#endif

	Point origin(std::max(lrect.origin.x, rrect.origin.x),
					std::max(lrect.origin.y, rrect.origin.y));

	Size size;

	if(lrect.origin.x + lrect.size.width > rrect.origin.x + rrect.size.width)
		size.width = rrect.size.width;
	else
		size.width = lrect.size.width;

	if(lrect.origin.y + lrect.size.height> rrect.origin.y + rrect.size.height)
		size.height = rrect.size.height;
	else
		size.height = lrect.size.height;

	Rect rect(origin.x,
			  origin.y,
			  size.width,
			  size.height);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	glTranslated(-origin.x, -origin.y, 0);

	glClear(GL_STENCIL_BUFFER_BIT);
	glEnable(GL_ALPHA_TEST);
	glEnable(GL_STENCIL_TEST);

	glAlphaFunc(GL_GEQUAL, 0.5f);
	glStencilFunc(GL_ALWAYS, 1, 1);
	glStencilOp(GL_REPLACE, GL_REPLACE, GL_REPLACE);

	lhs->DrawForCollisionTest();

 	glStencilFunc(GL_EQUAL, 1, 1);
 	glStencilOp(GL_KEEP, GL_INCR, GL_INCR);
 
	rhs->DrawForCollisionTest();

	size_t len = (rect.size.width) * (rect.size.height);

	std::shared_ptr<uchar> data(new uchar[len], [](uchar* p){delete[] p;});
	memset(data.get(), 0, len);

	glReadPixels(0,
		0,
		rect.size.width,
		rect.size.height,
		GL_STENCIL_INDEX, GL_UNSIGNED_BYTE, data.get());
		
	const uchar* ptr;
	const uchar* end = data.get() + len;

	for(ptr = data.get(); ptr != end && *ptr != 2; ++ptr);

	glDisable(GL_STENCIL_TEST);
	glDisable(GL_ALPHA_TEST);

 	glPopMatrix();
	return ptr != end;
	
}
}

