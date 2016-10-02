//
//  QFrameCompare.hpp
//  viewIt
//
//  Created by Quan Yuan on 9/15/16.
//  Copyright © 2016 Quan Yuan. All rights reserved.
//

#ifndef QFrameCompare_hpp
#define QFrameCompare_hpp

#include <stdio.h>
#include <vector>
namespace QFrames{
    template <class T>
    struct FrameBuffer{
        FrameBuffer(){data=nullptr; width=0; height=0; rowBytes=0;};
        T * data;
        size_t width, height, rowBytes;
    };
    struct LineSegment{
        size_t x0, y0, x1, y1;
    };
    
    
    
    class QFrameCompare{
    public:
        QFrameCompare(){};
    private:
        std::vector<FrameBuffer<uint8_t>> mFrames;
        
        void mSwapBuffers(FrameBuffer<uint8_t> &b1, FrameBuffer<uint8_t> &b2){std::swap(b1,b2);}
        
    };
    template <class T>
    FrameBuffer<T> allocFrameBuffer(int width, int height, int rowBytes);
    template <class T>
    void freeFrameBuffer(FrameBuffer<T>& im);
    LineSegment computeLineSegmentsInBlock(FrameBuffer<uint8_t>);
    void computeDxImage(FrameBuffer<uint8_t>& im, FrameBuffer<int8_t>& dxIm);
    void computeDyImage(FrameBuffer<uint8_t>& im, FrameBuffer<int8_t>& dyIm);
    void computeEdgeImage(FrameBuffer<uint8_t>& im, FrameBuffer<int8_t>& edgeIm);
    std::vector<LineSegment> computeLineSegments(FrameBuffer<uint8_t>& im);
    
    
}
#endif /* QFrameCompare_hpp */
