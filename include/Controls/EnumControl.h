/**
 * @file	EnumControl.h
 * @brief	Containing a class for a enum control that is showing a enumeration variable value and offers the possibility to edit the value.
 */ 

#ifndef ENUMCONTROL_H_
#define ENUMCONTROL_H_

#include "../Indicators/EnumIndicator.h"

/**
 * Class for a enum control that is showing a enumeration variable value and offers the possibility to edit the value.
 * @tparam T Type of enumeration handled by this control
 */
template <class T>
class EnumControl : public EnumIndicator<T>
{	
	private:
		void* _controlContext;							/**< Context pointer that is returned with the _onValueChanged function pointer */
		void(*_onValueChanged)(void* controlContext);	/**< Function pointer for _onValueChanged event. This function is called when the value of the valuePointer is changed. */
		bool _lastDrawnEditMode;						/**< The EditMode that was last drawn. Used to detect changes in the edit mode. */
		
	public:

		/**
		 * Constructor of the EnumControl.
		 * @param valuePointer Pointer to the enumeration variable that is shown and manipulated by this control.
		 * @param enumNames Pointer to an array of char pointers containing the display names for all enumeration values. The number of elements in this array must be at least the number of enum values.
		 * @param numEnumValues Number of enumeration values. This number must match the real number of enum values in T.
		 * @param controlContext Context pointer that is returned with the _onValueChanged function pointer
		 * @param onValueChanged Function pointer for _onValueChanged event. This function is called when the value of the valuePointer is changed.
		 * @param locX X Location of the upper left corner of the EnumControl 
		 * @param locY Y Location of the upper left corner of the EnumControl
		 */
		EnumControl(T* valuePointer, const char** enumNames, uint8_t numEnumValues, void* controlContext = NULL, void(*onValueChanged)(void* controlContext) = NULL, uint16_t locX = 0, uint16_t locY = 0);

		/**
		 * Method used for drawing of the EnumControl.
		 */
		virtual void Draw(bool redraw) override;
		
		/**
		 * Process the given key.
		 * If this element doesn't support the key type, the key is forwarded to the parent. This is done until the key is processed or the root of the visual tree is reached.
		 * @param key Key that should be processed. Supported keys are: KEYOK, KEYUP, KEYDOWN
		 * @return true if the key was processed; false if not.
		 */
		virtual bool KeyInput(Keys_t key) override;
		
		/**
		 * Process a touch input at the given point (x, y)
		 * @param x X-Coordinate of the touched point
		 * @param y Y-Coordinate of the touched point
		 * @param touchType Type of the touch
		 * @return true if the touch was processed; false if not.
		 */
		virtual bool TouchInput(uint16_t x, uint16_t y, TouchTypes touchType) override;

		/**
		 * Set the value of the controlled enumeration variable to the previous enum value.
		 * If the current value is the first one, the control selects the last enum value to implement a circular behaviour.
		 */
		bool PreviousValue();
		
		/**
		 * Set the value of the controlled enumeration variable to the next enum value.
		 * If the current value is the last one, the control selects the first enum value to implement a circular behaviour.
		 */
		bool NextValue();
		
		/**
		 * Toggle the control between display and edit mode.
		 * Some keys are only supported in edit mode.
		 */
		void ToggleEditMode();
};

#endif /* ENUMCONTROL_H_ */