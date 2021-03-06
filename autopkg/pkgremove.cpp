#include <iostream>
#include <cstdio>
#include <unistd.h>
#include <memory>
#include "pkgremove.h"
#include "subprocess.h"

extern int debug_mode;

int autopkg_pkgremove(std::string pkgname)
{
    std::string pkg_url;
    std::string pkg_dest;
    std::string pkg_decomp;

    if (debug_mode) {
        std::cout << "Running pkgremove for pkg '" + pkgname + "'" << std::endl;
    }

    std::shared_ptr<Subprocess> sp_pkgrm = std::make_shared<Subprocess>();
    sp_pkgrm->debug = debug_mode;

    char *argv_pkgrm[] = { const_cast<char *>("/usr/5bin/pkgrm"),
                           const_cast<char *>(pkgname.c_str()),
                           NULL
                         };
    sp_pkgrm->SetCommand("/usr/5bin/pkgrm", 1, argv_pkgrm );
    sp_pkgrm->StartProcess();

    while (!sp_pkgrm->StderrContains(const_cast<char *>("remove this package? [y,n,?,q]"))) {
        sp_pkgrm->BufferStderr();
        if (sp_pkgrm->StderrContains(const_cast<char *>("pkgrm: ERROR: no package associated with <"))) {
            std::cerr << "+++ Package named '" << pkgname << "' is not installed." << std::endl;
            int child_status = sp_pkgrm->Wait();
            //std::cout << "pkgrm exit code = " << child_status << std::endl;
            return 1;
        }
    }

    if (sp_pkgrm->debug) {
        sp_pkgrm->DumpStderr(stdout);
        std::cout << std::endl;
        std::cout << std::endl;
        std::cout << "+++ Found package removal prompt" << std::endl;
    }
    sp_pkgrm->ClearStderr();

    /* first response received from pkgadd, select package 1 */

    if (!sp_pkgrm->WriteStdin(const_cast<char *>("y\n"), 2)) {
        std::cerr << "Error confirming package removal." << std::endl;
        assert(NULL);
    }

    while (!(sp_pkgrm->StderrContains(const_cast<char *>("Removal of <")) &&
             (sp_pkgrm->StderrContains(const_cast<char *>("> was successful.\n"))))) {
        /* answer yes to all prompts - make this a bit more intelligent later on */
        sp_pkgrm->BufferStderr();
    }

    if (sp_pkgrm->debug) {
        sp_pkgrm->DumpStderr(stdout);
        std::cout << std::endl;
        std::cout << "+++ Found end of removal marker [success]" << std::endl;
    }
    sp_pkgrm->ClearStderr();

    int child_status = sp_pkgrm->Wait();
    //std::cout << "pkgrm exit code = " << child_status << std::endl;
    if (child_status == 0) {
        std::cout << "Removal of package <" << pkgname << "> [OKAY]" << std::endl;
    } else {
        std::cout << "Removal of package <" << pkgname << "> [FAIL] pkgrm exit status = " << child_status << std::endl;
    }

    return 0;
}
