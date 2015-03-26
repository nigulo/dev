#include "transformationcontroller.h"
#include "engine3d/scenegraph/camera.h"

using namespace engine3d;

// class constructor
TransformationController::TransformationController(const Scene& rScene, bool relative) :
    Controller(rScene),
    mRelative(relative)
{
}

// class destructor
TransformationController::~TransformationController()
{
}

void TransformationController::AddTarget(Spatial* target)
{
    mTargets.Add(target);
}

void TransformationController::Rotate(const Vector& rotVector, double rotAngle) 
{
     
    for (LinkedList<Spatial*>::Iterator i = mTargets.Begin(); !i.Done(); i++) {
        Spatial* p_target = (*i);
        assert(p_target);
        Camera* p_camera = dynamic_cast<Camera*>(p_target);
        Vector realRotation = rotVector;
        if (p_camera) { // Camera
            Vector up = p_camera->GetUp();
            Vector center = p_camera->GetCenter() - p_camera->GetEye();
            Vector right = center.CrossProduct(up);
            up.Normalize();
            center.Normalize();
            right.Normalize();
            realRotation = (right * rotVector[0] + up * rotVector[1] + center * rotVector[2]);
        }
        Transformation t1;
        t1.SetRotation(realRotation, rotAngle);

        if (mRelative) {
            Transformation t = p_target->GetTransformation();
            
            // Previous rotation first, then new rotation and then
            // previous translation
            p_target->SetTransformation(t.GetRotation() * t1 * t.GetTranslation());
        }
        else {
            p_target->SetTransformation(t1);
        }
    }
}

void TransformationController::Translate(const Vector& trVector) 
{
    for (LinkedList<Spatial*>::Iterator i = mTargets.Begin(); !i.Done(); i++) {
        Spatial* p_target = (*i);
        assert(p_target);
        Camera* p_camera = dynamic_cast<Camera*>(p_target);
        
        Vector realTranslation = trVector;
        
        if (p_camera) { // Camera
        
            Vector up = p_camera->GetUp();
            Vector center = p_camera->GetCenter() - p_camera->GetEye();
            Vector right = center.CrossProduct(up);
            up.Normalize();
            center.Normalize();
            right.Normalize();
            realTranslation = (right * trVector[0] + up * trVector[1] + center * trVector[2]);
        }
        
        Transformation t1;
        t1.SetTranslation(realTranslation);
        if (mRelative) {
            p_target->SetTransformation(p_target->GetTransformation() * t1);
        }
        else {
            p_target->SetTransformation(t1);
        }
    }
}

void TransformationController::Transform(const Vector& rotVector, double rotAngle, const Vector& trVector)
{
    for (LinkedList<Spatial*>::Iterator i = mTargets.Begin(); !i.Done(); i++) {
        Spatial* p_target = (*i);
        assert(p_target);
        Camera* p_camera = dynamic_cast<Camera*>(p_target);
        
        Vector realTranslation = trVector;
        Vector realRotation = rotVector;
        
        if (p_camera) { // Camera
        
            Vector up = p_camera->GetUp();
            Vector center = p_camera->GetCenter() - p_camera->GetEye();
            Vector right = center.CrossProduct(up);
            up.Normalize();
            center.Normalize();
            right.Normalize();
            realTranslation = (right * trVector[0] + up * trVector[1] + center * trVector[2]);
            realRotation = (right * rotVector[0] + up * rotVector[1] + center * rotVector[2]);
        }
        
        Transformation t1;
        t1.SetTranslation(realTranslation);
        t1.SetRotation(realRotation, rotAngle);
        if (mRelative) {
            Transformation t = p_target->GetTransformation();
            // Old and new rotations first, then translations
            p_target->SetTransformation(t.GetRotation() * t1 * t.GetTranslation());
        }
        else {
            p_target->SetTransformation(t1);
        }
    }
}
