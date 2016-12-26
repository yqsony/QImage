//
//  QImage.hpp
//  QImageTest
//
//  Created by Quan Yuan on 10/2/16.
//  Copyright © 2016 Quan Yuan. All rights reserved.
//

#ifndef QImage_hpp
#define QImage_hpp

#include <stdio.h>
#include <cstdint>
#include <vector>
#include "QSticks.hpp"


namespace QImage{
    
    template <class T>
    struct QImageBuffer{
        QImageBuffer(){data=nullptr; width=0; height=0; rowBytes=0;};
        QImageBuffer(T* d, size_t w, size_t h, size_t rb){data=d; width=w; height=h; rowBytes=rb;};
        T * data;
        size_t width, height, rowBytes;
    };
    
    template <class T>
    QImageBuffer<T> allocImageBuffer(size_t width, size_t height, size_t rowBytes);
    
    template <class T>
    void freeImageBuffer(QImageBuffer<T>& im);
    
    // gradients and line segments
    
    void computeDxImage(QImageBuffer<uint8_t>& im, QImageBuffer<int8_t>& dxIm);
    void computeDyImage(QImageBuffer<uint8_t>& im, QImageBuffer<int8_t>& dyIm);
    void computeEdgeIntensity(QImageBuffer<int8_t>& dxIm, QImageBuffer<int8_t>& dyIm, QImageBuffer<uint8_t>& edgeIntensity);
        
}

#endif /* QImage_hpp */
