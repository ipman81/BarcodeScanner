#ifndef __FirstStepsOpenCV__EAN_Scanner__
#define __FirstStepsOpenCV__EAN_Scanner__

#include <opencv2/opencv.hpp>
#include "BarcodeAnalyser.h"
#define CODINGOF13THDIGITCOUNTER 10 
#define EQUAL 0
#define UNEQUAL 1

class EAN_Scanner : public BarcodeAnalyser
{
public:
    EAN_Scanner(void);
    ~EAN_Scanner(void);
    std::deque<int> scaningBarcode(cv::Mat& image);
    
protected:
    //check the number of the left side of coding table
    bool checkEquality(bool numbers[], int value);
};

#endif /* defined(__FirstStepsOpenCV__EAN_Scanner__) */
