//
// Created by Yuhong Zhong on 2018/1/26.
//

#ifndef RAY_TRACING_RAY_TRACER_H
#define RAY_TRACING_RAY_TRACER_H


#include <stdint-gcc.h>
#include <eigen/dense>
#include <opencv2/core/core.hpp>
#include <opencv/cxeigen.hpp>
#include "surface.h"
#include <cmath>

#define MAX_RAY_COLOR_RECURSION 5
#define POSITION_DELTA 1e-3

using namespace Eigen;


class Camera {
public:
    Vector3f eyePoint;
    Vector3f imagePlanePoint;
    Vector3f u, v, w;

    Camera(Vector3f eyePoint, Vector3f imagePlanePoint, Vector3f u, Vector3f v, Vector3f w);

    Camera(Vector3f imagePlanePoint, Vector3f viewDirection, float focusLength);

    Camera();
};

class LightSource {
public:
    Vector3f direction;
    float intensity;
};

class Scene {
public:
    std::vector<Surface &> surfaces;
    std::vector<LightSource &> lightSources;
    float ambientLightIntensity;
    Vector3f backgroundColor;
};

class RayTracer {
private:
    Scene &scene;
    Camera &camera;

    void convertToMat(MatrixXf &matrixR, Matrix &matrixG, Matrix &matrixB, cv::Mat &mat);

    Vector3f rayColor(Vector3f startPoint, Vector3f direction, float start, float end, int times = 0);

    bool hit(Vector3f startPoint, Vector3f direction, float start, float end, HitInfo &hitInfo);

public:
    RayTracer(Scene &scene, Camera &camera);

    cv::Mat render(float left, float right, float top, float bottom, size_t horizontalPixel, size_t verticalPixel);
};


#endif //RAY_TRACING_RAY_TRACER_H
