#pragma once

#include <opencv2/imgproc.hpp>

class FacePreprocessor
{
private:
    cv::Mat equalizeIntensity(const cv::Mat& inputImage);
public:
    FacePreprocessor() = default;
    ~FacePreprocessor() = default;

    cv::Mat preprocess(const cv::Mat& frame);
};


