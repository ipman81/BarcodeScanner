//
//  CameraHandler.h
//  FirstStepsOpenCV
//
//  Created by Adrian Watzlawczyk on 20/01/14.
//  Copyright (c) 2014 Adrian Watzlawczyk. All rights reserved.
//

#ifndef __FirstStepsOpenCV__CameraHandler__
#define __FirstStepsOpenCV__CameraHandler__

#include <iostream>
#include <opencv2/opencv.hpp>

class CameraHandler
{
public:
    CameraHandler();
    ~CameraHandler(void);
    void readContoursOfFrame(cv::Mat &mask, int valueOfThresh);
    void repairBarcode(cv::Mat &binaryMask);
    void repaintBarcode(cv::Mat& binaryImage);
    
};



#endif /* defined(__FirstStepsOpenCV__CameraHandler__) */
