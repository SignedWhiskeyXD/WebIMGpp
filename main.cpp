#include <iostream>
#include "ImageServer.h"

int main()
{
    ImageServer server("127.0.0.1", 52480);
    server.run();

    return 0;
}
