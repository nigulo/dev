/*
 * ortho.cpp
 *
 *  Created on: Mar 25, 2015
 *      Author: nigul
 */

#include "ortho.h"
#include "engine3d/scenegraph/camera.h"
#include <GL/glew.h>

using namespace engine3d;

Ortho::Ortho(Program& rProgram,
			float left, float right, float bottom, float top, float near, float far) :
		Projection(rProgram, near, far),
		mLeft(left),
		mRight(right),
		mBottom(bottom),
		mTop(top) {

	Update();
}

Ortho::~Ortho() {
}

const Plane Ortho::GetTopPlane(const Camera& r_camera) const {
	return Plane(r_camera.GetEye() + r_camera.GetUp() * mTop, -r_camera.GetUp());
}

const Plane Ortho::GetBottomPlane(const Camera& r_camera) const {
	return Plane(r_camera.GetEye() + (-r_camera.GetUp()) * mBottom, r_camera.GetUp());
}

const Plane Ortho::GetLeftPlane(const Camera& r_camera) const {
	Vector right = r_camera.GetDirection().CrossProduct(r_camera.GetUp());
	return Plane(r_camera.GetEye() + (-right) * mLeft, right);
}

const Plane Ortho::GetRightPlane(const Camera& r_camera) const {
	Vector left = r_camera.GetUp().CrossProduct(r_camera.GetDirection());
	return Plane(r_camera.GetEye() + (-left) * mRight, left);
}

void Ortho::Update(int width, int height) {
	mRight = mLeft + float (width);
	mTop = mBottom + float (height);
	Update();
}

void Ortho::Update() {
	float w = mRight - mLeft;
	float h = mTop - mBottom;
	float d = mZFar - mZNear;
	float tx = (mRight + mLeft) / w;
	float ty = (mTop + mBottom) / h;
	float tz = (mZFar + mZNear) / d;
	mMatrix.Set(0, 0, 2 / w); mMatrix.Set(0, 1, 0);     mMatrix.Set(0, 2, 0);      mMatrix.Set(0, 3, tx);
	mMatrix.Set(1, 0, 0);     mMatrix.Set(1, 1, 2 / h); mMatrix.Set(1, 2, 0);      mMatrix.Set(1, 3, ty);
	mMatrix.Set(2, 0, 0);     mMatrix.Set(2, 1, 0);     mMatrix.Set(2, 2, -2 / d); mMatrix.Set(2, 3, tz);
	mMatrix.Set(3, 0, 0);     mMatrix.Set(3, 1, 0);     mMatrix.Set(3, 2, 0);      mMatrix.Set(3, 3, 1);
}
