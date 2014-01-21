#ifndef __FirstStepsOpenCV__CameraScanner__
#define __FirstStepsOpenCV__CameraScanner__

#include "VideoEngine.h"
#include "EAN_Scanner.h"
#include "UPC_Scanner.h"
#include "CameraHandler.h"
#include <opencv2/opencv.hpp>

class CameraScanner: public VideoEngine
{
public:
	CameraScanner(int kindOfScan);
    ~CameraScanner(void);
    bool open(const std::string& path);
    void run(void);
    void resizeMask(cv::Mat &mask);
    void linesHalfCrosses(cv::Mat &img, const cv::Scalar &color, int value1, int value2);
	void showVideoFrame(const cv::Mat& videoFrame);
    void processFrame(const cv::Mat& videoFrame, cv::Mat& processedFrame);
    void showProcessedFrame(const cv::Mat& processedFrame);
    void filterImage(cv::Mat& toFilterImage);
    cv::Mat binarizeMask(cv::Mat& grayFrame);
    void showNumberOfBarcode(std::deque<int> numberOfBarcode);
    bool keyPressed(bool keyWasPressed);
    
protected:
    cv::Mat cannyFilteredImage;
    cv::Mat copyImage;
    cv::Mat processedImage;
    int valueOfDistance;
    int valueThresh;
    int kindOfScanner;
    bool keyWasPressed=false;
    cv::Rect rectangle;
    BarcodeAnalyser *scanner;
    EAN_Scanner scannerEAN;
    UPC_Scanner scannerUPC;
    CameraHandler camHandler;
    
    
};
#endif /* defined(__FirstStepsOpenCV__CameraScanner__) */
