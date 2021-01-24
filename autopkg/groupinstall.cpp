#include <iostream>
#include <cstdio>
#include <unistd.h>
#include "groupinstall.h"
#include <curl/curl.h>

int autopkg_groupinstall(std::string groupname)
{
    std::string group_url;

    std::cout << "Running groupinstall for group '" + groupname + "'" << std::endl;

    group_url = std::string(AUTOPKG_PROTO) +
                std::string(AUTOPKG_SERVER) +
                std::string(AUTOPKG_PKGROOT) +
                std::string(groupname);

    std::cout << "Reading from " << group_url << " ... " << std::endl;

    return 0;

}
