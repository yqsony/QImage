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

namespace QImage{
    struct LineSegment{
        size_t x0, y0, x1, y1;
        float weight;
    };
    
    template <class T>
    struct QImageBuffer{
        QImageBuffer(){data=nullptr; width=0; height=0; rowBytes=0;};
        T * data;
        size_t width, height, rowBytes;
    };
    
    template <class T>
    QImageBuffer<T> allocImageBuffer(int width, int height, int rowBytes);
    
    template <class T>
    void freeImageBuffer(QImageBuffer<T>& im);
    // gradients and line segments
    LineSegment computeLineSegmentsInBlock(QImageBuffer<uint8_t>);
    void computeDxImage(QImageBuffer<uint8_t>& im, QImageBuffer<int8_t>& dxIm);
    void computeDyImage(QImageBuffer<uint8_t>& im, QImageBuffer<int8_t>& dyIm);
    void computeEdgeImage(QImageBuffer<uint8_t>& im, QImageBuffer<int8_t>& edgeIm);
    std::vector<LineSegment> computeLineSegments(QImageBuffer<uint8_t>& im, uint8_t block_size);
    
}

#endif /* QImage_hpp */
