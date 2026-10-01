# Film Finder

A film search app built with C++ and openFrameworks. It uses the TMDB API to
search for films and shows the title, release date, rating, plot summary and poster.

## How to run
1. Install openFrameworks 0.12.x and add the ofxJSON addon.
2. Create a project called FilmFinder in apps/myApps (tick ofxJSON in the Project Generator) and copy the `src` and `bin/data` files from this folder into it.
3. Get a free API key from themoviedb.org (Settings > API).
4. Copy `bin/data/apikey.txt.example` to `bin/data/apikey.txt` and replace the text with your key.
5. Add a font called `font.ttf` to `bin/data` (I used Roboto Regular).
6. Build and run.

## Controls
- Type a film name and press Enter to search
- Up / Down arrows or mouse click to choose a film
- Backspace to delete
