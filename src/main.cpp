#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include<face_detector.h>

#include <iostream>

int main()
{
    cv::namedWindow("detections", CV_WINDOW_AUTOSIZE);
    cv::Mat frame;// = cv::imread("input1.jpg");

    FaceDetector _detector;

    cv::VideoCapture cap = cv::VideoCapture("video1.mp4");
    if ( !cap.isOpened() )
    {
        std::cout << "Cannot open the video file. \n";
        return -1;
    }

    while (1) {

        cap >> frame;

        if (frame.empty()) {
            std::cout<<"\n Cannot read the video file. \n";
            break;
        }

        auto rectangles = _detector.detect(frame);
        for(const auto & r : rectangles) {
            cv::rectangle(frame, r, cv::Scalar(255, 0, 255), 2);
        }

        // Press ESC on keyboard to exit
        cv::imshow("detections", frame);
        char c=(char)cv::waitKey(25);
        if(c==27)
            break;
    }

    cap.release();
    cv::destroyAllWindows();
    return 0;
}
