#include "face/face_context.h"
#include "face/face_node.h"
#include <iostream>
#include <opencv2/highgui.hpp>

int main()
{
    const int mask_x = 500;
    const int mask_y = 50;
    const int mask_width = 1000;
    const int mask_height = 1000;
    const cv::Rect mask(
        mask_x,
        mask_y,
        mask_width,
        mask_height
        );

    cv::namedWindow("detections", CV_WINDOW_AUTOSIZE);
    cv::TickMeter timeRecorder_;
    cv::VideoCapture cap 
        = cv::VideoCapture(
            "2021-12-25 14-26-21_ET879_cut_2.mp4");

    if ( !cap.isOpened() )
    {
        std::cout
         << "Cannot open the video file. \n";
        return -1;
    }

    cv::VideoWriter video (
            "output.avi", 
            cv::VideoWriter::fourcc('M', 'J', 'P', 'G'),
            30, 
            cv::Size(mask_width, mask_height)
            );

    int frame_width = cap.get(cv::CAP_PROP_FRAME_WIDTH);
    int frame_height = cap.get(cv::CAP_PROP_FRAME_HEIGHT);

    std::queue<cv::Mat> _image_input;
    std::queue<cv::Mat> _image_output;
    FaceNode _node(_image_input, _image_output);

    while (1) {

        cv::Mat frame;
        cap >> frame;

        _image_input.push(frame(mask));

        // ----------------------------------------------
        // Log frame info
        // ----------------------------------------------

        if (gLOGMAIN) {
            timeRecorder_.reset();
            timeRecorder_.start();
            std::cout
                << "\n Processing Frame:("  
                << frame_width << " x " 
                << frame_height <<  ")\n";
        }

        _node.spin_once();

        // ----------------------------------------------
        // Log performance
        // ----------------------------------------------

        if (gLOGMAIN) {
            timeRecorder_.stop();
            std::cout 
                << " - total time: "
                << timeRecorder_.getTimeMilli()
                << "ms"
                << std::endl;
        }

        // ----------------------------------------------
        // Visualize face detections
        // ----------------------------------------------

        if ( ! _image_output.empty()) {
            cv::Mat annotated = _image_output.front();
            video.write(annotated);
            cv::imshow("detections", annotated);
            _image_output.pop();
        }
        // ----------------------------------------------
        // Check for quit signal
        // ----------------------------------------------

        // Press ESC on keyboard to exit
        char c=(char)cv::waitKey(1);
        if(c==27)
            break;
    }

    cap.release();
    video.release();
    cv::destroyAllWindows();
    return 0;
}
