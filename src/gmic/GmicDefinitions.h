#ifndef GMICDEFINITIONS_H
#define GMICDEFINITIONS_H

#include <QString>

namespace Cascade
{
namespace Gmic
{
    enum InputMode
    {
      NoInput,
      Active,
      All,
      ActiveAndBelow,
      ActiveAndAbove,
      AllVisible,
      AllInvisible,
      AllVisiblesDesc_UNUSED,   /* Removed since 2.8.2 */
      AllInvisiblesDesc_UNUSED, /* Removed since 2.8.2 */
      AllDesc_UNUSED,           /* Removed since 2.8.2 */
      UnspecifiedInputMode = 100
    };
    static InputMode DefaultInputMode;

    enum OutputMode
    {
      InPlace,
      NewLayers,
      NewActiveLayers,
      NewImage,
      UnspecifiedOutputMode = 100
    };
    static OutputMode DefaultOutputMode;

    enum PreviewMode
    {
      FirstOutput,
      SecondOutput,
      ThirdOutput,
      FourthOutput,
      First2SecondOutput,
      First2ThirdOutput,
      First2FourthOutput,
      AllOutputs,
      UnspecifiedPreviewMode = 100
    };
    static PreviewMode DefaultPreviewMode;

    enum OutputMessageMode
    {
      Quiet,
      VerboseLayerName,
      VerboseConsole,
      VerboseLogFile,
      VeryVerboseConsole,
      VeryVerboseLogFile,
      DebugConsole,
      DebugLogFile,
      UnspecifiedOutputMessageMode = 100
    };
    static OutputMessageMode DefaultOutputMessageMode;

}
}

#endif // GMICDEFINITIONS_H
