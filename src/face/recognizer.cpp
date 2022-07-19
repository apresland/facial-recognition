#include "face/recognizer.h"

#include <dlib/opencv.h>

static cv::Rect dlibRectangleToOpenCV(dlib::rectangle r)
{
  return cv::Rect(cv::Point2i(r.left(), r.top()), cv::Point2i(r.right() + 1, r.bottom() + 1));
}

static dlib::rectangle openCVRectToDlib(cv::Rect r)
{
  return dlib::rectangle((long)r.tl().x, (long)r.tl().y, (long)r.br().x - 1, (long)r.br().y - 1);
}

Recognizer::Recognizer() 
{
    dlib::deserialize
        ("shape_predictor_5_face_landmarks.dat") 
            >> predictor_;    
}

std::vector<cv::Point2i> Recognizer::recognize(const cv::Mat& frame, const cv::Rect& rect)
{
    dlib::cv_image<dlib::bgr_pixel>
        img(frame);
    
    dlib::rectangle det
        = openCVRectToDlib(rect);

    dlib::full_object_detection shape
        = predictor_(img, det);

    unsigned long num_parts 
        = shape.num_parts();

    std::vector<cv::Point2i> landmarks;
    for (unsigned long i = 0; i < num_parts; i++) {
        cv::Point2i landmark;
        landmark.x = shape.part(i).x();
        landmark.y = shape.part(i).y();
        landmarks.push_back(landmark);
    }

    return landmarks;
}