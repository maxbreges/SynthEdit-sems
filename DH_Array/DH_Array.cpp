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
    std::string lastFilePath; // To detect changes in file path
    bool fileLoaded = false;
    bool dataModified = false;

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
        std::string currentPath = static_cast<string>(pinFile);

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

        std::string filePathW = static_cast<string>(pinFile);
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

    void onSetPins() override
    {
        std::string currentPath = static_cast<string>(pinFile);
        if (currentPath != lastFilePath)
        {
            fileLoaded = false; // force reload
        }
        loadFile();

        if (pinClear)
        {
            // Clear the file content
            std::string filePathW = static_cast<string>(pinFile);

            std::ofstream file(filePathW, std::ios::trunc);
            file.close();

            // Clear the in-memory array
            arrayValues.clear();

            return; // Exit early since we've cleared
        }

        int size = pinArraySize.getValue();
        int index = pinIndex;
        bool writeMode = pinBool;

        if (pinArraySize.isUpdated() || arrayValues.size() != static_cast<size_t>(size))
        {
            arrayValues.resize(size);
        }

        if (index < 0 || index >= size)
        {
            return; // Out of bounds
        }

        switch (writeMode)
        {
        case 0:
        {
            saveFile(); 
            auto value = arrayValues[index];
            pinValueOut = value;            
        }; break;

        case 1:
        {
            //loadFile(); // optional, only if you want to ensure latest data
            arrayValues[index] = static_cast<string>(pinValueIn);
            dataModified = true; // mark for later save  
             //saveFile(); // Save immediately after write
        }; break;
        }        
    }
};
namespace
{
    auto r = Register<ArrayTest>::withId(L"Array");
}