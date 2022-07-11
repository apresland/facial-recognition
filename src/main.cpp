#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/tracking.hpp>

#include <iostream>
#include <vector>

#include "face_context.h"
#include "face_preprocessor.h"
#include "face_detector.h"
#include "face_tracker.h"

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

    int N{0};
    int frame_width = cap.get(cv::CAP_PROP_FRAME_WIDTH);
    int frame_height = cap.get(cv::CAP_PROP_FRAME_HEIGHT);

    FacePreprocessor _preprocessor;

    FaceDetector _detector;
    int detection_count = 0;

    FaceTracker _tracker;
    bool tracking_face = false;
    int undetected_count = 0;

    while (1) {

        ++N;

        cv::Mat frame;
        cap >> frame;
        if (frame.empty()) {
            std::cout
             << "\n Cannot read the video file. \n";
            break;
        }

        // ------------------------------------------------
        // Preprocess masked frame
        // ------------------------------------------------

        if (gLOGGING) {
            std::cout
                << "\n Processing Frame: " << N << " (" << frame_width << "x" << frame_height <<  ")\n";
            timeRecorder_.reset();
            timeRecorder_.start();
        }

        cv::Mat preprocessed_frame
            = _preprocessor.preprocess(frame(mask));

        if (gLOGGING) {        
            timeRecorder_.stop();
            std::cout 
                << " - capture took: "
                << timeRecorder_.getTimeMilli()
                << "ms" << std::endl;
        }

        // -------------------------------------------------
        // Periodically detected faces in preprocessed frame
        // -------------------------------------------------

        if (gLOGGING) {
            timeRecorder_.reset();
            timeRecorder_.start();
        }

        std::vector<cv::Rect> detected_faces;
        if (detection_count == 0) {
            detected_faces
                = _detector.detect(preprocessed_frame);
        }

        if (++detection_count >= 5) {
            detection_count = 0;
        } 

        if (gLOGGING) {
            timeRecorder_.stop();
            std::cout 
                << " - face detection took: "
                << timeRecorder_.getTimeMilli()
                << "ms" << std::endl;
        }

        // ----------------------------------------------
        // Track faces between detections
        // ----------------------------------------------

        if (gLOGGING) {
            timeRecorder_.reset();
            timeRecorder_.start();
        }

        if ( ! detected_faces.empty()) {
            tracking_face = true;
            cv::Rect2d detected_face = detected_faces[0];
            _tracker.init(preprocessed_frame, detected_face);
            undetected_count = 0;
        } else {
            undetected_count++;
        }
        
        cv::Rect2d tracked_face;  
        if (true == tracking_face && undetected_count < 10) {   
            _tracker.track(preprocessed_frame, tracked_face);
        } else {
            tracking_face = false;
        }

         if (gLOGGING) {
            timeRecorder_.stop();
            std::cout 
                << " - tracking took: "
                << timeRecorder_.getTimeMilli()
                << "ms" << std::endl;
        }

        // ----------------------------------------------
        // Visualize face detections
        // ----------------------------------------------

        if (gLOGGING) {
            timeRecorder_.reset();
            timeRecorder_.start();
        }

        cv::rectangle(
            preprocessed_frame, tracked_face, 
            cv::Scalar(0, 255, 0), 2);
        
        for(const auto & face : detected_faces) {
            cv::rectangle(
                preprocessed_frame, face, 
                cv::Scalar(255, 0, 255), 2);
        }

        video.write(preprocessed_frame);
        cv::imshow("detections", preprocessed_frame);

        if (gLOGGING) {
            timeRecorder_.stop();
            std::cout 
                << " - visualization took: "
                << timeRecorder_.getTimeMilli()
                << "ms" << std::endl;
        }

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
