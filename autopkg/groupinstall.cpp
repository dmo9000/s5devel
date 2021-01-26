#include <iostream>
#include <cstdio>
#include <fstream>
#include <sstream>
#include <string>
#include <unistd.h>
#include <curl/curl.h>
#include "groupinstall.h"

extern int debug_mode;

int autopkg_groupinstall(std::string groupname)
{
    std::string group_file;
    std::string group_url;
    std::string local_group_file;

    std::cout << "Running groupinstall for group '" + groupname + "'" << std::endl;

    group_file = std::string(groupname) +
		std::string(".pkgs");

    group_url = std::string(AUTOPKG_PROTO) +
                std::string(AUTOPKG_SERVER) +
                std::string(AUTOPKG_PKGROOT) +
		std::string(group_file);

    local_group_file = std::string(AUTOPKG_GROUPDATA) + std::string(group_file); 

    std::cout << "Reading grouplist from " << group_url << " ... " << std::endl;

    int s = http_download(group_url, local_group_file);
    if (s != 0) {
	std::cerr << "+++ couldn't retrieve groupinstall file '" << groupname << '"' << std::endl;
	return 1;	
    	}
    
    std::ifstream infile(local_group_file); 
    std::string pkgname;
    while (std::getline(infile, pkgname)) {
	std::istringstream iss(pkgname);
	//std::cout << "Package: " << pkgname << std::endl;
	autopkg_pkginstall(pkgname);
    }
    
    return 0;

}
