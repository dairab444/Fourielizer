#include "Visualizer.h"

Visualizer::Visualizer()
{
    this->drawShape();
}

Visualizer::~Visualizer(){};

//functions

std::vector<sf::RectangleShape> &Visualizer::getFreqRangeRects()
{
    return freqRangeRects;
}
sf::RectangleShape &Visualizer::getBackgroundRect()
{
    return backgroundRect;
}

std::vector<sf::CircleShape> &Visualizer::getAmplitudeParticles()
{
    return amplitudeParticles;
}

void Visualizer::setAmplitudeVisualizationVector(std::vector<double> amplitudeVisualizationVector)
{
    this->amplitudeVisualizationVector = amplitudeVisualizationVector;
}

void Visualizer::update(std::vector<std::vector<double>> &frequencyVisualizationVector, double songPlayingOffset)
{
    this->updateShape(frequencyVisualizationVector, songPlayingOffset);
}
