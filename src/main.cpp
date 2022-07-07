#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include<face_detector.h>

#include <iostream>

int main()
{
    cv::namedWindow("detections", CV_WINDOW_AUTOSIZE);
    cv::TickMeter timeRecorder_;

    cv::VideoWriter video 
        = cv::VideoWriter(
            "output.mp4", 10, 17, 
            cv::Size(mask_width, mask_height));

    cv::VideoCapture cap 
        = cv::VideoCapture(
            "2021-12-25 14-26-21_ET879_cut_3.mp4");

    if ( !cap.isOpened() )
    {
        std::cout
         << "Cannot open the video file. \n";
        return -1;
    }

    int N{0};
    int frame_width = cap.get(cv::CAP_PROP_FRAME_WIDTH);
    int frame_height = cap.get(cv::CAP_PROP_FRAME_HEIGHT);

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


    FaceDetector _detector;

    while (1) {

        if (gLOGGING) {
            std::cout
                << "\n Processing Frame: " << ++N << " (" << frame_width << "x" << frame_height <<  ")\n";
            timeRecorder_.reset();
            timeRecorder_.start();
        }

        cv::Mat frame;
        cap >> frame;
        cv::Mat cropped_frame 
            = frame(mask);

        if (gLOGGING) {        
            timeRecorder_.stop();
            std::cout 
                << " - capture took: "
                << timeRecorder_.getTimeMilli()
                << "ms" << std::endl;
        }

        if (frame.empty()) {
            std::cout
             << "\n Cannot read the video file. \n";
            break;
        }

        if (gLOGGING) {
            timeRecorder_.reset();
            timeRecorder_.start();
        }

        auto rectangles = _detector.detect(cropped_frame);
        
        if (gLOGGING) {
            timeRecorder_.stop();
            std::cout 
                << " - face detection took: "
                << timeRecorder_.getTimeMilli()
                << "ms" << std::endl;
        }

        if (gLOGGING) {
            timeRecorder_.reset();
            timeRecorder_.start();
        }

        for(const auto & r : rectangles) {
            cv::rectangle(cropped_frame, r, cv::Scalar(255, 0, 255), 2);
        }

        video.write(cropped_frame);
        cv::imshow("detections", cropped_frame);

        if (gLOGGING) {
            timeRecorder_.stop();
            std::cout 
                << " - visualization took: "
                << timeRecorder_.getTimeMilli()
                << "ms" << std::endl;
        }

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
