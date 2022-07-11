#pragma once

#include <vector>
#include <opencv2/dnn.hpp>

class FaceDetector
{
private:
    /// Face detection network
    cv::dnn::Net network_;
    /// Required DNN input image size
    const cv::Size input_size_{300, 300};
    /// Required DNN input image blob scale
    const double scale_factor_{1.0};
    /// Mean normalization values network was trained with  
    const cv::Scalar mean_values_{104.0, 177.0, 123.0};
    /// Face detection confidence threshold
    const float confidence_threshold_ {0.5f};

    /// Benchmarking timer
    cv::TickMeter timeRecorder_;

public:
    explicit FaceDetector();
    std::vector<cv::Rect> detect(const cv::Mat& frame);
};
