#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include<face_detector.h>

int main()
{
    cv::namedWindow("detections", CV_WINDOW_AUTOSIZE);
    cv::Mat frame = cv::imread("input1.jpg");
  
    FaceDetector _detector;
    auto rectangles = _detector.detect(frame);
    for(const auto & r : rectangles) {
        cv::rectangle(frame, r, cv::Scalar(255, 0, 255), 2);
    }

    //cv::imwrite("output.jpg", frame);
    cv::imshow("detections", frame);
    cv::waitKey(0);
}
