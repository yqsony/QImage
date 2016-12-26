//
//  QImageTests.m
//  QImageTests
//
//  Created by Quan Yuan on 11/23/16.
//  Copyright © 2016 Quan Yuan. All rights reserved.
//

#import <XCTest/XCTest.h>
#include "QImageUtils.hpp"
#include "QImage.hpp"
#include "QSticksFromImage.hpp"

@interface QImageTests : XCTestCase
-(void)testLoadBMP;
-(void)testSticks;
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

- (void)testLocalStick: (uint8_t*)data ofWidth: (int) w andHeight: (int) h  {
    QImage::QImageBuffer<uint8_t> imGray(data, w, abs(h), w);
    uint8_t block_size = 8;
    QImage::QImageBuffer<int8_t> dxIm = QImage::allocImageBuffer<int8_t>(w, abs(h), w);
    QImage::QImageBuffer<int8_t> dyIm = QImage::allocImageBuffer<int8_t>(w, abs(h), w);
    QImage::QImageBuffer<uint8_t> edgeIm = QImage::allocImageBuffer<uint8_t>(w, abs(h), w);
    computeDxImage(imGray, dxIm);
    computeDyImage(imGray, dyIm);
    computeEdgeIntensity(dxIm, dyIm, edgeIm);
    QImage::utils::write2BMP3(edgeIm.data, w, abs(h), "/tmp/edge.bmp");
    std::vector<LineSegment> sticks = QImage::QStick::computeLineSegments(edgeIm,block_size, 0.5f);
    QImage::utils::writeLineStick2BMP(sticks, nullptr, w, abs(h),"/tmp/sticks.bmp");
    //QImage::freeImageBuffer(imGray);
    
}

- (void)testLoadBMP {
    NSString *resourcePath = [[NSBundle bundleForClass:[self class]] resourcePath];
    NSString *testBmpFilename = [resourcePath stringByAppendingPathComponent:@"testPatch.bmp"];
    std::string bmpFileStr = std::string([testBmpFilename UTF8String]);
    int w, h, nch;
    uint8_t *data = nullptr;
    if(0<=QImage::utils::loadBmpWidthHeightChannels(bmpFileStr, w, h, nch, nullptr)){
        data = (uint8_t *)malloc(w*abs(h)*nch*sizeof(uint8_t));
        QImage::utils::loadBmpWidthHeightChannels(bmpFileStr, w, h, nch, data);
        uint8_t *dataGreen = (uint8_t *)malloc(w*abs(h)*sizeof(uint8_t));
        for(size_t i=0;i<abs(h);i++){
            for(size_t j=0;j<w;j++){
                dataGreen[i*w+j] = data[i*w*nch+j*nch];
            }
        }
        //todo: fix output
        QImage::utils::write2BMP3(dataGreen, w, abs(h), "/tmp/gray.bmp");
        free(dataGreen);
    }
    
    if(data){
        free(data);
    }
}

- (void)testSticks{
    NSString *resourcePath = [[NSBundle bundleForClass:[self class]] resourcePath];
    NSString *testBmpFilename = [resourcePath stringByAppendingPathComponent:@"testPatch.bmp"];
    std::string bmpFileStr = std::string([testBmpFilename UTF8String]);
    int w, h, nch;
    
    uint8_t blockTest[256];
    // test 8 by 8 image diagnal
    memset(blockTest, 0, 256);
    for(int k =0 ;k<16;k++){
        blockTest[k+k*16] = 255;
    }
    [self testLocalStick:blockTest ofWidth:16 andHeight:16];
    // test 8 by 8 image horizontal
    memset(blockTest, 0, 256);
    for(int k =0 ;k<16;k++){
        blockTest[8*16+k] = 255;
    }
    [self testLocalStick:blockTest ofWidth:16 andHeight:16];
    // test 8 by 8 image vertical
    memset(blockTest, 0, 256);
    for(int k =0 ;k<16;k++){
        blockTest[k*16+8] = 255;
    }
    [self testLocalStick:blockTest ofWidth:16 andHeight:16];
    
    uint8_t *data = nullptr;
    if(0<=QImage::utils::loadBmpWidthHeightChannels(bmpFileStr, w, h, nch, nullptr)){
        data = (uint8_t *)malloc(w*abs(h)*nch*sizeof(uint8_t));
        QImage::utils::loadBmpWidthHeightChannels(bmpFileStr, w, h, nch, data);
        uint8_t *dataGray = (uint8_t *)malloc(w*abs(h)*sizeof(uint8_t));
        for(size_t i=0;i<abs(h);i++){
            for(size_t j=0;j<w;j++){
                dataGray[i*w+j] = data[i*w*nch+j*nch+1];
            }
        }
        QImage::utils::write2BMP3(dataGray, w, abs(h), "/tmp/green.bmp");
        //test real image
        [self testLocalStick:dataGray ofWidth:w andHeight:h];
    }
    
    if(data){
        free(data);
    }

}


- (void)testPerformanceExample {
    // This is a performance test case.
    
    [self measureBlock:^{
        // Put the code you want to measure the time of here.
    }];
}

@end
