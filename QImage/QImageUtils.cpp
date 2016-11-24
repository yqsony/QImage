//
//  QImageUtils.cpp
//  QImageTest
//
//  Created by Quan Yuan on 11/21/16.
//  Copyright © 2016 Quan Yuan. All rights reserved.
//

#include "QImageUtils.hpp"
#include <assert.h>
#include <iostream>
#include <math.h>

namespace QImage{
    namespace utils{
        
        
        int loadBmpWidthHeightChannels(std::string bmpFileName, int& width, int& height, int& nch, uint8_t* data){
            BMPFILEHEADER bitmapFileHeader;
            FILE *f = fopen(bmpFileName.c_str(),"rb");
            fread(&bitmapFileHeader, sizeof(BMPFILEHEADER),1,f);
            if (bitmapFileHeader.type !=0x4D42)
            {
                fclose(f);
                return -1;
            }
            else{
                BITMAPINFOHEADER bitmapInfoHeader;
                fread(&bitmapInfoHeader, sizeof(BITMAPINFOHEADER),1,f);
                width = bitmapInfoHeader.biWidth;
                height = bitmapInfoHeader.biHeight;
                int bitCount = bitmapInfoHeader.biBitCount;
                nch = bitCount/8;
                if(data){
                    fseek(f, bitmapFileHeader.offset, SEEK_SET); //bitmapFileHeader.offset
                    if(height > 0){ //bottom up bmp
                        uint8_t *p = data+width*(height-1); //last row to first row
                        for(size_t i=0;i<height;i++){
                            size_t t = fread(p, sizeof(uint8_t), width,f); //read from last row to first row
                            assert(t == width);
                            fseek(f, (4-(width*nch)%4)%4, SEEK_CUR);
                            p -= width;
                        }
                        fclose(f);
                        return 0;
                    }
                    else{ //height < 0, top down bmp
                        uint8_t *p = data; //top row to bottm row
                        for(size_t i=0;i<-height;i++){
                            size_t t = fread(p, sizeof(uint8_t), width,f); //read from last row to first row
                            assert(t == width);
                            fseek(f, (4-(width*nch)%4)%4, SEEK_CUR);
                            p += width;
                        }
                        fclose(f);
                        return 0;
                    }
                }
                else{ // read width,height,nchannels only
                    fclose(f);
                    return 0;
                }
            }
            return 0;
        }
        
        int write2BMP(uint8_t* data,int w,int h, int nch, std::string bmpFileName){
            
            int filesize = 54 + nch*w*h;  //w is your image width, h is image height, both int
            unsigned char bmpfileheader[14] = {'B','M', 0,0,0,0, 0,0, 0,0, 54,0,0,0};
            unsigned char bmpinfoheader[40] = {40,0,0,0, 0,0,0,0, 0,0,0,0, 1,0, static_cast<unsigned char>(8*nch),0};
            
            unsigned char bmppad[3] = {0,0,0};
            
            bmpfileheader[ 2] = (unsigned char)(filesize    );
            bmpfileheader[ 3] = (unsigned char)(filesize>> 8);
            bmpfileheader[ 4] = (unsigned char)(filesize>>16);
            bmpfileheader[ 5] = (unsigned char)(filesize>>24);
            
            bmpinfoheader[ 4] = (unsigned char)(       w    );
            bmpinfoheader[ 5] = (unsigned char)(       w>> 8);
            bmpinfoheader[ 6] = (unsigned char)(       w>>16);
            bmpinfoheader[ 7] = (unsigned char)(       w>>24);
            bmpinfoheader[ 8] = (unsigned char)(       h    );
            bmpinfoheader[ 9] = (unsigned char)(       h>> 8);
            bmpinfoheader[10] = (unsigned char)(       h>>16);
            bmpinfoheader[11] = (unsigned char)(       h>>24);
            
            FILE *f = fopen(bmpFileName.c_str(),"wb");
            if(f == 0){
                std::cout<<"can't open bmp file to write: "<<bmpFileName<<std::endl;
                return -1;
            }
            
            fwrite(bmpfileheader,1,14,f);
            fwrite(bmpinfoheader,1,40,f);
            for(unsigned i=0; i<h; i++)
            {
                fwrite(data+(w*(h-i-1)*nch),nch,w,f);
                fwrite(bmppad,1,(4-(w*nch)%4)%4,f);
            }
            fclose(f);
            
            return 0;
        }

        HoughLines::HoughLines(size_t w, size_t h){
            mMapAngleMax = w;
            mMapPhoMax = h;
            mVoteMap = (float*)malloc(mMapAngleMax*mMapPhoMax*sizeof(float));
            cleanup();
        }
        
        HoughLines::~HoughLines(){
            free(mVoteMap);
        }
        
        int HoughLines::cleanup(){
            memset(mVoteMap, 0, sizeof(float)*mMapAngleMax*mMapPhoMax);
            return 0;
        }
        
        void  HoughLines::addEdgePoint(size_t x, size_t y, float dx, float dy){
            float weight = sqrtf(dx*dx+dy*dy);
            float angle = atan2f(dy, dx);
            // range in within pi/2
            
            for(float a = angle-M_PI_4;a<angle-M_PI_4;a+=0.1){
                float r = x*cosf(a)+y*sinf(a);
                std::tuple<float, float, float> p = std::make_tuple(r, a, weight);
                this->addPoint(p);
            }
        }
        
        int HoughLines::addPoint(std::tuple<float, float, float> &p){
            size_t aIdx = (int)round(std::get<0>(p))%mMapAngleMax;
            size_t rIdx = (int)round(std::get<1>(p))%mMapPhoMax;
            mVoteMap[rIdx*mMapAngleMax+aIdx] += std::get<2>(p);
            return 0;
        }
        
        float HoughLines::findMaxLine(float& maxPho, float& maxAngle){
            float mapMax = 0;
            for(size_t i=0;i<mMapPhoMax;i++){
                for(size_t j=0;j<mMapAngleMax;j++){
                    if(mVoteMap[i*mMapAngleMax+j] > mapMax){
                        mapMax = mVoteMap[i*mMapAngleMax+j];
                        maxAngle = j;
                        maxPho = i;
                    }
                }
            }
            return mapMax;
        }
        
        
        
        
        
    }
}
