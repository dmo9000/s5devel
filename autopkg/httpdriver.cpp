#include <iostream>
#include <cstdio>
#include <unistd.h>
#include <curl/curl.h>
#include "httpdriver.h"

static size_t write_data(void *ptr, size_t size, size_t nmemb, void *stream)
{
    size_t written = fwrite(ptr, size, nmemb, (FILE*) stream);
    //std::cout << "write_data(...)" << std::endl;
    return size * nmemb;
}

int http_download(std::string source, std::string dest)
{

    FILE *spoolfile;
    CURL *curl;
    CURLU *urlp;
    CURLcode res;
    CURLUcode uc;
    std::string readBuffer;

    std::cout << "http_download(" + source + ", " + dest + ");" << std::endl;

    //result = curl_url_set(curlu, CURLUPART_URL, source.c_str(), 0);

    curl_global_init(CURL_GLOBAL_DEFAULT);

    curl = curl_easy_init();
    if (!curl) {
        return 1;
    }

    spoolfile = fopen(dest.c_str(), "wb");
    if (!spoolfile) {
        std::cerr << "Couldn't open destination file " << dest << std::endl;
        return 1;
    }

    urlp = curl_url();
    uc = curl_url_set(urlp, CURLUPART_URL,  "http://192.168.20.50/s5linux/S5LXcurl.pkg.gz", 0);

    if (uc) {
        std::cerr << "curl_set_url failed" << std::endl;
        return 1;
    }

    if(curl) {
        curl_easy_setopt(curl, CURLOPT_CURLU, urlp);
        //curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, spoolfile);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
        res = curl_easy_perform(curl);
        if (res != CURLE_OK)  {
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
            return 1;
        }
    }

    fclose(spoolfile);

    curl_url_cleanup(urlp);
    curl_easy_cleanup(curl);

    return 0;

}
