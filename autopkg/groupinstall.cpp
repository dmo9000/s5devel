#include <iostream>
#include <cstdio>
#include <unistd.h>
#include <curl/curl.h>

int autopkg_groupinstall(std::string groupname)
{
    std::cout << "Running groupinstall for group '" + groupname + "'" << std::endl;
    return 0;

}
