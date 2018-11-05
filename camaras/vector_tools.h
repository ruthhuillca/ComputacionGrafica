#ifndef TOOLS_H
#define TOOLS_H
#include <math.h>
#ifndef TRUE
#define TRUE 1
#define FALSE 0
#endif

#define VECTOR_EPSILON 0.00001f
#define DISTANCE_EPSILON 1e-08f
#define ANGLE_EPSILON 0.00872665f


#define MOD(A,B,C) (float) sqrt( A*A + B*B + C*C )
#define PI_VALUE 3.14159265359f
#define DEGREE_TO_RAD 0.0174533f

void VectorNormalize( int *ierr, float *vX, float *vY, float *vz );
void UnitVectorPP( int *ierr, float *wX, float *wY, float *wZ,
float aX, float aY, float aZ,
float bX, float bY, float bz );
void UnitVectorVV( int *ierr, float *wX, float *wY, float *wZ,
float uX, float uY, float uZ,
float vX, float vY, float vz );
void VectorRotY( float *v, float inc );

void VectorRotXZ( float *vIn, float inc, int flagStop );

#endif
