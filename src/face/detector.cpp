#include "face/detector.h"
#include "face/context.h"
#include <iostream>

FaceDetector::FaceDetector() 
{
    network_ 
        = cv::dnn::readNetFromCaffe(
            FACE_DETECTION_CONFIGURATION,
            FACE_DETECTION_WEIGHTS);

    if (network_.empty()) {
        std::ostringstream ss;
        ss << "Failed to load network with the following settings:\n"
           << "Configuration: " + std::string(FACE_DETECTION_CONFIGURATION) + "\n"
           << "Binary: " + std::string(FACE_DETECTION_WEIGHTS) + "\n";
        throw std::invalid_argument(ss.str());
    }
}

void FaceDetector::detectAsync(const cv::Mat& frame)
{
    detections_future_
        = std::async(std::launch::async, [&](){
            return detect(frame);
            });
}

std::vector<Detection> FaceDetector::getAsync() 
{
    detections_future_.wait();
    return detections_future_.get();   
}

std::vector<Detection> FaceDetector::detect(const cv::Mat& frame)
{
    if (gLOGGING) {
        timeRecorder_.reset();
        timeRecorder_.start();
    }

    int height = frame.rows;
    int width = frame.cols;

    cv::Mat input_blob = cv::dnn::blobFromImage(
        frame, 
        scale_factor_, 
        input_size_,
        mean_values_,
        false,
        false);

    network_.setInput(input_blob);
    cv::Mat detection 
        = network_.forward(); 

    cv::Mat detection_matrix(
        detection.size[2],
        detection.size[3], 
        CV_32F, 
        detection.ptr<float>());

    std::vector<Detection> detections;    
    for (int i = 0; i < detection_matrix.rows; i++) {

        float confidence = detection_matrix.at<float>(i, 2);
        if (confidence < DETECTION_THRESHOLD) {
            continue;
        }

        int xbl = static_cast<int>(detection_matrix.at<float>(i, 3) * frame.cols);
        int ybl = static_cast<int>(detection_matrix.at<float>(i, 4) * frame.rows);
        int xtr = static_cast<int>(detection_matrix.at<float>(i, 5) * frame.cols);
        int ytr = static_cast<int>(detection_matrix.at<float>(i, 6) * frame.rows);
        cv::Rect2d rectangle(xbl, ybl, (xtr - xbl), (ytr - ybl));

        Detection detection;
        detection.rectangle = rectangle;
        detection.score = confidence;
        detections.emplace_back(detection);
    }
    
    if (gLOGGING) {
        timeRecorder_.stop();
        std::cout 
            << " - detection took: "
            << timeRecorder_.getTimeMilli()
            << "ms" << std::endl;
    }

    return detections;
}