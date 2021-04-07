#ifndef LIJN_H
#define LIJN_H

#include <opencv2/core/types.hpp>
using namespace cv;

// Class waarin we een virtuele lijn aanmaken
class Lijn{
    // begin en eindpunten van de lijn
    Point2f p1,p2;
    public:

    // Indien geen parameters worden meegegeven maak dan gewoon lijn aan met punten waarvan
    // de waarde NULL is.
    Lijn();

    // Lijn maken met begin en eindpunten p1 en p2
    Lijn(Point2f p1, Point2f p2);

    // geef de locatie van 2 punten mee en de punten van de lijn worden hierin weggeschreven
    void getLijn(Point2f *p1, Point2f *p2);

    // Geef punt 1 terug
    Point2f getP1();

    // Geef punt 2 terug
    Point2f getP2();

    // Geef lengte terug
    float getLengte();

    // Geef lengte in de x-richting terug
    float getLengteX();

    // Geef lengte in de y-richting terug
    float getLengteY();

    // Geef midpunt terug
    Point2f getMidPunt();

    // Kijk of de lijn wel punten heeft
    bool isEmpty();

    // Geef de hoek weer tov de x-as
    float getHoekTovXAs(bool rad = false);

    // Print de lijn in cout
    void printLijn();

};

#endif
