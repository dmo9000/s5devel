#include <iostream>
#include <cstdio>
#include <unistd.h>
#include <curl/curl.h>
#include "httpdriver.h"

static size_t write_data(void *ptr, size_t size, size_t nmemb, void *stream)
{
    size_t written = fwrite(ptr, size, nmemb, (FILE*) stream);
    return size * nmemb;
}

int http_download(std::string source, std::string dest)
{

    FILE *spoolfile;
    CURL *curl;
    CURLcode res;
    std::string readBuffer;

    std::cout << "http_download(" + source + ", " + dest + ");" << std::endl;

    curl = curl_easy_init();
    if (!curl) {
        return 1;
    }

    spoolfile = fopen(dest.c_str(), "wb");
    if (!spoolfile) {
        std::cerr << "Couldn't open destination file " << dest << std::endl;
        return 1;
    }

    std::cout << "curl_easy_init() done" << std::endl;
    curl_easy_setopt(curl, CURLOPT_CURLU, source);
    curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, spoolfile);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
    std::cout << "curl_easy_setopt() done" << std::endl;
    res = curl_easy_perform(curl);
    std::cout << "curl_easy_perform() done" << std::endl;
    curl_easy_cleanup(curl);
    std::cout << "curl_easy_cleanup() done" << std::endl;

    fclose(spoolfile);

    return 0;

}
