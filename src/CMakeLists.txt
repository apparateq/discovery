include_directories(.)

link_libraries(pthread)

# discovery (server)
set(discovery_src discovery.cpp Device.cpp McReceive.cpp McSend.cpp Server.cpp Client.cpp)
set(discovery_inc Device.h McReceive.h McSend.h Server.h)
add_executable(server ${discovery_src} ${discovery_inc})


#
# Build client
add_executable(client main.cpp Client.cpp McReceive.cpp McReceive.h Device.cpp Device.h)

install(TARGETS server client DESTINATION bin)
