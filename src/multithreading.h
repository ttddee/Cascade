#ifndef MULTITHREADING_H
#define MULTITHREADING_H

#include <QString>

#include <OpenColorIO/OpenColorIO.h>
#include <OpenImageIO/imagebuf.h>
#include <tbb/tbb.h>

using namespace OpenColorIO;
using namespace OIIO;
using namespace tbb;

namespace Cascade
{

//void copyPixel(const float* source, float* dst)
//{
//    memcpy(dst, source, 4);
//}

//void parallelArrayCopy(const float* source, float* dst, size_t n)
//{
//    parallel_for(blocked_range<size_t>(0, n),
//        [=](const tbb::blocked_range<size_t>& r)
//    {
//        for(size_t i = r.begin(); i!=r.end(); ++i)

//            copyPixel(source + i, dst + i);
//    });

//}

void applyColorToScanline(
        OpenColorIO::ConstProcessorRcPtr processor,
        float* pStart,
        int idx,
        int lineWidth)
{
    OpenColorIO::PackedImageDesc desc(
                pStart + idx * lineWidth * 4,
                lineWidth,
                1,
                4);
    processor->apply(desc);
}

void parallelApplyColorSpace(
        OpenColorIO::ConstConfigRcPtr ocioConfig,
        const QString& sourceColor,
        const QString& dstColor,
        float* pStart,
        int width,
        int height)
{
    OpenColorIO::ConstProcessorRcPtr processor = ocioConfig->getProcessor(
                sourceColor.toLocal8Bit(), dstColor.toLocal8Bit());

    parallel_for(blocked_range<size_t>(0, height),
        [=](const tbb::blocked_range<size_t>& r)
    {
        for(size_t i = r.begin(); i!=r.end(); ++i)

            applyColorToScanline(processor, pStart, i, width);
    });
}



}

#endif // MULTITHREADING_H
