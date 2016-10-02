//
//  QFrameCompare.cpp
//  viewIt
//
//  Created by Quan Yuan on 9/15/16.
//  Copyright © 2016 Quan Yuan. All rights reserved.
//
#include <cstdlib>
#include <assert.h>
#include "QFrameCompare.hpp"
#include <math.h>

namespace QFrames{
     template <class T>
    FrameBuffer<T> allocFrameBuffer(int width, int height, int rowBytes){
        FrameBuffer<T> newBuffer;
        newBuffer.data = (T*)malloc(height*rowBytes);
        newBuffer.width = width;
        newBuffer.height = height;
        newBuffer.rowBytes = rowBytes;
        return newBuffer;
    }
    
    template <class T>
    void freeFrameBuffer(FrameBuffer<T>& im){
        free(im.data);
        im.width=0;
        im.height=0;
        im.rowBytes=0;
    }

    void computeDxImage(FrameBuffer<uint8_t>& im, FrameBuffer<int8_t>& dxIm){
        assert(dxIm.data!=nullptr);
        for(size_t i=0;i<im.height;i++){
            dxIm.data[i*im.rowBytes] = 0;
            for(size_t j=1;j<im.width;j++){
                dxIm.data[i*im.rowBytes+j] = im.data[i*im.rowBytes+j] - im.data[i*im.rowBytes+j-1];
            }
        }
    }
    // edge intensities are reduced by factor of 2, in uint8_t format
    void computeDyImage(FrameBuffer<uint8_t>& im, FrameBuffer<int8_t>& dyIm){
        assert(dyIm.data!=nullptr);
        for(size_t j=0;j<im.width;j++){
            dyIm.data[j] = 0;
            for(size_t i=1;i<im.height;i++){
                dyIm.data[i*im.rowBytes+j] = ((int)im.data[i*im.rowBytes+j] - (int)im.data[(i-1)*im.rowBytes+j-1])/2;
            }
        }
    }
    
    void computeEdgeIntensity(FrameBuffer<int8_t>& dxIm, FrameBuffer<int8_t>& dyIm, FrameBuffer<uint8_t>& edgeIntensity){
        assert(dxIm.width==dyIm.width);
        assert(dxIm.height == dyIm.height);
        
        for(size_t i=0;i<dxIm.height;i++){
            for(size_t j=0;i<dxIm.width;j++){
                int xs = dxIm.data[i*dxIm.rowBytes + j];
                int ys = dyIm.data[i*dxIm.rowBytes + j];
                edgeIntensity.data[i*dxIm.rowBytes+j] = (uint8_t)(sqrt(xs*xs+ys*ys)+0.5);
            }
        }
    }
    
    
}