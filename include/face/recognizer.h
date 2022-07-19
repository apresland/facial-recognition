#pragma once

#include <vector>
#include <opencv2/core.hpp>
#include <dlib/image_processing/frontal_face_detector.h>
#include <dlib/image_processing/render_face_detections.h>
#include <dlib/image_processing.h>

class Recognizer 
{
private:
    dlib::shape_predictor predictor_;
public:
    explicit Recognizer();
    ~Recognizer() = default;

    std::vector<cv::Point2f> recognize(const cv::Mat& frame, const cv::Rect& rect);
};