#include "face/track_observer.h"

#include <iostream>

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

void TrackObserver::bestShot(const DetectionDescr& descr)
{
    std::cout 
        << " - TrackObserver::bestShot:\n" 
        << "   >> track_id: " << descr.track_id << "\n"
        << "   >> rectangle: " << descr.rectangle.width << " x " << descr.rectangle.height << "\n"
        << "   >> score: " << descr.score << "\n";
}
