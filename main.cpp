#include <iostream>
#include <map>
#include "wex.h"
#include "cRunWatch.h"


using namespace std;

class cL
{
public:
    cL()
        : myAlphabet("AB+-")
    {
    }

    void germinate(
        const std::map< char, std::string>& rules,
        const std::string& axiom,
        int generations )
    {
            raven::set::cRunWatch aWatcher("germinate");
        myRules = rules;
        myMaxDepth = generations;
        myDepth = 0;
        myPlant = axiom;
        grow();
    }

    std::string plant()
    {
        return myPlant;
    }

private:
    std::string myAlphabet;
    std::map< char, std::string> myRules;
    int myDepth;
    int myMaxDepth;
    std::string myPlant;

    std::string production( const char c )
    {

        if( (int)myAlphabet.find( c ) < 0 )
            throw std::runtime_error(
                "production character not in alphabet " );
        auto it = myRules.find( c );
        if( it == myRules.end() )
            throw std::runtime_error(
                "production missing rule");

        return it->second;
    }

    /// recursive growth

    void grow()
    {
        raven::set::cRunWatch aWatcher("grow");
        // std::cout << myPlant << "\n";

        // check for completion
        if( myDepth == myMaxDepth )
        {
            std::cout << myPlant << "\n\n";
            return;
        }

        // produce the next growth spurt
        std::string next;
        for( auto c : myPlant )
        {
            next += production( c );
        }
        myPlant = next;

        // recurse
        myDepth++;
        grow();
    }
};

void Draw( const std::string& plant )
{

    // construct top level application window
    wex::gui& form = wex::maker::make();
    form.move({ 50,50,800,800});
    form.text("Hello world from windex");

    form.events().draw([&]( PAINTSTRUCT& ps )
    {
             raven::set::cRunWatch aWatcher("draw");

        int x = 10;
        int y = 10;
        int xinc = 10;
        int yinc = 0;
        float angle;
        wex::shapes S( ps );
        //S.text( "hello world", ps.rcPaint );
        for( auto c : plant )
        {
            switch( c )
            {
            case 'A':
            case 'B':
                break;;
            case '+':
                angle += 1;
                xinc = 5 * cos( angle );
                yinc = 5 * sin( angle );
                break;
            case '-':
                angle -= 1;
                xinc = 5 * cos( angle );
                yinc = 5 * sin( angle );
                break;
            }
            S.line( { x,y,x+xinc,y+yinc});
            x += xinc;
            y += yinc;
        }
    });

    // show the application
    form.show();

    //Pass the control of the application to the windows message queue.
    form.run();
}

std::map< char, std::string> RulesAlgae()
{
    std::map< char, std::string> Rules;
    Rules.insert(std::make_pair('A',std::string("AB")));
    Rules.insert(std::make_pair('B',std::string("A")));
    return Rules;
}

std::map< char, std::string> RulesSierpinski()
{
    // https://en.wikipedia.org/wiki/L-system#Example_5:_Sierpinski_triangle

    std::map< char, std::string> Rules;
    Rules.insert(std::make_pair('A',std::string("B-A-B")));
    Rules.insert(std::make_pair('B',std::string("A+B+A")));
    Rules.insert(std::make_pair('+',std::string("+")));
    Rules.insert(std::make_pair('-',std::string("-")));
    return Rules;
}

int main()
{
    raven::set::cRunWatch::Start();
    raven::set::cRunWatch aWatcher("application");
    cL L;

    L.germinate(
        RulesSierpinski(),
        "A", 10 );

    Draw( L.plant() );

    raven::set::cRunWatch::Report();

    return 0;
}
