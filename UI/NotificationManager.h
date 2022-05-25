#ifndef NOTIFICATIONMANAGER_H
#define NOTIFICATIONMANAGER_H


#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <vector>
#include <algorithm>


#include "../Utils/handler.h"
#include "Notification.h"

namespace nesemulator
{
    namespace ui 
    {

        class NotificationManager
        {
            public:
                NotificationManager(SDL_Renderer* renderer, TTF_Font* font);

                void addNotification(const std::string& message, const SDL_Color& color, TIMER durationInMilliseconds);
                void removeAllNotifications();
                void update();
                void render();
                void setRenderer(SDL_Renderer* renderer){ this->renderer = renderer; };
                void setFont(TTF_Font* font){ this->font = font; };

            private:
                std::vector<Notification> notifications;
                SDL_Renderer* renderer;
                TTF_Font* font;
                TIMER lastSystemTime;
        };

        inline NotificationManager::NotificationManager(SDL_Renderer* renderer, TTF_Font* font)
        {
            this->renderer = renderer;
            this->font = font;
            lastSystemTime = SDL_GetTicks64();
        }

        inline void NotificationManager::addNotification(const std::string& message, const SDL_Color& color, TIMER duration)
        {
            notifications.push_back(Notification(message, duration, color, font, renderer));
        }

        inline void NotificationManager::removeAllNotifications()
        {
            notifications.clear();
        }

        inline void NotificationManager::update()
        {
            auto temp = SDL_GetTicks64();
            for (auto& notification : notifications)
            {
                notification.update(temp - lastSystemTime);
                if(notification.isFinished())
                    notifications.erase(std::remove(notifications.begin(), notifications.end(), notification), notifications.end());
            }

            lastSystemTime = temp;
        }

        inline void NotificationManager::render()
        {
            auto deltaHeight = 0;
            for (auto& notification : notifications)
            {
                notification.draw(0, deltaHeight);
                deltaHeight += 8;
            }
        }



    }

}




#endif 