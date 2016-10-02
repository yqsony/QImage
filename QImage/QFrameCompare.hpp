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
#include "QImage.hpp"
using namespace QImage;

namespace QFrames{
    
    
    
    
    class QFrameCompare{
    public:
        QFrameCompare(){};
    private:
        std::vector<QImageBuffer<uint8_t>> mFrames;
        
        void mSwapBuffers(QImageBuffer<uint8_t> &b1, QImageBuffer<uint8_t> &b2){std::swap(b1,b2);}
        
    };
    
    
    
}
#endif /* QFrameCompare_hpp */
