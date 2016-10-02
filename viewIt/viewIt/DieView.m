//
//  DieView.m
//  viewIt
//
//  Created by Quan Yuan on 9/10/16.
//  Copyright © 2016 Quan Yuan. All rights reserved.
//

#import "DieView.h"

@implementation DieView

/*
// Only override drawRect: if you perform custom drawing.
// An empty implementation adversely affects performance during animation.
- (void)drawRect:(CGRect)rect {
    // Drawing code
}
*/
- (id)initWithFrame:(CGRect)frame
{
    self = [super initWithFrame:frame];
    if (self) {
        // Initialization code
    }
    return self;
}

- (void)showDie:(int)num
{
    if (self.dieImage == nil)
    {
        self.dieImage = [[UIImageView alloc] initWithFrame:CGRectMake(0, 0, 90, 90)];
        
        [self addSubview:self.dieImage];
    }
    
    NSString *fileName = [NSString stringWithFormat:@"dice%d.png", num];
    
    self.dieImage.image = [UIImage imageNamed:fileName];
}

@end
