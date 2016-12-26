//
//  QSticksFromImage.hpp
//  QImageTest
//
//  Created by Quan Yuan on 11/23/16.
//  Copyright © 2016 Quan Yuan. All rights reserved.
//

#ifndef QSticksFromImage_hpp
#define QSticksFromImage_hpp

#include <stdio.h>
#include "QImage.hpp"
#include "QImageUtils.hpp"

namespace QImage{
    namespace QStick{
        LineSegment computeLineSegmentsInBlock(QImageBuffer<uint8_t>);
        std::vector<LineSegment> computeLineSegments(QImageBuffer<uint8_t>& im, size_t block_size, float overlap);
        
    }
}


#endif /* QSticksFromImage_hpp */
