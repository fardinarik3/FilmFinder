#pragma once
#include "ofMain.h"
#include "Movie.h"
#include "MovieSearcher.h"

// AI ACKNOWLEDGEMENT (edit this before submitting):
// Drafted with help from Claude (Anthropic AI). Prompt used: "<paste the prompt>".

class ofApp : public ofBaseApp {
public:
    void setup();
    void update();
    void draw();

    void keyPressed(int key);
    void mousePressed(int x, int y, int button);

private:
    // the different screens/situations the app can be in
    enum AppState { WAITING, SEARCHING, SHOWING_RESULTS, SHOWING_ERROR };
    AppState state;

    MovieSearcher searcher;
    string searchText;      // what the user has typed so far
    int selectedIndex;      // which film in the list is highlighted
    int searchStartFrame;   // used so "Searching..." gets drawn before the search runs

    ofTrueTypeFont titleFont;
    ofTrueTypeFont headingFont;
    ofTrueTypeFont bodyFont;
    ofTrueTypeFont smallFont;

    void startSearch();
    void selectMovie(int index);

    void drawSearchBox();
    void drawResultsList();
    void drawDetails();

    // text helpers
    string wrapText(string text, float maxWidth, ofTrueTypeFont& font);
    string fitText(string text, float maxWidth, ofTrueTypeFont& font);
    string getYear(string releaseDate);
};
