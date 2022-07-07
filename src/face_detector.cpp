#include <face_detector.h>
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

std::vector<cv::Rect> FaceDetector::detect(const cv::Mat& frame)
{
    cv::Mat input_blob = cv::dnn::blobFromImage(
        frame, 
        scale_factor_, 
        input_size_,
        mean_values_,
        false,
        false);

    network_.setInput(input_blob);

    timeRecorder_.reset();
    timeRecorder_.start();
    cv::Mat detection = network_.forward(); 
    timeRecorder_.stop();

    if (gLOGGING) {
        std::cout 
            << " - forward propogation took: " 
            << timeRecorder_.getTimeMilli()
            << "ms" << std::endl;
    }

    cv::Mat detection_matrix(
        detection.size[2],
        detection.size[3], 
        CV_32F, 
        detection.ptr<float>());

    std::vector<cv::Rect> rectangles;    
    for (int i = 0; i < detection_matrix.rows; i++) {

        float confidence = detection_matrix.at<float>(i, 2);
        if (confidence < confidence_threshold_) {
            continue;
        }

        int xbl = static_cast<int>(detection_matrix.at<float>(i, 3) * frame.cols);
        int ybl = static_cast<int>(detection_matrix.at<float>(i, 4) * frame.rows);
        int xtr = static_cast<int>(detection_matrix.at<float>(i, 5) * frame.cols);
        int ytr = static_cast<int>(detection_matrix.at<float>(i, 6) * frame.rows);

        rectangles.emplace_back(xbl, ybl, (xtr - xbl), (ytr - ybl));
    }
    
    return rectangles;
}