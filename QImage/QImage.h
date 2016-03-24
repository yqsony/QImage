//
//  QImage.h
//  QImageTest
//
//  Created by Quan Yuan on 1/3/16.
//  Copyright © 2016 Quan Yuan. All rights reserved.
//

#ifndef QImage_h
#define QImage_h

#include <stdio.h>
#include <stdlib.h>



void calculate_line_segments(uint8_t* im, size_t width, size_t height, size_t blockSize,
                             float * outputLines, size_t* numLines);



#endif /* QImage_h */
