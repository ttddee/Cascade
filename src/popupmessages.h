/*
 *  Cascade Image Editor
 *
 *  Copyright (C) 2022 Till Dechent and contributors
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

#ifndef POPUPMESSAGES_H
#define POPUPMESSAGES_H

#include <QWidget>
#include <QMap>
#include <QMessageBox>

namespace Cascade {

struct MessageBoxProperties
{
    const QString title;
    const QString text;
    const QSize size;
};

enum MessageBoxType
{
    MESSAGEBOX_NO_GPU_FOUND,
    MESSAGEBOX_DEVICE_LOST,
    MESSAGEBOX_FILE_SAVE_SUCCESS,
    MESSAGEBOX_FILES_SAVE_SUCCESS,
    MESSAGEBOX_FILE_SAVE_PROBLEM,
    MESSAGEBOX_NO_FOLDER_SET,
    MESSAGEBOX_FAILED_INITIALIZATION
};

const static MessageBoxProperties noGpuFoundBoxProperties =
{
    "Error",
    "No compatible GPU found. Shutting down!",
    QSize(500, 200)
};

const static MessageBoxProperties deviceLostBoxProperties =
{
    "Device Lost",
    "We lost contact to the GPU and need to shut down. Sorry!",
    QSize(500, 200)
};

const static MessageBoxProperties fileSaveSuccessBoxProperties =
{
    "Success",
    "File saved successfully.",
    QSize(500, 200)
};

const static MessageBoxProperties filesSaveSuccessBoxProperties =
{
    "Success",
    "Files saved successfully.",
    QSize(500, 200)
};

const static MessageBoxProperties fileSaveProblemBoxProperties =
{
    "Error",
    "There was a problem saving the file.",
    QSize(500, 200)
};

const static MessageBoxProperties noFolderSetBoxProperties =
{
    "No folder set",
    "Please select a folder.",
    QSize(500, 200)
};

const static MessageBoxProperties failedInitializationBoxProperties =
{
    "Error",
    "Failed to initialize Vulkan.\n"
    "\n"
    "It could be that your GPU is not supported, or your driver needs to be updated.\n"
    "You can find a list of supported GPUs at vulkan.gpuinfo.org\n"
    "\n"
    "You can open an issue at: github.com/ttddee/Cascade",
    QSize(500, 200)
};

const static std::unordered_map<MessageBoxType, MessageBoxProperties> messageBoxes =
{
    { MESSAGEBOX_NO_GPU_FOUND, noGpuFoundBoxProperties },
    { MESSAGEBOX_DEVICE_LOST, deviceLostBoxProperties },
    { MESSAGEBOX_FILE_SAVE_SUCCESS, fileSaveSuccessBoxProperties },
    { MESSAGEBOX_FILES_SAVE_SUCCESS, filesSaveSuccessBoxProperties },
    { MESSAGEBOX_FILE_SAVE_PROBLEM, fileSaveProblemBoxProperties },
    { MESSAGEBOX_NO_FOLDER_SET, noFolderSetBoxProperties },
    { MESSAGEBOX_FAILED_INITIALIZATION, failedInitializationBoxProperties }
};

inline void executeMessageBox(const MessageBoxType type)
{
    QMessageBox box;
    auto props = messageBoxes.at(type);
    box.setWindowTitle(props.title);
    box.setText(props.text);
    box.setMinimumSize(props.size);
    box.exec();
}

} // namespace Cascade

#endif // POPUPMESSAGES_H
