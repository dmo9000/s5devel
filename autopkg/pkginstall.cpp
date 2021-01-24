#include <iostream>
#include <cstdio>
#include <unistd.h>
#include "pkginstall.h"
//#include <curl/curl.h>

int autopkg_pkginstall(std::string pkgname)
{
    std::string pkg_url;
    std::string pkg_dest;

    std::cout << "Running pkginstall for pkg '" + pkgname + "'" << std::endl;

    pkg_url = std::string(AUTOPKG_PROTO) +
              std::string(AUTOPKG_SERVER) +
              std::string(AUTOPKG_PKGROOT) +
              std::string(pkgname) + ".pkg";

    pkg_dest = std::string(AUTOPKG_SPOOL) + "/" + pkgname + ".pkg";

    std::cout << "Reading from " << pkg_url << " ... " << std::endl;
    std::cout << "Writing to " << pkg_dest << " ... " << std::endl;

    int s = http_download(pkg_url, pkg_dest);


    return 0;

}
