#include "mp_sdk_audio.h"
#include <vector>
#include <fstream>

using namespace std;
using namespace gmpi;

class ArrayTest final : public MpBase2
{
    StringInPin pinFile;
    IntInPin pinArraySize;
    IntInPin pinIndex;
    StringInPin pinValueIn;
    BoolInPin pinBool;    // true: write; false: read
    BoolInPin pinClear;

    StringOutPin pinValueOut;

    vector<string> arrayValues;
    std::wstring currentFilePath; // To detect changes in file path
    bool fileLoaded = false;
    bool dataModified = false;
    std::wstring lastFilePath;

public:
    ArrayTest()
    {
        initializePin(pinFile);
        initializePin(pinArraySize);
        initializePin(pinIndex);
        initializePin(pinValueIn);
        initializePin(pinBool);
        initializePin(pinClear);
        initializePin(pinValueOut);
    }

    void loadFile()
    {
        std::wstring currentPath = pinFile.getValue();

        if (fileLoaded && currentPath == lastFilePath)
            return; // Already loaded for this path

        lastFilePath = currentPath; // Save current path

        std::ifstream file(currentPath);
        if (file.is_open())
        {
            arrayValues.clear();
            std::string line;
            while (std::getline(file, line))
            {
                try
                {
                    arrayValues.push_back(line);
                }
                catch (...)
                {
                    // ignore invalid lines
                }
            }
            file.close();
            fileLoaded = true;
        }
    }

    void saveFile()
    {
        if (!dataModified) return;

        std::wstring filePathW = pinFile.getValue();
        std::ofstream file(filePathW);
        if (file.is_open())
        {
            for (size_t i = 0; i < arrayValues.size(); ++i)
            {
                file << arrayValues[i] << std::endl;
            }
            file.close();
            dataModified = false;
        }
    }

    void run()
    {
        // Save current array to file if modified
        saveFile();

        // Write data if in write mode
        bool writeMode = pinBool.getValue();
        int size = pinArraySize.getValue();
        int index = pinIndex.getValue();

        // Load file if path changed
        if (pinFile.isUpdated() || (pinFile.getValue() != currentFilePath))
        {
            loadFile();
        }

        // Handle clearing the array and file
        if (pinClear.isUpdated() && pinClear.getValue())
        {
            std::wstring filePathW = pinFile.getValue();

            std::ofstream file(filePathW, std::ios::trunc);
            file.close();

            arrayValues.clear();

            // Reset pinClear after clearing
            pinClear.setValue(false);
            return; // Exit early after clearing
        }

        // Resize array if needed
        if (pinArraySize.isUpdated() || arrayValues.size() != static_cast<size_t>(size))
        {
            arrayValues.resize(size);
        }

        // Check bounds
        if (index < 0 || index >= size)
        {
            return; // Out of bounds
        }

        if (writeMode)
        {
            // Write to array
            arrayValues[index] = static_cast<string>(pinValueIn);
            dataModified = true; // mark for later save
        }
        else
        {
            // Read from array
            if (index >= 0 && index < static_cast<int>(arrayValues.size()))
            {
                auto value = arrayValues[index];
                pinValueOut = value;
                pinValueOut.isUpdated();
            }
        }
    }
};
namespace
{
    auto r = Register<ArrayTest>::withId(L"Array");
}