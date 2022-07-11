#include "face_preprocessor.h"
#include "context.h"

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
        = equalizeIntensity(frame);

    return output;
}

cv::Mat FacePreprocessor::equalizeIntensity(const cv::Mat& inputImage)
{
    if(inputImage.channels() >= 3)
    {
        cv::Mat ycrcb;
        cv::Mat result;

        std::vector<cv::Mat> channels;
        cv::cvtColor(inputImage, ycrcb, CV_BGR2YCrCb);
        cv::split(ycrcb,channels);
        cv::equalizeHist(channels[0], channels[0]);
        cv::merge(channels,ycrcb);

        cv::cvtColor(ycrcb, result, CV_YCrCb2BGR);

        return result;
    }

    return cv::Mat();
}