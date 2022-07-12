#include "face_annotator.h"
#include "face_context.h"

#include <iostream>

#include <opencv2/imgproc.hpp>

cv::Mat FaceAnnotator::annotate(const cv::Mat& input_frame, std::vector<cv::Rect2d>& detections)
{
    if (gLOGGING) {
        timeRecorder_.reset();
        timeRecorder_.start();
    }

    cv::Mat output_frame 
        = input_frame.clone();

    for (const auto & detection : detections) {
        cv::rectangle(
            output_frame, detection, 
            cv::Scalar(0, 255, 0), 2);
    }

    if (gLOGGING) {
        timeRecorder_.stop();
        std::cout 
            << " - annotation took: "
            << timeRecorder_.getTimeMilli()
            << "ms" << std::endl;
    }

    return output_frame;
}