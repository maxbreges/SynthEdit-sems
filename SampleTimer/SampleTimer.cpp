#include "./SampleTimer.h"

REGISTER_PLUGIN2 ( SampleTimer, L"SE Sample Timer" );

SampleTimer::SampleTimer( ) 
	: outValue_(0), timer_(0)
{
	// Register pins.
	initializePin(pinGate);
	initializePin(pinTimeIn);
	initializePin(  pinSignalOut );
}

void SampleTimer::subProcess(int sampleFrames)
{
    float* gate = getBuffer(pinGate);
    float* signalOut = getBuffer(pinSignalOut);

    if (*gate > 0)
    {
        for (int s = 0; s < sampleFrames; ++s)
        {
            if (timer_ >= pinTimeIn)
            {
                // Timer expired, output is 0
                outValue_ = 0;
            }
            else
            {
                // Timer running, output is 1
                outValue_ = 1;
                ++timer_;
            }

            // When timer hits pinTimeIn, set output to 0 at this sample
            if (timer_ == pinTimeIn)
            {
                pinSignalOut.setUpdated(this->getBlockPosition() + s);
            }

            *signalOut++ = outValue_;
        }
    }
    else
    {
        // Gate is not active, output is 0, reset timer
        outValue_ = 0;
        *signalOut++ = 0;
        timer_ = 0;
    }
}

void SampleTimer::onSetPins(void)
{
	
	// Set state of output audio pins.
	pinSignalOut.setStreaming(false);

	// Set processing method.
	SET_PROCESS2(&SampleTimer::subProcess);

	// Set sleep mode (optional).
	setSleep(false);
}

