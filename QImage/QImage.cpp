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

using namespace QImage;

#define GRADIENT_INTENSITY_TH 8  //threshold of gradient intensity to be considered in a stick
#define MAX_STICK_RANSAC_ATTEMPT 64 //max number of ransac trials
#define EDGE_RANDOM_PRIME_BASE 7901 //base for edge random
#define EDGE_WIDTH_DISTANCE 1.5 // edge distance to center


namespace QImage{
    
    
    template <class T>
    QImageBuffer<T> allocImageBuffer(int width, int height, int rowBytes){
        QImageBuffer<T> newBuffer;
        newBuffer.data = (T*)malloc(height*rowBytes);
        newBuffer.width = width;
        newBuffer.height = height;
        newBuffer.rowBytes = rowBytes;
        return newBuffer;
    }
    
    template <class T>
    void freeImageBuffer(QImageBuffer<T>& im){
        free(im.data);
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
                dxIm.data[i*im.rowBytes+j] = im.data[i*im.rowBytes+j] - im.data[i*im.rowBytes+j-1];
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
            for(size_t j=0;i<dxIm.width;j++){
                int xs = dxIm.data[i*dxIm.rowBytes + j];
                int ys = dyIm.data[i*dxIm.rowBytes + j];
                edgeIntensity.data[i*dxIm.rowBytes+j] = (uint8_t)(sqrt(xs*xs+ys*ys)+0.5);
            }
        }
    }

    inline LineSegment computeStickWeight(std::vector<std::tuple<int, int, float>>& edgePoints, size_t i, size_t j){
        LineSegment l;
        //define an edge fill-up buffer
        uint8_t *edge_fill_buffer
        
        //todo: compute stick weight with a kernel
        std::pair<int, int> r;
        r.first = std::get<0>(edgePoints[i]);
        r.second = std::get<1>(edgePoints[i]);
        float dx = std::get<0>(edgePoints[i]) - std::get<0>(edgePoints[j]);
        float dy = std::get<1>(edgePoints[i]) - std::get<1>(edgePoints[j]);
        float dn = sqrtf(dx*dx+dy*dy);
        assert(dn > 0.0f);
        dx /= dn;
        dy /= dn;
        l.weight = 0;
        for(size_t pi=0;pi<edgePoints.size();pi++){
            //todo: compute distance to center line, find start and end of l
            float xd = std::get<0>(edgePoints[pi]) - r.first;
            float yd = std::get<1>(edgePoints[pi]) - r.second;
            float d2line = sqrtf(xd*xd+yd*yd);
            if(d2line<EDGE_WIDTH_DISTANCE){
                continue;
            }
        }
        
        return l;
    }
    
    inline LineSegment computeLineSegmentInBlock(QImageBuffer<uint8_t>& gradIm, size_t xStart, size_t yStart, size_t block_size){
        LineSegment stick = {.x0=0, .x1=0, .y0=0, .y1=0, .weight=-1.0f};
        
        // get edge points
        std::vector<std::tuple<int, int, float>> edgePoints;
        for(size_t i=yStart;i<yStart+block_size;i++){
            for(size_t j=xStart;j<xStart+block_size;j++){
                if(gradIm.data[i*gradIm.rowBytes+j]>= GRADIENT_INTENSITY_TH){
                    edgePoints.push_back(std::tuple<size_t, size_t, float>(j-xStart, i-yStart,gradIm.data[i*gradIm.rowBytes+j]));
                }
            }
        }
        if(edgePoints.size()>0){
            // simple ransac to find a line
            float maxStickWeight=0.0f;
            for(size_t r=0;r<MAX_STICK_RANSAC_ATTEMPT;r++){
                int i = rand()%edgePoints.size();
                int j = rand()%edgePoints.size();
                LineSegment stickTmp = computeStickWeight(edgePoints, i, j);
                if(stickTmp.weight > maxStickWeight){
                    maxStickWeight = stickTmp.weight;
                    //todo: normalized weight by length
                    stick = stickTmp;
                }
            }
        }
        return stick;
    }
    
    std::vector<LineSegment> computeLineSegments(QImageBuffer<uint8_t>& gradIm, size_t block_size, float overlap){
        //srand (0); // init random numbers
        std::vector<LineSegment> sticks;
        size_t overlapSize = (size_t)round(block_size*overlap);
        for(size_t i=0;i<gradIm.height;i+=overlapSize){
            for(size_t j=0;j<gradIm.width;j+=overlapSize){
                LineSegment stick = computeLineSegmentInBlock(gradIm, j, i, block_size);
                if(stick.weight>0){
                    sticks.push_back(stick);
                }
            }
        }
        
        return sticks;
    }
    
    

}
