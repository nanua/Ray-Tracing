#include <iostream>
#include <Eigen/Dense>

using namespace Eigen;

int main() {
    Vector3f v1;
    Vector3f v2;
    v1.setConstant(1);
    v2(0) = 1;
    v2(1) = 2;
    v2(2) = 3;
    v2 = v2.array() - 1;
    std::cout << std::max({1, 2, 3}) << std::endl;
}