//
// Created by Вадим Кадырин on 18.12.2024.
//

#include "GameScene.h"
GameScene::GameScene(GameEnvironment &env, DeprecatedGameEnvironment &deprEnv)
    : env_(env), deprEnv_(deprEnv), levelLoader_(deprEnv.scr, env.input(), deprEnv.audio) {}
