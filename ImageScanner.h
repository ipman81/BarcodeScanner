#ifndef __FirstStepsOpenCV__ImageScanner__
#define __FirstStepsOpenCV__ImageScanner__


#include "VideoEngine.h"
#include "EAN_Scanner.h"
#include "UPC_Scanner.h"
#include <opencv2/opencv.hpp>

class ImageScanner: public VideoEngine
{
public:
	ImageScanner(int kindOfScan);
    ~ImageScanner(void);
    bool open(const std::string& path);
    void run(void);
    void filterImage(cv::Mat& toFilterImage);
	void showVideoFrame(const cv::Mat& videoFrame);
    void processFrame(const cv::Mat& videoFrame, cv::Mat& processedFrame);
    void showProcessedFrame(const cv::Mat& processedFrame);
    cv::Mat binarizeMask(cv::Mat& grayFrame);
    void showNumberOfBarcode(std::deque<int> numberOfBarcode);
   
protected:
    int kindOfScanner;
    cv::Mat image;
    cv::Mat mask;
    BarcodeAnalyser *scanner;
    EAN_Scanner scannerEAN;
    UPC_Scanner scannerUPC;
    int valueThresh;
        
};


#endif /* defined(__FirstStepsOpenCV__ImageScanner__) */
