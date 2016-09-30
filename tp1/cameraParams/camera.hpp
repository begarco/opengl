#ifndef HEADER_CAMERA_HPP
#define HEADER_CAMERA_HPP

class Camera{
	double mPosition[3];
	double mVisee[3];
	double mVertical[3];
	
	public:
		Camera(	double px, double py, double pz,
					double vx, double vy, double vz,
					double vertx, double verty, double vertz);
};



#endif
