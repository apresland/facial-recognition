#pragma once

#include "face/types.h"

class ITrackObserver 
{
public:
    virtual void bestShot(const DetectionDescr& descr) = 0;
    virtual void trackEnd(const TrackId& trackId) = 0;
};

class TrackObserver : public ITrackObserver
{
private:
    
public:
    TrackObserver() = default;
    ~TrackObserver() = default;

    void bestShot(const DetectionDescr& descr) override;
    void trackEnd(const TrackId& track_id) override;
};


