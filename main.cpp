
#include "main.h"
#include "Audio.h"
#include <direct.h>
#include <dirent.h>
#include <cstring>
#include <string>
using namespace std;

//set window width and height
const int WINDOW_X = 1280;
const int WINDOW_Y = 720;

const int TEXTURE_X = 0.8 * WINDOW_X;
const int TEXTURE_Y = 0.35 * WINDOW_Y;

//function for displaying content of folder
string listfiles()
{
    DIR *dir = opendir("./audio");
    if (dir == NULL)
    {
        cerr << "The folder is empty!!!!!!";
    }
    struct dirent *entity;
    entity = readdir(dir);
    string path[10000];
    int u = 0;
    int inputNum;
    cout << "Enter the number of song you want to listen!!" << endl;
    while (entity != NULL)
    {
        if (strcmp(entity->d_name, ".") != 0 && strcmp(entity->d_name, "..") != 0)
        {
            path[u] = entity->d_name;
            cout << u - 1 << ". " << path[u] << endl;
        }
        u++;
        entity = readdir(dir);
    }
    cin >> inputNum;
    inputNum += 1;
    cout << "Selected- " << path[inputNum] << endl;
    string songAddress = path[inputNum];
    return songAddress;
}

int main()
{
    //folder function called
    string songAddress = listfiles();

    int elapsedMinutes = 0;
    int disSec = 0;

    // display topbar icon
    sf::Image icon;
    icon.loadFromFile("./images/icon.png"); // File/Image/Pixel
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;

    //initialize display window
    sf::RenderWindow window(sf::VideoMode(1280, 720), "Fourielizer", sf::Style::Default, settings);
    window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
    window.setFramerateLimit(60);

    //constructor called from Audio class
    Audio audio(songAddress);
    audio.returnPath(songAddress);

    //Timeline components
    sf::RectangleShape timeline(sf::Vector2f(TEXTURE_X, 1));
    timeline.setFillColor(sf::Color::Black);
    timeline.setPosition((WINDOW_X - TEXTURE_X) / 2, 0.9 * WINDOW_Y);
    //Seek
    sf::CircleShape seek(3);
    seek.setPointCount(64);
    seek.setOrigin(3, 3);
    seek.setFillColor(sf::Color::Black);
    //load font(arial)
    sf::Font font;

    if (!font.loadFromFile("./fonts/arial.ttf"))
    {
        cerr << "Error loading file" << endl;
    }
    //set font size
    int characterSize = 32;
    //setup for displaying all the static text on screen
    sf::Text text1;
    sf::Text text2;
    sf::Text text3;
    text1.setFont(font);
    text1.setCharacterSize(characterSize);
    text1.setColor(sf::Color::Black);
    text1.setString("Log Base Bars");
    float text1Width = text1.getGlobalBounds().width;
    text1.setPosition((window.getSize().x - text1Width) / 2, 290.f);
    text2.setFont(font);
    text2.setCharacterSize(characterSize);
    text2.setColor(sf::Color::Black);
    text2.setString("Wave");
    float text2Width = text2.getGlobalBounds().width;
    float text2X = (window.getSize().x - text2Width) / 2;
    text2.setPosition(text2X, 550.f);
    text3.setFont(font);
    text3.setCharacterSize(characterSize);
    text3.setColor(sf::Color::Black);
    string songName = audio.getPath();
    int len = songName.length();
    songName.erase(len - 4, songName.length());
    text3.setString("Playing: " + songName);
    float text3Width = text3.getGlobalBounds().width;
    text3.setPosition((window.getSize().x - text3Width) / 2, 10.f);

    // Window Loop
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            //Esc for exit
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
                window.close();
        }
        //set background white
        window.clear(sf::Color::White);
        //get elapsed time in seconds
        int elapsedSeconds = audio.song.getPlayingOffset().asSeconds();

        // mapping keys for volume and position change
        if (event.type == sf::Event::KeyPressed)
        {

            //Up and Down to control volume
            if (event.key.code == sf::Keyboard::Key::Down)
                audio.song.setVolume(audio.song.getVolume() - 10);
            if (event.key.code == sf::Keyboard::Key::Up)
                audio.song.setVolume(audio.song.getVolume() + 10);
            //Left and Right to control tracking position
            if (event.key.code == sf::Keyboard::Key::Right)
            {
                auto newPos = audio.song.getPlayingOffset() + sf::seconds(2.5f);
                audio.song.setPlayingOffset(newPos);
            }
            if (event.key.code == sf::Keyboard::Key::Left)
            {
                auto newPos = audio.song.getPlayingOffset() - sf::seconds(2.5f);
                audio.song.setPlayingOffset(newPos);
            }
        }

        //wave and log bar function called for rendering
        audio.updateFFT();
        audio.draw(window);
        //check for song play status
        if (!audio.songPlayed())
        {
            audio.playSong();
        }

        //calculation for seek circle
        int now_sec = audio.song.getPlayingOffset().asSeconds();

        int dur = audio.duration.asSeconds();
        int pos = (WINDOW_X - TEXTURE_X) / 2 + now_sec * TEXTURE_X / dur;
        seek.setPosition(pos, WINDOW_Y * 0.9);

        //calculation for displaying elapsed time
        switch (elapsedSeconds)
        {
        case 60:
            elapsedMinutes = 1;
            break;
        case 120:
            elapsedMinutes = 2;
            break;
        case 180:
            elapsedMinutes = 3;
            break;
        case 240:
            elapsedMinutes = 4;
            break;
        case 300:
            elapsedMinutes = 5;
            break;
        case 360:
            elapsedMinutes = 6;
            break;
        case 420:
            elapsedMinutes = 7;
            break;
        case 480:
            elapsedMinutes = 8;
            break;
        case 540:
            elapsedMinutes = 9;
            break;
        }

        disSec = elapsedSeconds;
        if ((elapsedSeconds != 0) && (elapsedSeconds % 60 == 0) || (elapsedSeconds > 60))
        {
            disSec -= 60;
        }

        if (elapsedSeconds != 0 && elapsedSeconds % 120 == 0 || elapsedSeconds > 120)
        {
            disSec -= 60;
        }
        if (elapsedSeconds != 0 && elapsedSeconds % 180 == 0 || elapsedSeconds > 180)
        {
            disSec -= 60;
        }
        if (elapsedSeconds != 0 && elapsedSeconds % 240 == 0 || elapsedSeconds > 240)
        {
            disSec -= 60;
        }
        if (elapsedSeconds != 0 && elapsedSeconds % 300 == 0 || elapsedSeconds > 300)
        {
            disSec -= 60;
        }
        if (elapsedSeconds != 0 && elapsedSeconds % 360 == 0 || elapsedSeconds > 360)
        {
            disSec -= 60;
        }
        if (elapsedSeconds != 0 && elapsedSeconds % 420 == 0 || elapsedSeconds > 420)
        {
            disSec -= 60;
        }
        if (elapsedSeconds != 0 && elapsedSeconds % 480 == 0 || elapsedSeconds > 480)
        {
            disSec -= 60;
        }
        if (elapsedSeconds != 0 && elapsedSeconds % 540 == 0 || elapsedSeconds > 540)
        {
            disSec -= 60;
        }
        //converting time into string for display
        std::stringstream ss;
        ss.str(std::string()); //clear the string
        ss << disSec;
        string secondsString = ss.str().c_str();

        std::stringstream ss1;
        ss1.str(std::string()); //clear the string
        ss1 << elapsedMinutes;
        string minutesString = ss1.str().c_str();
        //conversion from second to minute
        int musicDurationSeconds = audio.music.getDuration().asSeconds();
        int musicDurationMinutes = (musicDurationSeconds) / 60;
        musicDurationSeconds = musicDurationSeconds % 60;
        std::stringstream ss2;
        ss2.str(std::string()); //clear the string
        ss2 << musicDurationMinutes;
        string musicDurationMinutesString = ss2.str().c_str();

        std::stringstream ss3;
        ss3.str(std::string()); //clear the string
        ss3 << musicDurationSeconds;
        string musicDurationSecondsString = ss3.str().c_str();
        //display song's current time
        sf::Text text4;
        text4.setPosition(((WINDOW_X - TEXTURE_X) / 2) - 3, (0.9 * WINDOW_Y) + 10);
        text4.setCharacterSize(14);
        text4.setFillColor(sf::Color::Black);
        text4.setFont(font);
        text4.setString(minutesString + ":" + secondsString + "/" + musicDurationMinutesString + ":" + musicDurationSecondsString);

        //render and display screen components
        window.draw(timeline);
        window.draw(seek);
        window.draw(text1);
        window.draw(text2);
        window.draw(text3);
        window.draw(text4);

        window.display();
    }

    return 0;
}
