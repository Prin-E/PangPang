

SET(PANGPANG_MAIN_SRCS AngleView.cpp Button.cpp Common.cpp GameApplication.cpp Label.cpp LocalPlayer.cpp Map.cpp Renderer.cpp Stage.cpp Tank.cpp UIElement.cpp WaitingRoom.cpp Bar.cpp Collidable.cpp Event.cpp ImageView.cpp Line.cpp MainMenu.cpp Player.cpp Sprite.cpp State.cpp Texture.cpp UIManager.cpp Weapon.cpp InGameState.cpp AIPlayer.cpp main.cpp posix/ResourcePosix.cpp sdl/WindowSDL.cpp)

ADD_LIBRARY(pangpang_main ${PANGPANG_MAIN_SRCS})


