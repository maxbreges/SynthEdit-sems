#include "mp_sdk_gui2.h"

using namespace std;
using namespace gmpi;

class TextAppendGuiGui final : public SeGuiInvisibleBase
{

 	void onSetTextOut()
	{
		// pinTextOut changed
	}

    void onSetTextIn()
    {
        if (pinTextIn.size()) // 
        {
            wstring out;

            for (int i = 0; i < pinTextIn.size(); i++)
            {
                out += pinTextIn[i].getValue() + L' '; // You can use '+' to append strings
            }

            pinTextOut = out;
        }
    }

 	StringGuiPin pinTextOut;
    vector<StringGuiPin> pinTextIn;

public:
	TextAppendGuiGui()
	{
		initializePin( pinTextOut, static_cast<MpGuiBaseMemberPtr2>(&TextAppendGuiGui::onSetTextOut) );
	}

    int32_t initialize() override
    {
        // Retrieve total number of pins from the host
        int32_t pinCount = 0;
        // Access the base host interface (IMpUserInterfaceHost) to obtain pin count
        gmpi::IMpUserInterfaceHost* host = nullptr;
        if (getHost()->queryInterface(gmpi::MP_IID_UI_HOST, (void**)&host) == gmpi::MP_OK)
        {
            host->getPinCount(pinCount); // Safely call getPinCount
            host->release(); // Release the host interface when done
        }

        // Assuming that we reserve at least 2 pins (one output and one internal)
        const int numInputPins = max(0, pinCount - 2);

        try
        {
            pinTextIn.resize(numInputPins); // Resize vector to hold input pins

            // Initialize each input pin in the vector
            for (size_t i = 0; i < numInputPins; ++i)
            {
                initializePin(pinTextIn[i], static_cast<MpGuiBaseMemberPtr2>(&TextAppendGuiGui::onSetTextIn));
            }
        }
        catch (const std::exception& e)
        {
            // Log or handle the exception if pin initialization fails
            return MP_FAIL; // Indicate failure
        }

        return SeGuiInvisibleBase::initialize(); // Ensure to call base class's initialize method
    }
};

namespace
{
	auto r = Register<TextAppendGuiGui>::withId(L"TextAppendGuiL");
}
