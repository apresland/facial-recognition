#include "face/node.h"
#include "face/context.h"
#include "face/recognition.h"

#include <iostream>

#include <opencv2/imgproc.hpp>
#include <opencv2/tracking.hpp>

FaceNode::FaceNode(std::queue<cv::Mat>& input,
                   std::queue<cv::Mat>& output) 
    : _image_input(input)
    , _image_output(output)
{

    _track_observer = std::make_unique<Recognition>();
    _preprocessor = std::make_unique<FacePreprocessor>();
    _selector = std::make_unique<FaceFrameSelection>();
    _detector = std::make_unique<FaceDetector>();
    _annotator = std::make_unique<FaceAnnotator>();
    _tracker = std::make_unique<Multitracker>(*_track_observer);
}

void FaceNode::spin_once()
{
    if (_image_input.empty()) {
        return;
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
            << " - frame id: " 
            << _frame_id << "\n";
    }

    // ------------------------------------------------
    // Select frame for detection/tracking
    // ------------------------------------------------

    bool detect_more_faces 
        = _selector
            ->select(
                _frame_id);

    // ------------------------------------------------
    // Preprocess frame
    // ------------------------------------------------

    cv::Mat preprocessed
        = _preprocessor
            ->preprocess(frame);

    // -------------------------------------------------
    // Detect faces in frame
    // -------------------------------------------------
    
    if(detect_more_faces) {
        _detector
            ->detectAsync(
                preprocessed);
    } 

    // ----------------------------------------------
    // Track faces in frame
    // ----------------------------------------------

    if ( ! _detected.empty()) {
        for (auto detection : _detected) {
            _tracker
                ->add(preprocessed, detection);
        }
    }

    _tracker
        ->trackAsync(
            preprocessed);

    // ----------------------------------------------
    // Merge face detections
    // ----------------------------------------------

    _detected.clear();
    if(detect_more_faces) {
        _detected
            = _detector
                ->getAsync();
    }

    _tracked.clear();
    _tracked 
        = _tracker
            ->getAsync();

    // ----------------------------------------------
    // Annotate face detections
    // ----------------------------------------------

    auto annotated_frame
        = _annotator
            ->annotate(
                frame,
                _tracked);

    _image_output
        .push(annotated_frame);

    // ----------------------------------------------
    // Log performance
    // ----------------------------------------------

    if (gLOGMAIN) {
        timeRecorder_.stop();
        std::cout 
            << " - processing time: "
            << timeRecorder_.getTimeMilli()
            << "ms (detection = "
            << detect_more_faces
            << ")" 
            << std::endl;
    }
}