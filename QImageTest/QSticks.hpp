//
//  QSticks.hpp
//  QImageTest
//
//  Created by Quan Yuan on 12/23/16.
//  Copyright © 2016 Quan Yuan. All rights reserved.
//

#ifndef QSticks_hpp
#define QSticks_hpp

#define STICK_BLOCK_OVERLAP     0.5f
#define STICK_BLOCK_SIZE        8

#include <stdio.h>

    struct LineSegment{
        float x0, y0, x1, y1;
        float weight;
    };


#endif /* QSticks_hpp */
