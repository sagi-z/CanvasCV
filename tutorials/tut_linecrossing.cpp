#include <canvascv/canvas.h>
#include <canvascv/shapes/linecrossing.h>
#include <canvascv/widgets/text.h>
#include <canvascv/widgets/button.h>
#include <canvascv/widgets/verticallayout.h>

#include <opencv2/highgui.hpp>

#include <random>
#include <chrono>

using namespace std;
using namespace cv;
using namespace canvascv;

class SimulatedPerson
{
public:
    SimulatedPerson(Canvas &c, const Point &from, const Point &to)
        : canvas(&c), fromPos(from), toPos(to), stepsTaken(0), currentPos(from)
    {
        // any widget could be used here
        widget = Text::create(c, from, "[Person]");
        widget->setAlpha(0.);

        static std::random_device rd;
        static std::mt19937 gen(rd());
        static std::uniform_int_distribution<> colorDist(50, 255);
        Scalar color(colorDist(gen), colorDist(gen), colorDist(gen));
        widget->setOutlineColor(color);

        movementVector = Point(to.x - from.x, to.y - from.y);
        distance = sqrt( pow(to.x - from.x, 2) + pow(to.y - from.y, 2) );
    }

    void move(int steps)
    {
        if (! isGone())
        {
            stepsTaken += steps;
            if (stepsTaken > distance)
            {
                stepsTaken = distance;
                setGone();
            }
            double scale = stepsTaken / distance;
            currentPos.x = movementVector.x * scale + fromPos.x;
            currentPos.y = movementVector.y * scale + fromPos.y;

            Rect rect(Point(0, 0), canvas->getSize());
            if (! rect.contains(currentPos))
                setGone();

            if (! isGone())
                widget->setLocation(currentPos);
        }
    }

    void setGone()
    {
        if (widget)
        {
            widget->rmvFromLayout();
            widget.reset();
        }
    }

    bool isGone() { return widget.get() == nullptr; }

    const Point &getFromPos() const { return fromPos; }

    const Point &getCurrentPos() const { return currentPos; }

private:
    Canvas *canvas;
    Point fromPos;
    Point toPos;
    double stepsTaken;
    Point currentPos;
    Point movementVector;
    double distance;
    shared_ptr<Widget> widget;
};

class CountingManager
{
public:
    CountingManager(Canvas &c) : canvas(c)
    {
        c.notifyOnShapeCreate([&c, this](Shape* shape)
        {
            if (shape->getType() == LineCrossing::type)
            {
                LineCrossing *pLineCrossing = (LineCrossing*) shape;
                lineCrossings.push_back(make_pair(pLineCrossing, 0));
            }
        });
        delCbId = c.notifyOnShapeDelete([&c, this](Shape* shape)
        {
            if (shape->getType() == LineCrossing::type)
            {
                // remove this LineCrossing
                auto i = find_if(lineCrossings.begin(),
                                 lineCrossings.end(),
                                 [shape](const CountingLine &item)
                {
                    return item.first == shape;
                });
                if (i != lineCrossings.end()) lineCrossings.erase(i);
            }
        });
    }

    // count people crossing with/against lines and return true for these cases
    bool evaluatePerson(const SimulatedPerson &person)
    {
        for (CountingLine &lineCrossing : lineCrossings)
        {
            int res = lineCrossing.first->isCrossedBySegment(person.getFromPos(),
                                                             person.getCurrentPos());
            lineCrossing.second += res;
            lineCrossing.first->setName(CCV_STR("Line was crossed: " << lineCrossing.second << " times"));
            if (res) return true;
        }
        return false;
    }
    
    ~CountingManager()
    {
        // The Canvas DTOR will otherwise call
        //  our cb after out death and crash
        canvas.rmvNotifyOnShapeDelete(delCbId);
    }

private:

    Canvas &canvas;
    Canvas::CBIDCanvasShape delCbId;
    typedef pair<LineCrossing*,int> CountingLine;
    list<CountingLine> lineCrossings;
};

class TrackerSimulator
{
public:
    TrackerSimulator(Canvas &c)
        : simulatorOn(false), canvas(&c), cm(c), rd(), gen(rd())
    {
        lastTime = secondCounter = chrono::high_resolution_clock::now();
    }

    void simulate(int newDetectionsPerSecond, const Point &center, int radius,
                  int minStepsPerSec, int maxStepsPerSec)
    {
        if (simulatorOn)
        {
            // calculate ratio for relative values
            auto now = chrono::high_resolution_clock::now();
            chrono::duration<double, std::milli> diff = now - lastTime;
            lastTime = now;
            double ratio = diff.count() / 1000;
            int minSteps = minStepsPerSec * ratio;
            int maxSteps = maxStepsPerSec * ratio;
            peopleMoved(minSteps, maxSteps);

            diff = now - secondCounter;
            if (diff.count() >= 1000)
            {
                secondCounter = now;
                peopleDetected(newDetectionsPerSecond, center, radius);
            }
        }
    }

    // generate count people at a certain radius with random target
    // positions at and beyond screen edges
    void peopleDetected(int count, const Point &center, int radius)
    {
        std::uniform_int_distribution<> disRadius(0, radius);
        std::uniform_real_distribution<> disAngle(0, CV_2PI);
        for (int i = 0; i< count; ++i)
        {
             int personRadius = disRadius(gen);
             float moveAngle = disAngle(gen);
             Point from(center.x + personRadius * cos(moveAngle),
                        center.y + personRadius * sin(moveAngle));
             Size screenSize = canvas->getSize();
             int maxDistance = max(screenSize.width, screenSize.height) / 2.;
             Point to(center.x + maxDistance * cos(moveAngle),
                      center.y + maxDistance * sin(moveAngle));
             trackedPeople.push_back(SimulatedPerson(*canvas, from, to));
        }
    }

    // move people random steps along their vector.
    // try to count them and remove them when they are gone.
    void peopleMoved(int minSteps, int maxSteps)
    {
        std::uniform_int_distribution<> disSteps(minSteps, maxSteps);
        list<list<SimulatedPerson>::iterator> gonePeople;
        for (auto i = trackedPeople.begin(); i != trackedPeople.end(); ++i)
        {
            i->move(disSteps(gen));
            if (cm.evaluatePerson(*i))
            {
                i->setGone();
                gonePeople.push_back(i);
            }
            else if (i->isGone())
            {
                gonePeople.push_back(i);
            }
        }
        for (list<SimulatedPerson>::iterator &i : gonePeople)
            trackedPeople.erase(i);
    }

    bool getSimulatorOn() const { return simulatorOn; }
    void setSimulatorOn(bool value)
    {
        simulatorOn = value;
        if (simulatorOn)
        {
            lastTime = secondCounter = chrono::high_resolution_clock::now();
        }
    }

private:
    bool simulatorOn;
    Canvas *canvas;
    CountingManager cm;
    std::random_device rd;
    std::mt19937 gen;
    chrono::high_resolution_clock::time_point secondCounter;
    chrono::high_resolution_clock::time_point lastTime;
    list<SimulatedPerson> trackedPeople;
};

static void help(Canvas &c)
{
    static bool showHelp = true;
    static string helpMsg =
            "* Click-Release-Drag\n"
            " to create LineCrossings.\n"
            "* Click on their middle arrow\n"
            " to change their direction.\n"
            "* Select/Unselect with the mouse.\n"
            "* Drag selected lines with the mouse.\n\n"
            "'h' hide this message\n"
            "'s' to save positions\n"
            "'l' to load positions\n"
            "'q' to load positions\n"
            "DEL deleted the selected shape.";

    if (showHelp) c.setScreenText(helpMsg);
    else c.setScreenText("");

    showHelp = ! showHelp;
}

int main()
{
    Mat image = Mat::zeros(768, 1024, CV_8UC3);
    Canvas c("Counting People", image.size());
    c.enableScreenText();

    help(c);

    c.enableStatusMsg();

    c.setShapeType(LineCrossing::type); // default shape type for direct GUI creation

    namedWindow("Counting People", WINDOW_AUTOSIZE);
    c.setMouseCallback(); // optional for mouse usage see also (example_selectbox.cpp)

    TrackerSimulator tracker(c);
    auto buttonAlignment = VerticalLayout::create(c);
    buttonAlignment->setStretchXToParent(true);
    auto button = Button::create(*buttonAlignment,
                                 Point(image.cols / 2, 0),
                                 "Start / Stop\nSimulation",
                                 "", [&tracker](Widget *w)
    {
            tracker.setSimulatorOn(! tracker.getSimulatorOn());
    });
    button->setLayoutAnchor(Widget::CENTER);

    int minStepsPerSeconds = min(image.cols / 2, image.rows / 2) / 10;
    int maxStepsPerSeconds = max(image.cols / 2, image.rows / 2) / 10;
    Point center(image.cols / 2, image.rows / 2);
    int key = 0;
    Mat out;
    int fps = 25;
    int delay = 1000 / fps;
    do
    {
        tracker.simulate(2, center, 50, minStepsPerSeconds, maxStepsPerSeconds);
        switch (key)
        {
        case 'h':
            help(c);
            break;
        case 's':
            c.writeShapesToFile("line-crossings.xml");
            break;
        case 'l':
            c.readShapesFromFile("line-crossings.xml");
            break;
        case 65535:
            c.deleteActive();
            break;
        }

        c.redrawOn(image, out);
        c.imshow(out);
        key = c.waitKeyEx(delay); // GUI and callbacks happen here
    } while (key != 'q');

    destroyAllWindows();

    return 0;
}




