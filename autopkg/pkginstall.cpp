#include <iostream>
#include <cstdio>
#include <unistd.h>
#include <iomanip>
#include <memory>
#include "pkginstall.h"
#include "subprocess.h"

extern int debug_mode;

int autopkg_pkginstall(std::string pkgname)
{
    std::string pkg_url;
    std::string pkg_dest;
    std::string pkg_decomp;

    if (debug_mode) {
        std::cout << "Running pkginstall for pkg '" + pkgname + "'" << std::endl;
    }
    std::string zpkgname = std::string(pkgname) + ".pkg.gz"; 

    pkg_url = std::string(AUTOPKG_PROTO) +
              std::string(AUTOPKG_SERVER) +
              std::string(AUTOPKG_PKGROOT) +
	      zpkgname; 

    std::string pkgmsg = "Installing package " + zpkgname + " ... ";

    std::cout << std::left << std::setw(60) << pkgmsg;

    pkg_dest = std::string(AUTOPKG_SPOOL) + "/" + pkgname + ".pkg.gz";
    pkg_decomp = std::string(AUTOPKG_SPOOL) + "/" + pkgname + ".pkg";

    /*
    std::cout << " - Reading from " << pkg_url << " ... " << std::endl;
    std::cout << " - Writing to " << pkg_dest << " ... " << std::endl;
    */

    int s = http_download(pkg_url, pkg_dest);

    if (s != 0) {
        std::cerr << "Couldn't retrieve file at " << pkg_url << std::endl;
        return 1;
    }

    std::shared_ptr<Subprocess> sp_gunzip = std::make_shared<Subprocess>();
    sp_gunzip->debug = debug_mode;

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

    //std::cout << "child status = " << child_status << std::endl;

    std::shared_ptr<Subprocess> sp_pkgadd = std::make_shared<Subprocess>();
    sp_pkgadd->debug = debug_mode;

    char *argv_pkgadd[] = { const_cast<char *>("/usr/5bin/pkgadd"),
                            const_cast<char *>("-d"),
                            const_cast<char *>(pkg_decomp.c_str()),
                            NULL
                          };
    sp_pkgadd->SetCommand("/usr/5bin/pkgadd", 2, argv_pkgadd );
//    sp_pkgadd->WriteStdin("1\ny\n", 4);
    sp_pkgadd->StartProcess();

    while (!sp_pkgadd->StderrContains(const_cast<char *>("(default: all)"))) {
        sp_pkgadd->BufferStderr();
    }

    if (sp_pkgadd->debug) {
        sp_pkgadd->DumpStderr(stdout);
        std::cout << "+++ Found package id prompt" << std::endl;
        std::cout << std::endl;
    }

    sp_pkgadd->ClearStderr();

    /* first response received from pkgadd, select package 1 */

    if (!sp_pkgadd->WriteStdin(const_cast<char *>("1\n"), 2)) {
        std::cerr << "pkgadd: error selecting package from list." << std::endl;
        assert(NULL);
    }

    while (!(sp_pkgadd->StderrContains(const_cast<char *>("Installation of <")) &&
             (sp_pkgadd->StderrContains(const_cast<char *>("was successful.\n"))))) {
        /* answer yes to all prompts - make this a bit more intelligent later on */
        if (!sp_pkgadd->WriteStdin(const_cast<char *> ("y\n"), 2)) {
            //std::cerr << "warning: error sending y to pkgadd" << std::endl;
        }

        sp_pkgadd->BufferStderr();
    }

    if (sp_pkgadd->debug) {
        sp_pkgadd->DumpStderr(stdout);
    }
    //std::cout << std::endl;
    if (sp_pkgadd->debug) {
        std::cout << "+++ Found end of installation marker [success]" << std::endl;
    }
    sp_pkgadd->ClearStderr();

    child_status = sp_pkgadd->Wait();
    if (child_status == 0) {
        std::cout << GRN << "[OK] " \
		<< RST << std::endl;
    } else {
        std::cout << RED << "[FAIL:" \
		<< child_status << "]" << RST << std::endl;
    }

    return 0;
}
