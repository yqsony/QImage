//
//  DieView.h
//  viewIt
//
//  Created by Quan Yuan on 9/10/16.
//  Copyright © 2016 Quan Yuan. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface DieView : UIView

#pragma mark Properties
@property (nonatomic, strong) UIImageView *dieImage;

#pragma mark Methods
- (void)showDie:(int)num;

@end
