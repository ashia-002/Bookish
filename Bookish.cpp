#include <iostream>
#include <string>
#include <curl/curl.h>
#include <rapidjson/document.h>

using namespace std;
using namespace rapidjson;

string apiKey = "YOUR_API_KEY"; // Replace with your actual API key

// Define a callback function to handle the data received from the API
size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* output) {
    size_t totalSize = size * nmemb;
    output->append(static_cast<char*>(contents), totalSize);
    return totalSize;
}


// Function to make an API request using libcurl
string makeApiRequest(const string& url) {
    CURL* curl = curl_easy_init();
    if (curl) {
        std::string response;
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

        CURLcode res = curl_easy_perform(curl);

        if (res == CURLE_OK) {
            curl_easy_cleanup(curl);
            return response;
        } else {
            curl_easy_cleanup(curl);
            return ""; // Return empty string on error
        }
    }

    return ""; // Return empty string if curl couldn't be initialized
}

// Function to search for books using the API
void searchBooks(const string& query) {
    string apiUrl = "https://www.googleapis.com/books/v1/volumes?q=" + query + "&key=" + apiKey;
    string apiResponse = makeApiRequest(apiUrl);

    // Parse JSON response
    Document doc;
    doc.Parse(apiResponse.c_str());

    // Display book titles from the response
    if (doc.HasMember("items") && doc["items"].IsArray()) {
        const Value& items = doc["items"];
        for (SizeType i = 0; i < items.Size(); i++) {
            const Value& volumeInfo = items[i]["volumeInfo"];
            if (volumeInfo.HasMember("title") && volumeInfo["title"].IsString()) {
                cout << "Title: " << volumeInfo["title"].GetString() << endl;
            }
        }
    }
}

int main() {
    string searchQuery;

    cout << "Book Library App" << endl;
    cout << "Enter a search query: ";
    getline(cin, searchQuery);

    // Search for books using the API
    searchBooks(searchQuery);

    return 0;
}
