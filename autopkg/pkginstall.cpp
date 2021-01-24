#include <iostream>
#include <cstdio>
#include <unistd.h>
#include <memory>
#include "pkginstall.h"
#include "subprocess.h"

int autopkg_pkginstall(std::string pkgname)
{
    std::string pkg_url;
    std::string pkg_dest;
    std::string pkg_decomp;

    std::cout << "Running pkginstall for pkg '" + pkgname + "'" << std::endl;

    pkg_url = std::string(AUTOPKG_PROTO) +
              std::string(AUTOPKG_SERVER) +
              std::string(AUTOPKG_PKGROOT) +
              std::string(pkgname) + ".pkg.gz";

    pkg_dest = std::string(AUTOPKG_SPOOL) + "/" + pkgname + ".pkg.gz";
    pkg_decomp = std::string(AUTOPKG_SPOOL) + "/" + pkgname + ".pkg";

    std::cout << " - Reading from " << pkg_url << " ... " << std::endl;
    std::cout << " - Writing to " << pkg_dest << " ... " << std::endl;

    int s = http_download(pkg_url, pkg_dest);

    if (s != 0) {
        std::cerr << "Couldn't retrieve file at " << pkg_url << std::endl;
        return 1;
    }

    std::shared_ptr<Subprocess> sp_gunzip = std::make_shared<Subprocess>();
    std::shared_ptr<Subprocess> sp_pkgadd = std::make_shared<Subprocess>();

    char *argv_gunzip[] = { const_cast<char *>("/usr/bin/gunzip"),
                            const_cast<char *>("-f"),
                            const_cast<char *>(pkg_dest.c_str()),
                            NULL
                          };
    sp_gunzip->SetCommand("/usr/bin/gunzip", 2, argv_gunzip);
    sp_gunzip->StartProcess();

    int child_status = 0;
    child_status = sp_gunzip->Wait();

    if (child_status != 0) {
        std::cerr << "+++ error unzipping package " << pkg_dest << std::endl;
        std::cerr << "+++ gunzip exit code is " << child_status << std::endl;
        return 1;
    }

    std::cout << "child status = " << child_status << std::endl;

    char *argv_pkgadd[] = { const_cast<char *>("/usr/5bin/pkgadd"),
                            const_cast<char *>("-d"),
                            const_cast<char *>(pkg_decomp.c_str()),
                            NULL
                          };
    sp_pkgadd->SetCommand("/usr/5bin/pkgadd", 2, argv_pkgadd );
    sp_pkgadd->WriteStdin("1\ny\n", 4);
    sp_pkgadd->StartProcess();

    char buffer[8192];
    int rd = -1;
    while (rd <= 0) {
        int rd = sp_pkgadd->ReadStderr((char *) &buffer, 8192);
        if (rd > 0) {
            std::cout << rd << " bytes read!" << std::endl;
        }
    }
    child_status = sp_pkgadd->Wait();
    std::cout << "pkgadd exit code = " << child_status << std::endl;

    return 0;
}