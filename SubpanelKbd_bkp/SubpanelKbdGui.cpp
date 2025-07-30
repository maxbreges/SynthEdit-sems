#include "mp_sdk_gui2.h"
#include "Drawing.h"
#include "sstream"

using namespace gmpi;
using namespace GmpiDrawing;

class SubpanelKbdGui final : public gmpi_gui::MpGuiGfxBase
{
    bool keyStates[12] = { false, false, false, false, false, false, false, false, false, false, false, false };
    int baseKeyNum = 0; // 0-11, key degree within octave
    int octaveShift = 3; // number of octaves shifted

    void updateNoteNumber()
    {
        int noteNumber = baseKeyNum + (octaveShift * 12);
        // Clamp to MIDI range 0-127
        if (noteNumber < 0)
            noteNumber = 0;
        else if (noteNumber > 127)
            noteNumber = 127;

        pinNoteNumber = noteNumber;
    }

/*    void onSetC() { baseKeyNum = 0; if (!pinC) updateNoteNumber(); }
    void onSetCs() { baseKeyNum = 1; if (!pinCs) updateNoteNumber(); }
    void onSetD() { baseKeyNum = 2; if (!pinD) updateNoteNumber(); }
    void onSetDs() { baseKeyNum = 3; if (!pinDs) updateNoteNumber(); }
    void onSetE() { baseKeyNum = 4; if (!pinE) updateNoteNumber(); }
    void onSetF() { baseKeyNum = 5; if (!pinF) updateNoteNumber(); }
    void onSetFs() { baseKeyNum = 6; if (!pinFs) updateNoteNumber(); }
    void onSetG() { baseKeyNum = 7; if (!pinG) updateNoteNumber(); }
    void onSetGs() { baseKeyNum = 8; if (!pinGs) updateNoteNumber(); }
    void onSetA() { baseKeyNum = 9; if (!pinA) updateNoteNumber(); }
    void onSetAs() { baseKeyNum = 10; if (!pinAs) updateNoteNumber(); }
    void onSetB() { baseKeyNum = 11; if (!pinB) updateNoteNumber(); }*/

    void onSetOctaveDown()
    {
        if (pinOctaveDown)
        {
            octaveShift -= 1;
            if (octaveShift < 0) octaveShift = 0; // Limit to 0
            pinOctave = octaveShift;
            updateRangeDisplay();
        }
    }

    void onSetOctaveUp()
    {
        if (pinOctaveUp)
        {
            octaveShift += 1;
            if (octaveShift > 9) octaveShift = 9; // Limit to 8
            pinOctave = octaveShift;
            updateRangeDisplay();
        }
    }

public:
    // Pins for key selection
/*    BoolGuiPin pinC;
    BoolGuiPin pinCs;
    BoolGuiPin pinD;
    BoolGuiPin pinDs;
    BoolGuiPin pinE;
    BoolGuiPin pinF;
    BoolGuiPin pinFs;
    BoolGuiPin pinG;
    BoolGuiPin pinGs;
    BoolGuiPin pinA;
    BoolGuiPin pinAs;
    BoolGuiPin pinB;*/

    // Pins for octave control
    BoolGuiPin pinOctaveDown;
    BoolGuiPin pinOctaveUp;

    // Output pins
    IntGuiPin pinNoteNumber;
    IntGuiPin pinOctave;
    StringGuiPin pinRangeDisplay;

    SubpanelKbdGui()
    {
/*        initializePin(pinC, static_cast<MpGuiBaseMemberPtr2>(&SubpanelKbdGui::onSetC));
        initializePin(pinCs, static_cast<MpGuiBaseMemberPtr2>(&SubpanelKbdGui::onSetCs));
        initializePin(pinD, static_cast<MpGuiBaseMemberPtr2>(&SubpanelKbdGui::onSetD));
        initializePin(pinDs, static_cast<MpGuiBaseMemberPtr2>(&SubpanelKbdGui::onSetDs));
        initializePin(pinE, static_cast<MpGuiBaseMemberPtr2>(&SubpanelKbdGui::onSetE));
        initializePin(pinF, static_cast<MpGuiBaseMemberPtr2>(&SubpanelKbdGui::onSetF));
        initializePin(pinFs, static_cast<MpGuiBaseMemberPtr2>(&SubpanelKbdGui::onSetFs));
        initializePin(pinG, static_cast<MpGuiBaseMemberPtr2>(&SubpanelKbdGui::onSetG));
        initializePin(pinGs, static_cast<MpGuiBaseMemberPtr2>(&SubpanelKbdGui::onSetGs));
        initializePin(pinA, static_cast<MpGuiBaseMemberPtr2>(&SubpanelKbdGui::onSetA));
        initializePin(pinAs, static_cast<MpGuiBaseMemberPtr2>(&SubpanelKbdGui::onSetAs));
        initializePin(pinB, static_cast<MpGuiBaseMemberPtr2>(&SubpanelKbdGui::onSetB));*/

        initializePin(pinOctaveDown, static_cast<MpGuiBaseMemberPtr2>(&SubpanelKbdGui::onSetOctaveDown));
        initializePin(pinOctaveUp, static_cast<MpGuiBaseMemberPtr2>(&SubpanelKbdGui::onSetOctaveUp));

        initializePin(pinNoteNumber, static_cast<MpGuiBaseMemberPtr2>(&SubpanelKbdGui::onSetOut));
        initializePin(pinOctave, static_cast<MpGuiBaseMemberPtr2>(&SubpanelKbdGui::onSetOut));
        initializePin(pinRangeDisplay, static_cast<MpGuiBaseMemberPtr2>(&SubpanelKbdGui::updateRangeDisplay));
    }

    void onSetOut()
    {
       updateNoteNumber();
    }

    void updateRangeDisplay()
    {
        // Generate the string based on current octaveShift
        std::wstringstream ss;
        int displayOctave = octaveShift -2;

        // For display, show range from C to B in current octave
        ss << L"C" << displayOctave << L"      -      B" << displayOctave;
        pinRangeDisplay = ss.str();
    }


    int32_t MP_STDCALL OnRender(GmpiDrawing_API::IMpDeviceContext* drawingContext) override
    {
        Graphics g(drawingContext);
        auto r = getRect();
        float width = r.getWidth();
        float height = r.getHeight();

        DrawPianoKeyboard(g, 0, 0, width, height);
        return MP_OK;
    }

    void DrawPianoKeyboard(Graphics& g, float startX, float startY, float width, float height)
    {
        const int whiteCount = 7;        
       
        const float whiteWidth = width / whiteCount;
        const float blackWidth = 11.17f;// whiteWidth * 0.7f;
        const float blackHeight = 9.0f;// height * 0.5f;

        //auto whiteColor = Color(Color::White);
        auto pressedWhiteColor = Color(0xffcdb080);
        auto blackColor = Color(0xff141614);
        auto pressedBlackColor = Color(0xffcdb080);

        // Map white keys to notes: C=0, D=2, E=4, F=5, G=7, A=9, B=11
        int whiteNoteMap[] = { 0, 2, 4, 5, 7, 9, 11 };

        // Draw white keys
        for (int i = 0; i < whiteCount; ++i)
        {
            float x = startX + i * whiteWidth;
            Rect rect(x - 0.57f, startY - 0.57f, x + whiteWidth - 0.57f, startY + height - 0.57f);
            auto brushColor = keyStates[whiteNoteMap[i]] ? pressedWhiteColor : blackColor;
            auto brush = g.Graphics_base::CreateSolidColorBrush(brushColor);
            g.FillRectangle(rect, brush);
            g.DrawRectangle(rect, g.Graphics_base::CreateSolidColorBrush(Color(0xffcdb080)), 0.67f);
        }
        // Black keys positions relative to white keys: after which white key they appear
        int blackPositions[] = { 0, 1, 3, 4, 5 };
        // Map black keys to notes: C#=1, D#=3, F#=6, G#=8, A#=10
        int blackNoteMap[] = { 1, 3, 6, 8, 10 };

        // Draw black keys
        for (int i = 0; i < 5; ++i)
        {
            int pos = blackPositions[i];
            float x = startX + (pos + 1) * whiteWidth - blackWidth / 2;
            Rect rect(x, 0 - 0.57f, x + blackWidth, blackHeight - 0.57f);
            auto brushColor = keyStates[blackNoteMap[i]] ? pressedBlackColor : blackColor;
            auto brush = g.Graphics_base::CreateSolidColorBrush(brushColor);
            g.FillRectangle(rect, brush);
            g.DrawRectangle(rect, g.Graphics_base::CreateSolidColorBrush(Color(0xffcdb080)), 0.67f);
        }
    }

    int32_t onPointerDown(int32_t flags, GmpiDrawing_API::MP1_POINT point)
    {
        if ((flags & gmpi_gui_api::GG_POINTER_FLAG_FIRSTBUTTON) == 0)
            return gmpi::MP_OK;

        auto r = getRect();
        float width = r.getWidth();
        float height = r.getHeight();

        float whiteWidth = 14.57f;// width / 7;
        float blackWidth = 11.07f;// whiteWidth * 0.7f;
        float blackHeight = 8.57f;// height * 0.5f;

        float x = point.x - r.left;
        float y = point.y - r.top;

        int noteNumber = 0; // 0-11
        bool blackKeyHit = false;

        // Check black keys first
        int blackPositions[] = { 0, 1, 3, 4, 5 };
        int blackNoteMap[] = { 1, 3, 6, 8, 10 }; // mapping to note numbers

        for (int i = 0; i < 5; ++i)
        {
            int pos = blackPositions[i];
            float bx = (pos + 1) * whiteWidth - blackWidth / 2;
            Rect blackRect(bx, 0, bx + blackWidth, blackHeight);

            if (x >= blackRect.left && x <= blackRect.right && y >= blackRect.top && y <= blackRect.bottom)
            {
                noteNumber = blackNoteMap[i];
                // Set states
                for (int j = 0; j < 12; ++j)
                    keyStates[j] = false;
                keyStates[noteNumber] = true;
                // Assign pinNote as 0-based
                baseKeyNum = noteNumber;
                blackKeyHit = true;
                break;
            }
        }

        // If no black key pressed, check white keys
        if (!blackKeyHit)
        {
            int whiteIndex = static_cast<int>(x / whiteWidth);
            if (whiteIndex >= 0 && whiteIndex < 7)
            {
                int whiteNoteMap[] = { 0, 2, 4, 5, 7, 9, 11 };
                noteNumber = whiteNoteMap[whiteIndex];
                for (int j = 0; j < 12; ++j)
                    keyStates[j] = false;
                keyStates[noteNumber] = true;
                // Assign pinNote as 0-based
                baseKeyNum = noteNumber;
            }
        }
      //  pinBoolState = true;
        updateNoteNumber();
        sendNoteState(pinNoteNumber, true);
        invalidateRect();
        setCapture();

        return gmpi::MP_HANDLED;
    }

    int32_t onPointerUp(int32_t flags, GmpiDrawing_API::MP1_POINT point)
    {
        if ((flags & gmpi_gui_api::GG_POINTER_FLAG_FIRSTBUTTON) == 0)
            return gmpi::MP_UNHANDLED;

        if (!getCapture())
            return gmpi::MP_OK;

        // Reset all keys
        for (int i = 0; i < 12; ++i)
            keyStates[i] = false;

      //  pinBoolState = false;
        updateNoteNumber();
        sendNoteState(pinNoteNumber, false);
        invalidateRect();
        releaseCapture();

        return gmpi::MP_OK;
    }

    void sendNoteState(int noteNumber, bool noteOn)
    {
        int messageId = 2213; // same message ID
        // Prepare data: two integers: noteNumber and noteOn (1 or 0)
        struct {
            int noteNumber;
            int noteOn; // 1 for note on, 0 for note off
        } dataStruct;

        dataStruct.noteNumber = noteNumber;
        dataStruct.noteOn = noteOn ? 1 : 0;

        getHost()->sendMessageToAudio(messageId, sizeof(dataStruct), &dataStruct);
    }
};

namespace
{
    auto r = Register<SubpanelKbdGui>::withId(L"SubpanelKbd");
}