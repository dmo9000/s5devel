#pragma once
#include <iostream>

#define AUTOPKG_PROTO   "http://"
#define AUTOPKG_SERVER	"192.168.20.50"
#define AUTOPKG_PKGROOT	"/s5linux/"
#define AUTOPKG_TMPDATA	"/var/spool/pkg"

int autopkg_groupinstall(std::string groupname);