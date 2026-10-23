#include "tasks/buff_detector.hpp"
#include <chrono>
#include <nlohmann/json.hpp>
#include <opencv2/opencv.hpp>
#include "tools/plotter.hpp"
int main()
{
    cv::VideoCapture cap("assets/test.avi"); 
    
    if (!cap.isOpened()) {
        std::cerr << "Could not open the video file!" << std::endl;
        return -1;
    }
    
    auto_buff::Buff_Detector detector;
    tools::Plotter plotter;
    while(true){
        cv::Mat img;
        cap >> img; 
        
        if (img.empty()) {
            std::cout << "The video is over, or a frame could not be read" << std::endl;
            break;
        }
        
        auto fanblades = detector.detect(img);
        
        cv::Mat display_img = img.clone();
        for (const auto& fanblade : fanblades) {
            cv::Scalar color;
            std::string type_name;
            switch (fanblade.type) {
                case auto_buff::_target:
                    color = cv::Scalar(0, 255, 0);  
                    type_name = "_target";
                    break;
                case auto_buff::_light:
                    color = cv::Scalar(0, 255, 255); 
                    type_name = "_light";
                    break;
                case auto_buff::_unlight:
                    color = cv::Scalar(0, 0, 255);  
                    type_name = "_unlight";
                    break;
            }
            
            // Draw the key points
            for (size_t i = 0; i < fanblade.points.size(); ++i) {
                cv::circle(display_img, fanblade.points[i], 3, color, -1);
                cv::putText(display_img, std::to_string(i), 
                           cv::Point(fanblade.points[i].x + 5, fanblade.points[i].y - 5),
                           cv::FONT_HERSHEY_SIMPLEX, 0.5, color, 1);
            }
            
            // Draw the center point
            cv::circle(display_img, fanblade.center, 5, color, -1);
            cv::putText(display_img, "CENTER", 
                       cv::Point(fanblade.center.x + 10, fanblade.center.y - 10),
                       cv::FONT_HERSHEY_SIMPLEX, 0.5, color, 1);
            
            // Draw the type label
            cv::putText(display_img, type_name, 
                       cv::Point(fanblade.center.x - 20, fanblade.center.y - 20),
                       cv::FONT_HERSHEY_SIMPLEX, 0.7, color, 2);
        }
        
        // Show what was found
        cv::resize(display_img,display_img,{},0.8,0.8);
        cv::imshow("Detection Results", display_img);
        if (cv::waitKey(30) == 27) { // press ESC to quit
            break;
        }

        // plotjuggler
        nlohmann::json data;
        if(fanblades.size())
        {
            data["fanblade_point"] = fanblades[0].points.size();
        }
        plotter.plot(data);

    }
    
    cap.release();
    cv::destroyAllWindows();
    return 0;
}