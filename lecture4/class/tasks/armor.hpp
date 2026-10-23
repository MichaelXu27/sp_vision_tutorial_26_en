#ifndef AUTO_AIM__ARMOR_HPP
#define AUTO_AIM__ARMOR_HPP

#include <opencv2/opencv.hpp>
#include <string>
#include <vector>

namespace auto_aim
{
enum Color
{
  red,
  blue,
  purple
};
const std::vector<std::string> COLORS = {"red", "blue", "purple"};

enum ArmorType
{
  big,
  small
};
const std::vector<std::string> ARMOR_TYPES = {"big", "small"};

enum ArmorName
{
  one,
  two,
  three,
  four,
  five,
  sentry,
  outpost,
  base,
  not_armor
};
const std::vector<std::string> ARMOR_NAMES = {"one",    "two",     "three", "four",     "five",
                                              "sentry", "outpost", "base",  "not_armor"};

enum ArmorPriority
{
  first = 1,
  second,
  third,
  forth,
  fifth
};

struct Lightbar
{
  std::size_t id;
  Color color;
  cv::Point2f center, top, bottom, top2bottom;
  std::vector<cv::Point2f> points;
  double angle, angle_error, length, ratio;

  Lightbar(const cv::RotatedRect & rotated_rect, std::size_t id)
  {
    std::vector<cv::Point2f> corners(4);
    rotated_rect.points(&corners[0]);
    std::sort(corners.begin(), corners.end(), [](const cv::Point2f & a, const cv::Point2f & b) {
      return a.y < b.y;
    });

    center = rotated_rect.center;
    top = (corners[0] + corners[1]) / 2;
    bottom = (corners[2] + corners[3]) / 2;
    top2bottom = bottom - top;

    points.emplace_back(top);
    points.emplace_back(bottom);

    auto width = cv::norm(corners[0] - corners[1]);
    angle = std::atan2(top2bottom.y, top2bottom.x);
    angle_error = std::abs(angle - CV_PI / 2);
    length = cv::norm(top2bottom);
    ratio = length / width;
  };
};

struct Armor
{
  Color color;
  const Lightbar left, right;
  cv::Point2f center;       // NOT where the diagonals cross, so do not use it as the real center!
  cv::Point2f center_norm;  // position scaled to 0..1
  std::vector<cv::Point2f> points;

  double ratio;              // distance between the middles of the two light bars, divided by the longer bar's length
  double side_ratio;         // longer light bar length / shorter light bar length
  double rectangular_error;  // how far the angle between a light bar and the line joining the middles is from 90 degrees

  ArmorType type;
  ArmorName name;
  ArmorPriority priority;
  cv::Mat pattern;
  double confidence;
  bool duplicated;

  double yaw_raw;  // rad

  Armor(const Lightbar & left, const Lightbar & right) : left(left), right(right)
  {
    color = left.color;
    center = (left.center + right.center) / 2;

    points.emplace_back(left.top);
    points.emplace_back(right.top);
    points.emplace_back(right.bottom);
    points.emplace_back(left.bottom);

    auto left2right = right.center - left.center;
    auto width = cv::norm(left2right);
    auto max_lightbar_length = std::max(left.length, right.length);
    auto min_lightbar_length = std::min(left.length, right.length);
    ratio = width / max_lightbar_length;
    side_ratio = max_lightbar_length / min_lightbar_length;

    auto roll = std::atan2(left2right.y, left2right.x);
    auto left_rectangular_error = std::abs(left.angle - roll - CV_PI / 2);
    auto right_rectangular_error = std::abs(right.angle - roll - CV_PI / 2);
    rectangular_error = std::max(left_rectangular_error, right_rectangular_error);
  };
};

}  // namespace auto_aim

#endif  // AUTO_AIM__ARMOR_HPP