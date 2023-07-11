#include <iostream>
#include "ImageServer.h"
#include "Servlet/HelloServlet.h"
#include "Servlet/Register.h"
#include "ServletMatcher.h"

int main()
{
    ServletPtr hello = std::make_shared<HelloServlet>();
    ServletPtr regi = std::make_shared<Register>();
    ServletMatcher::getInstance().addRule("/hello", hello);
    ServletMatcher::getInstance().addRule("/register", regi);

    ImageServer server("127.0.0.1", 52480);
    server.run();

    return 0;
}
