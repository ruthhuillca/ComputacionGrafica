#ifndef CAMERA_H
#define CAMERA_H
#define CAM_PARALLEL 1
#define CAM_CONIC 2
#define CAM_STOP 0
#define CAM_EXAMINAR 1
#define CAM_PASEAR 2

typedef struct _Camera
{

float camViewX;
float camViewY;
float camViewZ;

float camAtX;
float camAtY;
float camAtZ;
// Up vector
float camUpX;
float camUpY;
float camUpZ;
float camAperture;
float camNear;
float camFar;
int camProjection;

int camMovimiento;
float aspectRatio;

float x1, x2, y1, y2, z1, z2;

float camIX, camIY, camIZ;
float camJX, camJY, camJZ;
float camKX, camKY, camKZ;
} camera;


void DestroyCamera ( camera **theCamera );
camera *CreatePositionCamera( float positionX, float positionY, float positionZ );
void SetCamera( camera *thisCamera, float viewX, float viewY, float viewZ, float atX, float atY, float atZ, float upX, float upY, float upZ );
void SetDependentParametersCamera( camera *thisCamera );
void SetGLCamera( camera *thisCamera );
void SetGLAspectRatioCamera( camera *thisCamera );

void AvanceFreeCamera( camera *thisCamera, float step );

void YawCamera( camera *thisCamera, float angle );
void Rotar_Latitud( camera *thisCamera, float inc );
void Rotar_Longitud( camera *thisCamera, float inc );
#endif
