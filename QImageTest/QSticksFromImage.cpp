//
//  QSticksFromImage.cpp
//  QImageTest
//
//  Created by Quan Yuan on 11/23/16.
//  Copyright © 2016 Quan Yuan. All rights reserved.
//

#include "QSticksFromImage.hpp"
#include <assert.h>
#include <math.h>
#include <set>

#define EDGE_WIDTH_DISTANCE 2.5 // edge distance to center
#define EDGE_OCCUPY_RATIO 0.75  // threshold of occupied ratio of the edge length
#define MAX_STICK_RANSAC_ATTEMPT 64 //max number of ransac trials
#define GRADIENT_INTENSITY_TH 8  //threshold of gradient intensity to be considered in a stick

namespace QImage{
    namespace QStick{
        
        inline LineSegment computeStickWeight(std::vector<std::tuple<int, int, float>>& edgePoints, size_t i, size_t j){
            LineSegment l;
            std::set<int> occupied;
            occupied.clear();
            //define an edge fill-up buffer
            static const int m = 2;
            uint8_t edge_fill_buffer[STICK_BLOCK_SIZE*m];
            memset(edge_fill_buffer, 0, sizeof(edge_fill_buffer));
            //compute stick weight with a kernel
            int rx, ry; //x,y of point i
            rx = std::get<0>(edgePoints[i]);
            ry = std::get<1>(edgePoints[i]);
            float dx = std::get<0>(edgePoints[j]) - std::get<0>(edgePoints[i]);
            float dy = std::get<1>(edgePoints[j]) - std::get<1>(edgePoints[i]);
            float dn = sqrtf(dx*dx+dy*dy);
            assert(dn > 0.0f);
            dx /= dn;
            dy /= dn; //direction of line, normed
            l.weight = 0;
            float minProj=0, maxProj=0;
            for(size_t pi=0;pi<edgePoints.size();pi++){
                //todo: compute distance to center line, find start and end of l
                float xd = std::get<0>(edgePoints[pi]) - rx;
                float yd = std::get<1>(edgePoints[pi]) - ry;
                float projectionAlongEdge = dx*xd + dy*yd; // projection on the line
                float tx = projectionAlongEdge*dx;
                float ty = projectionAlongEdge*dy;  // find the projection point on the line, with point i as origin
                float d2x = xd - tx;
                float d2y = yd - ty;
                
                float d2line = sqrtf(d2x*d2x+d2y*d2y); //distance to line
                if(d2line>EDGE_WIDTH_DISTANCE){
                    continue;
                }
                float w = 1.0f/(1.0f+d2line)*std::get<2>(edgePoints[pi]); // reweight the original edge weight
                l.weight += w;
                
                occupied.insert((int)roundf(projectionAlongEdge));
                if(occupied.size()==1){
                    minProj=projectionAlongEdge;
                    maxProj=projectionAlongEdge;
                }
                else{
                    //get two ends of projected edge points
                    if(projectionAlongEdge < minProj){
                        minProj = projectionAlongEdge;
                    }
                    if(projectionAlongEdge > maxProj){
                        maxProj = projectionAlongEdge;
                    }
                }
            }
            if(occupied.size()> STICK_BLOCK_SIZE*EDGE_OCCUPY_RATIO){
                l.x0 = rx + minProj*dx;
                l.x1 = rx + maxProj*dx;
                l.y0 = ry + minProj*dy;
                l.y1 = ry + maxProj*dy;
            }
            else{
                l.weight = 0;
            }
            return l;
        }
        
        LineSegment computeLineSegmentInBlock(QImageBuffer<uint8_t>& gradIm, size_t xStart, size_t yStart, size_t block_size){
            LineSegment stick = {.x0=0, .x1=0, .y0=0, .y1=0, .weight=-1.0f};
            
            // get edge points
            std::vector<std::tuple<int, int, float>> edgePoints;
            edgePoints.clear();
            for(size_t i=yStart;i<yStart+block_size;i++){
                for(size_t j=xStart;j<xStart+block_size;j++){
                    if(gradIm.data[i*gradIm.rowBytes+j]>= GRADIENT_INTENSITY_TH){
                        edgePoints.push_back(std::tuple<size_t, size_t, float>(j-xStart, i-yStart,gradIm.data[i*gradIm.rowBytes+j]));
                    }
                }
            }
            if(edgePoints.size()>1){
                // simple ransac to find a line
                float maxStickWeight=0.0f;
                for(size_t r=0;r<MAX_STICK_RANSAC_ATTEMPT;r++){
                    int i = rand()%edgePoints.size();
                    int j = rand()%edgePoints.size();
                    //debug only
                    //i = 2;
                    //j = 7;
                    if(i==j){
                        continue;
                    }
                    LineSegment stickTmp = computeStickWeight(edgePoints, i, j);
                    if(stickTmp.weight > maxStickWeight){
                        maxStickWeight = stickTmp.weight;
                        stickTmp.x0+=xStart;
                        stickTmp.x1+=xStart;
                        stickTmp.y0+=yStart;
                        stickTmp.y1+=yStart;
                        //todo: normalized weight by length
                        stick = stickTmp;
                        
                    }
                }
            }
            return stick;
        }
        
        inline LineSegment computeHoughLineSegmentInBlock(QImageBuffer<int8_t>& dxIm,QImageBuffer<int8_t>& dyIm, QImageBuffer<uint8_t>& gradIm, size_t xStart, size_t yStart, size_t block_size){
            LineSegment stick = {.x0=0, .x1=0, .y0=0, .y1=0, .weight=-1.0f};
            
            
            return stick;
        }
        
        std::vector<LineSegment> computeLineSegments(QImageBuffer<uint8_t>& edgeIm, size_t block_size, float overlap){
            //srand (0); // init random numbers
            std::vector<LineSegment> sticks;
            //QImage::utils::HoughLines houghLines(HOUGH_ANGLE_BINS, ceilf(block_size*sqrtf(2.0f)));
            size_t overlapSize = (size_t)round(block_size*overlap);
            for(size_t i=0;i<=edgeIm.height-block_size;i+=overlapSize){
                for(size_t j=0;j<=edgeIm.width-block_size;j+=overlapSize){
                    //houghLines.cleanup();
                    
                    LineSegment stick = computeLineSegmentInBlock(edgeIm, j, i, block_size);
                    if(stick.weight>0){
                        sticks.push_back(stick);
                    }
                }
            }
            
            return sticks;
        }
        
    }
}
