#ifndef MULTITHREADING_H
#define MULTITHREADING_H

#include <QString>

#include <OpenColorIO/OpenColorIO.h>
#include <OpenImageIO/imagebuf.h>

// Prevent tbb emit() from clashing with Qt. Wtf.
#ifndef Q_MOC_RUN
#if defined(emit)
    #undef emit
    #include <tbb/tbb.h>
    #define emit
#else
    #include <tbb/tbb.h>
#endif // defined(emit)
#endif // Q_MOC_RUN

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
        OCIO::ConstCPUProcessorRcPtr processor,
        float* pStart,
        int idx,
        int lineWidth)
{
    OCIO::PackedImageDesc desc(
                pStart + idx * lineWidth * 4,
                lineWidth,
                1,
                4);
    processor->apply(desc);
}

void parallelApplyColorSpace(
        OCIO::ConstConfigRcPtr ocioConfig,
        const QString& sourceColor,
        const QString& dstColor,
        float* pStart,
        int width,
        int height)
{
    OCIO::ConstProcessorRcPtr processor = ocioConfig->getProcessor(
                sourceColor.toLocal8Bit(), dstColor.toLocal8Bit());

    OCIO::ConstCPUProcessorRcPtr cpuProcessor = processor->getOptimizedCPUProcessor(OCIO::OPTIMIZATION_DEFAULT);

    parallel_for(blocked_range<size_t>(0, height),
        [=](const tbb::blocked_range<size_t>& r)
    {
        for(size_t i = r.begin(); i!=r.end(); ++i)

            applyColorToScanline(cpuProcessor, pStart, i, width);
    });
}

}

#endif // MULTITHREADING_H
