#include "mp_sdk_gui2.h"

using namespace gmpi;

const float NUT_LINE_SIZE_FACTOR = 10.0f;
const float TUNING_RATIO = 2.0f;
const float FRETBOARD_RATIO = 17.817154f - 0.617154f;

class FretFormulaGui final : public SeGuiInvisibleBase
{
    float scaleLength = 650.f; // Default scale length

    // Constants
    const float fretFactor = 17.817154f; // Fret spacing factor

    void onSetWidth()
    {
        // Update the scaleLength variable based on the pin input
        scaleLength = pinWidthIn;  // Directly assigns the value from the pin
        calculateAnimationPosition(); // Recalculate the animation position if necessary
    }

    void onSetFret()
    {
        calculateAnimationPosition();
    }

    void onSetFretCount()
    {
        calculateAnimationPosition();
    }

    void calculateAnimationPosition()
    {
        // Assume pinFret is the fret number to calculate the position for (0 = Open, 1 = 1st fret, etc.)
        int fret = pinFret; // Current selected fret index (0 for open string)
        int fretCount = pinFretCount; // Total number of frets

        // Use proper checks
        if (fretCount > 0)
        {
            // We are calculating the position based on the nut width
            float totalNutWidth = scaleLength; // Adjust this if needed to fit your scale length logic
            float position = 0.0f;

            // Only calculate if fret is in a valid range
            if (fret >= 0 && fret <= fretCount)
            {
                for (int i = 0; i < fret; ++i) // Iterate up to the selected fret (exclude current fret calculation)
                {
                    // This is the same calculation from your draw method
                    position += (totalNutWidth / (FRETBOARD_RATIO * (1.0f - pow(TUNING_RATIO, -static_cast<double>(fretCount) / 12.0)))) * pow(TUNING_RATIO, -static_cast<double>(i + 1) / 12.0);
                }

                // Normalize the position for AnimationPosition, which will be in the range of 0.0 to 1.0
                pinAnimationPosition = position / totalNutWidth; // Normalize to 0.0 - 1.0
            }
            else
            {
                pinAnimationPosition = 0.0f; // Default for invalid fret case
            }
        }
        else
        {
            pinAnimationPosition = 0.0f; // Handle the case where there are no frets
        }
    }

    IntGuiPin pinFret;              // The selected fret
    IntGuiPin pinFretCount;         // Total number of frets
    FloatGuiPin pinWidthIn;         // Input the width which determines the scale length
    FloatGuiPin pinAnimationPosition; // Output position, normalized (0.0 to 1.0)

public:
    FretFormulaGui()
    {
        initializePin(pinFret, static_cast<MpGuiBaseMemberPtr2>(&FretFormulaGui::onSetFret));
        initializePin(pinFretCount, static_cast<MpGuiBaseMemberPtr2>(&FretFormulaGui::onSetFretCount));
        initializePin(pinWidthIn, static_cast<MpGuiBaseMemberPtr2>(&FretFormulaGui::onSetWidth)); // Hook up the width pin
        initializePin(pinAnimationPosition, static_cast<MpGuiBaseMemberPtr2>(&FretFormulaGui::calculateAnimationPosition));
    }
};

namespace
{
    auto r = Register<FretFormulaGui>::withId(L"FretFormula");
}