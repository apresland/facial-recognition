#include "face/preprocessor.h"
#include "face/context.h"
#include <iostream>

/**
Convert the color space of the image from RGB
into one of the color spaces which separate intensity values
from color components.
*/
cv::Mat FacePreprocessor::preprocess(const cv::Mat& frame)
{
    if ( ! gPREPROCESS) {
        return frame;
    }
    
    cv::Mat output
        = equalizeHistogramAdaptive(frame);

    return output;
}

cv::Mat FacePreprocessor::equalizeHistogramAdaptive(const cv::Mat& inputImage)
{
    if (gLOGGING) {
        timeRecorder_.reset();
        timeRecorder_.start();
    }

    cv::Mat result;

    if(inputImage.channels() >= 3)
    {
        cv::Mat ycrcb;
        std::vector<cv::Mat> channels;
        cv::cvtColor(inputImage, ycrcb, CV_BGR2YCrCb);
        cv::split(ycrcb, channels);

        cv::Ptr<cv::CLAHE> clahe = cv::createCLAHE();
        clahe->setClipLimit(4);
        clahe->apply(channels[0], channels[0]);
        cv::merge(channels, ycrcb);
        
        cv::cvtColor(ycrcb, result, CV_YCrCb2BGR);
    }

    if (gLOGGING) {        
        timeRecorder_.stop();
        std::cout 
            << " - preprocssing took: "
            << timeRecorder_.getTimeMilli()
            << "ms" << std::endl;
    }

    return result;
}