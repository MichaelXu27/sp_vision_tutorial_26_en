#include "tasks/detector.hpp"
#include "tools/img_tools.hpp"
#include "fmt/core.h"

// clang-format off
//  Camera settings that describe its lens (camera matrix)
static const cv::Mat camera_matrix =
    (cv::Mat_<double>(3, 3) <<  1286.307063384126 , 0                  , 645.34450819155256, 
                                0                 , 1288.1400736562441 , 483.6163720308021 , 
                                0                 , 0                  , 1                   );
// Lens distortion numbers (how much the lens bends the picture)
static const cv::Mat distort_coeffs =
    (cv::Mat_<double>(1, 5) << -0.47562935060124745, 0.21831745829617311, 0.0004957613589406044, -0.00034617769548693592, 0);
// clang-format on

static const double LIGHTBAR_LENGTH = 0.056; // light bar length,   in meters
static const double ARMOR_WIDTH = 0.135;     // armor plate width,  in meters

// #### Task 01 ############################################
// object_points are the positions of n points, measured from the object itself.
// For us, that means the 4 corner points, measured from the center of the armor plate.
// Please fill in object_points below:

static const std::vector<cv::Point3f> object_points{
    {-ARMOR_WIDTH / 2, -LIGHTBAR_LENGTH / 2, 0}, // point 1
    {ARMOR_WIDTH / 2, -LIGHTBAR_LENGTH / 2, 0},  // point 2
    {ARMOR_WIDTH / 2, LIGHTBAR_LENGTH / 2, 0},   // point 3
    {-ARMOR_WIDTH / 2, LIGHTBAR_LENGTH / 2, 0}   // point 4
};

// Hints:
// - The armor plate positions are 3D, but all four points are flat on the plate (z = 0), so the four 0s are already filled in for you.
// - The light bar length and armor plate width are defined above. Write your answers like "± ARMOR_WIDTH / 2".
// #########################################################

int main(int argc, char *argv[])
{
    auto_aim::Detector detector;

    cv::VideoCapture cap("video.avi");
    cv::Mat img;

    while (true)
    {
        cap >> img;
        if (img.empty()) // could not read a frame, or the video is over
            break;

        auto armors = detector.detect(img);

        if (!armors.empty())
        {
            auto_aim::Armor armor = armors.front(); // if at least one armor plate was found, use the first one
            tools::draw_points(img, armor.points);  // draw the armor plate

            // #### Task 02 ############################################
            // img_points are the positions of n points in the picture, in pixels.
            // For us, that means where the 4 corners of the armor plate are in the picture.
            // Please fill in img_points below:
            //
            std::vector<cv::Point2f> img_points{
                armor.left.top,
                armor.right.top,
                armor.right.bottom,
                armor.left.bottom};
            //
            // Hints:
            // - Look at what is inside the Armor struct.
            // - armor.left and armor.right are the two light bars (Lightbar).
            // - Lightbar is also a struct. The top and bottom ends of each light bar are the points we want.
            // #########################################################

            // #### Task 03 ############################################
            cv::Mat rvec, tvec;
            // We now have everything we need. Call solvePnP to work out where the armor plate is and which way it faces.
            // rvec (rotation) and tvec (position) hold the answers from solvePnP.
            // Fill in the inputs for solvePnP below:
            //
            cv::solvePnP(object_points, img_points, camera_matrix, distort_coeffs, rvec, tvec);
            //
            // #########################################################

            // #### Task 04 ############################################
            // Right now, draw_text only prints 0.0
            // Change the draw_text inputs below so they print the tvec and rvec you found
            //
            tools::draw_text(img, fmt::format("tvec:  x{: .2f} y{: .2f} z{: .2f}", tvec.at<double>(0), tvec.at<double>(1), tvec.at<double>(2)), cv::Point2f(10, 60), 1.7, cv::Scalar(0, 255, 255), 3);
            tools::draw_text(img, fmt::format("rvec:  x{: .2f} y{: .2f} z{: .2f}", rvec.at<double>(0), rvec.at<double>(1), rvec.at<double>(2)), cv::Point2f(10, 120), 1.7, cv::Scalar(0, 255, 255), 3);
            
            //
            // Hints:
            // - tvec.at<double>(0) gives you the first number in tvec, as a double.
            // #########################################################

            // #### Task 05 ############################################
            // Use cv::Rodrigues to turn rvec (a rotation vector) into rmat (a rotation matrix).
            // Then use inverse trig functions (like atan2 and asin) to turn the numbers in rmat into yaw, pitch and roll angles, and show them on screen.
            //

            cv::Mat rmat;
            cv::Rodrigues(rvec, rmat);
            double yaw = atan2(rmat.at<double>(0, 2), rmat.at<double>(2, 2));
            double pitch = -asin(rmat.at<double>(1, 2));
            double roll = atan2(rmat.at<double>(1, 0), rmat.at<double>(1, 1));
            tools::draw_text(img, fmt::format("euler angles:  yaw{: .2f} pitch{: .2f} roll{: .2f}", yaw, pitch, roll), cv::Point2f(10, 180), 1.7, cv::Scalar(0, 255, 255), 3);
            //
            // Hints:
            // - cv::Mat counts from 0, not 1.
            // - You get numbers out of a cv::Mat the same way as tvec above. For example: rmat.at<double>(0, 2)
            // #########################################################
        }

        cv::imshow("press q to quit", img);

        if (cv::waitKey(20) == 'q')
            break;
    }

    cv::destroyAllWindows();
    return 0;
}