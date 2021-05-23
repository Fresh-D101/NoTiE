#pragma once
//#include <SFML/Graphics/Rect.hpp>
#include "../Core/Vector2.h"
#include "../Graphics/Color.h"
#include "../Graphics/Sprite.h"

class Window;
using notie::Color;

namespace sf
{
    class Event;
    class RenderTarget;
    class RenderWindow;
    class Sprite;
    class Texture;
    class Window;
}

namespace ImGui
{
    namespace notie
    {
         void Init(Window& window, bool loadDefaultFont = true);

         bool ProcessEvent(const sf::Event& event);

         void Update(Window& window, float dt);
         void Update(const Vector2Int& mousePos, const Vector2Float& displaySize, float dt);

         void Render(sf::RenderTarget& target);
         void Render();

         void Shutdown();

         void UpdateFontTexture();
         sf::Texture& GetFontTexture();
    }

    // custom ImGui widgets for SFML stuff

    // Image overloads
     void Image(const sf::Texture& texture,
        const Color& tintColor = Color::White,
        const Color& borderColor = Color::Transparent);
     void Image(const sf::Texture& texture, const Vector2Float& size,
        const Color& tintColor = Color::White,
        const Color& borderColor = Color::Transparent);
     //void Image(const sf::Texture& texture, const sf::FloatRect& textureRect,
     //   const Color& tintColor = Color::White,
     //   const Color& borderColor = Color::Transparent);
     //void Image(const sf::Texture& texture, const Vector2Float& size, const sf::FloatRect& textureRect,
     //   const Color& tintColor = Color::White,
     //   const Color& borderColor = Color::Transparent);

     //void Image(const sf::Sprite& sprite,
     //   const Color& tintColor = Color::White,
     //   const Color& borderColor = Color::Transparent);
     //void Image(const sf::Sprite& sprite, const Vector2Float& size,
     //   const Color& tintColor = Color::White,
     //   const Color& borderColor = Color::Transparent);

    // ImageButton overloads
     bool ImageButton(const sf::Texture& texture, const int framePadding = -1,
        const Color& bgColor = Color::Transparent,
        const Color& tintColor = Color::White);
     bool ImageButton(const sf::Texture& texture, const Vector2Float& size, const int framePadding = -1,
        const Color& bgColor = Color::Transparent, const Color& tintColor = Color::White);

     bool ImageButton(const Sprite& sprite, const int framePadding = -1,
        const Color& bgColor = Color::Transparent,
        const Color& tintColor = Color::White);
     bool ImageButton(const Sprite& sprite, const Vector2Float& size, const int framePadding = -1,
        const Color& bgColor = Color::Transparent,
        const Color& tintColor = Color::White);

    // Draw_list overloads. All positions are in relative coordinates (relative to top-left of the current window)
     void DrawLine(const Vector2Float& a, const Vector2Float& b, const Color& col, float thickness = 1.0f);
     //void DrawRect(const sf::FloatRect& rect, const Color& color, float rounding = 0.0f, int rounding_corners = 0x0F, float thickness = 1.0f);
     //void DrawRectFilled(const sf::FloatRect& rect, const Color& color, float rounding = 0.0f, int rounding_corners = 0x0F);
}