#pragma once
#include "ofMain.h"
#include "Movie.h"

// AI ACKNOWLEDGEMENT (edit this before submitting):
// Drafted with help from Claude (Anthropic AI). Prompt used: "<paste the prompt>".

// MovieSearcher talks to the TMDB API. It sends the search, checks for
// errors, turns the JSON into Movie objects and keeps them in a vector.
class MovieSearcher {
public:
    // reads the API key from bin/data/apikey.txt, returns false if it's missing
    bool setup();

    // runs a search. Returns true if we got at least one film back.
    // If it returns false, call getError() to see what went wrong.
    bool search(string query);

    vector<Movie>& getResults() { return results; }
    string getError() const { return errorMessage; }

private:
    string apiKey;
    string errorMessage;
    vector<Movie> results;

    // makes text safe to put in a web address (spaces become %20 etc.)
    string urlEncode(string text);
};
