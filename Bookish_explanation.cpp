/*Function to make API RESPONSE
Now, let's explain each step:

CURL* curl = curl_easy_init();: This line initializes a libcurl easy handle (CURL*) using curl_easy_init(). 
This handle will be used to configure and execute the HTTP request.

if (curl) { ... }: 
This conditional statement checks whether the libcurl handle was successfully initialized. 
If curl is not nullptr, it means the initialization was successful, and the code inside the block will execute.

std::string response;:
This line declares a string variable named response, which will be used to store the response data
received from the API.

curl_easy_setopt(curl, CURLOPT_URL, url.c_str());: 
Here, we set the URL for the HTTP request using curl_easy_setopt. 
The url parameter passed to the function is the API endpoint we want to request.

curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);: 
This line sets the callback function WriteCallback to handle the data received from the API 
response. The function will be responsible for writing the received data into the response string.

curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);: 
This line sets the user-defined pointer to the response string. It tells libcurl where to store the received data.

CURLcode res = curl_easy_perform(curl);: 
This line executes the HTTP request using curl_easy_perform. It performs the request and stores
the result (success or failure) in the res variable.

if (res == CURLE_OK) { ... } else { ... }: 
Here, we check the result of the request. If res is equal to CURLE_OK, 
it means the request was successful, and the code inside the first block will execute. 
Otherwise, if there was an error, the code inside the else block will execute.

In case of success, the libcurl handle is cleaned up using curl_easy_cleanup(curl);, 
and the response data stored in the response string is returned.

In case of an error, the libcurl handle is still cleaned up, 
but an empty string is returned to indicate that an error occurred.

The function effectively sends an HTTP request to the specified URL, 
receives the API's response, and returns that response as a string. The API response 
typically contains data in JSON format, which can be parsed to extract the desired information.*/




/* WriteCallback
is used as the callback function to handle the data received from the API response.
However, the definition of this function is missing from the code snippet you provided.


Certainly! The WriteCallback function is a callback function used with the cURL library to handle 
the data received from an API during an HTTP request. It is called by the cURL library as it receives
data from the server, and its purpose is to process and store that data. 
Let's break down this function step by step:

Function Signature: The function is declared with a specific signature that is expected by cURL.
It returns a size_t value and takes four parameters:

contents: A pointer to the received data.
size: The size of an individual data element (usually 1 byte).
nmemb: The number of data elements received in this call.
output: A pointer to a C++ string (std::string) where the received data will be stored.
Total Size Calculation: This line calculates the total size of the received data in bytes. It's done by multiplying the size (size of an individual element) by nmemb (the number of elements received in this call). This gives the total size of the data received in this particular call to the callback.


size_t totalSize = size * nmemb;
Appending Data to Output String: The received data (contents) is of type void*, which is a pointer to
memory. This data is cast to a char* pointer and then appended to the output string. 
This step is crucial because it accumulates the received data in the output string, building it up as 
more data arrives.

output->append(static_cast<char*>(contents), totalSize);
Return Total Size: Finally, the function returns totalSize, which indicates how much data was processed 
in this call. cURL uses this value to keep track of how much data has been received so far.

return totalSize;
In summary, this callback function is used by cURL to receive data from an API in chunks, and it 
accumulates this data into a C++ std::string (output) as it arrives. This way, when the HTTP request 
is complete, the output string contains the entire response data from the API, which can then be processed 
as needed in the calling code.*/


/*In the provided code, several functions are used from both the `rapidjson` library and the `curl` library. 
Here's a breakdown of the functions and their uses:

### Functions from the `rapidjson` Library:

1. **`Document::Parse(const char* json)`**:
   - Use: This function is used to parse a JSON string and populate a `rapidjson::Document` object with the parsed data.
   - Description: It takes a JSON string as input and parses it, creating a structured representation of the JSON data that can be 
   easily accessed.

2. **`Document::HasMember(const char* memberName)`**:
   - Use: This function is used to check if a JSON object has a member with the given name.
   - Description: It returns `true` if the JSON object has a member with the specified name and `false` otherwise.

3. **`Value::IsArray()`** and **`Value::IsString()`**:
   - Use: These functions are used to check the type of a JSON value.
   - Description: `Value::IsArray()` checks if a JSON value is an array, and `Value::IsString()` checks if it's a string. These checks are  
   used to ensure that the JSON data is of the expected type before accessing it.

4. **`Value::GetString()`**, **`Value::GetInt()`**:
   - Use: These functions are used to retrieve the values of JSON members of specific types (string or integer).
   - Description: `Value::GetString()` retrieves the string value of a JSON member, and `Value::GetInt()` retrieves the integer value.

### Functions from the `curl` Library:

1. **`curl_easy_init()`**:
   - Use: This function is used to initialize a libcurl easy handle, which is used to make HTTP requests.
   - Description: It sets up the libcurl environment and prepares it for making HTTP requests.

2. **`curl_easy_setopt(CURL* handle, CURLoption option, parameter)`**:
   - Use: This function is used to set various options for the libcurl handle, such as the URL to request, callback functions, and more.
   - Description: It configures the behavior of the libcurl handle, allowing you to customize how the HTTP request is made.

3. **`curl_easy_perform(CURL* handle)`**:
   - Use: This function is used to perform the HTTP request with the configured libcurl handle.
   - Description: It initiates the HTTP request, sends it to the specified URL, and receives the response. The response data is stored 
   using the callback function provided.

4. **`curl_easy_cleanup(CURL* handle)`**:
   - Use: This function is used to clean up and release resources associated with the libcurl handle when you're done with it.
   - Description: It frees memory and resources used by the libcurl handle, ensuring proper cleanup.

5. **`size_t WriteCallback(void* contents, size_t size, size_t nmemb, string* output)`**:
   - Use: This is a custom callback function provided to libcurl. It is called when libcurl receives data from the HTTP response, and it's 
   used to store the received data in a string.
   - Description: The function appends the received data to the `string* output`, and its return value indicates how much data was processed.

These functions collectively allow the code to make an HTTP request to the Google Books API using libcurl, receive the JSON response, and then parse and extract information from the JSON response using the `rapidjson` library.*/