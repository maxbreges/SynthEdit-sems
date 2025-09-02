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

void SampleTimer::subProcess( int sampleFrames )
{
	// get pointers to in/output buffers.
	float* gate = getBuffer(pinGate);
	float* signalOut = getBuffer(pinSignalOut);	

	if (*gate > 0)
	{
		for (int s = sampleFrames; s > 0; --s)
		{
			outValue_ = 1;

			if (timer_++ == pinTimeIn)
			{
				outValue_ = 0;
				pinSignalOut.setUpdated(this->getBlockPosition() + sampleFrames - s);
			}

			(*gate)++;
			*signalOut++ = outValue_;			
		}
	}
	else
	{
		*signalOut++ = 0;
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

