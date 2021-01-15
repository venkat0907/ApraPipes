#pragma once

#include "AV4L2ElementPlane.h"
#include "V4L2CUYUV420Converter.h"

#include "ExtFrame.h"
#include <boost/pool/object_pool.hpp>

class H264EncoderV4L2Helper
{
public:
    typedef std::function<void (frame_sp&)> SendFrame;

    H264EncoderV4L2Helper(int width, int height, uint32_t bitrate, uint32_t fps, SendFrame sendFrame);
    ~H264EncoderV4L2Helper();    

    // data is cuda rgb data pointer and should be already synced
    bool process(uint8_t* data, size_t size);
    bool processEOS();

private:    
    void initV4L2();
    void termV4L2();

    void setBitrate(uint32_t bitrate);
    void setProfile();
    void setLevel();
    void setFrameRate(uint32_t framerate_num, uint32_t framerate_den);

    void initEncoderParams(uint32_t bitrate, uint32_t fps);
    int setExtControls(v4l2_ext_control &control);

    void capturePlaneDQCallback(AV4L2Buffer * buffer);
    void reuseCatureBuffer(ExtFrame *pointer, uint32_t index);

private:

    int mFD;
    std::unique_ptr<AV4L2ElementPlane> mOutputPlane;
    std::unique_ptr<AV4L2ElementPlane> mCapturePlane;

    boost::object_pool<ExtFrame> frame_opool;
    SendFrame mSendFrame;

    std::unique_ptr<V4L2CUYUV420Converter> mConverter;
};