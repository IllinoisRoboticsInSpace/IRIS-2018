
#include "CoordSystemKinect.hpp"

/**THIS FILE CONTAINS USEFUL FUNCTIONS FOR USING KINECT DEPTH STREAM**/
/**written by: Leon Frickensmith leonfrickensmith@gmail.com**/

/**converts the strange units to meters**/
namespace csk
{
float RawDepthToMilli(int depthValue)
{
    const float mystery1 = -0.0030711016f;//these were obtained from the internet
    const float mystery2 = 3.3309495161f;
    return (1.0f / (((static_cast<float>(depthValue) * mystery1) + mystery2)))*1000;
}


/**screen style coordinates, 0,0 top left; 639,0 top right; 639,479 bottom right;**/
int GetIndex(int x, int y)
{
    return (x+y*dimX);
}

/**get the x and y angle of the ray(spherical coordinate system)**/
cvec2f GetAngle(int x, int y)
{
    cvec2f angles;
    angles.x = ((dimX2-x)/dimX2)*fovX2;
    angles.y = pi2-(((dimY2-y)/dimY2)*fovY2);
    return angles;
}


/**get the cartesian style coordinates for some ray in the depth frame**/
/**kinect measures distance from the viewing plane of the camera**/
cvec3f GetCartCoord(int x, int y, float distance)
{
    cvec3f cartesian;

    cvec2f azmuthPolar(GetAngle(x,y));//theta is CCW angle from +X, Phi is angle from ZY plane

    cartesian.x = distance;
    cartesian.y = distance*tan(azmuthPolar.x);
    cartesian.z = distance*tan(-azmuthPolar.y+pi2);

    return cartesian;
}


/**this gives a Vec that points down in coordinate system where forward is +X, left is +Y, up is +Z**/
cvec3f FindDown(int16_t accelerometer_x, int16_t accelerometer_y, int16_t accelerometer_z)
{
    return cvec3f(-accelerometer_z, -accelerometer_x, -accelerometer_y);
}


/**Finds a matrix to multiply INPUT by so that this vector lies in the -Z axis**/
cmat3f FindDownMatrix(const cvec3f& rVec, float Yaw)
{
    cvec3f a(rVec);//a is vector to rotate
    const cvec3f b(0,0,-1);//        b
    a.normalize();//                a
    const float c = a.dot(b);//     c

    const cvec3f v = a.cross(b);//   v
    const float s = v.length();//   s
    cmat3f vx;
    vx.setSkewSymCrossProd(v);//    vx
    const cmat3f vx2(vx*vx);//       vx2

    cmat3f I;//                      I
    I.x1 = 1;
    I.y2 = 1;
    I.z3 = 1;

    cmat3f gravity_rot = I+vx+(vx2*((1-c)/(s*s)));
	cmat3f yaw_rot;
	yaw_rot.x1 = cos(Yaw);
	yaw_rot.x2 = sin(Yaw);
	yaw_rot.y1 = -yaw_rot.x2;
	yaw_rot.y2 = yaw_rot.x1;
	yaw_rot.z3 = 1;


	return yaw_rot*gravity_rot;
}
}
