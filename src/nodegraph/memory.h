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
 *
 *  NodeEditor code adapted from:
 *  Dmitry Pinaev et al, Qt Node Editor, (2017), GitHub repository, https://github.com/paceholder/nodeeditor
*/

#pragma once

#include <memory>
#include <utility>

namespace Cascade::NodeGraph
{
//  namespace detail {
//#if (!defined(_MSC_VER) && (__cplusplus < 201300)) || \
//    ( defined(_MSC_VER) && (_MSC_VER < 1800))
////_MSC_VER == 1800 is Visual Studio 2013, which is already somewhat C++14 compilant,
//// and it has make_unique in it's standard library implementation
//    template<typename T, typename... Args>
//    std::unique_ptr<T> make_unique(Args&&... args)
//    {
//      return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
//    }
//#else
//    template<typename T, typename... Args>
//    std::unique_ptr<T> make_unique(Args&&... args)
//    {
//      return std::make_unique<T>(std::forward<Args>(args)...);
//    }
//#endif
//  }
}
