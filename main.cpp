#include <iostream>
#include "ImageServer.h"
#include "HelloServlet.h"
#include "ServletMatcher.h"

int main()
{
    ServletPtr hello = std::make_shared<HelloServlet>();
    ServletMatcher::getInstance().addRule("/hello", hello);

    ImageServer server("127.0.0.1", 52480);
    server.run();

    return 0;
}
