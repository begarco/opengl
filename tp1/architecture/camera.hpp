#ifndef HEADER_CAMERA_HPP
#define HEADER_CAMERA_HPP

#include "abstractCamera.hpp"

class Camera : public AbstractCamera {
public:
	double mPosition[3];
	double mVisee[3];
	double mVertical[3];
	double mZ_proche;
	double mZ_eloigne;
	double mAngleOuverture;
	double mAspect;

	Camera(	double px, double py, double pz,
			    double vx, double vy, double vz,
			    double vertx, double verty, double vertz,
			    double zproche = 1, double zeloigne = 500,
			    double angleOuverture = 50);
	void ChangerRepereCamera(double position[3], double pointDeVisee[3], double vecteurVertical[3]);
	void ChangerRepereCamera();
};



#endif
