#include "ofApp.h"

// layout numbers kept in one place so they're easy to tweak
const float listX = 30;
const float listY = 140;
const float listWidth = 340;
const float itemHeight = 60;
const float detailX = 400;
const float detailWidth = 570;
const float detailHeight = 520;

void ofApp::setup() {
    ofSetWindowTitle("Film Finder");
    ofSetFrameRate(60);

    // font.ttf needs to be in bin/data (any .ttf file renamed to font.ttf works)
    titleFont.load("font.ttf", 32);
    headingFont.load("font.ttf", 22);
    bodyFont.load("font.ttf", 16);
    smallFont.load("font.ttf", 12);

    searchText = "";
    selectedIndex = 0;
    searchStartFrame = 0;
    state = WAITING;

    // if the key file is missing, go straight to the error screen
    if (!searcher.setup()) {
        state = SHOWING_ERROR;
    }
}

void ofApp::update() {
    // The search freezes the window while it waits for the reply.
    // Waiting one frame means "Searching..." gets drawn before the freeze.
    if (state == SEARCHING && ofGetFrameNum() > searchStartFrame) {
        bool found = searcher.search(searchText);

        if (found) {
            state = SHOWING_RESULTS;
            selectMovie(0);
        } else {
            state = SHOWING_ERROR;
        }
    }
}

void ofApp::draw() {
    ofBackground(20, 22, 30);

    ofSetColor(255);
    titleFont.drawString("Film Finder", 30, 50);

    drawSearchBox();

    if (state == WAITING) {
        ofSetColor(170);
        bodyFont.drawString("Type a film name and press Enter.", listX, listY + 30);
    } else if (state == SEARCHING) {
        ofSetColor(255, 200, 80);
        bodyFont.drawString("Searching...", listX, listY + 30);
    } else if (state == SHOWING_ERROR) {
        ofSetColor(255, 120, 120);
        bodyFont.drawString(wrapText(searcher.getError(), 900, bodyFont), listX, listY + 30);
    } else if (state == SHOWING_RESULTS) {
        drawResultsList();
        drawDetails();
    }
    ofSetColor(255);
}

void ofApp::drawSearchBox() {
    // box background and outline
    ofSetColor(35, 38, 50);
    ofDrawRectangle(30, 80, 940, 40);
    ofNoFill();
    ofSetColor(90, 120, 200);
    ofDrawRectangle(30, 80, 940, 40);
    ofFill();

    // blinking cursor: shows for half a second, hides for half a second
    string shown = searchText;
    if ((ofGetElapsedTimeMillis() / 500) % 2 == 0) {
        shown += "|";
    }

    ofSetColor(255);
    bodyFont.drawString(shown, 42, 107);
}

void ofApp::drawResultsList() {
    vector<Movie>& movies = searcher.getResults();

    for (int i = 0; i < movies.size(); i++) {
        float y = listY + i * itemHeight;

        // highlight whichever one is selected
        if (i == selectedIndex) {
            ofSetColor(60, 90, 160);
        } else {
            ofSetColor(35, 38, 50);
        }
        ofDrawRectangle(listX, y, listWidth, itemHeight - 6);

        ofSetColor(255);
        bodyFont.drawString(fitText(movies[i].getTitle(), listWidth - 20, bodyFont), listX + 10, y + 24);
        ofSetColor(170);
        smallFont.drawString(getYear(movies[i].getReleaseDate()), listX + 10, y + 43);
    }
    ofSetColor(255);
}

void ofApp::drawDetails() {
    Movie& movie = searcher.getResults()[selectedIndex];

    // panel background
    ofSetColor(35, 38, 50);
    ofDrawRectangle(detailX, listY, detailWidth, detailHeight);

    // poster on the left of the panel
    movie.drawPoster(detailX + 20, listY + 20, 200, 300);

    // title, date and rating to the right of the poster
    float textX = detailX + 245;
    ofSetColor(255);
    headingFont.drawString(wrapText(movie.getTitle(), 300, headingFont), textX, listY + 45);

    ofSetColor(170);
    bodyFont.drawString("Released: " + movie.getReleaseDate(), textX, listY + 200);

    bodyFont.drawString("Rating: " + ofToString(movie.getRating(), 1) + " / 10", textX, listY + 240);

    // rating bar: grey background, coloured part is rating out of 10
    ofSetColor(60);
    ofDrawRectangle(textX, listY + 255, 250, 12);
    ofSetColor(255, 200, 80);
    ofDrawRectangle(textX, listY + 255, 250 * (movie.getRating() / 10.0f), 12);

    // plot summary under the poster. Cut it short if it's really long.
    string overview = movie.getOverview();
    if (overview.size() > 380) {
        overview = overview.substr(0, 380);
        // cut back to the last space so we don't chop a word in half
        overview = overview.substr(0, overview.rfind(' ')) + "...";
    }
    ofSetColor(230);
    smallFont.drawString(wrapText(overview, detailWidth - 40, smallFont), detailX + 20, listY + 350);
    ofSetColor(255);
}

void ofApp::keyPressed(int key) {
    // ignore typing while a search is running
    if (state == SEARCHING) {
        return;
    }

    if (key == OF_KEY_RETURN) {
        startSearch();
    } else if (key == OF_KEY_BACKSPACE) {
        if (!searchText.empty()) {
            searchText.pop_back();
        }
    } else if (key == OF_KEY_DOWN && state == SHOWING_RESULTS) {
        if (selectedIndex < (int)searcher.getResults().size() - 1) {
            selectMovie(selectedIndex + 1);
        }
    } else if (key == OF_KEY_UP && state == SHOWING_RESULTS) {
        if (selectedIndex > 0) {
            selectMovie(selectedIndex - 1);
        }
    } else if (key >= 32 && key <= 126) {
        // normal printable character, with a length limit
        if (searchText.size() < 60) {
            searchText += (char)key;
        }
    }
}

void ofApp::mousePressed(int x, int y, int button) {
    if (state != SHOWING_RESULTS) {
        return;
    }

    // check if the click landed on one of the list items
    int count = searcher.getResults().size();
    for (int i = 0; i < count; i++) {
        float itemY = listY + i * itemHeight;
        if (x >= listX && x <= listX + listWidth && y >= itemY && y <= itemY + itemHeight - 6) {
            selectMovie(i);
        }
    }
}

void ofApp::startSearch() {
    state = SEARCHING;
    searchStartFrame = ofGetFrameNum();
}

void ofApp::selectMovie(int index) {
    selectedIndex = index;
    // poster is only downloaded when a film is first selected, then it's kept
    searcher.getResults()[index].loadPoster();
}

// Splits text over several lines so it fits inside maxWidth.
// It adds one word at a time and starts a new line when the line gets too wide.
string ofApp::wrapText(string text, float maxWidth, ofTrueTypeFont& font) {
    vector<string> words = ofSplitString(text, " ");
    string result = "";
    string line = "";

    for (int i = 0; i < words.size(); i++) {
        string testLine = (line == "") ? words[i] : line + " " + words[i];

        if (font.stringWidth(testLine) > maxWidth && line != "") {
            result += line + "\n";
            line = words[i];
        } else {
            line = testLine;
        }
    }
    result += line;
    return result;
}

// Shortens text with "..." if it's too wide for the space.
// (Could break odd characters in non-English titles - something to improve.)
string ofApp::fitText(string text, float maxWidth, ofTrueTypeFont& font) {
    if (font.stringWidth(text) <= maxWidth) {
        return text;
    }
    while (text.size() > 1 && font.stringWidth(text + "...") > maxWidth) {
        text.pop_back();
    }
    return text + "...";
}

string ofApp::getYear(string releaseDate) {
    if (releaseDate.size() >= 4) {
        return releaseDate.substr(0, 4);
    }
    return "Year unknown";
}
