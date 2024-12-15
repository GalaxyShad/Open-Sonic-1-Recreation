
#include "Game.h"

#include "core/SfmlArtist.h"
#include "core/SfmlTextureStore.h"

int main() {
    SfmlArtist sfmlArtist;
    SfmlTextureStore sfmlTextureStore;

//    auto texTitleId = sfmlTextureStore.loadFromFile("content/textures/texTitle.png");
//
//    while (1) {
//        auto& tex = sfmlTextureStore.get(texTitleId);
//
//        sfmlArtist.drawTextureRect(tex, {.x = 0, .y = 0, .width = 256, .height = 128}, {.x = 32, .y = 32}, artist_api::Artist::TransformProps {
//                                                                                                               .angle = 45,
//                                                                                                               .flipVertical = true,
//                                                                                                               .scale = {
//                                                                                                                   .x = 0.5,
//                                                                                                                   .y = 0.5
//                                                                                                               }
//                                                                                                           });
//        sfmlArtist.renderLoop();
//    }

    Screen scr(sfmlArtist, sfmlTextureStore);

    Game game(scr);
    game.init();

    while (game.isRunning()) {
        game.update();
    }

    return 0;
}