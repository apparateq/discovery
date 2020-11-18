
#include <Client.h>
#include <unistd.h>

int main(int argc, char * argv[]) {
  srand(getpid());
  Client client("239.1.2.3", 9876);

  client.mainThread();

}
