#include "imgui-NoTiE.h"
#include <imgui.h>

#include <SFML/Graphics/Rect.hpp>
#include <SFML/Config.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Window/Clipboard.hpp>
#include <SFML/Window/Cursor.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Window.hpp>

#include <cassert>
#include <cmath>    // abs
#include <cstddef>  // offsetof, NULL
#include <cstring>  // memcpy

#include "../Core/Input.h"
#include "../Core/Window.h"

#if __cplusplus >= 201103L  // C++11 and above
static_assert(sizeof(GLuint) <= sizeof(ImTextureID),
              "ImTextureID is not large enough to fit GLuint.");
#endif

#define AS_INT(...) static_cast<int>(__VA_ARGS__)

namespace {
// data
static bool s_windowHasFocus = false;
static bool s_mousePressed[3] = {false, false, false};
static bool s_mouseMoved = false;
static sf::Texture* s_fontTexture =
    NULL;  // owning pointer to internal font atlas which is used if user
           // doesn't set custom sf::Texture.

static const unsigned int NULL_JOYSTICK_ID = sf::Joystick::Count;
static unsigned int s_joystickId = NULL_JOYSTICK_ID;

static const unsigned int NULL_JOYSTICK_BUTTON = sf::Joystick::ButtonCount;
static unsigned int s_joystickMapping[ImGuiNavInput_COUNT];

// various helper functions
ImVec2 getTopLeftAbsolute(const sf::FloatRect& rect);
ImVec2 getDownRightAbsolute(const sf::FloatRect& rect);

ImTextureID convertGLTextureHandleToImTextureID(GLuint glTextureHandle);
GLuint convertImTextureIDToGLTextureHandle(ImTextureID textureID);

void RenderDrawLists(
    ImDrawData* draw_data);  // rendering callback function prototype

// Implementation of ImageButton overload
bool imageButtonImpl(const sf::Texture& texture,
                     const sf::FloatRect& textureRect, const Vector2Float& size,
                     const int framePadding, const Color& bgColor,
                     const Color& tintColor);

// Returns first id of connected joystick
unsigned int getConnectedJoystickId();

// clipboard functions
void setClipboardText(void* userData, const char* text);
const char* getClipboadText(void* userData);
std::string s_clipboardText;

// mouse cursors
void loadMouseCursor(ImGuiMouseCursor imguiCursorType,
                     sf::Cursor::Type sfmlCursorType);
void updateMouseCursor(Window& window);

sf::Cursor* s_mouseCursors[ImGuiMouseCursor_COUNT];
bool s_mouseCursorLoaded[ImGuiMouseCursor_COUNT];

}  // namespace

namespace ImGui {
namespace notie {

void Init(Window& window, bool loadDefaultFont) {
#if __cplusplus < 201103L  // runtime assert when using earlier than C++11 as no
                           // static_assert support
    assert(
        sizeof(GLuint) <=
        sizeof(ImTextureID));  // ImTextureID is not large enough to fit GLuint.
#endif

    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    ImGuiIO& io = ImGui::GetIO();
	
    io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
    io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;
    io.BackendPlatformName = "ImGui_NotiE";

    //init keyboard maps
    io.KeyMap[ImGuiKey_Tab] = AS_INT(Key::Tab);
    io.KeyMap[ImGuiKey_LeftArrow] = AS_INT(Key::Left);
    io.KeyMap[ImGuiKey_RightArrow] = AS_INT(Key::Right);
    io.KeyMap[ImGuiKey_UpArrow] = AS_INT(Key::Up);
    io.KeyMap[ImGuiKey_DownArrow] = AS_INT(Key::Down);
    io.KeyMap[ImGuiKey_PageUp] = AS_INT(Key::PageUp);
    io.KeyMap[ImGuiKey_PageDown] = AS_INT(Key::PageDown);
    io.KeyMap[ImGuiKey_Home] = AS_INT(Key::Home);
    io.KeyMap[ImGuiKey_End] = AS_INT(Key::End);
    io.KeyMap[ImGuiKey_Insert] = AS_INT(Key::Insert);
    io.KeyMap[ImGuiKey_Delete] = AS_INT(Key::Delete);
    io.KeyMap[ImGuiKey_Backspace] = AS_INT(Key::Backspace);
    io.KeyMap[ImGuiKey_Space] = AS_INT(Key::Space);
    io.KeyMap[ImGuiKey_Enter] = AS_INT(Key::Enter);
    io.KeyMap[ImGuiKey_Escape] = AS_INT(Key::Escape);
    io.KeyMap[ImGuiKey_A] = AS_INT(Key::A);
    io.KeyMap[ImGuiKey_C] = AS_INT(Key::C);
    io.KeyMap[ImGuiKey_V] = AS_INT(Key::V);
    io.KeyMap[ImGuiKey_X] = AS_INT(Key::X);
    io.KeyMap[ImGuiKey_Y] = AS_INT(Key::Y);
    io.KeyMap[ImGuiKey_Z] = AS_INT(Key::Z);

    s_joystickId = getConnectedJoystickId();

    for (unsigned int i = 0; i < ImGuiNavInput_COUNT; i++) {
        s_joystickMapping[i] = NULL_JOYSTICK_BUTTON;
    }

    // init rendering
    io.DisplaySize = window.GetSize();;

    // clipboard
    io.SetClipboardTextFn = setClipboardText;
    io.GetClipboardTextFn = getClipboadText;

    // load mouse cursors
    for (int i = 0; i < ImGuiMouseCursor_COUNT; ++i) {
        s_mouseCursorLoaded[i] = false;
    }

    loadMouseCursor(ImGuiMouseCursor_Arrow, sf::Cursor::Arrow);
    loadMouseCursor(ImGuiMouseCursor_TextInput, sf::Cursor::Text);
    loadMouseCursor(ImGuiMouseCursor_ResizeAll, sf::Cursor::SizeAll);
    loadMouseCursor(ImGuiMouseCursor_ResizeNS, sf::Cursor::SizeVertical);
    loadMouseCursor(ImGuiMouseCursor_ResizeEW, sf::Cursor::SizeHorizontal);
    loadMouseCursor(ImGuiMouseCursor_ResizeNESW,
                    sf::Cursor::SizeBottomLeftTopRight);
    loadMouseCursor(ImGuiMouseCursor_ResizeNWSE,
                    sf::Cursor::SizeTopLeftBottomRight);
    loadMouseCursor(ImGuiMouseCursor_Hand, sf::Cursor::Hand);

    if (s_fontTexture) {  // delete previously created texture
        delete s_fontTexture;
    }
    s_fontTexture = new sf::Texture;

    if (loadDefaultFont) {
        // this will load default font automatically
        // No need to call AddDefaultFont
        UpdateFontTexture();
    }

    s_windowHasFocus = window.HasFocus();
}

bool ProcessEvent(const sf::Event& event) {
    if (s_windowHasFocus) {
        ImGuiIO& io = ImGui::GetIO();
    	
        switch (event.type) {
            case sf::Event::MouseMoved:
                s_mouseMoved = true;
                return io.WantCaptureMouse;
            case sf::Event::MouseButtonPressed:  // fall-through
            case sf::Event::MouseButtonReleased: {
                int button = event.mouseButton.button;
                if (event.type == sf::Event::MouseButtonPressed &&
                    button >= 0 && button < 3)
                {
                    s_mousePressed[event.mouseButton.button] = true;
                }
            }
        	return io.WantCaptureMouse;
            case sf::Event::MouseWheelScrolled:
                if (event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel ||
                    (event.mouseWheelScroll.wheel == sf::Mouse::HorizontalWheel &&
                    io.KeyShift)) {
                    io.MouseWheel += event.mouseWheelScroll.delta;
                } else if (event.mouseWheelScroll.wheel == sf::Mouse::HorizontalWheel) {
                    io.MouseWheelH += event.mouseWheelScroll.delta;
                }
                return io.WantCaptureMouse;
            case sf::Event::KeyPressed:  // fall-through
            case sf::Event::KeyReleased:
                io.KeysDown[event.key.code] =
                    (event.type == sf::Event::KeyPressed);
                return io.WantCaptureKeyboard;
            case sf::Event::TextEntered:
                // Don't handle the event for unprintable characters
                if (event.text.unicode < ' ' || event.text.unicode == 127) {
                    break;
                }
                io.AddInputCharacter(event.text.unicode);
                return io.WantCaptureKeyboard;
            default:
                break;
        }
    }

    switch (event.type) {
        case sf::Event::LostFocus:
            s_windowHasFocus = false;
            break;
        case sf::Event::GainedFocus:
            s_windowHasFocus = true;
            break;
        default:
            break;
    }
	
    return false;
}

void Update(Window& window, float dt) {
    // Update OS/hardware mouse cursor if imgui isn't drawing a software cursor
    // TODO 
   // updateMouseCursor(window);

	if (s_mouseMoved)
	{
        Update(Input::GetMousePositionInWindow(window),
               static_cast<Vector2Float>(window.GetSize()), dt);
	}
    else
    {
        Update(Vector2Int(0, 0), static_cast<Vector2Float>(window.GetSize()), dt);
    }

    if (ImGui::GetIO().MouseDrawCursor) {
        // Hide OS mouse cursor if imgui is drawing it
        window.ShowCursor(false);
    }
}

void Update(const Vector2Int& mousePos, const Vector2Float& displaySize,
            float dt) {
    ImGuiIO& io = ImGui::GetIO();
    io.DisplaySize = displaySize;
    io.DeltaTime = dt;

    if (s_windowHasFocus) {
        if (io.WantSetMousePos) {
            Vector2Int mousePos(static_cast<int>(io.MousePos.x),
                                  static_cast<int>(io.MousePos.y));
            Input::SetMousePosition(mousePos);
        } else {
            io.MousePos = mousePos;
        }
        for (unsigned int i = 0; i < 3; i++) {
            io.MouseDown[i] = s_mousePressed[i] ||
                              Input::IsMousePressed((MouseButton)i);
            s_mousePressed[i] = false;
        }
    }

    // Update Ctrl, Shift, Alt, Super state
    io.KeyCtrl = io.KeysDown[sf::Keyboard::LControl] ||
                 io.KeysDown[sf::Keyboard::RControl];
    io.KeyAlt =
        io.KeysDown[sf::Keyboard::LAlt] || io.KeysDown[sf::Keyboard::RAlt];
    io.KeyShift =
        io.KeysDown[sf::Keyboard::LShift] || io.KeysDown[sf::Keyboard::RShift];
    io.KeySuper = io.KeysDown[sf::Keyboard::LSystem] ||
                  io.KeysDown[sf::Keyboard::RSystem];

#ifdef ANDROID
#ifdef USE_JNI
    if (io.WantTextInput && !s_wantTextInput) {
        openKeyboardIME();
        s_wantTextInput = true;
    }

    if (!io.WantTextInput && s_wantTextInput) {
        closeKeyboardIME();
        s_wantTextInput = false;
    }
#endif
#endif

    assert(io.Fonts->Fonts.Size > 0);  // You forgot to create and set up font
                                       // atlas (see createFontTexture)
    ImGui::NewFrame();
}

void Render(sf::RenderTarget& target) {
    target.resetGLStates();
    ImGui::Render();
    RenderDrawLists(ImGui::GetDrawData());
}

void Render() {
    ImGui::Render();
    RenderDrawLists(ImGui::GetDrawData());
}

void Shutdown() {
    ImGui::GetIO().Fonts->TexID = (ImTextureID)NULL;

    if (s_fontTexture) {  // if internal texture was created, we delete it
        delete s_fontTexture;
        s_fontTexture = NULL;
    }

    for (int i = 0; i < ImGuiMouseCursor_COUNT; ++i) {
        if (s_mouseCursorLoaded[i]) {
            delete s_mouseCursors[i];
            s_mouseCursors[i] = NULL;

            s_mouseCursorLoaded[i] = false;
        }
    }

    ImGui::DestroyContext();
}

void UpdateFontTexture() {
    ImGuiIO& io = ImGui::GetIO();
    unsigned char* pixels;
    int width, height;

    io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);

    sf::Texture& texture = *s_fontTexture;
    texture.create(width, height);
    texture.update(pixels);

    io.Fonts->TexID =
        convertGLTextureHandleToImTextureID(texture.getNativeHandle());
}

sf::Texture& GetFontTexture() { return *s_fontTexture; }

}  // end of namespace SFML

/////////////// Image Overloads

void Image(const sf::Texture& texture, const Color& tintColor,
           const Color& borderColor) {
    Image(texture, static_cast<Vector2Float>(texture.getSize()), tintColor,
          borderColor);
}

void Image(const sf::Texture& texture, const Vector2Float& size,
           const Color& tintColor, const Color& borderColor) {
    ImTextureID textureID =
        convertGLTextureHandleToImTextureID(texture.getNativeHandle());
    ImGui::Image(textureID, size, ImVec2(0, 0), ImVec2(1, 1), tintColor,
                 borderColor);
}

//void Image(const sf::Texture& texture, const sf::FloatRect& textureRect,
//           const Color& tintColor, const Color& borderColor) {
//    Image(
//        texture,
//        Vector2Float(std::abs(textureRect.width), std::abs(textureRect.height)),
//        textureRect, tintColor, borderColor);
//}
//
//void Image(const sf::Texture& texture, const Vector2Float& size,
//           const sf::FloatRect& textureRect, const Color& tintColor,
//           const Color& borderColor) {
//    Vector2Float textureSize = static_cast<Vector2Float>(texture.getSize());
//    ImVec2 uv0(textureRect.left / textureSize.x,
//               textureRect.top / textureSize.y);
//    ImVec2 uv1((textureRect.left + textureRect.width) / textureSize.x,
//               (textureRect.top + textureRect.height) / textureSize.y);
//
//    ImTextureID textureID =
//        convertGLTextureHandleToImTextureID(texture.getNativeHandle());
//    ImGui::Image(textureID, size, uv0, uv1, tintColor, borderColor);
//}
//
//void Image(const sf::Sprite& sprite, const Color& tintColor,
//           const Color& borderColor) {
//    sf::FloatRect bounds = sprite.getGlobalBounds();
//    Image(sprite, Vector2Float(bounds.width, bounds.height), tintColor,
//          borderColor);
//}
//
//void Image(const sf::Sprite& sprite, const Vector2Float& size,
//           const Color& tintColor, const Color& borderColor) {
//    const sf::Texture* texturePtr = sprite.getTexture();
//    // sprite without texture cannot be drawn
//    if (!texturePtr) {
//        return;
//    }
//
//    Image(*texturePtr, size,
//          static_cast<sf::FloatRect>(sprite.getTextureRect()), tintColor,
//          borderColor);
//}

/////////////// Image Button Overloads

bool ImageButton(const sf::Texture& texture, const int framePadding,
                 const Color& bgColor, const Color& tintColor) {
    return ImageButton(texture, static_cast<Vector2Float>(texture.getSize()),
                       framePadding, bgColor, tintColor);
}

bool ImageButton(const sf::Texture& texture, const Vector2Float& size,
                 const int framePadding, const Color& bgColor,
                 const Color& tintColor) {
    Vector2Float textureSize = static_cast<Vector2Float>(texture.getSize());
    return ::imageButtonImpl(
        texture, sf::FloatRect(0.f, 0.f, textureSize.x, textureSize.y), size,
        framePadding, bgColor, tintColor);
}

bool ImageButton(const Sprite& sprite, const int framePadding,
                 const Color& bgColor, const Color& tintColor) {
    sf::Sprite& nativeSprite = sprite.GetSprite();
    sf::FloatRect spriteSize = nativeSprite.getGlobalBounds();
    return ImageButton(sprite,
                       Vector2Float(spriteSize.width, spriteSize.height),
                       framePadding, bgColor, tintColor);
}

bool ImageButton(const Sprite& sprite, const Vector2Float& size,
                 const int framePadding, const Color& bgColor,
                 const Color& tintColor) {
    const sf::Texture* texturePtr = sprite.GetTexture();
    if (!texturePtr) {
        return false;
    }
    sf::Sprite& nativeSprite = sprite.GetSprite();
    return ::imageButtonImpl(
        *texturePtr, static_cast<sf::FloatRect>(nativeSprite.getTextureRect()), size,
        framePadding, bgColor, tintColor);
}

/////////////// Draw_list Overloads

void DrawLine(const Vector2Float& a, const Vector2Float& b,
              const Color& color, float thickness) {
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    Vector2Float pos = ImGui::GetCursorScreenPos();
    draw_list->AddLine(a + pos, b + pos, ColorConvertFloat4ToU32(color),
                       thickness);
}

//void DrawRect(const sf::FloatRect& rect, const Color& color, float rounding,
//              int rounding_corners, float thickness) {
//    ImDrawList* draw_list = ImGui::GetWindowDrawList();
//    draw_list->AddRect(getTopLeftAbsolute(rect), getDownRightAbsolute(rect),
//                       ColorConvertFloat4ToU32(color), rounding,
//                       rounding_corners, thickness);
//}
//
//void DrawRectFilled(const sf::FloatRect& rect, const Color& color,
//                    float rounding, int rounding_corners) {
//    ImDrawList* draw_list = ImGui::GetWindowDrawList();
//    draw_list->AddRectFilled(
//        getTopLeftAbsolute(rect), getDownRightAbsolute(rect),
//        ColorConvertFloat4ToU32(color), rounding, rounding_corners);
//}

}  // end of namespace ImGui

namespace {

ImVec2 getTopLeftAbsolute(const sf::FloatRect& rect) {
    ImVec2 pos = ImGui::GetCursorScreenPos();
    return ImVec2(rect.left + pos.x, rect.top + pos.y);
}
ImVec2 getDownRightAbsolute(const sf::FloatRect& rect) {
    ImVec2 pos = ImGui::GetCursorScreenPos();
    return ImVec2(rect.left + rect.width + pos.x,
                  rect.top + rect.height + pos.y);
}

ImTextureID convertGLTextureHandleToImTextureID(GLuint glTextureHandle) {
    ImTextureID textureID = (ImTextureID)NULL;
    std::memcpy(&textureID, &glTextureHandle, sizeof(GLuint));
    return textureID;
}
GLuint convertImTextureIDToGLTextureHandle(ImTextureID textureID) {
    GLuint glTextureHandle;
    std::memcpy(&glTextureHandle, &textureID, sizeof(GLuint));
    return glTextureHandle;
}

// Rendering callback
void RenderDrawLists(ImDrawData* draw_data) {
    ImGui::GetDrawData();
    if (draw_data->CmdListsCount == 0) {
        return;
    }

    ImGuiIO& io = ImGui::GetIO();
    assert(io.Fonts->TexID !=
           (ImTextureID)NULL);  // You forgot to create and set font texture

    // scale stuff (needed for proper handling of window resize)
    int fb_width =
        static_cast<int>(io.DisplaySize.x * io.DisplayFramebufferScale.x);
    int fb_height =
        static_cast<int>(io.DisplaySize.y * io.DisplayFramebufferScale.y);
    if (fb_width == 0 || fb_height == 0) {
        return;
    }
    draw_data->ScaleClipRects(io.DisplayFramebufferScale);

#ifdef GL_VERSION_ES_CL_1_1
    GLint last_program, last_texture, last_array_buffer,
        last_element_array_buffer;
    glGetIntegerv(GL_TEXTURE_BINDING_2D, &last_texture);
    glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &last_array_buffer);
    glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &last_element_array_buffer);
#else
    glPushAttrib(GL_ENABLE_BIT | GL_COLOR_BUFFER_BIT | GL_TRANSFORM_BIT);
#endif

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_SCISSOR_TEST);
    glEnable(GL_TEXTURE_2D);
    glDisable(GL_LIGHTING);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    glViewport(0, 0, (GLsizei)fb_width, (GLsizei)fb_height);

    glMatrixMode(GL_TEXTURE);
    glLoadIdentity();

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

#ifdef GL_VERSION_ES_CL_1_1
    glOrthof(0.0f, io.DisplaySize.x, io.DisplaySize.y, 0.0f, -1.0f, +1.0f);
#else
    glOrtho(0.0f, io.DisplaySize.x, io.DisplaySize.y, 0.0f, -1.0f, +1.0f);
#endif

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    for (int n = 0; n < draw_data->CmdListsCount; ++n) {
        const ImDrawList* cmd_list = draw_data->CmdLists[n];
        const unsigned char* vtx_buffer =
            (const unsigned char*)&cmd_list->VtxBuffer.front();
        const ImDrawIdx* idx_buffer = &cmd_list->IdxBuffer.front();

        glVertexPointer(2, GL_FLOAT, sizeof(ImDrawVert),
                        (void*)(vtx_buffer + offsetof(ImDrawVert, pos)));
        glTexCoordPointer(2, GL_FLOAT, sizeof(ImDrawVert),
                          (void*)(vtx_buffer + offsetof(ImDrawVert, uv)));
        glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(ImDrawVert),
                       (void*)(vtx_buffer + offsetof(ImDrawVert, col)));

        for (int cmd_i = 0; cmd_i < cmd_list->CmdBuffer.size(); ++cmd_i) {
            const ImDrawCmd* pcmd = &cmd_list->CmdBuffer[cmd_i];
            if (pcmd->UserCallback) {
                pcmd->UserCallback(cmd_list, pcmd);
            } else {
                GLuint textureHandle =
                    convertImTextureIDToGLTextureHandle(pcmd->TextureId);
                glBindTexture(GL_TEXTURE_2D, textureHandle);
                glScissor((int)pcmd->ClipRect.x,
                          (int)(fb_height - pcmd->ClipRect.w),
                          (int)(pcmd->ClipRect.z - pcmd->ClipRect.x),
                          (int)(pcmd->ClipRect.w - pcmd->ClipRect.y));
                glDrawElements(GL_TRIANGLES, (GLsizei)pcmd->ElemCount,
                               GL_UNSIGNED_SHORT, idx_buffer);
            }
            idx_buffer += pcmd->ElemCount;
        }
    }
#ifdef GL_VERSION_ES_CL_1_1
    glBindTexture(GL_TEXTURE_2D, last_texture);
    glBindBuffer(GL_ARRAY_BUFFER, last_array_buffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, last_element_array_buffer);
    glDisable(GL_SCISSOR_TEST);
#else
    glPopAttrib();
#endif
}

bool imageButtonImpl(const sf::Texture& texture,
                     const sf::FloatRect& textureRect, const Vector2Float& size,
                     const int framePadding, const Color& bgColor,
                     const Color& tintColor) {
    Vector2Float textureSize = static_cast<Vector2Float>(texture.getSize());

    ImVec2 uv0(textureRect.left / textureSize.x,
               textureRect.top / textureSize.y);
    ImVec2 uv1((textureRect.left + textureRect.width) / textureSize.x,
               (textureRect.top + textureRect.height) / textureSize.y);

    ImTextureID textureID =
        convertGLTextureHandleToImTextureID(texture.getNativeHandle());
    return ImGui::ImageButton(textureID, size, uv0, uv1, framePadding, bgColor,
                              tintColor);
}

unsigned int getConnectedJoystickId() {
    for (unsigned int i = 0; i < (unsigned int)sf::Joystick::Count; ++i) {
        if (sf::Joystick::isConnected(i)) return i;
    }

    return NULL_JOYSTICK_ID;
}

void setClipboardText(void* /*userData*/, const char* text) {
    sf::Clipboard::setString(sf::String::fromUtf8(text, text + std::strlen(text)));
}

const char* getClipboadText(void* /*userData*/) {
    std::basic_string<sf::Uint8> tmp = sf::Clipboard::getString().toUtf8();
    s_clipboardText = std::string(tmp.begin(), tmp.end());
    return s_clipboardText.c_str();
}

void loadMouseCursor(ImGuiMouseCursor imguiCursorType,
                     sf::Cursor::Type sfmlCursorType) {
    s_mouseCursors[imguiCursorType] = new sf::Cursor();
    s_mouseCursorLoaded[imguiCursorType] =
        s_mouseCursors[imguiCursorType]->loadFromSystem(sfmlCursorType);
}

void updateMouseCursor(Window& window) {
    ImGuiIO& io = ImGui::GetIO();
    if ((io.ConfigFlags & ImGuiConfigFlags_NoMouseCursorChange) == 0) {
        ImGuiMouseCursor cursor = ImGui::GetMouseCursor();
        if (io.MouseDrawCursor || cursor == ImGuiMouseCursor_None) {
            window.ShowCursor(false);
        } else {
            window.ShowCursor(true);

            sf::Cursor& c = s_mouseCursorLoaded[cursor]
                                ? *s_mouseCursors[cursor]
                                : *s_mouseCursors[ImGuiMouseCursor_Arrow];
            window.GetRenderWindow().setMouseCursor(c);
        }
    }
}

}  // end of anonymous namespace