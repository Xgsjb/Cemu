#include "AndroidAudio.h"

#include "Cafe/OS/libs/snd_core/ax.h"
#include "audio/IAudioAPI.h"

#if HAS_CUBEB
#include "audio/CubebAPI.h"
#endif // HAS_CUBEB

// 如果需要，可以引入 LC3 相关头文件
// #include "audio/LC3API.h"

namespace AndroidAudio
{
    void createAudioDevice(AudioAPI audioApi, sint32 channels, sint32 volume, bool isTV)
    {
        static constexpr int AX_FRAMES_PER_GROUP = 4;
        std::unique_lock lock(g_audioMutex);
        auto& audioDevice = isTV ? g_tvAudio : g_padAudio;

        switch (channels)
        {
        case 0:
            channels = 1; // 默认单声道
            break;
        case 2:
            channels = 6; // 处理环绕声或特殊配置
            break;
        default: // 默认立体声
            channels = 2;
            break;
        }

        switch (audioApi)
        {
#if HAS_CUBEB
        case AudioAPI::Cubeb:
        {
            audioDevice.reset();
            std::shared_ptr<CubebAPI::CubebDeviceDescription> deviceDescriptionPtr =
                std::make_shared<CubebAPI::CubebDeviceDescription>(nullptr, std::string(), std::wstring());
            audioDevice = IAudioAPI::CreateDevice(IAudioAPI::AudioAPI::Cubeb, deviceDescriptionPtr,
                48000, channels, snd_core::AX_SAMPLES_PER_3MS_48KHZ * AX_FRAMES_PER_GROUP, 16);
            audioDevice->SetVolume(volume);
            break;
        }
#endif // HAS_CUBEB

        case AudioAPI::LC3: // 处理 LC3 音频编码
        {
            audioDevice.reset();
            // 这里添加 LC3 设备创建逻辑
            // 例如，使用 LC3 API 创建音频设备
            // audioDevice = IAudioAPI::CreateDevice(...);
            audioDevice->SetVolume(volume);
            break;
        }

        default:
            cemuLog_log(LogType::Force, "Invalid audio api: {}", static_cast<int>(audioApi));
            break;
        }
    }

    void setAudioVolume(sint32 volume, bool isTV)
    {
        std::shared_lock lock(g_audioMutex);
        auto& audioDevice = isTV ? g_tvAudio : g_padAudio;
        if (audioDevice)
            audioDevice->SetVolume(volume);
    }

}; // namespace AndroidAudio
