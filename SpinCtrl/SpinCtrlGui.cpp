#include "mp_sdk_gui2.h"
#include "it_enum_list.h"
#include <thread>
#include <atomic>
#include <chrono>

using namespace gmpi;

	class SpinCtrlGui final : public SeGuiInvisibleBase
	{
		void onSetIndex()
		{
			UpdatePinSelection();
		}

		void onSetItemList()
		{
			// pinItemList changed
		}

		void onSetSelection()
		{			
		}

		bool prev_increment = {}, prev_decrement = {};
		int valueOnMouseDown = {};
		std::atomic<bool> runningIncrement = false;
		std::atomic<bool> runningDecrement = false;
		int repeatRate = 100;  // milliseconds
		int holdDelay = 500;    // milliseconds before starting auto-increment/decrement

		void autoIncrement() {
			// Wait for the hold delay before starting auto-increment
			std::this_thread::sleep_for(std::chrono::milliseconds(holdDelay));

			while (runningIncrement) {
				nextValue(1);
				std::this_thread::sleep_for(std::chrono::milliseconds(repeatRate));
			}
		}

		void autoDecrement() {
			// Wait for the hold delay before starting auto-decrement
			std::this_thread::sleep_for(std::chrono::milliseconds(holdDelay));

			while (runningDecrement) {
				nextValue(-1);
				std::this_thread::sleep_for(std::chrono::milliseconds(repeatRate));
			}
		}

		void onSetDecr() {
			if (prev_decrement != pinDecr) {
				if (pinDecr) {
					valueOnMouseDown = pinIndex;
					runningDecrement = true;
					// Start decrement thread
					std::thread(&SpinCtrlGui::autoDecrement, this).detach();
				}
				else {
					runningDecrement = false;
					// Perform immediate decrement if released
					if (valueOnMouseDown == pinIndex) {
						nextValue(-1);
					}
				}
			}
			prev_decrement = pinDecr;
		}

		void onSetIncr() {
			if (prev_increment != pinIncr) {
				if (pinIncr) {
					valueOnMouseDown = pinIndex;
					runningIncrement = true;
					// Start increment thread
					std::thread(&SpinCtrlGui::autoIncrement, this).detach();
				}
				else {
					runningIncrement = false;
					// Perform immediate increment if released
					if (valueOnMouseDown == pinIndex) {
						nextValue(1);
					}
				}
			}
			prev_increment = pinIncr;
		}

	void onSetWrap()
	{
		// pinItemText changed
	}
	void onSetMouseDown()
	{
		repeatRate = pinRepeatRate;
		holdDelay = pinHoldDelay;
	}

	IntGuiPin pinIndex;
	StringGuiPin pinItemList;
	StringGuiPin pinSelection;
	BoolGuiPin pinDecr;
	BoolGuiPin pinIncr;
	BoolGuiPin pinWrap;
	IntGuiPin pinHoldDelay;
	IntGuiPin pinRepeatRate;

public:
	SpinCtrlGui()
	{
		initializePin(pinIndex, static_cast<MpGuiBaseMemberPtr2>(&SpinCtrlGui::onSetIndex));
		initializePin(pinItemList, static_cast<MpGuiBaseMemberPtr2>(&SpinCtrlGui::onSetItemList));
		initializePin(pinSelection, static_cast<MpGuiBaseMemberPtr2>(&SpinCtrlGui::onSetSelection));
		initializePin(pinDecr, static_cast<MpGuiBaseMemberPtr2>(&SpinCtrlGui::onSetDecr));
		initializePin(pinIncr, static_cast<MpGuiBaseMemberPtr2>(&SpinCtrlGui::onSetIncr));
		initializePin(pinWrap, static_cast<MpGuiBaseMemberPtr2>(&SpinCtrlGui::onSetWrap));
		initializePin(pinHoldDelay, static_cast<MpGuiBaseMemberPtr2>(&SpinCtrlGui::onSetMouseDown));
		initializePin(pinRepeatRate, static_cast<MpGuiBaseMemberPtr2>(&SpinCtrlGui::onSetMouseDown));
	}

	void nextValue(int direction)
	{
		it_enum_list itr(pinItemList);
		itr.First();
		if (itr.IsDone()) // empty list
			return;

		int first_idx = itr.CurrentItem()->value;
		int cur_idx = pinIndex;
		int last_idx = first_idx;
		int prev_idx = first_idx;
		int next_idx = first_idx;
		bool found_current = false;

		for (; !itr.IsDone(); itr.Next())
		{
			enum_entry* e = itr.CurrentItem();			
			last_idx = e->value;
			
			if (last_idx == cur_idx)
			{
				found_current = true;
				//pinSelection = e->text;
				next_idx = cur_idx;
			}

			if (found_current && next_idx == cur_idx)
			{
				next_idx = last_idx;				
			}

			if (!found_current)
			{
				prev_idx = last_idx;
			}			
		}

		if (direction == 1) // increment
		{
			if ((cur_idx == last_idx) && pinWrap)
				next_idx = first_idx;

			pinIndex = next_idx;

		}
		else // decrement
		{
			if ((cur_idx == first_idx) && pinWrap)
				prev_idx = last_idx;

			pinIndex = prev_idx;
		}

		UpdatePinSelection();
	}
	void UpdatePinSelection() {
		// This method updates the pinSelection based on the current pinIndex
		it_enum_list itr(pinItemList);
		itr.First(); // Start at the beginning of the item list
		while (!itr.IsDone()) {
			enum_entry* e = itr.CurrentItem(); // Get the current item
			if (e->value == pinIndex) { // If the item's value matches the current index
				pinSelection = e->text; // Update pinSelection to the current item's text
				break; // Break out of the loop as we found the item
			}
			itr.Next(); // Move to the next item
		}
	}
};

namespace
{
	auto r = Register<SpinCtrlGui>::withId(L"SpinCtrl");
}
