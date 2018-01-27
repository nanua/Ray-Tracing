//
// Created by Yuhong Zhong on 2018/1/26.
//

#include "surface.h"

Surface::Surface(Vector3f diffuseColor, Vector3f specularColor, Vector3f ambientColor, Vector3f mirrorColor,
                 float specularParameter) {
    this->diffuseColor = diffuseColor;
    this->specularColor = specularColor;
    this->ambientColor = ambientColor;
    this->mirrorColor = mirrorColor;
    this->specularParameter = specularParameter;
}

bool Sphere::hit(Vector3f startPoint, Vector3f direction, float start, float end, HitInfo &hitInfo) {
    Vector3f startMinusCenter = startPoint - this->center;
    float startMinusCenterSquare = startMinusCenter.dot(startMinusCenter);
    float directionSquare = direction.dot(direction);
    float directionDotSmc = direction.dot(startMinusCenter);
    float t;

    float discriminant = static_cast<float>(pow(directionDotSmc, 2) -
                                            (directionSquare) * (startMinusCenterSquare -
                                                                 pow(this->radius, 2)));

    if (discriminant < 0) {
        return false;
    } else if (discriminant == 0) {
        t = (-1 * directionDotSmc) / directionSquare;
        if (t < start || t > end) {
            return false;
        }
    } else {
        t = static_cast<float>((-1 * directionDotSmc - pow(discriminant, 0.5)) / directionSquare);
        if (t > end) {
            return false;
        } else if (t < start) {
            t = static_cast<float>((-1 * directionDotSmc + pow(discriminant, 0.5) / directionSquare));
            if (t < start || t > end) {
                return false;
            }
        }
    }

    hitInfo.t = t;
    hitInfo.point = startPoint + t * direction;
    hitInfo.normal = (hitInfo.point - this->center).normalized();
    hitInfo.surface = this;
    hitInfo.surfaceType = 's';
    return true;
}

void Sphere::boundingBox(BoundingBox &box) {
    box.minPoint = this->center.array() - this->radius;
    box.maxPoint = this->center.array() + this->radius;
}

Sphere::Sphere(Vector3f center, float radius, Vector3f diffuseColor, Vector3f specularColor,
               Vector3f ambientColor, Vector3f mirrorColor,
               float specularParameter) : Surface(diffuseColor, specularColor, ambientColor, mirrorColor,
                                                  specularParameter) {
    this->center = center;
    this->radius = radius;
}

bool Triangle::hit(Vector3f startPoint, Vector3f direction, float start, float end, HitInfo &hitInfo) {
    this->coefficientMatrix.col(2) = direction;
    float coefficientDet = this->coefficientMatrix.determinant();
    if (coefficientDet == 0) {
        return false;
    }
    Vector3f b = this->vertices[0] - startPoint;

    // 计算t
    this->coefficientMatrix.col(2) = b;
    float t = this->coefficientMatrix.determinant() / coefficientDet;
    this->coefficientMatrix.col(2) = direction;
    if (t < start || t > end) {
        return false;
    }

    // 计算gamma
    this->coefficientMatrix.col(1) = b;
    float gamma = this->coefficientMatrix.determinant() / coefficientDet;
    this->coefficientMatrix.col(1) = this->vertices[0] - this->vertices[2];
    if (gamma < 0 || gamma > 1) {
        return false;
    }

    // 计算beta
    this->coefficientMatrix.col(0) = b;
    float beta = this->coefficientMatrix.determinant() / coefficientDet;
    this->coefficientMatrix.col(0) = this->vertices[0] - this->vertices[1];
    if (beta < 0 || beta > 1 - gamma) {
        return false;
    }

    hitInfo.surfaceType = 't';
    hitInfo.surface = this;
    hitInfo.point = startPoint + t * direction;
    hitInfo.normal = this->normal;
    hitInfo.t = t;
    return true;
}

void Triangle::boundingBox(BoundingBox &box) {
    for (int i = 0; i < 3; ++i) {
        box.maxPoint[i] = std::max({this->vertices[0][i], this->vertices[1][i], this->vertices[2][i]});
        box.minPoint[i] = std::min({this->vertices[0][i], this->vertices[1][i], this->vertices[2][i]});
    }
}

Triangle::Triangle(Vector3f v0, Vector3f v1, Vector3f v2, Vector3f diffuseColor, Vector3f specularColor,
                   Vector3f ambientColor, Vector3f mirrorColor, float specularParameter) :
        Surface(diffuseColor, specularColor, ambientColor,
                mirrorColor, specularParameter) {
    this->vertices[0] = v0;
    this->vertices[1] = v1;
    this->vertices[2] = v2;
    this->coefficientMatrix.col(0) = v0 - v1;
    this->coefficientMatrix.col(1) = v0 - v2;
    this->normal = (v0 - v1).cross(v0 - v2).normalized();
}

bool Plane::hit(Vector3f startPoint, Vector3f direction, float start, float end, HitInfo &hitInfo) {
    float directionDotNormal = direction.dot(this->normal);
    if (directionDotNormal == 0) {
        return false;
    }
    float t = (this->point - startPoint).dot(this->normal) / directionDotNormal;
    if (t < start || t > end) {
        return false;
    }
    hitInfo.normal = this->normal;
    hitInfo.point = startPoint + direction * t;
    hitInfo.t = t;
    hitInfo.surface = this;
    hitInfo.surfaceType = 'p';
    return true;
}

void Plane::boundingBox(BoundingBox &box) {
    box.minPoint.setConstant(FLT_MIN);
    box.maxPoint.setConstant(FLT_MAX);
    for (int i = 0; i < 3; ++i) {
        if (this->normal(i) == 0) {
            box.minPoint(i) = this->point(i);
        }
    }
}

Plane::Plane(Vector3f p, Vector3f n, Vector3f diffuseColor, Vector3f specularColor, Vector3f ambientColor,
             Vector3f mirrorColor, float specularParameter) :
        point(p), normal(n), Surface(diffuseColor, specularColor, ambientColor, mirrorColor, specularParameter) {
    ;
}