#include "mp_sdk_audio.h"
#include <vector>
#include <fstream>
#include <string>

using namespace std;
using namespace gmpi;

class IntArray final : public MpBase2
{
    IntInPin pinSize;
    IntInPin pinIndex;
    IntInPin pinValue;
    BoolInPin pinReadWrite;
    BoolInPin pinClear;
    StringInPin pinFilename;
    IntOutPin pinValueOut;
    StringOutPin pinDebug;

    std::vector<int> arrayValues;
    std::string lastFilePath; // To detect changes in file path
    bool fileLoaded = false;
    bool dataModified = false;

public:
    IntArray()
    {
        initializePin(pinSize);
        initializePin(pinIndex);
        initializePin(pinValue);
        initializePin(pinReadWrite);
        initializePin(pinClear);
        initializePin(pinFilename);
        initializePin(pinValueOut);
        initializePin(pinDebug);
    }

    std::wstring filename;

    void onSetPins() override
    {
        int size = pinSize.getValue();
        int index = pinIndex.getValue();
        bool writeMode = pinReadWrite.getValue();
        filename = pinFilename.getValue();
        loadFile();

        if (pinClear)
        {
            // Clear the file content
            std::string filePathW = static_cast<std::string>(pinFilename);

            std::ofstream file(filePathW, std::ios::trunc);
            file.close();

            // Clear the in-memory array
            arrayValues.clear();
            pinDebug = L"File cleared";
            return; // Exit early since we've cleared
        }

        // Resize array if size changed
        if (pinSize.isUpdated() || arrayValues.size() != size)
        {
            arrayValues.resize(size);
            // Optionally initialize new elements to 0
            for (int i = 0; i < size; ++i)
            {
                if (i >= arrayValues.size() || arrayValues[i] == 0)
                    arrayValues[i] = 0;
            }
        }

        // Boundary check
        if (index < 0 || index >= size)
        {
            return;
        }

        // If in write mode, store the value
        if (pinReadWrite.isUpdated() && writeMode)
        {
            arrayValues[index] = static_cast<int>(pinValue.getValue());
            pinValueOut = arrayValues[index];
            saveFile();
        }
        // If in read mode, load and output the value
        else if (!writeMode)
        {
            if (pinIndex.isUpdated() && index >= 0 && index < arrayValues.size())
            {
                pinValueOut = arrayValues[index]; 
            }
        }
    }

    void saveFile()
    {
        // Create and open a text file for writing
        std::ofstream file(pinFilename.getValue());

        if (!file.is_open())
        {
            pinDebug = L"Error opening file";
            return;
        }

        for (const auto& val : arrayValues)
        {
            file << val << std::endl; // Write int values directly to file
        }
        file.close();

        pinDebug = L"Data saved to file";
    }

    void loadFile()
    {
        std::string currentPath = static_cast<std::string>(pinFilename);

        if (fileLoaded && currentPath == lastFilePath)
            return; // Already loaded for this path

        lastFilePath = currentPath; // Save current path

        std::ifstream file(currentPath);
        int size = pinSize.getValue();

        if (file.is_open())
        {
            arrayValues.clear();

            int count = 0;
            std::string line;

            while (count < size && std::getline(file, line))
            {
                try
                {
                    int value = std::stoi(line); // Convert string to int
                    arrayValues.push_back(value);
                    ++count;
                }
                catch (...)
                {
                    // ignore invalid lines, or set to default
                    arrayValues.push_back(0);
                }
            }
            file.close();
            fileLoaded = true;
            if (fileLoaded)
            {
                pinDebug = L"File loaded";
            }
        }
        else
        {
            pinDebug = L"Failed to open file";
        }
    }
};

namespace
{
    auto r = Register<IntArray>::withId(L"IntArray");
}