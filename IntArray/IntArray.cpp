#include "mp_sdk_audio.h"
#include <vector>
#include <fstream>
#include <string>
#include "../shared/string_utilities.h"

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
    int lastIndex = 0;

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

    void onSetPins() override
    {
        int size = pinSize.getValue();
        int index = pinIndex.getValue();
        bool writeMode = pinReadWrite.getValue();
       
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
            pinValueOut = 0;
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

                if(lastIndex != pinIndex)
                {
                    pinDebug = L"Index changed";
                    lastIndex = pinIndex;
                }                
            }
        }
    }

    void saveFile()
    {
        // Create and open a text file for writing
        std::ofstream file(static_cast<std::string>(pinFilename));

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

    bool isFirstLoad = true;

    void loadFile()
    {
        std::string currentPath = static_cast<std::string>(pinFilename);

        // Detect filename change
        if (currentPath != lastFilePath)
        {
            lastFilePath = currentPath;

            // Only create/truncate new file if filename changed after initial load
            if (!isFirstLoad)
            {
                // Create empty file for new filename
                std::ofstream file(currentPath, std::ios::trunc);
                file.close();

                // Clear in-memory data
                arrayValues.clear();
                pinDebug = L"New file created for filename change.";
            }
            else
            {
                // First load: do NOT truncate, just load existing data
                isFirstLoad = false;
                pinDebug = L"Initial load, existing file data.";
            }
            fileLoaded = false; // Reset load flag
           // return; // Exit to avoid reloading data immediately
        }

        // Load existing data if not already loaded for this file
        if (fileLoaded && currentPath == lastFilePath)
            return;

        // Load the data from the file
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
                    int value = std::stoi(line);
                    arrayValues.push_back(value);
                    ++count;
                }
                catch (...)
                {
                    arrayValues.push_back(0);
                }
            }
            file.close();
            fileLoaded = true;
            pinDebug = L"File loaded.";
        }
        else
        {
            pinDebug = L"Failed to open file.";
        }
    }
};

namespace
{
    auto r = Register<IntArray>::withId(L"IntArray");
}