cmake_minimum_required(VERSION 3.12)

set(EXECUTABLE_NAME "Network_game")

project(Network_game)

set(CMAKE_CXX_STANDARD 14)

add_executable(Server Server/Server.cpp Server/ClientHandler.cpp Server/GameObject.cpp Server/World.cpp Server/Main_Server.cpp)
add_executable(Client Client/Client.cpp Client/Game.cpp Client/GraphObject.cpp Client/KeyboardInterface.cpp Client/Main_Client.cpp)
add_executable(main Client/main.cpp Client/engine.cpp Client/engine.h Client/Player.h Client/Player.cpp )

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake_modules")

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/Modules ${CMAKE_MODULE_PATH})

find_package(SFML REQUIRED system window graphics network audio)

find_package(X11 REQUIRED)

if(SFML_FOUND)

    include_directories(${SFML_INCLUDE_DIR})

    target_link_libraries(main ${SFML_LIBRARIES})
    target_link_libraries(Server ${SFML_LIBRARIES})
    target_link_libraries(Client ${SFML_LIBRARIES} ${X11_LIBRARIES})

endif()