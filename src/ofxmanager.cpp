#include "ofxmanager.h"

OfxManager::OfxManager()
    : ofxHost(CsOfxHost::Host()),
      imageEffectPluginCache(OFX::Host::ImageEffect::PluginCache(&ofxHost))
{

    // set the version label in the global cache
    OFX::Host::PluginCache::getPluginCache()->setCacheVersion("hostDemoV1");

    // create our derived image effect host which provides
    // a factory to make plugin instances and acts
    // as a description of the host application
    //CsOfxHost::Host ofxHost;

    // make an image effect plugin cache. This is what knows about
    // all the plugins.
   // imageEffectPluginCache = OFX::Host::ImageEffect::PluginCache(&ofxHost);

    // register the image effect cache with the global plugin cache
    imageEffectPluginCache.registerInCache(*OFX::Host::PluginCache::getPluginCache());

    // try to read an old cache
    std::ifstream ifs("CsOfxPluginCache.xml");
    try
    {
        OFX::Host::PluginCache::getPluginCache()->readCache(ifs);
    } catch (const std::exception &e)
    {
        std::cerr << "Error while reading XML cache: " << e.what() << std::endl;
    }
    OFX::Host::PluginCache::getPluginCache()->scanPluginFiles();
    ifs.close();

    /// flush out the current cache
    std::ofstream of("CsOfxPluginCache.xml");
    OFX::Host::PluginCache::getPluginCache()->writePluginCache(of);
    of.close();

    // get the invert example plugin which uses the OFX C++ support code
    plugin = imageEffectPluginCache.getPluginById("uk.co.thefoundry.OfxInvertExample");

    imageEffectPluginCache.dumpToStdOut();

}

std::shared_ptr<CsOfxHost::CsOfxImage> OfxManager::renderOFX(
        const OfxPointI renderSize,
        float* pInput)
{
    if(plugin)
    {
        std::cout << "Found a plugin" << std::endl;
        // create an instance of it as a filter
        // the first arg is the context, the second is client data we are allowed to pass down the call chain

        instance = OFX::Host::auto_ptr<OFX::Host::ImageEffect::Instance>(plugin->createInstance(kOfxImageEffectContextFilter, NULL));

        if(instance.get())
        {
            OfxStatus stat;

            // now we need to call the create instance action. Only call this once you have initialised all the params
            // and clips to their correct values. So if you are loading a saved plugin state, set up your params from
            // that state, _then_ call create instance.
            stat = instance->createInstanceAction();
            assert(stat == kOfxStatOK || stat == kOfxStatReplyDefault);

            // now we need to to call getClipPreferences on the instance so that it does the clip component/depth
            // logic and caches away the components and depth on each clip.
            bool ok = instance->getClipPreferences();
            assert(ok);

            // current render scale of 1
            OfxPointD renderScale;
            renderScale.x = renderScale.y = 1.0;

            // The render window is in pixel coordinates
            // ie: render scale and a PAR of not 1
            OfxRectI  renderWindow;
            renderWindow.x1 = renderWindow.y1 = 0;
            renderWindow.x2 = renderSize.x;
            renderWindow.y2 = renderSize.y;

            /// RoI is in canonical coords,
            OfxRectD  regionOfInterest;
            regionOfInterest.x1 = regionOfInterest.y1 = 0;
            regionOfInterest.x2 = renderSize.x;
            regionOfInterest.y2 = renderSize.y;

            int numFramesToRender = OFXCLIPLENGTH;

            // say we are about to render a bunch of frames
            stat = instance->beginRenderAction(
                        0, numFramesToRender,
                        1.0,
                        false,
                        renderScale,
                        /*sequential=*/true,
                        /*interactive=*/false,
                        /*draftRender=*/false);
            assert(stat == kOfxStatOK || stat == kOfxStatReplyDefault);

//            CsOfxHost::CsOfxClipInstance* inputClip = dynamic_cast<CsOfxHost::CsOfxClipInstance*>(instance->getClip("Source"));
//            assert(inputClip);
//            inputClip->setSourceImage(pInput, renderSize);

            // get the output clip
            outputClip = dynamic_cast<CsOfxHost::CsOfxClipInstance*>(instance->getClip("Output"));
            assert(outputClip);

            // call get region of interest on each of the inputs
            OfxTime frame = 0;

            // get the RoI for each input clip
            // the regions of interest for each input clip are returned in a std::map
            // on a real host, these will be the regions of each input clip that the
            // effect needs to render a given frame (clipped to the RoD).
            //
            // In our example we are doing full frame fetches regardless.
            std::map<OFX::Host::ImageEffect::ClipInstance *, OfxRectD> rois;
            stat = instance->getRegionOfInterestAction(
                        frame,
                        renderScale,
                        regionOfInterest,
                        rois);
            assert(stat == kOfxStatOK || stat == kOfxStatReplyDefault);

            // render a frame
            stat = instance->renderAction(
                        /*time*/ 0,
                        /*field*/ kOfxImageFieldNone,
                        renderWindow,
                        renderScale,
                        /*sequential=*/true,
                        /*interactive=*/false,
                        /*draftRender=*/false);
            assert(stat == kOfxStatOK);

            // get the output image buffer
            outputImage = std::shared_ptr<CsOfxHost::CsOfxImage>(outputClip->getOutputImage());

            //std::ostringstream ss;
            //ss << "Output." << t << ".ppm";
            //exportToPPM(ss.str(), outputImage);

            return outputImage;

            instance->endRenderAction(
                        0,
                        numFramesToRender,
                        1.0,
                        false,
                        renderScale,
                        /*sequential=*/true,
                        /*interactive=*/false,
                        /*draftRender=*/false);
        }
    }
    //return nullptr;
    OFX::Host::PluginCache::clearPluginCache();
}

void OfxManager::exportToPPM(const std::string& fname, CsOfxHost::CsOfxImage* im)
{
    std::ofstream op(fname.c_str());
    OfxRectI rod = im->getROD();
    op << "P3" << "\t# FORMAT" << std::endl;
    op << rod.x2 - rod.x1 << "\t#WIDTH" << std::endl;
    op << rod.y2 - rod.y1 << "\t#HEIGHT" <<std::endl;
    //This assumes 8-bit.
    op << "255" << std::endl;
    for (int y = rod.y1; y< rod.y2; ++y)
    {
      for (int x = rod.x1; x < rod.x2; ++x)
      {
        OfxRGBAColourB* pix = im->pixel(x,y);
        if(pix)
          op << (int)pix->r << " " << (int)pix->g << " " << (int)pix->b << " " << std::endl;
        else
          op << "0 0 0" << std::endl;
      }
    }
    std::cout << "Wrote PPM." << std::endl;
}
