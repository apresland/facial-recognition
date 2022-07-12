#pragma once

#include <vector>
#include <future>
#include <thread>

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
    /// Face detection future (async)
    std::future<std::vector<cv::Rect2d>> detections_future_;


    /// Benchmarking timer
    cv::TickMeter timeRecorder_;


    std::vector<cv::Rect2d> detect(const cv::Mat& frame);

public:
    explicit FaceDetector();

    void detectAsync(const cv::Mat& frame);
    std::vector<cv::Rect2d> getAsync();
};
