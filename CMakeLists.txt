cmake_minimum_required(VERSION 3.16)

set(CMAKE_CXX_STANDARD 23)
project(os1r LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.x)
FetchContent_MakeAvailable(SFML)

add_executable(os1r ${SOURCES}
        src/AnimMgr.cpp
        src/AnimMgr.h
        src/AudioMappings.h
        src/Bg.cpp
        src/Bg.h
        src/Camera.cpp
        src/Camera.h
        src/core/Audio.h
        src/core/DeprecatedGameEnv.h
        src/core/GameMath.cpp
        src/core/GameMath.h
        src/core/game_enviroment/artist/Artist.h
        src/core/game_enviroment/artist/ArtistStructs.h
        src/core/game_enviroment/artist/SpriteFont.cpp
        src/core/game_enviroment/artist/SpriteFont.h
        src/core/game_enviroment/artist/TextureLoader.h
        src/core/game_enviroment/EntityContext.h
        src/core/game_enviroment/EntityPool.cpp
        src/core/game_enviroment/EntityPool.h
        src/core/game_enviroment/EntityPoolEventLoop.h
        src/core/game_enviroment/EntityV3.h
        src/core/game_enviroment/GameEnvironment.h
        src/core/game_enviroment/InputMgr.h
        src/core/game_enviroment/ISceneDirector.h
        src/core/game_enviroment/IStorableResource.h
        src/core/game_enviroment/ResourceStore.cpp
        src/core/game_enviroment/ResourceStore.h
        src/core/game_enviroment/Scene.h
        src/core/game_enviroment/SceneContext.h
        src/core/game_enviroment/SceneDirector.cpp
        src/core/game_enviroment/SceneDirector.h
        src/core/Geometry.h
        src/core/Screen.h
        src/core/_index.hpp
        src/entities/entity-types.hpp
        src/entities/Entity.cpp
        src/entities/Entity.h
        src/entities/general/Enemy.h
        src/entities/general/LayerSwitcher.h
        src/entities/general/Monitor.h
        src/entities/general/Ring.cpp
        src/entities/general/Ring.h
        src/entities/general/Sfx.h
        src/entities/general/Spikes.h
        src/entities/general/Spring.cpp
        src/entities/general/Spring.h
        src/entities/general/_index.hpp
        src/entities/Player.cpp
        src/entities/Player.h
        src/entities/sonic-1/enemies/bullet.cpp
        src/entities/sonic-1/enemies/buzz-bomber.cpp
        src/entities/sonic-1/enemies/chopper.cpp
        src/entities/sonic-1/enemies/crabmeat.cpp
        src/entities/sonic-1/enemies/EnemiesGHZ.h
        src/entities/sonic-1/enemies/motobug.cpp
        src/entities/sonic-1/enemies/_index.hpp
        src/entities/sonic-1/ghz/GimmicksGHZ.cpp
        src/entities/sonic-1/ghz/GimmicksGHZ.h
        src/entities/sonic-1/ghz/_index.hpp
        src/entities/sonic-1/object-ids-s1.hpp
        src/entities/sonic-1/_index.hpp
        src/entities/TitleMenuEntity.cpp
        src/entities/TitleMenuEntity.h
        src/entities/_index.hpp
        src/entity-creator.cpp
        src/entity-creator.hpp
        src/entity-placement.hpp
        src/entity-pool.cpp
        src/entity-pool.hpp
        src/FramesMappings.h
        src/game-loop-ticker.h
        src/Game.cpp
        src/Game.h
        src/GameSettings.h
        src/hex-angle.hpp
        src/level-loader.cpp
        src/level-loader.hpp
        src/Level.cpp
        src/Level.h
        src/LevelInformer.h
        src/main.cpp
        src/makeunique.hpp
        src/new-terrain.cpp
        src/new-terrain.hpp
        src/player-collider.hpp
        src/player-mover.hpp
        src/player-sensor.hpp
        src/player-state-base.hpp
        src/player-state-jump.hpp
        src/player-state-machine.hpp
        src/player-state-normal.hpp
        src/scenes/GameScene.cpp
        src/scenes/GameScene.h
        src/scenes/TitleScreenScene.cpp
        src/scenes/TitleScreenScene.h
        src/sfml_game_environment/SfmlArtist.cpp
        src/sfml_game_environment/SfmlArtist.h
        src/sfml_game_environment/SfmlGameEnviroment.cpp
        src/sfml_game_environment/SfmlGameEnvironment.h
        src/sfml_game_environment/SfmlTextureLoader.cpp
        src/sfml_game_environment/SfmlTextureLoader.h
        src/terrain-drawer.cpp
        src/terrain-drawer.hpp
        src/terrain-loader-sonic3.cpp
        src/terrain-loader.cpp
        src/terrain-loader.hpp
        src/terrain-sensor.hpp
        src/TitleScreen.h
        src/zone-enums-sonic1.hpp
        src/zone-enums-sonic2.hpp
        src/zone-enums-sonic3.hpp
        src/core/game_enviroment/dj/Dj.h
        src/core/game_enviroment/dj/AudioLoader.h
        src/sfml_game_environment/SfmlDj.cpp
        src/sfml_game_environment/SfmlDj.h
        src/sfml_game_environment/SfmlAudioLoader.cpp
        src/sfml_game_environment/SfmlAudioLoader.h
        src/sonic/SonicResources.h
)

target_include_directories(os1r PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_link_libraries(os1r PRIVATE sfml-graphics sfml-system sfml-audio)
target_compile_features(os1r PRIVATE cxx_std_11)

add_custom_command(
    TARGET os1r
    COMMENT "Copy content folder"
    PRE_BUILD COMMAND ${CMAKE_COMMAND} -E 
        copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/content $<TARGET_FILE_DIR:os1r>/content
    VERBATIM)

if(WIN32)
    add_custom_command(
        TARGET os1r
        COMMENT "Copy OpenAL DLL"
        PRE_BUILD COMMAND ${CMAKE_COMMAND} -E 
            copy ${SFML_SOURCE_DIR}/extlibs/bin/$<IF:$<EQUAL:${CMAKE_SIZEOF_VOID_P},8>,x64,x86>/openal32.dll $<TARGET_FILE_DIR:os1r>
        VERBATIM)
endif()

install(TARGETS os1r)