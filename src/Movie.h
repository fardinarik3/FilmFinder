#pragma once
#include "ofMain.h"

// AI ACKNOWLEDGEMENT (edit this before submitting):
// This file was drafted with help from Claude (Anthropic AI).
// Prompt used: "<paste the prompt you gave>". I reviewed and changed it myself.

// A Movie stores the details for one film that came back from the search.
// It also looks after its own poster image so ofApp doesn't have to.
class Movie {
public:
    Movie();
    Movie(int id, string title, string releaseDate, string overview, float rating, string posterPath);

    // downloads the poster (only does it once)
    void loadPoster();

    // draws the poster, or a grey box if there isn't one
    void drawPoster(float x, float y, float w, float h);

    // simple getters
    int getId() const { return id; }
    string getTitle() const { return title; }
    string getReleaseDate() const { return releaseDate; }
    string getOverview() const { return overview; }
    float getRating() const { return rating; }

private:
    int id;
    string title;
    string releaseDate;
    string overview;
    float rating;
    string posterPath;

    ofImage poster;
    bool posterLoaded;   // true if the image downloaded ok
    bool posterTried;    // true once we've attempted the download, so we don't keep retrying
};
