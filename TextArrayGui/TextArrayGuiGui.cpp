#include "mp_sdk_gui2.h"
#include <fstream>
#include <vector>
#include <filesystem>

using namespace gmpi;
namespace fs = std::filesystem;

class TextArrayGuiGui final : public SeGuiInvisibleBase
{
	std::string lastFilePath; // To detect changes in file path
	int index = 0;
	int size = 0;
	int lastIndex = 0; //to detect index changes

	std::vector<std::string> arrayValues;
	std::string stringData; //Text In
	std::ofstream file;

	bool fileExists(const std::string& filename) 
	{
		std::ifstream file(filename);
		return file.good();
	}

 	void onSetFilePathIn()
	{
		std::string currentPath = static_cast<std::string>(pinFilePathIn);

		if (fileExists(currentPath))
		{	
			lastFilePath = currentPath;
			loadFile();
		}
		else 
		{
			// Create empty file for new filename
			std::ofstream file(currentPath, std::ios::trunc);
			file.close();
			pinDebug = "New file created.";
			lastFilePath = currentPath;
		}		
	}

	void onSetSize()
	{
		if (pinSize <= 0)
		{
			pinSize = 0;
			return;
		}

		size = pinSize + 1;

		// Resize array if size changed
		if (arrayValues.size() != size)
		{
			arrayValues.resize(size);
			// Optionally initialize new elements to 0
			for (int i = 0; i < size; ++i)
			{
				if (i > arrayValues.size())
					arrayValues[i] = "";
			}
		}
	}
	
	void onSetTextIn()
	{
		stringData = pinTextIn;
	}

	void onSetIndex()
	{
		index = pinIndex;
		
		// If in write mode, store the value
		if (pinWrite)
		{
			if (arrayValues.empty())
			{
				onSetSize();
			}

			arrayValues[index] = stringData;
			pinTextOut = arrayValues[index];
			saveFile();
		}
		// If in read mode, load and output the value
		else
		{
			if (index >= 0 && index < static_cast<int>(arrayValues.size()))
			{
				pinTextOut = arrayValues[index]; // output the line at index
			}

			if (index != lastIndex && fileLoadedFlag)
			{
				pinDebug = "Index is " + std::to_string(index);
				lastIndex = index;				
			}

			if (arrayValues.empty())
			{
				pinDebug = "File is empty";
			}
		}
		pinSizeOut = static_cast<int>(arrayValues.size());
		fileLoadedFlag = true;
	}

	void onSetWrite()
	{
		onSetSize();
	}

	void onSetClear()
	{
		if (pinClear)
		{
			// Clear the file content
			std::string filePathW = static_cast<std::string>(pinFilePathIn);
			std::ofstream file(filePathW, std::ios::trunc);
			file.close();

			arrayValues.clear();
			onSetSize();

			pinTextOut = "File cleared"; //
			
			pinIndex = 0;			
			pinDebug = "File cleared";
		}
		//else
		//{}
	}

 	StringGuiPin pinFilePathIn;
 	IntGuiPin pinSize;
 	IntGuiPin pinIndex;
 	StringGuiPin pinTextIn;
 	BoolGuiPin pinWrite;
 	BoolGuiPin pinClear;
	BoolGuiPin pinReload;
 	StringGuiPin pinTextOut;
 	StringGuiPin pinDebug;
	IntGuiPin pinSizeOut;

public:
	TextArrayGuiGui()
	{
		initializePin( pinFilePathIn, static_cast<MpGuiBaseMemberPtr2>(&TextArrayGuiGui::onSetFilePathIn) );
		initializePin( pinSize, static_cast<MpGuiBaseMemberPtr2>(&TextArrayGuiGui::onSetSize) );
		initializePin( pinIndex, static_cast<MpGuiBaseMemberPtr2>(&TextArrayGuiGui::onSetIndex) );
		initializePin( pinTextIn, static_cast<MpGuiBaseMemberPtr2>(&TextArrayGuiGui::onSetTextIn) );
		initializePin( pinWrite, static_cast<MpGuiBaseMemberPtr2>(&TextArrayGuiGui::onSetWrite) );
		initializePin( pinClear, static_cast<MpGuiBaseMemberPtr2>(&TextArrayGuiGui::onSetClear) );
		initializePin(pinReload, static_cast<MpGuiBaseMemberPtr2>(&TextArrayGuiGui::reloadFile));
		initializePin( pinTextOut);
		initializePin( pinDebug);
		initializePin(pinSizeOut);
	}

	void saveFile()
	{
		// Create and open a text file for writing
		std::ofstream file(static_cast<std::string>(pinFilePathIn));

		if (!file.is_open())
		{
			pinDebug = "Error opening file";
			return;
		}

		for (const auto& val : arrayValues)
		{
			file << val << std::endl; // Write int values directly to file
		}
		file.close(); //

		pinDebug = "Data saved to file";
	}
	void reloadFile()
	{
		if (pinReload)
		{
			loadFile();
			//
			std::string filename = fs::path(pinFilePathIn).filename().string();
			pinDebug = filename + " reloaded";
		}
		//else{}
	}

	void loadFile()
	{
		arrayValues.clear();

		std::ifstream file(pinFilePathIn.getValue());
		if (file)
		{
			std::string line;
			while (std::getline(file, line))
			{
				arrayValues.push_back(line);
			}

			//optional
			std::string filename = fs::path(pinFilePathIn).filename().string();
			pinDebug = filename + " loaded"; // Or just "File loaded"
			fileLoadedFlag = false;
			pinIndex = 0;			
		}
		else
		{
			pinDebug = "Cannot open file";
		}
	}
	bool fileLoadedFlag = true;
};

namespace
{
	auto r = Register<TextArrayGuiGui>::withId(L"TextArrayGui");
}
