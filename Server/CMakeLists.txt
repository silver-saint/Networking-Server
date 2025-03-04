# CMakeList.txt : CMake project for Server, include source and define
# project specific logic here.
#

# Add source to this project's executable.
add_executable (Server "Server.cpp" "TCPServer/TCPServer.h" "App/App.h" "App/App.cpp" "TCPServer/TCPServer.cpp" "Types/types.h" "Client/Client.h" "Client/Client.cpp" "Networking/NetworkingTypes.h" "Networking/Socket.h")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Server PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
