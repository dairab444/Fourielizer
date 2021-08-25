#ifndef AUDIO_H
#define AUDIO_H
#pragma once
#include "main.h"

using namespace std;
using namespace sf;

typedef complex<double> Complex;
typedef valarray<Complex> CArray;

class Audio
{
private:
    //setting up variables
    string finalPath;
    const sf::Int16 *samples;
    sf::Uint64 sampleSize;
    unsigned int sampleRate;
    int singleChannelSize;
    bool playedSong = false;

public:
    //variables
    int _bufferSize = 512;
    sf::SoundBuffer buffer;
    sf::Sound song;
    sf::Music music;
    sf::Time duration;
    std::string audioFilePath;

    vector<Complex> sample;
    vector<float> window;
    CArray bin;
    CArray fftbin;

    VertexArray VA1;
    VertexArray VA2;
    VertexArray VA3;
    vector<Vertex> cascade;

    int sRate;
    int sampleCount;
    int bufferSize;
    int mark;

    //constructor
    Audio();
    Audio(string path);

    //public functions
    void hammingWindow();
    void returnPath(string path);
    string getPath();
    void fft(CArray &x);
    void updateFFT();
    void bars(float const &max);
    void fftbars(float const &max);
    void draw(sf::RenderWindow &window);
    void playSong();
    bool songPlayed();
};

#endif // AUDIO_H
