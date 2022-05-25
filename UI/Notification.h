#ifndef  NOTIFICATION
#define  NOTIFICATION


#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "../Utils/handler.h"


namespace nesemulator
{
    namespace ui
    {

        /* SDL Notification colors with  RGB values as array */
        namespace notificationColors
        {
            constexpr SDL_Color info = { 64, 255, 208, 255 };
            constexpr SDL_Color warning = { 255, 255, 0, 255 };
            constexpr SDL_Color error = { 255, 0, 0, 255 };
            constexpr SDL_Color success = { 0, 255, 0, 255 };
            constexpr SDL_Color white = { 255, 255, 255, 255 };
            constexpr SDL_Color black = { 0, 0, 0, 255 };
            constexpr SDL_Color red = { 255, 0, 0, 255 };
            constexpr SDL_Color green = { 0, 255, 0, 255 };
            constexpr SDL_Color blue = { 0, 0, 255, 255 };
            constexpr SDL_Color yellow = { 255, 255, 0, 255 };
            constexpr SDL_Color cyan = { 0, 255, 255, 255 };
            constexpr SDL_Color magenta = { 255, 0, 255, 255 };
        }

        

        /* Class to create notifications using SDL_ttf with durations */
        class Notification
        {
            public:
                Notification(std::string _text, TIMER _duration);
                Notification(std::string _text, TIMER _duration, SDL_Color _color, TTF_Font* _font, SDL_Renderer* _renderer);
                Notification(std::string _text, TIMER _duration, TTF_Font* _font, SDL_Renderer* _renderer);
                void draw(int x = 0, int y = 0);
                void update(TIMER delta);
                bool isFinished() const { return finished; };
                void setFinished(bool _finished) { finished = _finished; };
                void setText(std::string _text) { text = _text; };
                void setDuration(TIMER _duration) { duration = _duration; };
                void setColor(int _r, int _g, int _b) { r = _r; g = _g; b = _b; };
                void setAlpha(int _alpha) { alpha = _alpha; };
                void setFont(TTF_Font* _font) { font = _font; };
                void setRenderer(SDL_Renderer* _renderer) { renderer = _renderer; };
                void setTextSize(int _size) { textSize = _size; };
                void setTextStyle(int _style) { textStyle = _style; };
                void setTextOutline(int _outline) { textOutline = _outline; };
                void setTextOutlineColor(int _r, int _g, int _b) { textOutlineR = _r; textOutlineG = _g; textOutlineB = _b; };
                void setTextShadow(int _shadow) { textShadow = _shadow; };
                void setTextShadowColor(int _r, int _g, int _b) { textShadowR = _r; textShadowG = _g; textShadowB = _b; };
                void setTextShadowOffset(int _x, int _y) { textShadowX = _x; textShadowY = _y; };
                void setTextShadowBlur(int _blur) { textShadowBlur = _blur; };
                void setTextShadowBlurRadius(int _radius) { textShadow = _radius; };
                void setTextShadowSpread(int _spread) { textShadowSpread = _spread; };
                void setTextShadowQuality(int _quality) { textShadowQuality = _quality; };
                void setTextShadowThickness(int _thickness) { textShadowThickness = _thickness; };
                void setTextShadowType(int _type) { textShadowType = _type; };
                bool operator==(const Notification& other) const;
            private:
                std::string text;
                int64_t duration;
                int r;
                int g;
                int b;
                int alpha;
                TTF_Font* font;
                SDL_Renderer* renderer;
                int textSize;
                int textStyle;
                int textOutline;
                int textOutlineR;
                int textOutlineG;
                int textOutlineB;
                int textShadow;
                int textShadowR;
                int textShadowG;
                int textShadowB;
                int textShadowX;
                int textShadowY;
                int textShadowBlur;
                int textShadowBlurRadius;
                int textShadowSpread;
                int textShadowQuality;
                int textShadowThickness;
                int textShadowType;
                bool finished;

        };

        inline Notification::Notification(std::string _text, TIMER _duration)
        {
            text = _text;
            duration = _duration;
            finished = false;
        }

        inline Notification::Notification(std::string _text, TIMER _duration, SDL_Color _color, TTF_Font* _font, SDL_Renderer* _renderer)
        {
            text = _text;
            duration = _duration;
            finished = false;
            font = _font;
            renderer = _renderer;
            r = _color.r;
            g = _color.g;
            b = _color.b;
            alpha = _color.a;
        }


        inline Notification::Notification(std::string _text, TIMER _duration, TTF_Font* _font, SDL_Renderer* _renderer)
        {
            text = _text;
            duration = _duration;
            finished = false;
            font = _font;
            renderer = _renderer;
        }


        inline void Notification::draw(int x,int y)
        {
            SDL_Surface* surface = TTF_RenderText_Blended(font, text.c_str(), {r, g, b, alpha});
            SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
            SDL_Rect rect = {x, y, surface->w / 5, surface->h / 5};
            SDL_RenderCopy(renderer, texture, NULL, &rect);
            SDL_FreeSurface(surface);
            SDL_DestroyTexture(texture);
        }

        inline void Notification::update(TIMER delta)
        {
            duration -= delta;
            if(duration <= 0)
                finished = true;
        }



        inline bool Notification::operator==(const Notification& other) const
        {
            return (text == other.text && duration == other.duration && r == other.r && g == other.g && b == other.b && alpha == other.alpha && font == other.font && renderer == other.renderer && textSize == other.textSize && textStyle == other.textStyle && textOutline == other.textOutline && textOutlineR == other.textOutlineR && textOutlineG == other.textOutlineG && textOutlineB == other.textOutlineB && textShadow == other.textShadow && textShadowR == other.textShadowR && textShadowG == other.textShadowG && textShadowB == other.textShadowB && textShadowX == other.textShadowX && textShadowY == other.textShadowY && textShadowBlur == other.textShadowBlur && textShadowBlurRadius == other.textShadowBlurRadius && textShadowSpread == other.textShadowSpread && textShadowQuality == other.textShadowQuality && textShadowThickness == other.textShadowThickness && textShadowType == other.textShadowType && finished == other.finished);
        }
    }

}


#endif 