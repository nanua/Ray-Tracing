#include <Eigen/Dense>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include "ray_tracer.h"
#include "surface.h"
#include <iostream>
#include <fstream>
#include "utils.h"


using namespace Eigen;
using namespace std;

void ballPlaneDemo() {
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

    // 构建三角形
    Vector3f v1(5, 15, -3);
    Vector3f v2(-2, 15, -3);
    Vector3f v3(0, 13, 5);
    Vector3f ambientTColor(0.1, 0.1, 0.1);
    Vector3f diffuseTColor(0, 0, 0);
    Vector3f specularTColor(0, 0, 0);
    Vector3f mirrorTColor(0.5, 0.5, 0.5);
    Triangle triangle(v1, v2, v3, diffuseTColor, specularTColor, ambientTColor, mirrorTColor, 0);

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
    scene.surfaces.push_back(&triangle);

    // 构建渲染器
    RayTracer rayTracer(scene, camera);
    cv::Mat mat(400, 800, CV_32FC3);
    rayTracer.render(-4, 4, -2, 2, 400, 800, mat);

    cv::namedWindow("Display", cv::WINDOW_NORMAL);
    cv::imshow("Display", mat);
    cv::waitKey(0);
}

void loadInput(vector<Vector3f> &vectorVertex,
               vector<Vector3f> &vectorNormal,
               vector<vector<unsigned>> &vectorFace,
               string filePath) {
    ifstream inFile;
    inFile.open(filePath);
    if (!inFile) {
        cerr << "Unable to open obj file";
        exit(1);
    }
    string line;
    while (getline(inFile, line)) {
        std::stringstream ss(line);
        std::string s;
        ss >> s;
        if (s == "v") {
            Vector3f v;
            ss >> v[0] >> v[1] >> v[2];
            vectorVertex.push_back(v);
        } else if (s == "vn") {
            Vector3f v;
            ss >> v[0] >> v[1] >> v[2];
            vectorNormal.push_back(v);
        } else if (s == "f") {
            vector<string> rawVVector;
            string rawV;
            for (int i = 0; i < 3; ++i) {
                ss >> rawV;
                rawVVector.push_back(rawV);
            }
            vector<unsigned> indexVec;
            for (string rawV : rawVVector) {
                stringstream ssInner(rawV);
                string curResult;
                while (getline(ssInner, curResult, '/')) {
                    stringstream ssInt(curResult);
                    int curInt;
                    ssInt >> curInt;
                    indexVec.push_back(curInt);
                }
            }
            vectorFace.push_back(indexVec);
        }
    }
}

void loadDemo() {
    // 构建相机
    Vector3f imagePoint(1, 0, 0);
    Vector3f viewDirection(-1, 0, 0);

    Camera camera(imagePoint, viewDirection, 5);

    // 构建光源
    LightSource lightSource;
    Vector3f lightSourceDirection(-1, 0.7, 0.7);
    lightSourceDirection.normalize();
    lightSource.intensity = 1;
    lightSource.direction = lightSourceDirection;

    // 构建场景
    Scene scene;
    Vector3f backgroundColor(0, 0, 0);
    scene.ambientLightIntensity = 1;
    scene.backgroundColor = backgroundColor;
    scene.lightSources.push_back(&lightSource);

    Vector3f ambientColor(0.5, 0, 0);
    Vector3f diffuseColor(0.5, 0, 0);
    Vector3f specularColor(0.2, 0.2, 0.2);
    Vector3f mirrorColor(0, 0, 0);

    vector<Vector3f> vectorVertex;
    vector<Vector3f> vectorNormal;
    vector<vector<unsigned>> vectorFace;
    loadInput(vectorVertex, vectorNormal, vectorFace, "../../data/sphere.obj");
    for (size_t index = 0; index < vectorFace.size(); ++index) {
        auto face = vectorFace[index];
        Triangle *curTriangle = new Triangle(vectorVertex[face[0] - 1],
                                             vectorVertex[face[3] - 1],
                                             vectorVertex[face[6] - 1],
                                             diffuseColor,
                                             specularColor,
                                             ambientColor,
                                             mirrorColor,
                                             2);
        scene.surfaces.push_back(curTriangle);
    }

    // 构建渲染器
    RayTracer rayTracer(scene, camera);
    cv::Mat mat(400, 400, CV_32FC3);
    rayTracer.render(-1, 1, -1, 1, 400, 400, mat);

    cv::namedWindow("Display", cv::WINDOW_NORMAL);
    cv::imshow("Display", mat);
    cv::waitKey(0);

    for (Surface *surface : scene.surfaces) {
        delete surface;
    }
}

int main(int argc, char **argv) {
    loadDemo();
}