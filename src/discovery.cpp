
#include <Server.h>
#include <Client.h>
#include <thread>

int main(int argc, char * argv[]) {
  Server server("239.1.2.3", 9876);

  std::thread t1(&Server::mainThread, &server);

  t1.join();
}
