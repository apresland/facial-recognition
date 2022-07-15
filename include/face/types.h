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

struct DetectionDescr
{
    TrackId track_id;
    cv::Rect2d rectangle;
    float score{0.f};
    bool is_full_detection{false};
};

struct TrackInfo
{
    TrackId track_id;
    cv::Rect2d rectangle;
    float score{0.f};
};

enum class TrackState : uint8_t {
    ACTIVE,
    SKIPPED,
    TERMINATED
};


