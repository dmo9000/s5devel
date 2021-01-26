#pragma once
#include <iostream>
#include "httpdriver.h"
#include "string.h"

#define AUTOPKG_PROTO   "http://"
#define AUTOPKG_SERVER	"192.168.20.50"
#define AUTOPKG_PKGROOT	"/s5linux/"
#define AUTOPKG_SPOOL	"/var/spool/pkg"

int autopkg_pkgremove(std::string pkgname);
