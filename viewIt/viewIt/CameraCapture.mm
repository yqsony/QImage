//
//  CameraCapture.m
//  viewIt
//
//  Created by Quan Yuan on 9/14/16.
//  Copyright © 2016 Quan Yuan. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "CameraCapture.h"

@interface CameraCapture () <AVCaptureVideoDataOutputSampleBufferDelegate>
{
    dispatch_semaphore_t processingSemaphore;
    AVCaptureDevicePosition currentPosition;
    AVCaptureStillImageOutput* stillOutput;
    AVCaptureVideoDataOutput * dataOutput;
#if TARGET_OS_IPHONE
    AVCaptureVideoPreviewLayer *previewLayer;
    __weak UIView* previewLayerView;
#endif
}
@end

@implementation CameraCapture

- (void)captureStill:(void(^)(UIImage*))block;
{
    [stillOutput captureStillImageAsynchronouslyFromConnection:[stillOutput connectionWithMediaType:AVMediaTypeVideo] completionHandler:^(CMSampleBufferRef imageDataSampleBuffer, NSError *error) {
        
        NSData* jpeg = [AVCaptureStillImageOutput jpegStillImageNSDataRepresentation:imageDataSampleBuffer];
        block([UIImage imageWithData:jpeg]);
        
    }];
}
///
- (id)init {
    self = [super init];
    if(!self)return nil;
    
    self.videoOrientation = AVCaptureVideoOrientationPortrait;
    self.processingQueue = dispatch_queue_create("com.apple.espresso.processingQueue", 0);
    self.cameraQueue = dispatch_queue_create("com.apple.espresso.camequeue", 0);
    processingSemaphore = dispatch_semaphore_create(1);
    currentPosition = AVCaptureDevicePositionFront;
    
    return self;
}

- (void)createSession {
    dispatch_async(self.cameraQueue, ^{
        
        _captureSession = [[AVCaptureSession alloc] init];
        
        if (!_captureSession) {
            NSLog(@">> ERROR: Couldnt create a capture session");
            assert(0);
        }
        
        [_captureSession beginConfiguration];
        [_captureSession setSessionPreset:AVCaptureSessionPreset640x480];
        //[_captureSession setSessionPreset:AVCaptureSessionPreset1280x720];
        // Get the a video device with preference to the front facing camera
        AVCaptureDevice* videoDevice = [self cameraWithPosition:currentPosition];
        
        if(videoDevice == nil)
        {
            NSLog(@">> ERROR: Couldnt create a AVCaptureDevice");
            assert(0);
        }
        
        NSError *error;
        
        // Device input
        AVCaptureDeviceInput *deviceInput = [AVCaptureDeviceInput deviceInputWithDevice:videoDevice error:&error];
        
        if (error)
        {
            NSLog(@">> ERROR: Couldnt create AVCaptureDeviceInput");
            assert(0);
        }
        
        [_captureSession addInput:deviceInput];
        
        // Create the output for the capture session.
        dataOutput = [[AVCaptureVideoDataOutput alloc] init];
        [dataOutput setAlwaysDiscardsLateVideoFrames:YES];
        
        // Set the color space.
        [dataOutput setVideoSettings:[NSDictionary dictionaryWithObject:[NSNumber numberWithInt:kCVPixelFormatType_420YpCbCr8BiPlanarFullRange]
                                                                 forKey:(id)kCVPixelBufferPixelFormatTypeKey]];
        
        
        //[dataOutput setVideoSettings:[NSDictionary dictionaryWithObject:[NSNumber numberWithInt:kCVPixelFormatType_420YpCbCr8BiPlanarFullRange] forKey:(id)kCVPixelBufferPixelFormatTypeKey]];
        // Set dispatch to be on the main thread to create the texture in memory and allow Metal to use it for rendering
        [dataOutput setSampleBufferDelegate:self queue:self.cameraQueue];
        
        
        [_captureSession addOutput:dataOutput];
        
        ///
        
        stillOutput = [AVCaptureStillImageOutput new];
        NSDictionary *outputSettings = [[NSDictionary alloc] initWithObjectsAndKeys: AVVideoCodecJPEG, AVVideoCodecKey, nil];
        [stillOutput setOutputSettings:outputSettings];
        [self.captureSession addOutput:stillOutput];
        
        //
        
        [_captureSession commitConfiguration];
        
        // this will trigger capture on its own queue
        
    });
    
}


- (AVCaptureDevicePosition)cameraPosition {
    return currentPosition;
}

// Find a camera with the specified AVCaptureDevicePosition, returning nil if one is not found
- (AVCaptureDevice *) cameraWithPosition:(AVCaptureDevicePosition) position
{
    NSArray *devices = [AVCaptureDevice devicesWithMediaType:AVMediaTypeVideo];
    for (AVCaptureDevice *device in devices)
    {
        if ([device position] == position) return device;
    }
    return [AVCaptureDevice defaultDeviceWithMediaType:AVMediaTypeVideo];
}

- (void)switchCamera {
    if(currentPosition == AVCaptureDevicePositionBack) {
        [self setCameraPosition:AVCaptureDevicePositionFront];
    } else {
        [self setCameraPosition:AVCaptureDevicePositionBack];
    }
}

- (void)setCameraPosition:(AVCaptureDevicePosition) pos {
    currentPosition = pos;
    
    dispatch_async(self.cameraQueue, ^{
        
        [_captureSession beginConfiguration];
        
        //Remove existing input
        AVCaptureInput* currentCameraInput = [_captureSession.inputs objectAtIndex:0];
        [_captureSession removeInput:currentCameraInput];
        
        //Get new input
        AVCaptureDevice *newCamera = [self cameraWithPosition:currentPosition]
        ;
        //Add input to session
        NSError *err = nil;
        AVCaptureDeviceInput *newVideoInput = [[AVCaptureDeviceInput alloc] initWithDevice:newCamera error:&err];
        if(!newVideoInput || err)
        {
            NSLog(@"Error creating capture device input: %@", err.localizedDescription);
        }
        else
        {
            [_captureSession addInput:newVideoInput];
        }
        
        //Commit all the configuration changes at once
        [_captureSession commitConfiguration];
        
        [self viewWillLayoutSubviews];
    });
}

- (void)captureOutput:(AVCaptureOutput *)captureOutput didOutputSampleBuffer:(CMSampleBufferRef)sampleBuffer fromConnection:(AVCaptureConnection *)connection
{
    if(captureOutput != dataOutput) return;
    //CVReturn error;
    
    CVImageBufferRef sourceImageBuffer = CMSampleBufferGetImageBuffer(sampleBuffer);
    //size_t width = CVPixelBufferGetWidth(sourceImageBuffer);
    //size_t height = CVPixelBufferGetHeight(sourceImageBuffer);
    //size_t bpr = CVPixelBufferGetBytesPerRow(sourceImageBuffer);
    
    //ENSLog(@"BUF %lld %lld %lld %d\n", width, height, bpr, sizeof(simd::uchar4));
    
    
    if(dispatch_semaphore_wait(processingSemaphore, DISPATCH_TIME_NOW) == 0) {
        
        CFRetain(sourceImageBuffer);
        
        dispatch_async(self.processingQueue, ^{
            
            CVPixelBufferLockBaseAddress(sourceImageBuffer, 0);
            
            //uint8_t *addr = (uint8_t *)CVPixelBufferGetBaseAddress(sourceImageBuffer);
            
            self.onCameraTextureBlock(sourceImageBuffer);
            
            CVPixelBufferUnlockBaseAddress(sourceImageBuffer, 0);
            
            dispatch_semaphore_signal(processingSemaphore);
            
            CFRelease(sourceImageBuffer);
        });
        
    }
}


- (void)start {
    dispatch_async(self.cameraQueue, ^{
        
        [self.captureSession startRunning];
        
    });
}
- (void)stop;
{
    dispatch_async(self.cameraQueue, ^{
        
        [self.captureSession stopRunning];
        
    });
}

#if TARGET_OS_IPHONE

- (void)addPreviewLayer:(UIView*)aView {
    
    previewLayerView = aView;
    
    dispatch_async(self.cameraQueue, ^{
        AVCaptureSession *captureSession = self.captureSession;;
        previewLayer = [AVCaptureVideoPreviewLayer layerWithSession:captureSession];
        dispatch_async(dispatch_get_main_queue(), ^{
            
            previewLayer.frame = aView.bounds; // Assume you want the preview layer to fill the view.
            [aView.layer addSublayer:previewLayer];
            
        });
    });
    
}

- (void)viewWillLayoutSubviews {
    
    previewLayer.frame = previewLayerView.bounds;
    if (previewLayer.connection.supportsVideoOrientation) {
        previewLayer.connection.videoOrientation = [self interfaceOrientationToVideoOrientation:[UIApplication sharedApplication].statusBarOrientation];
        self.videoOrientation = previewLayer.connection.videoOrientation;
        
        [[stillOutput connectionWithMediaType:AVMediaTypeVideo] setVideoOrientation:previewLayer.connection.videoOrientation];
        
    }
    
}

- (AVCaptureVideoOrientation)interfaceOrientationToVideoOrientation:(UIInterfaceOrientation)orientation {
    switch (orientation) {
        case UIInterfaceOrientationPortrait:
            return AVCaptureVideoOrientationPortrait;
        case UIInterfaceOrientationPortraitUpsideDown:
            return AVCaptureVideoOrientationPortraitUpsideDown;
        case UIInterfaceOrientationLandscapeLeft:
            return AVCaptureVideoOrientationLandscapeLeft;
        case UIInterfaceOrientationLandscapeRight:
            return AVCaptureVideoOrientationLandscapeRight;
        default:
            break;
    }
    //NSLog(@"Warning - Didn't recognise interface orientation (%d)",orientation);
    return AVCaptureVideoOrientationPortrait;
}

#endif

@end
