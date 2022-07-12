#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/tracking.hpp>

#include <iostream>
#include <vector>
#include <future>
#include <thread>
#include <chrono>

#include "face_context.h"
#include "face_preprocessor.h"
#include "face_frame_selection.h"
#include "face_detector.h"
#include "face_tracker.h"
#include "face_merging.h"
#include "face_annotator.h"

using namespace std::chrono_literals;

int main()
{
    const int mask_x = 500;
    const int mask_y = 50;
    const int mask_width = 1000;
    const int mask_height = 1000;

    const cv::Rect mask(
        mask_x,
        mask_y,
        mask_width,
        mask_height
        );

    cv::namedWindow("detections", CV_WINDOW_AUTOSIZE);
    cv::TickMeter timeRecorder_;
    cv::VideoCapture cap 
        = cv::VideoCapture(
            "2021-12-25 14-26-21_ET879_cut_2.mp4");

    if ( !cap.isOpened() )
    {
        std::cout
         << "Cannot open the video file. \n";
        return -1;
    }

    cv::VideoWriter video (
            "output.avi", 
            cv::VideoWriter::fourcc('M', 'J', 'P', 'G'),
            30, 
            cv::Size(mask_width, mask_height)
            );

    int frame_id{0};
    int frame_width = cap.get(cv::CAP_PROP_FRAME_WIDTH);
    int frame_height = cap.get(cv::CAP_PROP_FRAME_HEIGHT);

    FacePreprocessor _preprocessor;
    FaceFrameSelection _selector;
    FaceDetector _detector;
    FaceTracker _tracker;
    FaceMerging _merging;
    FaceAnnotator _annotator;

    while (1) {

        ++frame_id;

        cv::Mat frame;
        cap >> frame;
        
        if (frame.empty()) {
            std::cout
             << "\n Cannot read the video file. \n";
            break;
        } else {
            std::cout
                << "\n Processing Frame: " 
                << frame_id << " (" 
                << frame_width << "x" 
                << frame_height <<  ")\n";
        }

        // ------------------------------------------------
        // Preprocess masked frame
        // ------------------------------------------------

        cv::Mat raw_frame
            = frame(mask);

        cv::Mat preprocessed
            = _preprocessor
                .preprocess(raw_frame);

        // -------------------------------------------------
        // Detect faces in frames
        // -------------------------------------------------

        std::vector<cv::Rect> detections; 
        if( _selector.select(frame_id)) {
            auto detector_future = _detector.detectAsync(preprocessed);
            std::future_status status;
            do {
                status = detector_future.wait_for(5ms);
            } while (status != std::future_status::ready);
            detections = detector_future.get();
        } 

        // ----------------------------------------------
        // Track faces in frame
        // ----------------------------------------------

        if ( ! detections.empty()) {
            cv::Rect2d detection = detections[0];
            _tracker.init(preprocessed, detection);
        }

        cv::Rect2d updated_detection;  
        if ( _tracker.is_tracking()) {
            _tracker.track(
                preprocessed,
                updated_detection);
        } 

        // ----------------------------------------------
        // Merge face detections
        // ----------------------------------------------

        // TODO: Merger detected and tracked faces based on timestamp (they should run parallel).

        // ----------------------------------------------
        // Annotate face detections
        // ----------------------------------------------

        auto annotated_frame
            = _annotator.annotate(
                raw_frame,
                updated_detection);

        // ----------------------------------------------
        // Visualize face detections
        // ----------------------------------------------

        video.write(annotated_frame);
        cv::imshow("detections", annotated_frame);

        // ----------------------------------------------
        // Check for quit signal
        // ----------------------------------------------

        // Press ESC on keyboard to exit
        char c=(char)cv::waitKey(1);
        if(c==27)
            break;
    }

    cap.release();
    video.release();
    cv::destroyAllWindows();
    return 0;
}
