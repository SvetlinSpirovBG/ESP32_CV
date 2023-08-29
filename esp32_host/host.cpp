#include <stdio.h>
#include <curl/curl.h>
#include <iostream>
#include <chrono>
#include <thread>

using namespace std;

size_t callbackfunction(void *ptr, size_t size, size_t nmemb, void* userdata)
{
    printf("callback function doing nothing\n");

    FILE* stream = (FILE*)userdata;
    if (!stream)
    {
        printf("!!! No stream\n");
        return 0;
    }

    size_t written = fwrite((FILE*)ptr, size, nmemb, stream);
    return written;

    return 0;
}

bool download_jpeg(const char* url)
{
    uint8_t numFiles = 10;
    printf("Downloading %u files from %s\n", numFiles, url);

    //Start sleep to allow Svetlin to prepare the stopwatch
    std::this_thread::sleep_for(2000ms);

    CURL* curlCtx = curl_easy_init();
    printf("curl_easy_init\n");

    CURLcode rc = curl_easy_setopt(curlCtx, CURLOPT_URL, url);
    printf("curl_easy_init 2 rc %d\n", rc);

    for (int i = 0; i < numFiles; i++)
    {
        char file_name[256];
        snprintf(file_name, 256, "out_%u.jpg", i);
        FILE* fp = fopen(file_name, "wb");
        if (!fp)
        {
            printf("!!! Failed to create file on the disk\n");
            return false;
        }

        rc = curl_easy_setopt(curlCtx, CURLOPT_WRITEDATA, fp);
        printf("curl_easy_init 3 rc %d\n", rc);

        rc = curl_easy_setopt(curlCtx, CURLOPT_WRITEFUNCTION, callbackfunction);
        printf("curl_easy_init 4 rc %d\n", rc);

        // rc = curl_easy_setopt(curlCtx, CURLOPT_FOLLOWLOCATION, 1);
        // printf("curl_easy_init 5 rc %d\n", rc);

        // rc = curl_easy_setopt(curlCtx, CURLOPT_TIMEOUT, 5);
        // printf("curl_easy_init 6 rc %d\n", rc);

        rc = curl_easy_perform(curlCtx);
        if (rc)
        {
            printf("!!! Failed to download: %s\n", url);
            return false;
        }

        int res_code = 0;
        curl_easy_getinfo(curlCtx, CURLINFO_RESPONSE_CODE, &res_code);
        if (!((res_code == 200 || res_code == 201) && rc != CURLE_ABORTED_BY_CALLBACK))
        {
            printf("!!! Response code: %d\n", res_code);
            return false;
        }

        fclose(fp);
    }

    curl_easy_cleanup(curlCtx);

    return true;
}

int main (void)
{
    std::string url = "http://192.168.0.104/capture";
    download_jpeg(url.c_str());
}
