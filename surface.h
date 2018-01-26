//
// Created by Yuhong Zhong on 2018/1/26.
//

#ifndef RAY_TRACING_SURFACE_H
#define RAY_TRACING_SURFACE_H

#include <Eigen/Dense>
#include <Eigen/LU>
#include <cmath>
#include <cfloat>

using namespace Eigen;


class Surface;

/*
 * 存储视觉光线击中点信息的结构体
 */
struct HitInfo {
    Surface *surface;  // 击中的Surface的指针
    char surfaceType;  // 击中的Surface的类型
    Vector3f point;  // 击中的点的位置
    Vector3f normal;  // 击中的点在曲面上的单位法向量
    float t;  // 击中点的参数值
};

/*
 * 存储坐标轴对齐区域的信息的结构体
 */
struct BoundingBox {
    Vector3f minPoint;  // 约束区域靠近原点的顶点
    Vector3f maxPoint;  // 约束区域远离原点的顶点
};

/*
 * 可被视觉光线击中的几何物体
 */
class Surface {
public:
    Vector3f diffuseColor;
    Vector3f specularColor;
    Vector3f ambientColor;
    Vector3f mirrorColor;
    float specularParameter;

    virtual bool hit(Vector3f startPoint, Vector3f direction, float start, float end, HitInfo &hitInfo) = 0;

    virtual void boundingBox(BoundingBox &box) = 0;

    Surface(Vector3f diffuseColor, Vector3f specularColor, Vector3f ambientColor, Vector3f mirrorColor,
            float specularParameter);
};

/*
 * 三维球面
 */
class Sphere : public Surface {
private:
    float radius;
    Vector3f center;
public:
    bool hit(Vector3f startPoint, Vector3f direction, float start, float end, HitInfo &hitInfo) override;

    void boundingBox(BoundingBox &box) override;

    Sphere(Vector3f center, float radius, Vector3f diffuseColor, Vector3f specularColor,
           Vector3f ambientColor, Vector3f mirrorColor, float specularParameter);
};

/*
 * 平面三角形
 */
class Triangle : public Surface {
private:
    Vector3f vertices[3];
    Matrix3f coefficientMatrix;
    Vector3f normal;
public:
    bool hit(Vector3f startPoint, Vector3f direction, float start, float end, HitInfo &hitInfo) override;

    void boundingBox(BoundingBox &box) override;

    Triangle(Vector3f v0, Vector3f v1, Vector3f v2, Vector3f diffuseColor, Vector3f specularColor,
             Vector3f ambientColor, Vector3f mirrorColor, float specularParameter);
};

/*
 * 平面
 */
class Plane : public Surface {
private:
    Vector3f point;
    Vector3f normal;

public:
    bool hit(Vector3f startPoint, Vector3f direction, float start, float end, HitInfo &hitInfo) override;

    void boundingBox(BoundingBox &box) override;

    Plane(Vector3f p, Vector3f n, Vector3f diffuseColor, Vector3f specularColor,
          Vector3f ambientColor, Vector3f mirrorColor, float specularParameter);
};

#endif