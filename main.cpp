#include <Eigen/Dense>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include "ray_tracer.h"
#include "surface.h"
#include <iostream>


using namespace Eigen;

int main(int argc, char** argv)
{
    // 构建球面
    Vector3f sphere1Center(-3, 10, 0);
    float sphere1radius = 3;
    Vector3f ambient1Color(0.5, 0, 0);
    Vector3f diffuse1Color(0.5, 0, 0);
    Vector3f specular1Color(0.7, 0.7, 0.7);
    Vector3f mirror1Color(0, 0, 0);
    Sphere sphere1(sphere1Center, sphere1radius, diffuse1Color, specular1Color, ambient1Color, mirror1Color, 10);

    Vector3f sphere2Center(2, 10, -1);
    float sphere2radius = 2;
    Vector3f ambient2Color(0, 0.5, 0);
    Vector3f diffuse2Color(0, 0.5, 0);
    Vector3f specular2Color(0.7, 0.7, 0.7);
    Vector3f mirror2Color(0.3, 0.5, 0.3);
    Sphere sphere2(sphere2Center, sphere2radius, diffuse2Color, specular2Color, ambient2Color, mirror2Color, 50);

    // 构建平面
    Vector3f planePoint(0, 0, -3);
    Vector3f planeNormal(0, 0, 1);
    Vector3f ambientPColor(0.7, 0.7, 0.9);
    Vector3f diffusePColor(0.3, 0.3, 0.3);
    Vector3f specularPColor(0, 0, 0);
    Vector3f mirrorPColor(0.2, 0.2, 0.2);
    Plane plane(planePoint, planeNormal, diffusePColor, specularPColor, ambientPColor, mirrorPColor, 0);

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
    scene.surfaces.push_back(&sphere2);
    scene.surfaces.push_back(&plane);

    // 构建渲染器
    RayTracer rayTracer(scene, camera);
    cv::Mat mat(400, 800, CV_32FC3);
    rayTracer.render(-4, 4, -2, 2, 400, 800, mat);

    cv::namedWindow("Display", cv::WINDOW_NORMAL);
    cv::imshow("Display", mat);
    cv::waitKey(0);
}