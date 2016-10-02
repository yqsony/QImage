//
//  CameraCapture.h
//  viewIt
//
//  Created by Quan Yuan on 9/14/16.
//  Copyright © 2016 Quan Yuan. All rights reserved.
//

#ifndef CameraCapture_h
#define CameraCapture_h

#import <AVFoundation/AVFoundation.h>
#import <UIKit/UIKit.h>

//@class AVCaptureSession;
//@protocol MTLTexture;

@interface CameraCapture : NSObject

@property(nonatomic,strong)AVCaptureSession* captureSession;

@property(nonatomic,strong)dispatch_queue_t processingQueue;
@property(nonatomic,strong)dispatch_queue_t cameraQueue;

@property(nonatomic,copy)void(^onCameraTextureBlock)(CVImageBufferRef buf);

- (void)createSession;
- (void)start;
- (void)stop;
- (void)addPreviewLayer:(UIView*)aView;


- (void)setCameraPosition:(AVCaptureDevicePosition) pos;
- (void)switchCamera;
- (AVCaptureDevicePosition)cameraPosition;

- (void)viewWillLayoutSubviews;
@property(nonatomic,assign)AVCaptureVideoOrientation videoOrientation;


- (void)captureStill:(void(^)(UIImage*))block;

@end



#endif /* CameraCapture_h */
