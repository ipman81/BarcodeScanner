#ifndef __FirstStepsOpenCV__Image__
#define __FirstStepsOpenCV__Image__

#define CONSTANTFRAMEHEIGHT 360
#define CONSTANTFRAMEWIDTH 640

#include <opencv2/opencv.hpp>
#include <string>

class VideoEngine
{
public:
	VideoEngine();
	virtual ~VideoEngine(void);
	virtual bool open(const std::string& path);
    void run();
	virtual void showVideoFrame(const cv::Mat& videoFrame);
	virtual void processFrame(const cv::Mat& videoFrame, cv::Mat& processedFrame);
	virtual void showProcessedFrame(const cv::Mat& processedFrame);
    
protected:
	cv::VideoCapture videoCapture;
	int frameWidth;
	int frameHeight;
	int frameNumber;
};
#endif /* defined(__FirstStepsOpenCV__ImageScanner__) */
