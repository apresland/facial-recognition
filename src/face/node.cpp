#include "face/node.h"
#include "face/context.h"

#include <iostream>

#include <opencv2/imgproc.hpp>
#include <opencv2/tracking.hpp>

FaceNode::FaceNode(std::queue<cv::Mat>& input,
                   std::queue<cv::Mat>& output) 
    : _image_input(input)
    , _image_output(output)
{}

void FaceNode::spin_once()
{
    if (_image_input.empty()) {
        return; //continue;
    }

    ++_frame_id;
    cv::Mat frame
        = _image_input.front();
    _image_input.pop();

    // ----------------------------------------------
    // Log frame info
    // ----------------------------------------------

    if (gLOGMAIN) {
        timeRecorder_.reset();
        timeRecorder_.start();
        std::cout
            << " - id: " 
            << _frame_id << "\n";
    }

    // ------------------------------------------------
    // Select frame for detection/tracking
    // ------------------------------------------------

    bool detect_more_faces 
        = _selector
            .select(
                _frame_id);

    // ------------------------------------------------
    // Preprocess frame
    // ------------------------------------------------

    cv::Mat preprocessed
        = _preprocessor
            .preprocess(frame);

    // -------------------------------------------------
    // Detect faces in frame
    // -------------------------------------------------
    
    if(detect_more_faces) {
        _detector
            .detectAsync(
                preprocessed);
    } 

    // ----------------------------------------------
    // Track faces in frame
    // ----------------------------------------------

    _tracker
        .trackAsync(
            preprocessed,
            _detected);

    // ----------------------------------------------
    // Merge face detections
    // ----------------------------------------------

    _detected.clear();
    if(detect_more_faces) {
        _detected
            = _detector
                .getAsync();
    }

    _tracked.clear();
    _tracked 
        = _tracker
            .getAsync();

    std::vector<DetectionsDescr> merged
        = _merging
            .merge(
                _detected,
                _tracked);

    // ----------------------------------------------
    // Annotate face detections
    // ----------------------------------------------

    auto annotated_frame
        = _annotator
            .annotate(
                frame,
                merged);

    _image_output
        .push(annotated_frame);

    // ----------------------------------------------
    // Log performance
    // ----------------------------------------------

    if (gLOGMAIN) {
        timeRecorder_.stop();
        std::cout 
            << " - total time: "
            << timeRecorder_.getTimeMilli()
            << "ms ("
            << detect_more_faces
            << ")" 
            << std::endl;
    }
}