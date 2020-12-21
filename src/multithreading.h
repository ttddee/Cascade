#ifndef MULTITHREADING_H
#define MULTITHREADING_H

#include <QString>

#include <OpenColorIO/OpenColorIO.h>
#include <OpenImageIO/imagebuf.h>
#include <tbb/tbb.h>

namespace OCIO = OCIO_NAMESPACE;
using namespace OIIO;
using namespace tbb;

namespace Cascade
{

void copyRow(const float* source, float* dst, size_t width, size_t i)
{
    memcpy(dst + i * width, source + i * width, width * 4);
}

void parallelArrayCopy(const float* src, float* dst, size_t width, size_t height)
{

    parallel_for(blocked_range<size_t>(0, height * 4),
        [=](const tbb::blocked_range<size_t>& r)
    {
        for(size_t i = r.begin(); i!=r.end(); ++i)
        {
            copyRow(src, dst, width, i);
        }
    });

}

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
