#include <iostream>
#include <string>
#include <vector>
#include <regex>
#include <curl/curl.h>

size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp)
{
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

std::string downloadPage(const std::string& url)
{
    CURL* curl = curl_easy_init();
    std::string response;

    if (curl)
    {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
        curl_easy_setopt(curl, CURLOPT_USERAGENT, "Mozilla/5.0");

        CURLcode res = curl_easy_perform(curl);

        if (res != CURLE_OK)
        {
            std::cerr << "curl error: " << curl_easy_strerror(res) << std::endl;
        }

        curl_easy_cleanup(curl);
    }

    return response;
}

std::vector<std::string> extractLinks(const std::string& html)
{
    std::vector<std::string> links;

    std::regex linkRegex(R"(https?://[^\s"'<>]+)");

    for (std::sregex_iterator i(html.begin(), html.end(), linkRegex), end; i != end; ++i)
    {
        links.push_back(i->str());
    }

    return links;
}

int main()
{
    std::string query = "porn+games";

    std::string searchUrl = "https://www.google.com/search?q=" + query;

    std::string html = downloadPage(searchUrl);

    auto links = extractLinks(html);

    for (const auto& link : links)
    {
        std::cout << link << std::endl;
    }

    return 0;
}