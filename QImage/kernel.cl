//
//  kernel.cl
//  CLProcessor kernel functions
//
//  Created by Quan Yuan on 2/24/16.
//  Copyright © 2016 Quan Yuan. All rights reserved.
//

#include "kernel.cl.h"

// Kernel block.
kernel void square(
                   global uint8_t * input,               
                   global float   * output)
{
    size_t i = get_global_id(0);
    output[i] = input[i] * input[i];
}
