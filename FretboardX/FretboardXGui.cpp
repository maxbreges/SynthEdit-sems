// FretboardXGui.cpp

#include "mp_sdk_gui2.h"
#include "Drawing.h"
#include <cmath> // for pow()
#include <algorithm> // for clamp

using namespace std;
using namespace gmpi;
using namespace GmpiDrawing;

const float NUT_LINE_SIZE_FACTOR = 10.0f;
const float TUNING_RATIO = 2.0f;
const float FRETBOARD_RATIO = 17.817154f - 0.617154f;

class FretboardXGui final : public gmpi_gui::MpGuiGfxBase
{

 	void onSetFingerboardColor() {invalidateRect();}

 	void onSetNutColor()
	{
		invalidateRect();
	}

 	void onSetFretsColor()
	{
		invalidateRect();
	}

	void onSetMarkersColor()
	{
		invalidateRect();
	}

 	void onSetStringsColor()
	{
		invalidateRect();
	}

	void onSetStringsThickness()
	{
		invalidateRect();
	}

 	void onSetFretCount()
	{
		invalidateRect();
	}

 	void onSetStringsCount()
	{
		invalidateRect();
	}	
	void onSetFretsThickness()
	{
		invalidateRect();
	}
	void onSetMarkerRadius()
	{
		invalidateRect();
	}
	void onSetWidth()
	{
		invalidateRect();
	}


 	StringGuiPin pinFingerboardColor;
 	StringGuiPin pinNutColor; 	
 	StringGuiPin pinFretsColor;
	StringGuiPin pinMarkersColor;
 	StringGuiPin pinStringsColor;
	FloatGuiPin pinStringsThickness;
 	IntGuiPin pinFretCount;
 	IntGuiPin pinStringsCount;
	FloatGuiPin pinFretsThickness;
	FloatGuiPin pinMarkerRadius;
	FloatGuiPin pinWidth;

	
	Brush brushFingerboard;
	Brush lineBrush;
	Brush nutBrush;
	Brush brushBlack;
	Brush fretBrush;
	Brush markerBrush;
	Brush stringBrush;
	bool brushesInitialized = false;  // Flag to indicate if brushes are initialized
 	
	// Marker positions
	const std::vector<int> markerPositions = { 3, 5, 7, 9, 12, 15, 17, 19, 21, 24 };
	float markerRadius = 5.f;
	std::vector<float> markerXPositions;

public:
	FretboardXGui()
	{
		initializePin( pinFingerboardColor, static_cast<MpGuiBaseMemberPtr2>(&FretboardXGui::onSetFingerboardColor) );
		initializePin( pinNutColor, static_cast<MpGuiBaseMemberPtr2>(&FretboardXGui::onSetNutColor) );		
		initializePin( pinFretsColor, static_cast<MpGuiBaseMemberPtr2>(&FretboardXGui::onSetFretsColor) );
		initializePin(pinMarkersColor, static_cast<MpGuiBaseMemberPtr2>(&FretboardXGui::onSetMarkersColor));
		initializePin( pinStringsColor, static_cast<MpGuiBaseMemberPtr2>(&FretboardXGui::onSetStringsColor) );
		initializePin(pinStringsThickness, static_cast<MpGuiBaseMemberPtr2>(&FretboardXGui::onSetStringsThickness));
		initializePin( pinFretCount, static_cast<MpGuiBaseMemberPtr2>(&FretboardXGui::onSetFretCount) );
		initializePin( pinStringsCount, static_cast<MpGuiBaseMemberPtr2>(&FretboardXGui::onSetStringsCount) );
		initializePin(pinFretsThickness, static_cast<MpGuiBaseMemberPtr2>(&FretboardXGui::onSetFretsThickness));
		initializePin(pinMarkerRadius, static_cast<MpGuiBaseMemberPtr2>(&FretboardXGui::onSetMarkerRadius));
		initializePin(pinWidth, static_cast<MpGuiBaseMemberPtr2>(&FretboardXGui::onSetWidth));
	}

	void initializeBrushes(Graphics& g)
	{
		if (!brushesInitialized)
		{
			brushFingerboard = g.CreateSolidColorBrush(Color::FromHexString(pinFingerboardColor));
			lineBrush = g.CreateSolidColorBrush(Color::FromHexString(pinFretsColor));
			nutBrush = g.CreateSolidColorBrush(Color::FromHexString(pinNutColor));
			brushBlack = g.CreateSolidColorBrush(Color::Black);
			fretBrush = g.CreateSolidColorBrush(Color::FromHexString(pinFretsColor));
			markerBrush = g.CreateSolidColorBrush(Color::FromHexString(pinMarkersColor));
			stringBrush = g.CreateSolidColorBrush(Color::FromHexString(pinStringsColor));
			brushesInitialized = true;
		}
	}

	void updateBrushes(Graphics& g)
	{
		// Brushes are recreated on render.  No explicit "isDirty" check is needed
		//  because the framework handles invalidation automatically
	
		brushFingerboard = g.CreateSolidColorBrush(Color::FromHexString(pinFingerboardColor));
		lineBrush = g.CreateSolidColorBrush(Color::FromHexString(pinFretsColor));
		nutBrush = g.CreateSolidColorBrush(Color::FromHexString(pinNutColor));
		brushBlack = g.CreateSolidColorBrush(Color::Black);
		fretBrush = g.CreateSolidColorBrush(Color::FromHexString(pinFretsColor));
		markerBrush = g.CreateSolidColorBrush(Color::FromHexString(pinMarkersColor));
		stringBrush = g.CreateSolidColorBrush(Color::FromHexString(pinStringsColor));
	}

	void drawFingerboard(Graphics& g, const Rect& r, Brush& brushFingerboard)
	{
		g.FillRectangle(r, brushFingerboard);
	}

	void drawBorders(Graphics& g, const Rect& r, Brush& lineBrush, float lineSize)
	{
		g.DrawRectangle(r, lineBrush, lineSize);
	}

	void drawNut(Graphics& g, const Rect& r, Brush& nutBrush, Brush& fretBrush, float lineSize, float nutThickness)
	{
		Rect nutRect(r.left, r.top, r.left + nutThickness, r.bottom); // Correct nut width
		g.FillRectangle(nutRect, nutBrush);
		g.DrawRectangle(nutRect, brushBlack, 0.5);
	}

	void drawFrets(Graphics& g, const Rect& r, Brush& fretBrush, float lineSize, int fretCount, float nutWidth, float stretchFactor, float nutThickness) // Added nutThickness
	{
		float thickness = pinFretsThickness;
		// Pre-calculate fret positions and store them
		std::vector<float> fretPositions(fretCount);
		float currentPosition = nutThickness; // Start drawing after the nut
		for (int i = 0; i < fretCount; ++i)
		{
			currentPosition += nutWidth * pow(TUNING_RATIO, -static_cast<double>(i + 1) / 12.0) * stretchFactor; // Correct fret position
			fretPositions[i] = r.left + currentPosition;
		}

		// Draw frets using precalculated positions
		for (int i = 0; i < fretCount; ++i)
		{
			g.DrawLine(Point(fretPositions[i], r.top), Point(fretPositions[i], r.bottom), fretBrush, thickness);
		}
	}
	
	void calculateMarkerPositions(float nutThickness, float nutWidth, float stretchFactor, int fretCount, const Rect& r)
	{		
		markerXPositions.clear();
		markerRadius = (r.getHeight() * pinMarkerRadius * 0.01f); // Simplied calculation
		markerRadius = (markerRadius > 0) ? markerRadius * (r.getWidth() / pinFretCount) * 0.005f : 0;

		for (int pos : markerPositions)
		{
			if (pos <= fretCount)
			{
				float fretStart = nutThickness;  // Start at the end of the nut.
				for (int j = 1; j < pos; j++)
				{
					fretStart += nutWidth * pow(TUNING_RATIO, -static_cast<double>(j) / 12.0) * stretchFactor;
				}

				float fretEnd = nutThickness;    // Start at the end of the nut.
				for (int j = 1; j <= pos; j++)
				{
					fretEnd += nutWidth * pow(TUNING_RATIO, -static_cast<double>(j) / 12.0) * stretchFactor;
				}

				markerXPositions.push_back(r.left + (fretStart + fretEnd) / 2.0f);
			}
			else
			{
				markerXPositions.push_back(0.0f); // Or some other invalid value
			}
		}
	}

	void drawMarkers(Graphics& g, const Rect& r, Brush& markerBrush, int fretCount)
	{
		float stringCount = pinStringsCount;

		if (stringCount > 5)
		{
			stringCount = stringCount / 2;
		}

		for (size_t i = 0; i < markerPositions.size(); ++i) // Use markerPositions.size() to iterate
		{
			int pos = markerPositions[i];
			if (pos <= fretCount && i < markerXPositions.size())
			{
				float markerPosX = markerXPositions[i];
				float markerY1 = (r.top + ((pos == 12) || (pos == 24)) ? r.getHeight() / stringCount : r.getHeight() / 2);
				g.FillEllipse(GmpiDrawing::Ellipse(Point(markerPosX, markerY1), markerRadius, markerRadius), markerBrush);
				g.DrawEllipse(GmpiDrawing::Ellipse(Point(markerPosX, markerY1), markerRadius, markerRadius), brushBlack, 0.5f);

				if ((pos == 12) || (pos == 24))
				{
					float markerY2 = r.top + ((stringCount - 1) * r.getHeight() / stringCount);
					g.FillEllipse(GmpiDrawing::Ellipse(Point(markerPosX, markerY2), markerRadius, markerRadius), markerBrush);
					g.DrawEllipse(GmpiDrawing::Ellipse(Point(markerPosX, markerY2), markerRadius, markerRadius), brushBlack, 0.5f);
				}
			}
		}
	}

	void drawStrings(Graphics& g, const Rect& r, Brush& stringBrush, int stringCount, float stringSpacing, float baseThickness, float thicknessIncrement)
	{
		for (int i = 1; i <= stringCount; ++i)
		{
			float stringY = (r.top + stringSpacing * i) - (stringSpacing / 2.f);
			float currentThickness = baseThickness + (i - 1) * thicknessIncrement;
			g.DrawLine(Point(r.left, stringY), Point(r.right, stringY), stringBrush, currentThickness);
		}
	}

	int32_t MP_STDCALL OnRender(GmpiDrawing_API::IMpDeviceContext* drawingContext) override
	{
		Graphics g(drawingContext);

		// Initialize Brushes here
		initializeBrushes(g);  // Initialize the brushes once on the first render
		updateBrushes(g);      // Update the brushes on every render

		// Get the drawing area
		Rect r = getRect();
		ClipDrawingToBounds clip(g, r);

		// Check if drawing area is valid.
		if (r.getWidth() <= 0 || r.getHeight() <= 0) {
			return MP_OK;
		}
		pinWidth = r.getWidth();
		// Fretboard parameters        
		float lineSize = 1;

		// Clamp the fret count between 1 and 24
		int fretCount = std::clamp(pinFretCount.getValue(), 1, 24);

		// Clamp the string count between 1 and 7
		int stringCount = std::clamp(pinStringsCount.getValue(), 1, 6);

		// Pre-calculate values
		float stretchFactor = (r.getWidth() / (r.getWidth() - lineSize));
		float usableWidth = r.getWidth() - lineSize;
		float stringSpacing = r.getHeight() / static_cast<float>(stringCount);

		// Calculate nut width separately
		float nutWidth = usableWidth / (FRETBOARD_RATIO * (1.0f - pow(TUNING_RATIO, -fretCount / 12.0)));

		// Nut Thickness
		float nutThickness = stretchFactor * NUT_LINE_SIZE_FACTOR;
		calculateMarkerPositions(nutThickness, nutWidth, stretchFactor, fretCount, r); // essential for drawing markers

		// Draw
		drawFingerboard(g, r, brushFingerboard);
		drawBorders(g, r, nutBrush, lineSize);
		drawNut(g, r, nutBrush, lineBrush, lineSize, nutThickness);  // Pass nutThickness
		drawFrets(g, r, fretBrush, lineSize, fretCount, nutWidth, stretchFactor, nutThickness); // Pass nutThickness
		drawMarkers(g, r, markerBrush, fretCount); // Pass nutThickness
		drawStrings(g, r, stringBrush, stringCount, stringSpacing, pinStringsThickness.getValue(), r.getHeight() / (stringCount * 100));

		return gmpi::MP_OK;
	}
};

namespace
{
	auto r = Register<FretboardXGui>::withId(L"FretboardX");
}
