#pragma once
#include <iostream>
#include "httpdriver.h"
#include "pkginstall.h"

#define AUTOPKG_PROTO   	"http://"
#define AUTOPKG_SERVER		"192.168.20.50"
#define AUTOPKG_PKGROOT		"/s5linux/"
#define AUTOPKG_TMPDATA		"/var/spool/pkg"
#define AUTOPKG_GROUPDATA	"/var/spool/pkg/_autopkg"


int autopkg_groupinstall(std::string groupname);
