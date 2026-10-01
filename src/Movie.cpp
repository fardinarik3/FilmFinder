#include "Movie.h"

// default constructor - gives everything a safe starting value
Movie::Movie() {
    id = 0;
    title = "Unknown";
    releaseDate = "Unknown";
    overview = "";
    rating = 0;
    posterPath = "";
    posterLoaded = false;
    posterTried = false;
}

// constructor used when we build a Movie from the JSON data
Movie::Movie(int id, string title, string releaseDate, string overview, float rating, string posterPath) {
    this->id = id;
    this->title = title;
    this->releaseDate = releaseDate;
    this->overview = overview;
    this->rating = rating;
    this->posterPath = posterPath;
    posterLoaded = false;
    posterTried = false;
}

void Movie::loadPoster() {
    // already had a go at this one, no need to download again
    if (posterTried) {
        return;
    }
    posterTried = true;

    // some films don't have a poster at all
    if (posterPath == "") {
        return;
    }

    // TMDB gives us just the end of the address, so we stick the base on the front
    // w342 is the image width - bigger than we need but still quick to load
    string url = "https://image.tmdb.org/t/p/w342" + posterPath;
    posterLoaded = poster.load(url);
}

void Movie::drawPoster(float x, float y, float w, float h) {
    if (posterLoaded) {
        ofSetColor(255);
        poster.draw(x, y, w, h);
    } else {
        // placeholder so the layout doesn't jump around
        ofSetColor(60);
        ofDrawRectangle(x, y, w, h);
        ofSetColor(150);
        ofDrawBitmapString("No poster", x + w / 2 - 30, y + h / 2);
    }
    ofSetColor(255);
}
