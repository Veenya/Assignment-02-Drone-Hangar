#ifndef __BUTTONIMPL__
#define __BUTTONIMPL__

#include "Button.h"

class ButtonImpl: public Button {
    public:
        ButtonImpl(int pin);
        bool isPressed();
        bool isClicked();
        void sync();
    protected:
        int pin;
        bool pressed;
        bool clicked;
};

#endif