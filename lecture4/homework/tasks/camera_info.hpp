#ifndef AUTO_BUFF__CAMERA_INFO_HPP
#define AUTO_BUFF__CAMERA_INFO_HPP

#include <opencv2/opencv.hpp>

namespace auto_buff
{
// clang-format off
// Camera settings to use with assets/test.avi (1440 x 1080 Hikrobot camera).
// These come from the original team's Power Rune test settings (configs/sentry.yaml in
// TongjiSuperPower/sp_vision_25). We don't know for sure that this exact camera recorded
// test.avi, so treat the distances you get as approximate. The plots will still work.
//
// If you test with your own camera, calibrate it and replace these numbers.
static const cv::Mat camera_matrix =
    (cv::Mat_<double>(3, 3) << 2414.9359264386621, 0,                  717.26243105567414,
                               0,                  2418.0489262208148, 582.68540529942845,
                               0,                  0,                  1);
// Lens distortion numbers (how much the lens bends the picture)
static const cv::Mat distort_coeffs =
    (cv::Mat_<double>(1, 5) << -0.0209453389287673, 0.15028138841073832, -0.0006517722113234505, -0.0016861906197686788, 0);
// clang-format on
}  // namespace auto_buff

#endif  // AUTO_BUFF__CAMERA_INFO_HPP
