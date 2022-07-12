#include "face_merging.h"

std::vector<cv::Rect2d> FaceMerging::merge(std::vector<cv::Rect2d> detected, 
                                           std::vector<cv::Rect2d> tracked) 
{
    return detected.empty() ? tracked : detected;
}