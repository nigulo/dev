#include "viewport.h"
#include <GL/glew.h>

using namespace engine3d;

// class constructor
Viewport::Viewport()
{
    int params[4];
	glGetIntegerv(GL_VIEWPORT, params);
	x = params[0];
	y = params[1];
	width = params[2];
	height = params[3];
	xCenter =  x + width >> 1;
	yCenter =  y + height >> 1;
}

Viewport::Viewport(const Viewport& v)
{
	x = v.x;
	y = v.y;
	width = v.width;
	height = v.height;
	xCenter =  v.xCenter;
	yCenter =  v.yCenter;
}

// class destructor
Viewport::~Viewport()
{
	// insert your code here
}

int Viewport::GetX() const 
{
    return x;
}

int Viewport::GetY() const 
{
    return y;
}

int Viewport::GetWidth() const 
{
    return width;
}

int Viewport::GetHeight() const 
{
    return height;
}

int Viewport::GetXC() const 
{
    return xCenter;
}

int Viewport::GetYC() const 
{
    return yCenter;
}
