#pragma once

#include <vector>
#include <opencv2/dnn.hpp>

class FaceDetector
{
private:
    /// Face detection network
    cv::dnn::Net network_;
    /// Required DNN input image width
    const int input_image_width_{300};
    /// Required DNN input image height
    const int input_image_height_{300};
    /// Required DNN input image blob scale
    const double scale_factor_{1.0};
    /// Mean normalization values network was trained with  
    const cv::Scalar mean_values_{104.0, 177.0, 123.0};
    /// Face detection confidence threshold
    const float confidence_threshold_ {0.6f};

public:
    explicit FaceDetector();
    std::vector<cv::Rect> detect(const cv::Mat& frame);
};
