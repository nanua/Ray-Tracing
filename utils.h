//
// Created by Yuhong Zhong on 2019-04-17.
//

#ifndef RAY_TRACING_UTILS_H
#define RAY_TRACING_UTILS_H

#include <iostream>
#include <Eigen/Dense>

using namespace std;
using namespace Eigen;

void printVector3f(Vector3f &vec) {
    size_t vectorSize = vec.size();
    for (size_t index = 0; index < vectorSize; ++index) {
        cout << vec[index];
        if (index != vectorSize - 1) {
            cout << ",";
        }
    }
    cout << endl;
}

#endif //RAY_TRACING_UTILS_H
