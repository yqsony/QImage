//
//  QImageUtils.hpp
//  QImageTest
//
//  Created by Quan Yuan on 11/21/16.
//  Copyright © 2016 Quan Yuan. All rights reserved.
//

#ifndef QImageUtils_hpp
#define QImageUtils_hpp

#include <stdio.h>
#include <vector>
#include <string>

namespace QImage{
    namespace utils{
        #pragma pack(push, 1)
        struct BITMAPINFOHEADER
        {
            uint32_t biSize;  //specifies the number of bytes required by the struct
            int32_t biWidth;  //specifies width in pixels
            int32_t biHeight;  //species height in pixels
            uint16_t biPlanes; //specifies the number of color planes, must be 1
            uint16_t biBitCount; //specifies the number of bit per pixel
            int32_t biCompression;//spcifies the type of compression
            int32_t biSizeImage;  //size of image in bytes
            int32_t biXPelsPerMeter;  //number of pixels per meter in x axis
            int32_t biYPelsPerMeter;  //number of pixels per meter in y axis
            uint32_t biColorUsed;  //number of colors used by th ebitmap
            uint32_t biColorrImportant;  //number of colors that are important
        };
        #pragma pack(pop)
        #pragma pack(push, 1)
        struct BMPFILEHEADER{
            uint16_t type;                 /* Magic identifier            */
            uint32_t size;                       /* File size in bytes          */
            uint32_t reserved;
            uint32_t offset;                     /* Offset to image data, bytes */
        };
        #pragma pack(pop)
        
        //load bmp files, set data=nullptr to load w,h, nch only
        int loadBmpWidthHeightChannels(std::string bmpFileName, int& width, int& height, int& channels, uint8_t* data);
        //write to bmp image
        int write2BMP(uint8_t* data,int w,int h, int nch, std::string bmpFileName);
        
        class HoughLines{
        public:
            HoughLines(size_t w, size_t h);
            ~HoughLines();
            // add an edge point and cast vote on hough map
            void  addEdgePoint(size_t x, size_t y, float dx, float dy);
            // find the max line
            float findMaxLine(float& maxPho, float& maxAngle);
            // set map to zeros
            int cleanup();
        private:
            float* mVoteMap;
            size_t mMapAngleMax, mMapPhoMax;
            int addPoint(std::tuple<float, float, float>& p);
        };
        
        
    }
}





#endif /* QImageUtils_hpp */
