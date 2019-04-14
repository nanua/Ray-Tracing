//
// Created by Yuhong Zhong on 2018/1/26.
//

#ifndef RAY_TRACING_RAY_TRACER_H
#define RAY_TRACING_RAY_TRACER_H


#include <Eigen/Dense>
#include <opencv2/core.hpp>
#include <opencv2/core/eigen.hpp>
#include <cmath>
#include "surface.h"

#define MAX_RAY_COLOR_RECURSION 5
#define POSITION_DELTA 1e-2

using namespace Eigen;


class Camera {
public:
    Vector3f eyePoint;
    Vector3f imagePlaneOrigin;
    Vector3f u, v, w;

    Camera(Vector3f eyePoint, Vector3f imagePlaneOrigin, Vector3f u, Vector3f v, Vector3f w);

    Camera(Vector3f imagePlaneOrigin, Vector3f viewDirection, float focusLength);

    Camera();
};

class LightSource {
public:
    Vector3f direction;
    float intensity;
};

class Scene {
public:
    std::vector<Surface *> surfaces;
    std::vector<LightSource *> lightSources;
    float ambientLightIntensity;
    Vector3f backgroundColor;
};

class RayTracer {
private:
    Scene &scene;
    Camera &camera;

    Vector3f rayColor(Vector3f startPoint, Vector3f direction, float start, float end, int times = 0);

    bool hit(Vector3f startPoint, Vector3f direction, float start, float end, HitInfo &hitInfo);

public:
    RayTracer(Scene &scene, Camera &camera);

    void render(float left, float right, float bottom, float top, size_t verticalPixel, size_t horizontalPixel, cv::Mat &mat);
};


#endif //RAY_TRACING_RAY_TRACER_H
