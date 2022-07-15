#pragma once

#include "face/types.h"

class ITrackObserver 
{
public:
    virtual void trackStart(const TrackId& trackId) = 0;
    virtual void trackEnd(const TrackId& trackId) = 0;
    virtual void bestShot(const DetectionDescr& descr) = 0;
};

class TrackObserver : public ITrackObserver
{
private:
    
public:
    TrackObserver() = default;
    ~TrackObserver() = default;

    void trackStart(const TrackId& trackId) override;
    void trackEnd(const TrackId& track_id) override;
    void bestShot(const DetectionDescr& descr) override;
};


