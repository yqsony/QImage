//
//  DiceDataController.m
//  viewIt
//
//  Created by Quan Yuan on 9/10/16.
//  Copyright © 2016 Quan Yuan. All rights reserved.
//

#import "DiceDataController.h"

@implementation DiceDataController

- (int)getDiceRoll
{
    int roll = (arc4random() % 6) + 1;
    
    return roll;
}


@end
