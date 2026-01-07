#pragma once

#include <cstdint>

namespace Engine {

    enum class KeyCode : uint16_t {
        Space           = 32,
        Apostrophe      = 39, /* ' */
        Comma           = 44, /* , */
        Minus           = 45, /* - */
        Period          = 46, /* . */
        Slash           = 47, /* / */

        D0              = 48,
        D1              = 49,
        D2              = 50,
        D3              = 51,
        D4              = 52,
        D5              = 53,
        D6              = 54,
        D7              = 55,
        D8              = 56,
        D9              = 57,

        Semicolon       = 59, /* ; */
        Equal           = 61, /* = */

        A               = 65,
        B               = 66,
        C               = 67,
        D               = 68,
        E               = 69,
        F               = 70,
        G               = 71,
        H               = 72,
        I               = 73,
        J               = 74,
        K               = 75,
        L               = 76,
        M               = 77,
        N               = 78,
        O               = 79,
        P               = 80,
        Q               = 81,
        R               = 82,
        S               = 83,
        T               = 84,
        U               = 85,
        V               = 86,
        W               = 87,
        X               = 88,
        Y               = 89,
        Z               = 90,

        Escape          = 256,
        Enter           = 257,
        Tab             = 258,
        Backspace       = 259,
        Insert          = 260,
        Delete          = 261,
        Right           = 262,
        Left            = 263,
        Down            = 264,
        Up              = 265,
        
        LeftShift       = 340,
        LeftControl     = 341,
        LeftAlt         = 342,
        LeftSuper       = 343,
        RightShift      = 344,
        RightControl    = 345,
        RightAlt        = 346,
        RightSuper      = 347,
        Menu            = 348,

        // Keypad / Numpad keys (GLFW-style codes)
        Keypad0         = 320,
        Keypad1         = 321,
        Keypad2         = 322,
        Keypad3         = 323,
        Keypad4         = 324,
        Keypad5         = 325,
        Keypad6         = 326,
        Keypad7         = 327,
        Keypad8         = 328,
        Keypad9         = 329,
        KeypadDecimal   = 330,
        KeypadDivide    = 331,
        KeypadMultiply  = 332,
        KeypadSubtract  = 333,
        KeypadAdd       = 334,
        KeypadEnter     = 335,
        KeypadEqual     = 336
    };
    
    enum class MouseCode : uint16_t {
        Button0         = 0,
        Button1         = 1,
        Button2         = 2,
        Button3         = 3,
        Button4         = 4,
        Button5         = 5,
        
        Left            = Button0,
        Right           = Button1,
        Middle          = Button2
    };
}

#define ENG_KEY_SPACE           ::Engine::KeyCode::Space
#define ENG_KEY_APOSTROPHE      ::Engine::KeyCode::Apostrophe
#define ENG_KEY_COMMA           ::Engine::KeyCode::Comma
#define ENG_KEY_MINUS           ::Engine::KeyCode::Minus
#define ENG_KEY_PERIOD          ::Engine::KeyCode::Period
#define ENG_KEY_SLASH           ::Engine::KeyCode::Slash

#define ENG_KEY_0               ::Engine::KeyCode::D0
#define ENG_KEY_1               ::Engine::KeyCode::D1
#define ENG_KEY_2               ::Engine::KeyCode::D2
#define ENG_KEY_3               ::Engine::KeyCode::D3
#define ENG_KEY_4               ::Engine::KeyCode::D4
#define ENG_KEY_5               ::Engine::KeyCode::D5
#define ENG_KEY_6               ::Engine::KeyCode::D6
#define ENG_KEY_7               ::Engine::KeyCode::D7
#define ENG_KEY_8               ::Engine::KeyCode::D8
#define ENG_KEY_9               ::Engine::KeyCode::D9

#define ENG_KEY_SEMICOLON       ::Engine::KeyCode::Semicolon
#define ENG_KEY_EQUAL           ::Engine::KeyCode::Equal

#define ENG_KEY_A               ::Engine::KeyCode::A
#define ENG_KEY_B               ::Engine::KeyCode::B
#define ENG_KEY_C               ::Engine::KeyCode::C
#define ENG_KEY_D               ::Engine::KeyCode::D
#define ENG_KEY_E               ::Engine::KeyCode::E
#define ENG_KEY_F               ::Engine::KeyCode::F
#define ENG_KEY_G               ::Engine::KeyCode::G
#define ENG_KEY_H               ::Engine::KeyCode::H
#define ENG_KEY_I               ::Engine::KeyCode::I
#define ENG_KEY_J               ::Engine::KeyCode::J
#define ENG_KEY_K               ::Engine::KeyCode::K
#define ENG_KEY_L               ::Engine::KeyCode::L
#define ENG_KEY_M               ::Engine::KeyCode::M
#define ENG_KEY_N               ::Engine::KeyCode::N
#define ENG_KEY_O               ::Engine::KeyCode::O
#define ENG_KEY_P               ::Engine::KeyCode::P
#define ENG_KEY_Q               ::Engine::KeyCode::Q
#define ENG_KEY_R               ::Engine::KeyCode::R
#define ENG_KEY_S               ::Engine::KeyCode::S
#define ENG_KEY_T               ::Engine::KeyCode::T
#define ENG_KEY_U               ::Engine::KeyCode::U
#define ENG_KEY_V               ::Engine::KeyCode::V
#define ENG_KEY_W               ::Engine::KeyCode::W
#define ENG_KEY_X               ::Engine::KeyCode::X
#define ENG_KEY_Y               ::Engine::KeyCode::Y
#define ENG_KEY_Z               ::Engine::KeyCode::Z

#define ENG_KEY_ESCAPE          ::Engine::KeyCode::Escape
#define ENG_KEY_ENTER           ::Engine::KeyCode::Enter
#define ENG_KEY_TAB             ::Engine::KeyCode::Tab
#define ENG_KEY_BACKSPACE       ::Engine::KeyCode::Backspace
#define ENG_KEY_INSERT          ::Engine::KeyCode::Insert
#define ENG_KEY_DELETE          ::Engine::KeyCode::Delete

#define ENG_KEY_RIGHT           ::Engine::KeyCode::Right
#define ENG_KEY_LEFT            ::Engine::KeyCode::Left
#define ENG_KEY_DOWN            ::Engine::KeyCode::Down
#define ENG_KEY_UP              ::Engine::KeyCode::Up

#define ENG_KEY_LEFT_SHIFT      ::Engine::KeyCode::LeftShift
#define ENG_KEY_LEFT_CONTROL    ::Engine::KeyCode::LeftControl
#define ENG_KEY_LEFT_ALT        ::Engine::KeyCode::LeftAlt
#define ENG_KEY_LEFT_SUPER      ::Engine::KeyCode::LeftSuper
#define ENG_KEY_RIGHT_SHIFT     ::Engine::KeyCode::RightShift
#define ENG_KEY_RIGHT_CONTROL   ::Engine::KeyCode::RightControl
#define ENG_KEY_RIGHT_ALT       ::Engine::KeyCode::RightAlt
#define ENG_KEY_RIGHT_SUPER     ::Engine::KeyCode::RightSuper

#define ENG_KEY_MENU            ::Engine::KeyCode::Menu

// Keypad / Numpad macros
#define ENG_KEY_KP_0            ::Engine::KeyCode::Keypad0
#define ENG_KEY_KP_1            ::Engine::KeyCode::Keypad1
#define ENG_KEY_KP_2            ::Engine::KeyCode::Keypad2
#define ENG_KEY_KP_3            ::Engine::KeyCode::Keypad3
#define ENG_KEY_KP_4            ::Engine::KeyCode::Keypad4
#define ENG_KEY_KP_5            ::Engine::KeyCode::Keypad5
#define ENG_KEY_KP_6            ::Engine::KeyCode::Keypad6
#define ENG_KEY_KP_7            ::Engine::KeyCode::Keypad7
#define ENG_KEY_KP_8            ::Engine::KeyCode::Keypad8
#define ENG_KEY_KP_9            ::Engine::KeyCode::Keypad9
#define ENG_KEY_KP_DECIMAL      ::Engine::KeyCode::KeypadDecimal
#define ENG_KEY_KP_DIVIDE       ::Engine::KeyCode::KeypadDivide
#define ENG_KEY_KP_MULTIPLY     ::Engine::KeyCode::KeypadMultiply
#define ENG_KEY_KP_SUBTRACT     ::Engine::KeyCode::KeypadSubtract
#define ENG_KEY_KP_ADD          ::Engine::KeyCode::KeypadAdd
#define ENG_KEY_KP_ENTER        ::Engine::KeyCode::KeypadEnter
#define ENG_KEY_KP_EQUAL        ::Engine::KeyCode::KeypadEqual

#define ENG_MOUSE_BUTTON_0      ::Engine::MouseCode::Button0
#define ENG_MOUSE_BUTTON_1      ::Engine::MouseCode::Button1
#define ENG_MOUSE_BUTTON_2      ::Engine::MouseCode::Button2
#define ENG_MOUSE_BUTTON_3      ::Engine::MouseCode::Button3
#define ENG_MOUSE_BUTTON_4      ::Engine::MouseCode::Button4
#define ENG_MOUSE_BUTTON_5      ::Engine::MouseCode::Button5

#define ENG_MOUSE_LEFT          ::Engine::MouseCode::Left
#define ENG_MOUSE_RIGHT         ::Engine::MouseCode::Right
#define ENG_MOUSE_MIDDLE        ::Engine::MouseCode::Middle