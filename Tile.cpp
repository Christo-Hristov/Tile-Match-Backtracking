#include "Tile.h"
#include "error.h"
#include "strlib.h"
#include "testing/SimpleTest.h"
#include "vector.h"
#include <cctype>

using namespace std;

Tile::Tile() {
    for (int i = 0; i < NUM_SIDES; i++) {
        sides[i] = "X";
    }
    orientation = 0; // top faces up
}

Tile::Tile(string s) {
    orientation = 0;
    string number; // initializing a string to hold the number representing the orientation
    if (s.length() == 9) {
        number += (s[8]); // if the string specifies orientation of the tile, add the orientation to the string
        orientation = stringToInteger(number); // convert the string to integer
    }
    string edges = s.substr(0, 7);
    for (int i = 0; i < 7; i += 2) {
        sides[i / 2] = edges[i]; // fill in the sides string with the sides given skipping the spaces of the inputted string
    }
}

int Tile::getOrientation() {
    return orientation;
}

void Tile::setOrientation(int newOrientation) {
    orientation = newOrientation;
}

string Tile::sidesStr() {
    string result = "";
    for (string side : sides) {
        result += side;
    }
    return result;
}

Vector<string> Tile::orientate() {
    Vector<string> result(4, "0"); // creates a Vector with 4 values to be replaced
    int newIndex = 0;
    for (int i = 0; i < NUM_SIDES; i++) {
        newIndex = (i + orientation) % 4; // this is the calculation that will shift the side accordingly
        result[newIndex] = sides[i];
    }
    return result;
}


bool Tile::isMatched(Tile &other, Connection otherLocation) {
    Vector<string> myTile = orientate(); // return a vector with orientated postions of the tile
    Vector<string> otherTile = other.orientate(); // return a vector with orientated postions of the other given tile
    if (otherLocation == Tile::ABOVE) {
        // check if corresponding positions are matched
        return (toLowerCase(myTile[0]) == toLowerCase(otherTile[2]) && (myTile[0] != otherTile[2]));
    }
    if (otherLocation == Tile::RIGHT) {
        // check if corresponding positions are matched
        return (toLowerCase(myTile[1]) == toLowerCase(otherTile[3]) && (myTile[1] != otherTile[3]));
    }
    if (otherLocation == Tile::BELOW) {
        // check if corresponding positions are matched
        return (toLowerCase(myTile[2]) == toLowerCase(otherTile[0]) && (myTile[2] != otherTile[0]));
    }
    // check if corresponding positions given the other tile is to the left are matched
    return (toLowerCase(myTile[3]) == toLowerCase(otherTile[1]) && (myTile[3] != otherTile[1]));
}

/* The functions below have already been written for you! */

ostream &operator<<(ostream &out, Tile const &tile) {
    // prints as follows:
    // A B C D 0
    // Which means that with an orientation of 0, the piece
    // would look like this:
    //   A
    // C   B
    //   D
    //
    // If we had this:
    // A B C D 3
    // The piece would look like this, because it has been
    // rotated clockwise by 270 degrees:
    //   B
    // A   C
    //   D
    //
    for (string s : tile.sides) {
        out << s << " ";
    }
    out << tile.orientation;
    return out;
}

string Tile::displayTileStr() {
    string s;
    int start = (4 - orientation) % 4;
    s += "  " + sides[start] + "\n";
    s += sides[(start + 3) % 4];
    s += "   " + sides[(start + 1) % 4] + "\n";
    s += "  " + sides[(start + 2) % 4] + "\n";
    return s;
}

PROVIDED_TEST("Testing Display Form") {
    Tile t1("A a B b 3");
    stringstream ss;
    ss << t1.displayTileStr();
    EXPECT("  a\nA   B\n  b\n" == ss.str());
}

PROVIDED_TEST("Testing Display Form") {
    Tile t1("C c B a");
    stringstream ss;
    ss << t1.displayTileStr();
    EXPECT("  C\na   c\n  B\n" == ss.str());
}

PROVIDED_TEST("Testing << overload") {
    Tile t1("A a B b 3");
    stringstream ss;
    ss << t1;
    EXPECT_EQUAL(ss.str(), "A a B b 3");
}

PROVIDED_TEST("Testing << overload") {
    Tile t1("C c B a");
    stringstream ss;
    ss << t1;
    EXPECT_EQUAL(ss.str(), "C c B a 0");
}

PROVIDED_TEST("Testing getOrientation()") {
    Tile t1("A a B b 3");
    Tile t2("C c B a");
    EXPECT_EQUAL(3, t1.getOrientation());
    EXPECT_EQUAL(0, t2.getOrientation());
}

PROVIDED_TEST("Testing setOrientation()") {
    Tile t1("A a B b 3");
    Tile t2("C c B a");
    t1.setOrientation(1);
    t2.setOrientation(1);
    stringstream ss1, ss2;
    ss1 << t1;
    ss2 << t2;
    EXPECT_EQUAL(1, t1.getOrientation());
    EXPECT_EQUAL(1, t2.getOrientation());
    EXPECT_EQUAL("A a B b 1", ss1.str());
    EXPECT_EQUAL("C c B a 1", ss2.str());
}

PROVIDED_TEST("Testing isMatched()") {
    Tile t1("A a B b 3");
    Tile t2("C c B a 2");

    /*
        t1:
           a
         A   B
           b

    t2:
        B
                    c   a
                        C
                            */

    cout << endl << "t1:" << endl;
    cout << t1.displayTileStr() << endl;
    cout << endl << "t2:" << endl;
    cout << t2.displayTileStr() << endl;

    cout << "if t2 is above t1, no match" << endl;
    EXPECT(!t1.isMatched(t2, Tile::ABOVE));

    cout << "if t2 is below t1, match" << endl;
    EXPECT(t1.isMatched(t2, Tile::BELOW));

    cout << "if t2 is to the left of t1, match" << endl;
    EXPECT(t1.isMatched(t2, Tile::LEFT));

    cout << "if t2 is to the right of t1, no match" << endl;
    EXPECT(!t1.isMatched(t2, Tile::RIGHT));

    cout << "if t1 is above t2, match" << endl;
    EXPECT(t2.isMatched(t1, Tile::ABOVE));

    cout << "if t1 is below t2, no match" << endl;
    EXPECT(!t2.isMatched(t1, Tile::BELOW));

    cout << "if t1 is to the left of t2, no match" << endl;
    EXPECT(!t2.isMatched(t1, Tile::LEFT));

    cout << "if t1 is to the right of t2, match" << endl;
    EXPECT(t2.isMatched(t1, Tile::RIGHT));
}

/*
STUDENT_TEST("orientate with non orientated tile") {
    Tile t1("A a B b 3");
    Vector<string> check = {"a", "B", "b", "A"};
    EXPECT_EQUAL(t1.orientate(), check);
}

STUDENT_TEST("orientate with orientated tile") {
    Tile t1("A a B b 0");
    Vector<string> check = {"A", "a", "B", "b"};
    EXPECT_EQUAL(t1.orientate(), check);
}

These tests have to be commented out becuase orientate is a private function.
*/
