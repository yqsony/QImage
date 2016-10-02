//
//  QCLUtils.h
//  QImageTest
//
//  Created by Quan Yuan on 3/30/16.
//  Copyright © 2016 Quan Yuan. All rights reserved.
//

#ifndef QCLUtils_h
#define QCLUtils_h

#include <stdio.h>
#include <OpenCL/opencl.h>
#include "kernel.cl.h"

// A utility function that checks that our kernel execution performs the
// requested work over the entire range of data.
static int validate(cl_float* input, cl_float* output);

int mainExample(int argc, const char * argv[]);

#endif /* QCLUtils_h */
