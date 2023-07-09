//
// Created by wsmrxd on 2023/7/9.
//

#ifndef WEBIMG_SERVLETMATCHER_H
#define WEBIMG_SERVLETMATCHER_H

#include <unordered_map>
#include "IMGServlet.h"

class ServletMatcher{
public:

    static ServletMatcher& getInstance(){
        static ServletMatcher instance;
        return instance;
    }

    ServletPtr match(HTTPRequest& request);

    bool addRule(std::string_view uri, ServletPtr servlet);

private:
    std::unordered_map<std::string, ServletPtr> matchTable;
};

#endif //WEBIMG_SERVLETMATCHER_H
