#pragma warning(suppress : 6386) // Suppress specific compiler warning
#include "mp_sdk_gui2.h"
#include <cmath>
#include "Drawing.h"

using namespace gmpi;
using namespace gmpi_gui;
using namespace GmpiDrawing;

const float TUNING_RATIO = 2.0f;
const float FRETBOARD_RATIO = 17.817154f - 0.617154f;

class DisplayFretGui final : public gmpi_gui::MpGuiGfxBase
{
    float scaleLength = 650.f; // Default scale length    


    // Method to recalculate position for each string along with drawing
    void onSetString1() { calculateAnimationPosition(1); invalidateRect(); }
    void onSetString2() { calculateAnimationPosition(2); invalidateRect(); }
    void onSetString3() { calculateAnimationPosition(3); invalidateRect(); }
    void onSetString4() { calculateAnimationPosition(4); invalidateRect(); }
    void onSetString5() { calculateAnimationPosition(5); invalidateRect(); }
    void onSetString6() { calculateAnimationPosition(6); invalidateRect(); }

    void onSetFretCount() { invalidateRect(); } // Re-trigger the rendering on fret count change
    void onSetStringCount() { invalidateRect(); }
    void onSetDebug() { invalidateRect(); }
    void onSetHex() { invalidateRect(); };

    IntGuiPin pinString1;
    IntGuiPin pinString2;
    IntGuiPin pinString3;
    IntGuiPin pinString4;
    IntGuiPin pinString5;
    IntGuiPin pinString6;

    IntGuiPin pinFretCount; // Total count of frets
    IntGuiPin pinStringCount;
    StringGuiPin pinHex;

    IntGuiPin pinTransposeString1;
    IntGuiPin pinTransposeString2;
    IntGuiPin pinTransposeString3;
    IntGuiPin pinTransposeString4;
    IntGuiPin pinTransposeString5;
    IntGuiPin pinTransposeString6;

    FloatGuiPin pinCircleRadius;
  
    float circlePositions[6] = { 0.f }; // Store positions for all 6 strings
    int frets[6] = { -1 }; // Store fret numbers for all 6 strings

public:
    DisplayFretGui()
    {
        initializePin(pinString1, static_cast<MpGuiBaseMemberPtr2>(&DisplayFretGui::onSetString1));
        initializePin(pinString2, static_cast<MpGuiBaseMemberPtr2>(&DisplayFretGui::onSetString2));
        initializePin(pinString3, static_cast<MpGuiBaseMemberPtr2>(&DisplayFretGui::onSetString3));
        initializePin(pinString4, static_cast<MpGuiBaseMemberPtr2>(&DisplayFretGui::onSetString4));
        initializePin(pinString5, static_cast<MpGuiBaseMemberPtr2>(&DisplayFretGui::onSetString5));
        initializePin(pinString6, static_cast<MpGuiBaseMemberPtr2>(&DisplayFretGui::onSetString6));

        initializePin(pinFretCount, static_cast<MpGuiBaseMemberPtr2>(&DisplayFretGui::onSetFretCount));
        initializePin(pinStringCount, static_cast<MpGuiBaseMemberPtr2>(&DisplayFretGui::onSetStringCount));
        initializePin(pinHex, static_cast<MpGuiBaseMemberPtr2>(&DisplayFretGui::onSetHex));

        initializePin(pinTransposeString1, static_cast<MpGuiBaseMemberPtr2>(&DisplayFretGui::onSetString1));
        initializePin(pinTransposeString2, static_cast<MpGuiBaseMemberPtr2>(&DisplayFretGui::onSetString2));
        initializePin(pinTransposeString3, static_cast<MpGuiBaseMemberPtr2>(&DisplayFretGui::onSetString3));
        initializePin(pinTransposeString4, static_cast<MpGuiBaseMemberPtr2>(&DisplayFretGui::onSetString4));
        initializePin(pinTransposeString5, static_cast<MpGuiBaseMemberPtr2>(&DisplayFretGui::onSetString5));
        initializePin(pinTransposeString6, static_cast<MpGuiBaseMemberPtr2>(&DisplayFretGui::onSetString6));
        initializePin(pinCircleRadius, static_cast<MpGuiBaseMemberPtr2>(&DisplayFretGui::onSetHex));
    }

    void calculateAnimationPosition(int stringIndex)
    {    
        if (stringIndex < 1 || stringIndex > 6) return;  // Prevent buffer overrun        

        // Convert to zero-based index
        int idx = stringIndex - 1;

        int transpStr1 = pinTransposeString1;
        int transpStr2 = pinTransposeString2;
        int transpStr3 = pinTransposeString3;
        int transpStr4 = pinTransposeString4;
        int transpStr5 = pinTransposeString5;
        int transpStr6 = pinTransposeString6;

        int fret = (stringIndex == 1) ? (pinString1 - transpStr1) :
            (stringIndex == 2) ? (pinString2 - transpStr2) :
            (stringIndex == 3) ? (pinString3 - transpStr3) :
            (stringIndex == 4) ? (pinString4 - transpStr4) :
            (stringIndex == 5) ? (pinString5 - transpStr5) :
            (stringIndex == 6) ? (pinString6 - transpStr6) : -1;

        frets[stringIndex - 1] = fret; // Save the fret number
        int fretCount = pinFretCount; // Get total count of frets

        if (fret > 0 && fretCount > 0)
        {
            float position = 0.0f;
            for (int i = 0; i < fret; ++i)
            {
                position += (scaleLength / (FRETBOARD_RATIO * (1.0f - pow(TUNING_RATIO, -static_cast<double>(fretCount) / 12.0))))
                    * pow(TUNING_RATIO, -static_cast<double>(i + 1) / 12.0);
            }
            circlePositions[stringIndex - 1] = position / scaleLength; // Normalize to 0.0 - 1.0
        }
        else
        {
            circlePositions[stringIndex - 1] = 0.0f; // Reset for invalid fret
        }
    }

    void onTransportRunChange()
    {
        invalidateRect(); // To potentially trigger a redraw when changing the TransportRun state
    }

    int32_t MP_STDCALL OnRender(GmpiDrawing_API::IMpDeviceContext* drawingContext) override
    {
        Graphics g(drawingContext);
        ClipDrawingToBounds z(g, getRect());
        Rect r = getRect();

        scaleLength = r.getWidth();
        float radius = pinCircleRadius; // Circle radius

        for (int i = 0; i < 6; ++i)
        {
            float normalisedX = circlePositions[i] * scaleLength;           
            float x = (frets[i] == 0) ? normalisedX : normalisedX - radius; // Adjust x position
            float y = ((r.getHeight() / pinStringCount * (i + 0.5)) - radius);

            // Create the circle
            GmpiDrawing::Ellipse circle({ x + radius, y + radius }, radius, radius);
            auto brush = g.CreateSolidColorBrush(frets[i] < 0 ? Color(0.0f, 0.0f, 0.0f, 0.0f) : Color::FromHexString(pinHex));
            auto outlineBrush = g.CreateSolidColorBrush(frets[i] < 0 ? Color(0.0f, 0.0f, 0.0f, 0.0f) : Color::Black);

            g.FillEllipse(circle, brush);
            g.DrawEllipse(circle, outlineBrush, 1.0f); // Draw outline
        }

        return gmpi::MP_OK;
    }
};

namespace
{
    auto r = Register<DisplayFretGui>::withId(L"DisplayFret");
}
