#include <optix.h>
#include <optix_world.h>

#include "raydata.cuh"

// Sphere variables
rtDeclareVariable(float3, center0, , );
rtDeclareVariable(float3, center1, , );
rtDeclareVariable(float, radius, , );
rtDeclareVariable(float, time0, , );
rtDeclareVariable(float, time1, , );

// The ray that will be intersected against
rtDeclareVariable(optix::Ray, theRay, rtCurrentRay, );
rtDeclareVariable(PerRayData, thePrd, rtPayload,  );

// The point and normal of intersection
//   the "attribute" qualifier is used to communicate between intersection and shading programs
//   These may only be written between rtPotentialIntersection and rtReportIntersection
rtDeclareVariable(HitRecord, hitRecord, attribute hitRecord, );

inline __device__ float dot(float3 a, float3 b)
{
    return a.x*b.x + a.y*b.y + a.z*b.z;
}

__device__ float3 center(float time) {
    return center0;
    if (time0 == time1)
        return center0;
    else
        return center0 + ((time - time0) / (time1 - time0)) * (center1 - center0);
}

// The sphere bounding box program
//   The pid parameter enables specifying a primitive withing this geometry
//   since there is only 1 primitive (the sphere), the pid is ignored here
RT_PROGRAM void getBounds(int pid, float result[6])
{
    optix::Aabb* box0 = (optix::Aabb*)result;
    box0->m_min = center(time0) - abs(radius);
    box0->m_max = center(time0) + abs(radius);

    return;

    optix::Aabb box1;
    box1.m_min = center(time1) - abs(radius);
    box1.m_max = center(time1) + abs(radius);

    box0->include(box1);
}


// The sphere intersection program
//   this function calls rtReportIntersection if an intersection occurs
//   As above, pid refers to a specific primitive, is ignored
RT_PROGRAM void intersection(int pid)
{
    float3 oc = theRay.origin - center(thePrd.gatherTime);
    float a = dot(theRay.direction, theRay.direction);
    float b = dot(oc, theRay.direction);
    float c = dot(oc, oc) - radius*radius;
    float discriminant = b*b - a*c;

    if (discriminant < 0.0f) return;

    float t = (-b - sqrtf(discriminant)) / a;
    if (t < theRay.tmax && t > theRay.tmin)
        if (rtPotentialIntersection(t))
        {
            hitRecord.point = theRay.origin + t * theRay.direction;
            hitRecord.normal = (hitRecord.point - center(thePrd.gatherTime)) / radius;
            rtReportIntersection(0);
        }

    t = (-b + sqrtf(discriminant)) / a;
    if (t < theRay.tmax && t > theRay.tmin)
        if (rtPotentialIntersection(t))
        {
            hitRecord.point = theRay.origin + t * theRay.direction;
            hitRecord.normal = (hitRecord.point - center(thePrd.gatherTime)) / radius;
            rtReportIntersection(0);
        }
}
