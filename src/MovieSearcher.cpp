#include "MovieSearcher.h"
#include "ofxJSON.h"

// AI ACKNOWLEDGEMENT (edit before submitting):
// Drafted with help from Claude (Anthropic AI). Prompt used: "<paste the prompt>".
// Based on the ofxJSON starter code provided in the CodeLab II repo README.

// The API sometimes sends null instead of text (e.g. a film with no poster).
// Asking for a string from a null value gives a bad result, so I check the type first.
static string readString(const Json::Value& obj, const string& key, const string& fallback) {
    if (obj.isMember(key.c_str()) && obj[key.c_str()].isString()) {
        return obj[key.c_str()].asString();
    }
    return fallback;
}

bool MovieSearcher::setup() {
    // the key lives in its own file so it never ends up on GitHub
    ofBuffer buffer = ofBufferFromFile("apikey.txt");
    apiKey = ofTrim(buffer.getText());

    if (apiKey == "") {
        errorMessage = "No API key found. Put your TMDB key in bin/data/apikey.txt";
        return false;
    }
    return true;
}

bool MovieSearcher::search(string query) {
    // wipe anything left over from the last search
    results.clear();
    errorMessage = "";

    query = ofTrim(query);

    // input validation
    if (query == "") {
        errorMessage = "Please type a film name first.";
        return false;
    }
    if (apiKey == "") {
        errorMessage = "No API key loaded.";
        return false;
    }

    string url = "https://api.themoviedb.org/3/search/movie?api_key=" + apiKey
               + "&query=" + urlEncode(query);

    // I use ofLoadURL here instead of json.open() because it gives me the
    // status code, which lets me show a proper error message.
    ofHttpResponse response = ofLoadURL(url);

    if (response.status != 200) {
        if (response.status == 401) {
            errorMessage = "The API key was rejected. Check apikey.txt.";
        } else if (response.status == 429) {
            errorMessage = "Too many requests. Wait a few seconds and try again.";
        } else if (response.status <= 0) {
            errorMessage = "Couldn't reach the server. Check your internet connection.";
        } else {
            errorMessage = "The server sent an error (code " + ofToString(response.status) + ").";
        }
        return false;
    }

    // turn the reply text into JSON using ofxJSON
    ofxJSONElement json;
    if (!json.parse(response.data.getText())) {
        errorMessage = "Couldn't read the data from the server.";
        return false;
    }

    if (!json.isMember("results") || !json["results"].isArray()) {
        errorMessage = "The reply was in a format I didn't expect.";
        return false;
    }

    // try/catch in case the data has something unexpected in it
    try {
        Json::Value list = json["results"];

        // loop through the films, keeping the first 8 so the list fits the screen
        for (unsigned int i = 0; i < list.size() && i < 8; i++) {
            Json::Value item = list[i];

            int id = 0;
            if (item["id"].isNumeric()) {
                id = item["id"].asInt();
            }

            float rating = 0;
            if (item["vote_average"].isNumeric()) {
                rating = item["vote_average"].asFloat();
            }

            string title = readString(item, "title", "Untitled");
            string date = readString(item, "release_date", "");
            string overview = readString(item, "overview", "");
            string posterPath = readString(item, "poster_path", "");

            // fill in friendly text for missing bits
            if (date == "") date = "Unknown";
            if (overview == "") overview = "No description available.";

            results.push_back(Movie(id, title, date, overview, rating, posterPath));
        }
    } catch (const std::exception& e) {
        errorMessage = "Couldn't read the data from the server.";
        ofLogError("MovieSearcher") << "JSON problem: " << e.what();
        return false;
    }

    if (results.empty()) {
        errorMessage = "No films found for \"" + query + "\". Try a different title.";
        return false;
    }
    return true;
}

string MovieSearcher::urlEncode(string text) {
    string encoded = "";
    for (char c : text) {
        // letters, numbers and these few symbols are safe as they are
        if (isalnum((unsigned char)c) || c == '-' || c == '_' || c == '.' || c == '~') {
            encoded += c;
        } else {
            // everything else becomes % followed by its hex code
            char buffer[4];
            snprintf(buffer, sizeof(buffer), "%%%02X", (unsigned char)c);
            encoded += buffer;
        }
    }
    return encoded;
}
