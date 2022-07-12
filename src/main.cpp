#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/tracking.hpp>

#include <iostream>
#include <memory>
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

    std::vector<cv::Rect2d> detected;
    std::vector<cv::Rect2d> tracked;

    while (1) {

        ++frame_id;
        cv::Mat frame;
        cap >> frame;

        // ----------------------------------------------
        // Log frame info
        // ----------------------------------------------

        if (gLOGMAIN) {
            timeRecorder_.reset();
            timeRecorder_.start();
            std::cout
                << "\n Processing Frame: " 
                << frame_id << " (" 
                << frame_width << "x" 
                << frame_height <<  ")\n";
        }

        // ------------------------------------------------
        // Select frame for detection/tracking
        // ------------------------------------------------

        bool detect_more_faces 
            = _selector
                .select(
                    frame_id);

        // ------------------------------------------------
        // Preprocess frame
        // ------------------------------------------------

        cv::Mat raw_frame
            = frame(mask);

        cv::Mat preprocessed
            = _preprocessor
                .preprocess(raw_frame);

        // -------------------------------------------------
        // Detect faces in frames
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
                detected);

        // ----------------------------------------------
        // Merge face detections
        // ----------------------------------------------

        detected.clear();
        if(detect_more_faces) {
            detected
                = _detector
                    .getAsync();
        }

        tracked.clear();
        tracked 
            = _tracker
                .getAsync();

        std::vector<cv::Rect2d> merged
            = _merging
                .merge(
                    detected,
                    tracked);

        // ----------------------------------------------
        // Annotate face detections
        // ----------------------------------------------

        auto annotated_frame
            = _annotator
                .annotate(
                    raw_frame,
                    merged);

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
