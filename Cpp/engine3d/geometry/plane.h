#ifndef PLANE_H
#define PLANE_H

#include "vector.h"

using namespace base;

namespace engine3d {
/*
 * Class defining a geometrical plane
 */
class Plane
{
	public:
        enum {
            PLACEMENT_FRONT,
            PLACEMENT_BACK,
            PLACEMENT_COINCIDE,
            PLACEMENT_UNKNOWN
        };
        
	public:
        
		/**
		 * @rPoint the coordinates of an arbitrary point in plane
		 * @rNormal normal vector of the plane
		 **/
		Plane(const Vector& rPoint, const Vector& rNormal);

		/**
		 * Constructs a plane from 3 points.
		 * The normal of this plane will point
         * towards the movement of the screw which is rotated in 
         * the direction point 1 -> point 2 -> point 3
         * @see Triangle
		 **/
		Plane(const Vector& rPoint1, const Vector& rPoint2, const Vector& rPoint3);
		~Plane();
		// No description
		const Vector& GetNormal() const;
		// No description
		const Vector& GetPoint() const;
		
		/**
		 * Checks whether the given point lays in the front of the
		 * plane or in the back of the plane.
		 * @return PLACEMENT_FRONT, PLACEMENT_BACK or PLACEMENT_COINCIDE
		 **/
		int WhichSide(const Vector& rPoint) const;
		
		/**
		 * @return the distance from the plane to the point
		 **/
		double GetDistance(const Vector& rPoint) const;
		/**
		 * @return the distance vector from the plane to the point
		 **/
		Vector GetDistanceVector(const Vector& rPoint) const;

		string ToString() const;

	protected:
        Vector mPoint;
        Vector mNormal;
};
}
#endif // PLANE_H
