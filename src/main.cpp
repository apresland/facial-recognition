#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include<face_detector.h>

#include <iostream>

int main()
{
    cv::namedWindow("detections", CV_WINDOW_AUTOSIZE);
    //cv::Mat frame = cv::imread("input1.jpg");
    cv::TickMeter timeRecorder_;

    FaceDetector _detector;

    cv::VideoCapture cap = cv::VideoCapture("2021-12-25 14-26-21_ET879_cut_2.mp4");
    if ( !cap.isOpened() )
    {
        std::cout
         << "Cannot open the video file. \n";
        return -1;
    }

    int N{0};
    int frame_width = cap.get(cv::CAP_PROP_FRAME_WIDTH);
    int frame_height = cap.get(cv::CAP_PROP_FRAME_HEIGHT);
    cv::VideoWriter video("output.mp4", 10, 17, cv::Size(frame_width, frame_height));

    while (1) {

        if (gLOGGING) {
            std::cout
                << "\n Processing Frame: " << ++N << "\n";
            timeRecorder_.reset();
            timeRecorder_.start();
        }

        cv::Mat frame;
        cap >> frame;

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

        cv::Mat normalized_frame;
        normalize(frame, normalized_frame, 0, 255, cv::NORM_MINMAX);
        auto rectangles = _detector.detect(normalized_frame);
        
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
            cv::rectangle(normalized_frame, r, cv::Scalar(255, 0, 255), 2);
        }

        video.write(normalized_frame);
        cv::imshow("detections", normalized_frame);
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
