#include "Image.h"
#include <cstdint>  // 包含 uint8_t 定义
#include <vector>   // 包含 std::vector 定义

#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_TGA

#include "stb_image.h"

Image::Image(Image&& image) noexcept
{
    m_image = image.m_image;
    m_width = image.m_width;
    m_height = image.m_height;
    m_channels = image.m_channels;
    image.m_image = nullptr;
}

Image::Image(const std::vector<uint8_t>& imageBytes)  // 使用 uint8_t
{
    m_image = stbi_load_from_memory(imageBytes.data(), imageBytes.size(), &m_width, &m_height, &m_channels, STBI_rgb_alpha);
    if (m_image)
    {
        for (int i = 0; i < m_width * m_height * 4; i += 4)
        {
            uint8_t r = m_image[i];
            uint8_t g = m_image[i + 1];
            uint8_t b = m_image[i + 2];
            uint8_t a = m_image[i + 3];
            m_image[i] = b;
            m_image[i + 1] = g;
            m_image[i + 2] = r;
            m_image[i + 3] = a;
        }
    }
}

bool Image::isOk() const
{
    return m_image != nullptr;
}

int* Image::intColors() const
{
    return reinterpret_cast<int*>(m_image);
}

Image::~Image()
{
    if (m_image)
        stbi_image_free(m_image);
}
