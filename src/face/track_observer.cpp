#include "face/track_observer.h"

#include <experimental/filesystem>
#include <iostream>

TrackObserver::TrackObserver () 
{

}

void TrackObserver::trackStart(const TrackId& track_id)
{
    std::cout 
        << " - TrackObserver::trackStart:\n" 
        << "   >> track_id: " << track_id<< "\n";
}

void TrackObserver::trackEnd(const TrackId& track_id)
{
    std::cout 
        << " - TrackObserver::trackEnd:\n" 
        << "   >> track_id: " << track_id<< "\n";
}

void TrackObserver::bestShot(const cv::Mat& frame, const DetectionDescr& descr)
{
    std::cout 
        << " - TrackObserver::bestShot:\n" 
        << "   >> track_id: " << descr.track_id << "\n"
        << "   >> rectangle: " << descr.rectangle.width 
                      << " x " << descr.rectangle.height << "\n"
        << "   >> score: " << descr.score << "\n";

            cv::Rect mask
                = descr.rectangle;
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
