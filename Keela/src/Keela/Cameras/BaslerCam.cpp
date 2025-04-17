#include "keepch.h"

#include "BaslerCam.h"
#include "Keela/Log.h"

#include <pylon/PylonIncludes.h>
#include <pylon/usb/BaslerUsbInstantCamera.h>
#include <GenApi/GenApi.h>
#include <vector>

using namespace Pylon;
using namespace GenApi;
using namespace Basler_UsbCameraParams;

using Matrix = std::vector<std::vector<int>>;

namespace Keela {

    Matrix FetchImage()
    {
        // Initialize Pylon runtime
        Pylon::PylonInitialize();

        Matrix img;  // Declare here so it's available even on early exit

        try
        {
            CBaslerUsbInstantCamera camera(CTlFactory::GetInstance().CreateFirstDevice());

            camera.Open();

            // Set pixel format to Mono8 (grayscale 8-bit)
            INodeMap& nodemap = camera.GetNodeMap();
            CEnumerationPtr pixelFormat(nodemap.GetNode("PixelFormat"));
            if (IsWritable(pixelFormat))
            {
                pixelFormat->FromString("Mono8");
            }

            // Grab a single image
            camera.StartGrabbing(1);
            CGrabResultPtr ptrGrabResult;

            camera.RetrieveResult(5000, ptrGrabResult, TimeoutHandling_ThrowException);

            if (ptrGrabResult->GrabSucceeded())
            {
                int width = ptrGrabResult->GetWidth();
                int height = ptrGrabResult->GetHeight();
                const uint8_t* buffer = static_cast<const uint8_t*>(ptrGrabResult->GetBuffer());

                img.resize(height, std::vector<int>(width));
                for (int y = 0; y < height; ++y)
                {
                    for (int x = 0; x < width; ++x)
                    {
                        img[y][x] = buffer[y * width + x];  // Mono8 = 1 byte per pixel
                    }
                }

                KEE_CORE_INFO("Top-left pixel value: {}", img[0][0]);
            }
            else
            {
                KEE_CORE_ERROR("Grab failed!");
            }

            camera.Close();
        }
        catch (const GenericException& e)
        {
            KEE_CORE_ERROR("Pylon Exception: {}", e.GetDescription());
        }

        // Clean up the Pylon runtime
        Pylon::PylonTerminate();

        return img;  // Return even if empty
    }
}
