#include "face/track_observer.h"

#include <iostream>

void TrackObserver::bestShot(const DetectionDescr& descr)
{
    std::cout 
        << " - TrackObserver::bestShot:\n" 
        << "   >> TrackId: " << descr.track_id << "\n"
        << "   >> Rectangle: " << descr.rectangle.width << " x " << descr.rectangle.height << "\n";
}

void TrackObserver::trackEnd(const TrackId& track_id)
{
    std::cout 
        << " - TrackObserver::trackEnd:\n" 
        << "   >> TrackId: " << track_id<< "\n";
}