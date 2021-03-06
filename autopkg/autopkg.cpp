#include <iostream>
#include <cstdio>
#include <unistd.h>
#include "groupinstall.h"
#include "pkginstall.h"
#include "pkgremove.h"


int debug_mode = 0;


void usage()
{

    std::cerr << "Usage message goes here." << std::endl;
    return;

}


int main(int argc, char *argv[])
{
    std::string groupname, packagename;
    int c = 0;
    enum MODE {
        groupinstall,
        install,
        list,
        none,
        remove
    } AutoPkgMode = none;

    /* must be root */

    if (geteuid() != 0) {
        std::cerr << "Not effectively running as root." << std::endl;
        return 255;
    }

    /* check command line options */

    while ((c = getopt(argc, argv, ":di:g:hr:")) != -1) {
        switch(c) {
        case '?':
            std::cerr << "unknown option " << optopt << std::endl;
            break;
        case ':':
            std::cerr << "option " << optopt << "requires a value " << std::endl;
            break;
        case 'h':
            usage();
            return 1;
            break;
        case 'd':
            debug_mode = 1;
            break;
        case 'i':
            if (AutoPkgMode != none) {
                std::cerr << "Only one mode can be specified." << std::endl;
                return 254;
            } else {
                AutoPkgMode = install;
                packagename = optarg;
            }
            break;
        case 'g':
            if (AutoPkgMode != none) {
                std::cerr << "Only one mode can be specified." << std::endl;
                return 254;
            } else {
                AutoPkgMode = groupinstall;
                groupname = optarg;
            }
            break;
        case 'r':
            if (AutoPkgMode != none) {
                std::cerr << "Only one mode can be specified." << std::endl;
                return 254;
            } else {
                AutoPkgMode = remove;
                packagename = optarg;
            }
            break;

        default:
            std::cerr << "Unknown option '" << c << "'" << std::endl;
            return 1;
        }
    }


    switch(AutoPkgMode) {
    case none:
        std::cerr << "No operation was specified." << std::endl;
        return 1;
        break;
    case groupinstall:
        autopkg_groupinstall(groupname);
        break;
    case install:
        autopkg_pkginstall(packagename);
        break;
    case remove:
        autopkg_pkgremove(packagename);
        break;
    default:
        std::cerr << "Unknown operation:" << AutoPkgMode << std::endl;
        return 1;
        break;
    }


    return 0;

}
