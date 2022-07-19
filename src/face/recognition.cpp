#include "face/recognition.h"
#include "face/recognizer.h"

#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <experimental/filesystem>
#include <iostream>

Recognition::Recognition()
{
    recognizer_
        = std::make_unique<Recognizer>();
}

Recognition::~Recognition()
{
}

void Recognition::trackStart(const TrackId& track_id)
{
    std::cout 
        << " - Recognition::trackStart:\n" 
        << "   >> track_id: " << track_id<< "\n";
}

void Recognition::trackEnd(const TrackId& track_id)
{
    std::cout 
        << " - Recognition::trackEnd:\n" 
        << "   >> track_id: " << track_id<< "\n";
}

void Recognition::bestShot(const cv::Mat& frame, const DetectionDescr& descr)
{
    std::cout 
        << " - Recognition::bestShot:\n" 
        << "   >> track_id: " << descr.track_id << "\n"
        << "   >> rectangle: " << descr.rectangle.width 
                      << " x " << descr.rectangle.height << "\n"
        << "   >> score: " << descr.score << "\n";

    cv::Rect mask
        = descr.rectangle;

    std::vector<cv::Point2i> landmarks 
        = recognizer_
            ->recognize(frame, mask);

    for (const auto & landmark : landmarks) {
        cv::circle(frame, landmark, 2, cv::Scalar(0,255,0), 1);
    }

    cv::Mat detection 
        = frame(mask);

    std::experimental::filesystem
        ::create_directory("./detections");

    std::stringstream filename;
    filename << "detections/" 
        << descr.track_id << "_detection_" 
        << descr.score << ".png";

    cv::imwrite(
        filename.str(),
        detection);
}



