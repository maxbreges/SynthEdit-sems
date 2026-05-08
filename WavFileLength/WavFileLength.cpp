#include "mp_sdk_audio.h"
#include <fstream>
#include <string>

using namespace gmpi;

class WavFileLength final : public MpBase2
{
    StringInPin pinFileName;
    IntOutPin pinFormat; // Using this pin to output length in samples
    StringOutPin pinDebug;
public:
    WavFileLength()
    {
        initializePin(pinFileName);
        initializePin(pinFormat);
        initializePin(pinDebug);
    }

    void onSetPins() override
    {
        if (pinFileName.isUpdated())
        {
            std::wstring filename = pinFileName.getValue();

            // Open WAV file
            std::ifstream file(filename, std::ios::binary);
            if (!file)
            {
                // Failed to open file, set output to 0 or handle error
                pinFormat.setValue(-1);
                pinDebug = L"Failed to open file";
                return;
            }

            // Read WAV header
            struct WAVHeader {
                char riff[4];            // "RIFF"
                uint32_t chunkSize;      // Size of the overall file - 8 bytes
                char wave[4];            // "WAVE"
                char fmt[4];             // "fmt "
                uint32_t subchunk1Size;   // Size of the fmt chunk
                uint16_t audioFormat;     // PCM = 1
                uint16_t numChannels;     // Number of channels
                uint32_t sampleRate;      // Sampling rate
                uint32_t byteRate;        // bytes per second
                uint16_t blockAlign;      // block align
                uint16_t bitsPerSample;   // bits per sample
            } header;

            file.read(reinterpret_cast<char*>(&header), sizeof(WAVHeader));

            // Search for 'data' chunk
            char subchunkID[4];
            uint32_t subchunkSize;

            while (true)
            {
                if (!file.read(subchunkID, 4))
                {
                    // Failed to find 'data' chunk
                    pinFormat.setValue(-1);
                    pinDebug = L"Failed to find 'data' chunk";
                    return;
                }

                if (!file.read(reinterpret_cast<char*>(&subchunkSize), 4))
                {
                    // Failed to read subchunk size
                    pinFormat.setValue(-2);
                    pinDebug = L"Failed to read subchunk size";
                    return;
                }

                if (std::string(subchunkID, 4) == "data")
                {
                    pinDebug = L"Found data chunk";
                    break; // Found data chunk
                }
                // Skip this chunk
                file.seekg(subchunkSize, std::ios::cur);
            }

            // Calculate total samples
            uint32_t dataSize = subchunkSize;
            uint32_t totalSamples = dataSize / (header.numChannels * header.bitsPerSample / 8);

            // Output the length in samples
            pinFormat.setValue(totalSamples);
        }

        if (pinFormat.isUpdated())
        {
            // Optionally handle if pinFormat is also updated
        }
    }
};

namespace
{
    auto r = Register<WavFileLength>::withId(L"WavFileLength");
}