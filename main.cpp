#include <Eigen/Dense>
#include <opencv2/core.hpp>
#include <opencv/cxeigen.hpp>
#include <opencv/cv.hpp>
#include "ray_tracer.h"
#include "surface.h"
#include <iostream>


using namespace Eigen;

int main(int argc, char** argv)
{
    // 构建球面
    Vector3f sphere1Center(0, 10, 0);
    float sphere1radius = 3;
    Vector3f ambientColor(0.5, 0, 0);
    Vector3f diffuseColor(0.5, 0, 0);
    Vector3f specularColor(0.7, 0.7, 0.7);
    Vector3f mirrorColor(0, 0, 0);
    Sphere sphere1(sphere1Center, sphere1radius, diffuseColor, specularColor, ambientColor, mirrorColor, 10);

    // 构建相机
    Vector3f imagePoint(0, 0, 0);
    Vector3f viewDirection(0, 1, 0);
    Camera camera(imagePoint, viewDirection, 3);

    // 构建光源
    LightSource lightSource;
    Vector3f lightSourceDirection(1, 1, -1);
    lightSourceDirection.normalize();
    lightSource.intensity = 1;
    lightSource.direction = lightSourceDirection;

    // 构建场景
    Scene scene;
    Vector3f backgroundColor(0, 0, 0);
    scene.ambientLightIntensity = 1;
    scene.backgroundColor = backgroundColor;
    scene.lightSources.push_back(&lightSource);
    scene.surfaces.push_back(&sphere1);

    // 构建渲染器
    RayTracer rayTracer(scene, camera);
    cv::Mat mat(400, 800, CV_32FC3);
    rayTracer.render(-4, 4, -2, 2, 400, 800, mat);
    cv::namedWindow("Display", CV_WINDOW_NORMAL);
    cv::imshow("Display", mat);
    cv::waitKey(0);
}