#ifndef __CLIENT__SCREEN__GAME__
#define __CLIENT__SCREEN__GAME__

#include <Types.h>

#include "PGUI/PGUI.Screen.h"

class Item;

namespace FOC
{
    namespace Screen
    {
        class Game : public PGUI::Screen
        {
public:
            struct ID
            {

                static const uint  HandLeft = 1;
                static const uint  HandRight = 2;
                static const uint  BodyArmor = 3;
                static const uint8 HitPoints = 10;
                static const uint8 ActionPoints = 11;
            };

private:
            bool MiniMap;

public:
            Game( PGUI::Core* gui );
            virtual ~Game();

            virtual void Update() override;
            virtual void Draw() override;
            void         DrawItem( uint id, Item* item );

            virtual void OnOpen( bool& modal ) override;
            virtual void OnClose() override;
        };
    };
};

#endif // __CLIENT__SCREEN__GAME__ //
