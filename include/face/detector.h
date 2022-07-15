#pragma once

#include "face/types.h"
#include <opencv2/dnn.hpp>
#include <vector>
#include <future>
#include <thread>

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
    /// Face detection future (async)
    std::future<std::vector<Detection>> detections_future_;
    /// Benchmarking timer
    cv::TickMeter timeRecorder_;

    std::vector<Detection> detect(const cv::Mat& frame);

public:
    explicit FaceDetector();

    void detectAsync(const cv::Mat& frame);
    std::vector<Detection> getAsync();
};
