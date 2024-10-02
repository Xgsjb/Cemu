#pragma once

#include "audio/IAudioAPI.h"

namespace AndroidAudio
{
    // 添加 LC3 枚举值
    enum class AudioAPI
    {
        Default,
        Cubeb,
        LC3 // 新增的 LC3 音频编码格式
    };

    void createAudioDevice(AudioAPI audioApi, sint32 channels, sint32 volume, bool isTV = true);
    void setAudioVolume(sint32 volume, bool isTV = true);
}; // namespace AndroidAudio
