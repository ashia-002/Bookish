#include <iostream>
#include <string>
#include <vector>
#include <curl/curl.h>
#include <rapidjson/document.h>

using namespace std;
using namespace rapidjson;

string apiKey = "YOUR_API_KEY"; // Replace with your actual API key

// Define the BookInfo class
class BookInfo
{
public:
    string title;
    string subtitle;
    vector<string> authors;
    string publisher;
    string publishedDate;
    string description;
    int pageCount;
    string thumbnail;
    string previewLink;
    string infoLink;
    string buyLink;

    // Constructor for BookInfo class
    BookInfo(string _title, string _subtitle, const vector<string> &_authors, string _publisher,
             string _publishedDate, string _description, int _pageCount, string _thumbnail,
             string _previewLink, string _infoLink, string _buyLink) : title(_title), subtitle(_subtitle), authors(_authors), publisher(_publisher),
                                                                       publishedDate(_publishedDate), description(_description), pageCount(_pageCount),
                                                                       thumbnail(_thumbnail), previewLink(_previewLink), infoLink(_infoLink), buyLink(_buyLink) {}
};

// Define a callback function to handle the data received from the API
size_t WriteCallback(void *contents, size_t size, size_t nmemb, string *output)
{
    size_t totalSize = size * nmemb;
    output->append(static_cast<char *>(contents), totalSize);
    return totalSize;
}

// Function to make an API request using libcurl
string makeApiRequest(const string &url)
{
    // Initialize a libcurl easy handle
    CURL *curl = curl_easy_init();

    // Check if curl was initialized successfully
    if (curl)
    {
        string response;

        // Set the URL for the HTTP request
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

        // Set the callback function for writing data received from the request
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);

        // Set the user-defined pointer to store the response data
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

        // Perform the HTTP request and receive the response
        CURLcode res = curl_easy_perform(curl);

        // Check if the request was successful (CURLE_OK)
        if (res == CURLE_OK)
        {
            // Cleanup and close the libcurl handle
            curl_easy_cleanup(curl);

            // Return the response data received from the API as a string
            return response;
        }
        else
        {
            // Cleanup and close the libcurl handle on error
            curl_easy_cleanup(curl);

            // Return an empty string to indicate an error occurred
            return "\nError! Request wasn't successful.";
        }
    }

    // Return an empty string if libcurl couldn't be initialized
    return "\n Error! We can't request Data";
}

// Function to search for books using the API
void searchBooks(const string &query)
{
    string apiUrl = "https://www.googleapis.com/books/v1/volumes?q=" + query + "&key=" + apiKey;
    string apiResponse = makeApiRequest(apiUrl);

    // Parse JSON response
    Document doc;
    doc.Parse(apiResponse.c_str());

    // Create a vector to store BookInfo objects
    vector<BookInfo> books;

    // Loop through the "items" array
    if (doc.HasMember("items") && doc["items"].IsArray())
    {
        const Value &items = doc["items"];
        for (SizeType i = 0; i < items.Size(); i++)
        {
            const Value &volumeInfo = items[i]["volumeInfo"];
            if (volumeInfo.HasMember("title") && volumeInfo["title"].IsString())
            {
                string title = volumeInfo["title"].GetString();
                string subtitle = ""; // Initialize with a default value

                if (volumeInfo.HasMember("subtitle") && volumeInfo["subtitle"].IsString())
                {
                    subtitle = volumeInfo["subtitle"].GetString();
                }

                // Handling the "authors" field
                vector<string> authors;
                if (volumeInfo.HasMember("authors") && volumeInfo["authors"].IsArray())
                {
                    const Value &authorsArray = volumeInfo["authors"];
                    for (SizeType j = 0; j < authorsArray.Size(); j++)
                    {
                        if (authorsArray[j].IsString())
                        {
                            authors.push_back(authorsArray[j].GetString());
                        }
                    }
                }

                // Handling the "publisher" field
                string publisher = "";
                if (volumeInfo.HasMember("publisher") && volumeInfo["publisher"].IsString())
                {
                    publisher = volumeInfo["publisher"].GetString();
                }

                // Handling the "publishedDate" field
                string publishedDate = "";
                if (volumeInfo.HasMember("publishedDate") && volumeInfo["publishedDate"].IsString())
                {
                    publishedDate = volumeInfo["publishedDate"].GetString();
                }

                // Handling the "description" field
                string description = "";
                if (volumeInfo.HasMember("description") && volumeInfo["description"].IsString())
                {
                    description = volumeInfo["description"].GetString();
                }

                // Handling the "pageCount" field
                int pageCount = 0;
                if (volumeInfo.HasMember("pageCount") && volumeInfo["pageCount"].IsInt())
                {
                    pageCount = volumeInfo["pageCount"].GetInt();
                }

                // Handling the "thumbnail" field
                string thumbnail = "";
                if (volumeInfo.HasMember("imageLinks") && volumeInfo["imageLinks"].IsObject())
                {
                    const Value &imageLinks = volumeInfo["imageLinks"];
                    if (imageLinks.HasMember("thumbnail") && imageLinks["thumbnail"].IsString())
                    {
                        thumbnail = imageLinks["thumbnail"].GetString();
                    }
                }

                // Handling the "previewLink" field
                string previewLink = "";
                if (volumeInfo.HasMember("previewLink") && volumeInfo["previewLink"].IsString())
                {
                    previewLink = volumeInfo["previewLink"].GetString();
                }

                // Handling the "infoLink" field
                string infoLink = "";
                if (volumeInfo.HasMember("infoLink") && volumeInfo["infoLink"].IsString())
                {
                    infoLink = volumeInfo["infoLink"].GetString();
                }

                // Handling the "buyLink" field
                string buyLink = "";
                if (volumeInfo.HasMember("buyLink") && volumeInfo["buyLink"].IsString())
                {
                    buyLink = volumeInfo["buyLink"].GetString();
                }

                // Create a BookInfo object and store data
                BookInfo book(title, subtitle, authors, publisher, publishedDate,
                              description, pageCount, thumbnail, previewLink,
                              infoLink, buyLink);

                // Add the book object to the vector
                books.push_back(book);
            }
        }
    }

    // Display book details from the BookInfo vector
    for (const BookInfo &book : books)
    {
        cout << "Title: " << book.title << endl;

        if (!book.subtitle.empty())
        {
            cout << "Subtitle: " << book.subtitle << endl;
        }
        else
        {
            cout << "Subtitle: Info not found" << endl;
        }

        cout << "Authors: ";
        if (!book.authors.empty())
        {
            for (const string &author : book.authors)
            {
                cout << author << ", ";
            }
            cout << endl;
        }
        else
        {
            cout << "Info not found" << endl;
        }

        cout << "Publisher: ";
        if (!book.publisher.empty())
        {
            cout << book.publisher << endl;
        }
        else
        {
            cout << "Info not found" << endl;
        }

        cout << "Published Date: ";
        if (!book.publishedDate.empty())
        {
            cout << book.publishedDate << endl;
        }
        else
        {
            cout << "Info not found" << endl;
        }

        cout << "Description: ";
        if (!book.description.empty())
        {
            cout << book.description << endl;
        }
        else
        {
            cout << "Info not found" << endl;
        }

        cout << "Page Count: ";
        if (book.pageCount > 0)
        {
            cout << book.pageCount << endl;
        }
        else
        {
            cout << "Info not found" << endl;
        }

        cout << "Thumbnail: ";
        if (!book.thumbnail.empty())
        {
            cout << book.thumbnail << endl;
        }
        else
        {
            cout << "Info not found" << endl;
        }

        cout << "Preview Link: ";
        if (!book.previewLink.empty())
        {
            cout << book.previewLink << endl;
        }
        else
        {
            cout << "Info not found" << endl;
        }

        cout << "Info Link: ";
        if (!book.infoLink.empty())
        {
            cout << book.infoLink << endl;
        }
        else
        {
            cout << "Info not found" << endl;
        }

        cout << "Buy Link: ";
        if (!book.buyLink.empty())
        {
            cout << book.buyLink << endl;
        }
        else
        {
            cout << "Info not found" << endl;
        }

        // ... Continue to print other fields of BookInfo ...
    }
}

int main()
{
    string searchQuery;

    cout << "Book Library App" << endl;
    cout << "Enter a search query: ";
    getline(cin, searchQuery);

    // Search for books using the API
    searchBooks(searchQuery);

    return 0;
}
