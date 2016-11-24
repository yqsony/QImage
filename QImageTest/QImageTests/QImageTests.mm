//
//  QImageTests.m
//  QImageTests
//
//  Created by Quan Yuan on 11/23/16.
//  Copyright © 2016 Quan Yuan. All rights reserved.
//

#import <XCTest/XCTest.h>
#include "QImageUtils.hpp"

@interface QImageTests : XCTestCase

@end

@implementation QImageTests

- (void)setUp {
    [super setUp];
    // Put setup code here. This method is called before the invocation of each test method in the class.
}

- (void)tearDown {
    // Put teardown code here. This method is called after the invocation of each test method in the class.
    [super tearDown];
}

- (void)testLoadBMP {
    NSString *resourcePath = [[NSBundle bundleForClass:[self class]] resourcePath];
    NSString *testBmpFilename = [resourcePath stringByAppendingPathComponent:@"test1.bmp"];
    std::string bmpFileStr = std::string([testBmpFilename UTF8String]);
    int w, h, nch;
    uint8_t *data = nullptr;
    if(0<=QImage::utils::loadBmpWidthHeightChannels(bmpFileStr, w, h, nch, nullptr)){
        data = (uint8_t *)malloc(w*abs(h)*nch*sizeof(uint8_t));
        QImage::utils::loadBmpWidthHeightChannels(bmpFileStr, w, h, nch, data);
        uint8_t *dataGreen = (uint8_t *)malloc(w*abs(h)*sizeof(uint8_t));
        for(size_t i=0;i<abs(h);i++){
            for(size_t j=0;j<w;j++){
                dataGreen[i*w+j] = data[i*w*nch+j*nch+2];
            }
        }
        //todo: fix output
        QImage::utils::write2BMP(dataGreen, w, abs(h), 1, "/tmp/gray.bmp");
        free(dataGreen);
    }
    
    if(data){
        free(data);
    }
    
}

- (void)testPerformanceExample {
    // This is an example of a performance test case.
    [self measureBlock:^{
        // Put the code you want to measure the time of here.
    }];
}

@end
