/*
 *  Cascade Image Editor
 *
 *  Copyright (C) 2020 The Cascade developers
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef OBJECTMANAGER_H
#define OBJECTMANAGER_H

#include "../vulkanwindow.h"
#include "csimage.h"

namespace Cascade
{
    class ObjectManager
    {
    public:
        static void init(VulkanWindow* w, QVulkanDeviceFunctions* df, VkDevice* d);

        static std::shared_ptr<CsImage> createImage(int width, int height);

        static void cleanup();

    private:
        static VulkanWindow* window;
        static QVulkanDeviceFunctions *devFuncs;
        static VkDevice* device;

        static std::vector<std::shared_ptr<CsImage>> images;
    };
}

#endif // OBJECTMANAGER_H
