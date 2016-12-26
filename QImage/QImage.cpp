//
//  QImage.cpp
//  QImageTest
//
//  Created by Quan Yuan on 10/2/16.
//  Copyright © 2016 Quan Yuan. All rights reserved.
//

#include "QImage.hpp"
#include <cstdlib>
#include <assert.h>
#include <math.h>
#include "QImageUtils.hpp"
#include <set>
using namespace QImage;

namespace QImage{
    
    template <class T>
    QImageBuffer<T> allocImageBuffer(size_t width, size_t height, size_t rowBytes){
        QImageBuffer<T> newBuffer;
        newBuffer.data = (T*)malloc(height*rowBytes);
        newBuffer.width = width;
        newBuffer.height = height;
        newBuffer.rowBytes = rowBytes;
        return newBuffer;
    }
    
    template <class T>
    void freeImageBuffer(QImageBuffer<T>& im){
        if(im.data!=nullptr){
            free(im.data);
        }
        im.data=nullptr;
        im.width=0;
        im.height=0;
        im.rowBytes=0;
    }

    void computeDxImage(QImageBuffer<uint8_t>& im, QImageBuffer<int8_t>& dxIm){
        assert(dxIm.data!=nullptr);
        assert(im.data!=nullptr);
        for(size_t i=0;i<im.height;i++){
            dxIm.data[i*im.rowBytes] = 0;
            for(size_t j=1;j<im.width;j++){
                dxIm.data[i*im.rowBytes+j] = ((int)im.data[i*im.rowBytes+j] - (int)im.data[i*im.rowBytes+j-1])/2;
            }
        }
    }
    
    // edge intensities are reduced by factor of 2, in uint8_t format
    void computeDyImage(QImageBuffer<uint8_t>& im, QImageBuffer<int8_t>& dyIm){
        assert(dyIm.data!=nullptr);
        assert(im.data!=nullptr);
        for(size_t j=0;j<im.width;j++){
            dyIm.data[j] = 0;
            for(size_t i=1;i<im.height;i++){
                dyIm.data[i*im.rowBytes+j] = ((int)im.data[i*im.rowBytes+j] - (int)im.data[(i-1)*im.rowBytes+j-1])/2;
            }
        }
    }
    
    void computeEdgeIntensity(QImageBuffer<int8_t>& dxIm, QImageBuffer<int8_t>& dyIm, QImageBuffer<uint8_t>& edgeIntensity){
        assert(dxIm.width==dyIm.width);
        assert(dxIm.height == dyIm.height);
        
        for(size_t i=0;i<dxIm.height;i++){
            for(size_t j=0;j<dxIm.width;j++){
                int xs = dxIm.data[i*dxIm.rowBytes + j];
                int ys = dyIm.data[i*dxIm.rowBytes + j];
                edgeIntensity.data[i*dxIm.rowBytes+j] = (uint8_t)(sqrt(xs*xs+ys*ys)+0.5);
            }
        }
    }

    template class QImageBuffer<uint8_t>;
    template class QImageBuffer<int8_t>;
    template void freeImageBuffer<uint8_t>(QImageBuffer<uint8_t>& im);
    template QImageBuffer<uint8_t> allocImageBuffer(size_t width, size_t height, size_t rowBytes);
    template QImageBuffer<int8_t> allocImageBuffer(size_t width, size_t height, size_t rowBytes);
    
    //template std::vector<LineSegment> computeLineSegments<uint8_t>(QImageBuffer<uint8_t>&, size_t, float);
}
