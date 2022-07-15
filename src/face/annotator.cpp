#include "face/annotator.h"
#include "face/context.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <limits>
#include <opencv2/imgproc.hpp>


cv::Mat FaceAnnotator::annotate(const cv::Mat& input, std::vector<DetectionDescr>& detections)
{
    if (gLOGGING) {
        timeRecorder_.reset();
        timeRecorder_.start();
    }

    cv::Mat output
        = input.clone();

    int font = cv::FONT_HERSHEY_SIMPLEX;
    int font_thickness = 1;
    int font_baseline = 0;
    double font_scale = 1.0;

    cv::Size text_size = cv::getTextSize(
        "000: 0.00", font, 
        font_scale, 
        font_thickness,
        &font_baseline);

    for (const auto & detection : detections) {

        std::stringstream sstream;
        sstream 
        << detection.track_id 
        <<  ": "
        << std::fixed << std::setprecision(2) 
        << detection.score;

        std::string description_text = sstream.str();

        cv::Point origin(detection.rectangle.x, detection.rectangle.y);

        cv::rectangle(
            output, 
            origin + cv::Point(0, font_baseline),
            origin + cv::Point(text_size.width, -text_size.height), 
            cv::Scalar(0, 255, 0), CV_FILLED);

        cv::rectangle(
            output, detection.rectangle, 
            cv::Scalar(0, 255, 0), 2);

        cv::putText(
            output, description_text,
            cv::Point(detection.rectangle.x,
                      detection.rectangle.y),
            cv::FONT_HERSHEY_SIMPLEX, 1,
            cv::Scalar(255, 255, 255), 2);
    }

    if (gLOGGING) {
        timeRecorder_.stop();
        std::cout 
            << " - annotation took: "
            << timeRecorder_.getTimeMilli()
            << "ms" << std::endl;
    }

    return output;
}
