#pragma once

#include <opencv2/imgproc.hpp>

class FacePreprocessor
{
private:
    cv::Mat equalizeHistogramAdaptive(const cv::Mat& inputImage);
public:
    FacePreprocessor() = default;
    ~FacePreprocessor() = default;

    cv::Mat preprocess(const cv::Mat& frame);
};


