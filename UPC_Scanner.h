#ifndef __FirstStepsOpenCV__UPC_Scanner__
#define __FirstStepsOpenCV__UPC_Scanner__

#include <opencv2/opencv.hpp>
#include "BarcodeAnalyser.h"


class UPC_Scanner : public BarcodeAnalyser
{
public:
   
    UPC_Scanner(void);
    ~UPC_Scanner(void);
    std::deque<int> scaningBarcode(cv::Mat& image);
};


#endif /* defined(__FirstStepsOpenCV__UPC_Scanner__) */
