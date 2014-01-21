#ifndef __FirstStepsOpenCV__ScannerMenu__
#define __FirstStepsOpenCV__ScannerMenu__

#include <iostream>
#include "ImageScanner.h"
#include "CameraScanner.h"
#include <opencv2/opencv.hpp>

class ScannerMenu
{
public:
    
    ScannerMenu(void);
    ~ScannerMenu(void);
    void scannerStart();
    void menuEAN();
    void menuUPC();
    
protected:
    std::string pathName;
    std::string concatinateString;
};


#endif /* defined(__FirstStepsOpenCV__ScannerMenu__) */
