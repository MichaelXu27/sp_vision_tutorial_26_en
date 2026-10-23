#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
#include <cmath>

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        std::cerr << "Usage: " << argv[0] << " <input_image>\n";
        return 1;
    }
    std::string inPath = argv[1];
    std::cout << "Input image path: " << inPath;
    // Load the picture
    cv::Mat bgr_img = cv::imread(inPath);
    if (bgr_img.empty())
    {
        std::cout << "fail to load image" << std::endl;
        return -1;
    }
    // Turn the picture gray (no color)
    cv::Mat gray_img;
    cv::cvtColor(bgr_img, gray_img, cv::COLOR_BGR2GRAY);
    cv::imshow("1_gray_img", gray_img);
    // Turn every pixel pure black or pure white
    double threshold_ = 200;
    cv::Mat binary_img;
    cv::threshold(gray_img, binary_img, threshold_, 255, cv::THRESH_BINARY);
    cv::imshow("2_binary_img", binary_img);
    // Find the outline of each shape
    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(binary_img, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_NONE);
    cv::Mat contour_img = bgr_img.clone();
    cv::drawContours(contour_img, contours, -1, cv::Scalar(0, 0, 255), 6);
    cv::imshow("3_contours", contour_img);
    // Settings
    double min_lightbar_ratio_ = 2.0;  // lowest allowed length / width
    double max_lightbar_ratio_ = 9.0;  // highest allowed length / width
    double min_lightbar_length_ = 5.0; // shortest allowed length

    // Look at each outline one by one
    for (const auto &contour : contours)
    {
        if (contour.size() < 5)
            continue; // skip outlines that are too small
        auto rect = cv::minAreaRect(contour);

        // Work out the length and width
        float width = std::min(rect.size.width, rect.size.height);
        float height = std::max(rect.size.width, rect.size.height);
        float ratio = height / width;

        // Keep only shapes that are long and thin enough
        if (ratio > min_lightbar_ratio_ && ratio < max_lightbar_ratio_ &&
            height > min_lightbar_length_)
        {
            // Decide the color: is there more blue or more red?
            int red_sum = 0, blue_sum = 0;
            for (const auto &p : contour)
            {
                auto pix = bgr_img.at<cv::Vec3b>(p);
                red_sum += pix[2];
                blue_sum += pix[0];
            }

            // Draw the tilted box around it
            cv::Point2f vertices[4];
            rect.points(vertices);
            for (int i = 0; i < 4; i++)
            {
                cv::line(bgr_img, vertices[i], vertices[(i + 1) % 4], cv::Scalar(0, 0, 255), 5);
            }
            std::cout << "Lightbar color: " << (blue_sum > red_sum ? "Blue" : "Red") << std::endl;
        }
    }

    // Show result
    cv::imshow("4_Lightbars", bgr_img);
    cv::waitKey(0);
    return 0;
}
