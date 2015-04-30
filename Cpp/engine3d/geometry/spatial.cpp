// Class automatically generated by Dev-C++ New Class wizard

#include "spatial.h" // class's header file

using namespace engine3d;

// class constructor
Spatial::Spatial() :
    mChanged(true) 
{
}

Spatial::Spatial(const string& rName) :
    mChanged(true), 
    Object(rName)
{
}

Spatial::Spatial(const Spatial& rSpatial) 
{
    Copy(rSpatial);
}

void Spatial::Copy(const Spatial& rSpatial)
{
    mChanged = true;
    mTransformation = rSpatial.mTransformation;
    mNewTransformation = rSpatial.mNewTransformation;
}

// class destructor
Spatial::~Spatial()
{
}

void Spatial::SetTransformation(const Transformation& rT)
{
	mNewTransformation = rT;
	mChanged = true;
}

const Transformation& Spatial::GetTransformation() const
{
    return mNewTransformation;
}

void Spatial::Transform()
{
    mTransformation = mNewTransformation;
}
