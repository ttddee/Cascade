#ifndef GLOBALS_H
#define GLOBALS_H

#include <vector>
#include <array>
#include <algorithm>
#include <stdexcept>

namespace Cascade
{
    typedef std::vector<unsigned int> ShaderCode;

    template <typename Key, typename Value, std::size_t Size>
    struct Map {
        std::array<std::pair<Key, Value>, Size> data;

        [[nodiscard]] constexpr Value at(const Key& key) const
        {
            const auto itr = std::find_if(begin(data), end(data), [&key](const auto &v)
            {
                return v.first == key;
            });
            if (itr != end(data))
            {
                return itr->second;
            }
            else
            {
                throw std::range_error("Not found.");
            }
        }
    };
}

#endif // GLOBALS_H
