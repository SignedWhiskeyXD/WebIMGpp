//
// Created by wsmrxd on 2023/7/9.
//

#include "ServletMatcher.h"
#include <stack>

bool ServletMatcher::addRule(std::string_view uri, ServletPtr servlet) {
    if(matchTable.contains(uri.data())) return false;
    matchTable[uri.data()] = std::move(servlet);
    return true;
}

ServletPtr ServletMatcher::match(HTTPRequest &request) {
    using iter = decltype(std::string::iterator());
    std::stack<iter> candidates;
    for(auto i = request.uri.begin(); i != request.uri.end(); ++i){
        if(*i == '/') candidates.push(i);
    }
    auto query = request.uri.find('?');
    if(query != std::string::npos)
        candidates.push(request.uri.begin() + query);
    candidates.push(request.uri.end());

    while(!candidates.empty()){
        std::string candidate(request.uri.begin(), candidates.top());
        if(matchTable.contains(candidate)) return matchTable[candidate];
        candidates.pop();
    }
    return matchTable["/"];
}
