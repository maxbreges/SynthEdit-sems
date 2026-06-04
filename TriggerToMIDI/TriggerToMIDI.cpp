#include "Processor.h"

using namespace gmpi;

struct TriggerToMIDI final : public Processor
{
	BoolInPin pinTrigger;
	BoolInPin pinGate;
	AudioInPin pinPitch;
	AudioInPin pinVelocity;
	IntInPin pinChannel;
	MidiOutPin pinMIDIOut;

	TriggerToMIDI()
	{
	}

	void subProcess( int sampleFrames )
	{
		// get pointers to in/output buffers.
		auto pitch = getBuffer(pinPitch);
		auto velocity = getBuffer(pinVelocity);

		for( int s = sampleFrames; s > 0; --s )
		{
			// TODO: Signal processing goes here.

			// Increment buffer pointers.
			++pitch;
			++velocity;
		}
	}

	void onSetPins() override
	{
		// Check which pins are updated.
		if( pinTrigger.isUpdated() )
		{
		}
		if( pinGate.isUpdated() )
		{
		}
		if( pinPitch.isStreaming() )
		{
		}
		if( pinVelocity.isStreaming() )
		{
		}
		if( pinChannel.isUpdated() )
		{
		}

		// Set processing method.
		setSubProcess(&TriggerToMIDI::subProcess);
	}
};

namespace
{
auto r = Register<TriggerToMIDI>::withXml(R"XML(
<?xml version="1.0" encoding="UTF-8"?>
<Plugin id="My Trigger To MIDI" name="Trigger To MIDI" category="MIDI">
    <Audio>
        <Pin name="Trigger" datatype="bool"/>
        <Pin name="Gate" datatype="bool"/>
        <Pin name="Pitch" datatype="float" rate="audio" default="0.5"/>
        <Pin name="Velocity" datatype="float" rate="audio" default="0.5"/>
        <Pin name="Channel" datatype="enum" ignorePatchChange="true" metadata="All=-1,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16"/>
        <Pin name="MIDI Out" datatype="midi" direction="out"/>
    </Audio>
</Plugin>
)XML");
}
