#include "Audio.h"

//Audio::Audio() {};

Audio::Audio(string path)
{
    //address for audio folder
    audioFilePath = "./audio/" + path;
    if (!buffer.loadFromFile(audioFilePath))
    {
        std::cout << "Couldn't load buffer" << std::endl;
    }
    else
    {
        //get song data
        song.setBuffer(buffer);
        duration = buffer.getDuration();
        sampleRate = buffer.getSampleRate();
        //Original sample (unsplit: the samples holding the left and right channels)
        samples = buffer.getSamples();
        sampleSize = buffer.getSampleCount();
        //singleChannelSize = sampleSize / 2;
    }

    VA1.setPrimitiveType(LineStrip);
    sRate = buffer.getSampleRate() * buffer.getChannelCount();
    sampleCount = buffer.getSampleCount();
    if (_bufferSize < sampleCount)
        bufferSize = _bufferSize;
    else
        bufferSize = sampleCount;
    mark = 0;

    //hamming window calculation
    for (int i(0); i < bufferSize; i++)
        window.push_back(0.54 - 0.46 * cos(2 * PI * i / (float)bufferSize));

    sample.resize(bufferSize);
    VA1.resize(bufferSize);
}

//wave window
void Audio::hammingWindow()
{
    mark = song.getPlayingOffset().asSeconds() * sampleRate;
    if (mark + bufferSize < sampleCount)
    {
        for (int i(mark); i < bufferSize + mark; i++)
        {
            sample[i - mark] = Complex(buffer.getSamples()[i] * window[i - mark], 0);

            VA1[i - mark] = Vertex(Vector2f(310, 475) + Vector2f((i - mark) / (float)bufferSize * 700, sample[i - mark].real() * 0.005), sf::Color::Black);
        }
    }
}

void Audio::returnPath(string path)
{
    this->finalPath = path;
}

string Audio::getPath()
{
    return (this->finalPath);
}

//render wave

//fft calculating recursive function
void Audio::fft(CArray &x)
{
    const int N = x.size();
    if (N <= 1)
        return;

    CArray even = x[slice(0, N / 2, 2)];
    CArray odd = x[slice(1, N / 2, 2)];

    fft(even);
    fft(odd);

    for (int k = 0; k < N / 2; k++)
    {
        Complex t = polar(1.0, -2 * PI * k / N) * odd[k];
        x[k] = even[k] + t;
        x[k + N / 2] = even[k] - t;
    }
}
//main function to call fft function
void Audio::updateFFT()
{
    hammingWindow();

    VA2.clear();
    VA3.clear();

    bin = CArray(sample.data(), bufferSize);

    fftbin = bin;
    fft(fftbin);
    float max = 10000000;

    bars(max);
    fftbars(max * 10);
}
//function for calculating log bars
void Audio::bars(float const &max)
{
    VA2.setPrimitiveType(Lines);
    Vector2f position(620, 260);
    for (float i(3); i < min(bufferSize / 2.f, 20000.f); i *= 1.01)
    {
        Vector2f samplePosition(log(i) / log(min((bufferSize) / 2.f, 20000.f)), abs(bin[(int)i]));
        VA2.append(Vertex(position + Vector2f(samplePosition.x * 620, -samplePosition.y / max * 150000), sf::Color::Black));
        //for reflection
        VA2.append(Vertex(position + Vector2f(samplePosition.x * 620, 0), sf::Color::Black));
        VA2.append(Vertex(position + Vector2f(samplePosition.x * 620, 0), sf::Color(0, 0, 0, 100)));
        VA2.append(Vertex(position + Vector2f(samplePosition.x * 620, samplePosition.y / max * 150000 / 2.f), sf::Color(0, 0, 0, 0)));
    }
}

//function for calculating log bars with fft
void Audio::fftbars(float const &max)
{
    VA3.setPrimitiveType(Lines);
    Vector2f position(5, 260);
    for (float i(3); i < min(bufferSize / 2.f, 20000.f); i *= 1.01)
    {
        Vector2f samplePosition(log(i) / log(min((bufferSize) / 2.f, 20000.f)), abs(fftbin[(int)i]));
        VA3.append(Vertex(position + Vector2f(samplePosition.x * 620, -samplePosition.y / max * 15000), sf::Color::Black));
        //for reflection
        VA3.append(Vertex(position + Vector2f(samplePosition.x * 620, 0), sf::Color::Black));
        VA3.append(Vertex(position + Vector2f(samplePosition.x * 620, 0), sf::Color(0, 0, 0, 100)));
        VA3.append(Vertex(position + Vector2f(samplePosition.x * 620, samplePosition.y / max * 15000 / 2.f), sf::Color(0, 0, 0, 0)));
    }
}

//function to draw wave and bars
void Audio::draw(sf::RenderWindow &window)
{
    window.draw(VA1);
    window.draw(VA2);
    window.draw(VA3);
}

//check if song is playing
void Audio::playSong()
{

    song.play();
    music.openFromFile(audioFilePath);
    music.play();
    music.setVolume(0.f);

    song.setLoop(true);
    this->playedSong = true;
}

bool Audio::songPlayed()
{
    return this->playedSong;
}
