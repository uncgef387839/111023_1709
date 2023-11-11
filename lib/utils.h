#include <Eigen/Dense>
#include <boost/math/tools/roots.hpp>
#include <cassert>
#include <iostream>

#define PI                                                                     \
    3.1415926535897932384626433832795028841971693993751058209749445923078164062

// from
// https://stackoverflow.com/questions/24469927/does-c-have-an-equivalent-to-nets-notimplementedexception
class NotImplemented : public std::logic_error {
public:
    NotImplemented() : std::logic_error("Function not yet implemented"){};
};
// using namespace Eigen;

// // cbrt function
// double cbrt(double x) {
//     if (x < 0) {
//         return -pow(-x, 1.0 / 3.0);
//     } else {
//         return pow(x, 1.0 / 3.0);
//     }
// }

// std::tuple<double, double, double>
// cubic_and_derivatives(double a, double b, double c, double d, double x) {
//     return std::make_tuple(a * x * x * x + b * x * x + c * x + d,
//                            3 * a * x * x + 2 * b * x + c, 6 * a * x + 2 * b);
// }

// // Solve cubic equation
// int solve_cubic(double a, double b, double c, double d, double *roots) {
//     // Using schroder's iterative from boost

//     auto f = [a, b, c, d](double x) {
//         return cubic_and_derivatives(a, b, c, d, x);
//     };

//     double root = boost::math::tools::schroder_iterate(f, 0.0, 0.0, 10.0,
//     10); roots[0] = root; return 1;
//     // std::cout << "root = " << root << std::endl;
//     // return root;
// }

// double polish_root(double unpolished_root, std::tuple<double, double, double>
// f,
//                    double margin, unsigned int precision) {
//     double root = boost::math::tools::schroder_iterate(
//         f, unpolished_root, unpolished_root - margin, unpolished_root +
//         margin, precision);
//     return root;
// }

// bool when_are_fourpts_coplanar(
//     const Eigen::Vector3d &x1, const Eigen::Vector3d &x2,
//     const Eigen::Vector3d &x3, const Eigen::Vector3d &x4,
//     const Eigen::Vector3d &v1, const Eigen::Vector3d &v2,
//     const Eigen::Vector3d &v3, const Eigen::Vector3d &v4) {
//     // Finding roots of the cubic equation:
//     //      (x_21 + tv_21) x (x31 + tv_31) . (x_41 + tv_41) = 0
//     // where x_ij = p_i - p_j and v_ij = v_i - v_j

//     // Compute x_ij and v_ij
//     Eigen::Vector3d x_21 = x2 - x1;
//     Eigen::Vector3d x_31 = x3 - x1;
//     Eigen::Vector3d x_41 = x4 - x1;
//     // print out x_21, x_31, x_41
//     // std::cout << "x_21\n " << x_21 << std::endl;
//     // std::cout << "x_31\n " << x_31 << std::endl;
//     // std::cout << "x_41\n " << x_41 << std::endl;

//     Eigen::Vector3d v_21 = v2 - v1;
//     Eigen::Vector3d v_31 = v3 - v1;
//     Eigen::Vector3d v_41 = v4 - v1;
//     // print out v_21, v_31, v_41
//     // std::cout << "v_21\n " << v_21 << std::endl;
//     // std::cout << "v_31\n " << v_31 << std::endl;
//     // std::cout << "v_41\n " << v_41 << std::endl;

//     // Compute coefficients of cubic equation
//     // Explanation of coefficient calculation from
//     //      (x_21 + tv_21) x (x31 + tv_31) . (x_41 + tv_41) = 0:

//     //

//     double a = v_21.dot(v_31.cross(v_41));
//     double b = v_21.dot(v_31.cross(x_41) + x_31.cross(v_41) +
//     v_41.cross(x_31)); double c = v_21.dot(x_31.cross(x_41)) +
//     x_21.dot(v_31.cross(x_41)) +
//                x_21.dot(x_31.cross(v_41));
//     double d = x_21.dot(x_31.cross(x_41));

//     // print coefficients
//     std::cout << "a = " << a << ", b = " << b << ", c = " << c << ", d = " <<
//     d
//               << std::endl;

//     // Solve cubic equation
//     double roots[3];

//     int num_roots = solve_cubic(a, b, c, d, roots);

//     // print all roots
//     std::cout << "roots = " << roots[0] << std::endl;

//     return num_roots;
// }

Eigen::Vector3d unit_triangle_normal(const Eigen::Vector3d &t1,
                                     const Eigen::Vector3d &t2,
                                     const Eigen::Vector3d &t3) {
    const Eigen::Vector3d v12 = t2 - t1;
    const Eigen::Vector3d v13 = t3 - t1;
    return v12.cross(v13).normalized();
}
