#pragma once	

#include <opencv2/core.hpp>

using TrackId = uint32_t;
using FrameId = uint32_t;

struct Frame
{
    FrameId frame_id;
    cv::Mat image;
};

struct Detection
{
    cv::Rect2d rectangle;
    float score{0.f};
};

struct TrackInfo
{
    TrackId track_id;
    cv::Rect2d rectangle;
    float detection_score{0.f};
};

struct DetectionsDescr
{
    TrackId track_id;
    cv::Rect2d rectangle;
    float score{0.f};
};


