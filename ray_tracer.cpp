//
// Created by Yuhong Zhong on 2018/1/26.
//


#include "ray_tracer.h"

Camera::Camera() {
    this->eyePoint.setConstant(0);
    this->imagePlanePoint.setConstant(1);
    this->u.setConstant(0);
    this->v.setConstant(0);
    this->w.setConstant(0);
    this->u[0] = 1;
    this->v[2] = 1;
    this->w[1] = -1;
}

Camera::Camera(Vector3f imagePlanePoint, Vector3f viewDirection, float focusLength) {
    viewDirection.normalize();
    this->imagePlanePoint = imagePlanePoint;
    this->eyePoint = imagePlanePoint - viewDirection * (-1 * focusLength);
    if (viewDirection[2] == 1) {
        this->w.setConstant(0);
        this->u.setConstant(0);
        this->v.setConstant(0);
        this->w[2] = -1;
        this->u[0] = 1;
        this->v[1] = -1;
    } else if (viewDirection[2] == -1) {
        this->w.setConstant(0);
        this->u.setConstant(0);
        this->v.setConstant(0);
        this->w[2] = 1;
        this->u[0] = 1;
        this->v[1] = 1;
    } else {
        this->w = -1 * viewDirection;
        this->v.setConstant(0);
        this->v[2] = 1;
        this->u = this->v.cross(this->w).normalized();
        this->v = this->w.cross(this->u).normalized();
    }
}

Camera::Camera(Vector3f eyePoint, Vector3f imagePlanePoint, Vector3f u, Vector3f v, Vector3f w) {
    this->eyePoint = eyePoint;
    this->imagePlanePoint = imagePlanePoint;
    this->u = u;
    this->v = v;
    this->w = w;
}

void RayTracer::convertToMat(MatrixXf &matrixR, Matrix &matrixG, Matrix &matrixB, cv::Mat &mat) {
    cv::Mat matR(matrixR.rows(), matrixR.cols(), CV_32FC1);
    cv::Mat matG(matrixR.rows(), matrixR.cols(), CV_32FC1);
    cv::Mat matB(matrixR.rows(), matrixR.cols(), CV_32FC1);
    cv::eigen2cv(matrixR, matR);
    cv::eigen2cv(matrixG, matG);
    cv::eigen2cv(matrixB, matB);
    std::vector<cv::Mat> src;
    src.push_back(matR);
    src.push_back(matG);
    src.push_back(matB);
    cv::merge(src, mat);
}

bool RayTracer::hit(Vector3f startPoint, Vector3f direction, float start, float end, HitInfo &hitInfo) {
    float curStart = start;
    bool hit = false;
    for (auto c : this->scene.surfaces) {
        if (c.hit(startPoint, direction, curStart, end, hitInfo)) {
            curStart = hitInfo.t;
            hit = true;
        }
    }
    return hit;
}

Vector3f RayTracer::rayColor(Vector3f startPoint, Vector3f direction, float start, float end, int times = 0) {
    // 找到被击中的对象
    HitInfo hitInfo;
    this->hit(startPoint, direction, start, end, hitInfo);

    // 计算颜色
    if (hit) {
        // 设置初始颜色环境光分量
        Vector3f color = this->scene.ambientLightIntensity * hitInfo.surface.ambientColor;

        Surface &surface = hitInfo.surface;
        Vector3f point = hitInfo.point;
        Vector3f normal = hitInfo.normal;

        // 增加光源相关分量
        for (auto lightSource : this->scene.lightSources) {
            // 计算是否处于阴影中
            HitInfo shadowHitInfo;
            if (!this->hit(point, -1 * lightSource.direction, POSITION_DELTA, FLT_MAX, shadowHitInfo)) {
                // 增加漫反射分量
                color += surface.diffuseColor * lightSource.intensity
                         * std::max(0, normal.dot(lightSource.direction * -1));
                // 增加镜面反射分量
                Vector3f h = ((-1 * direction) + (-1 * lightSource.direction)).normalized();
                color += surface.specularColor * lightSource.intensity
                         * pow(std::max(0, normal.dot(h)), surface.specularParameter);
            }
        }
        // 增加理想镜面反射分量
        if (times <= MAX_RAY_COLOR_RECURSION) {
            Vector3f r = ((-1 * direction) - 2 * ((-1 * direction).dot(normal)) * normal).normalized();
            color += surface.mirrorColor.cwiseProduct(this->rayColor(point, r, POSITION_DELTA, FLT_MAX, times + 1));
        }
        return color;
    } else {
        return this->scene.backgroundColor;
    }
}

cv::Mat RayTracer::render(float left, float right, float top, float bottom, size_t horizontalPixel,
                          size_t verticalPixel) {
    MatrixXf matR(verticalPixel, horizontalPixel);
    MatrixXf matG(verticalPixel, horizontalPixel);
    MatrixXf matB(verticalPixel, horizontalPixel);

    // 计算每个像素的颜色
    for (size_t i = 0; i < verticalPixel; ++i) {
        for (size_t j = 0; j < horizontalPixel; ++j) {
            // 计算该像素的视觉方向
            Vector3f pixelDirection = this->camera.imagePlanePoint - this->camera.eyePoint;
            float u = static_cast<float>(left + ((right - left) * (i + 0.5) / horizontalPixel));
            float v = static_cast<float>(bottom + ((top - bottom) * (j + 0.5) / verticalPixel));
            pixelDirection += u * this->camera.u;
            pixelDirection += v * this->camera.v;
            pixelDirection.normalize();
            // 计算该像素的位置
            Vector3f pixelPoint = this->camera.imagePlanePoint;
            pixelPoint += u * this->camera.u;
            pixelPoint += v * this->camera.v;
            // 计算像素点的颜色
            Vector3f pixelColor = this->rayColor(this->camera.eyePoint, pixelDirection,
                                                 (pixelPoint - this->camera.eyePoint).dot(pixelDirection), FLT_MAX);
            matR(i, j) = pixelColor(0);
            matG(i, j) = pixelColor(1);
            matB(i, j) = pixelColor(2);
        }
    }

    cv::Mat matRGB(verticalPixel, horizontalPixel, CV_32FC3);
    this->convertToMat(matR, matG, matB, matRGB);
    return matRGB;
}

RayTracer::RayTracer(Scene &s, Camera &c) : scene(s), camera(c) {
    ;
}