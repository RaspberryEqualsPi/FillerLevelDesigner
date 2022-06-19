#include <map>
#include <sstream>
#include <fstream>
#include <iostream>
#include "FillerUI.h"
#include "ObjectTypes.h"
#include "rapidjson/document.h"
using namespace rapidjson;
using namespace std;
int startY = 0;
int startX = 0;
std::vector<FillerUI::RectButton> tiles;
std::vector<FillerUI::CircleButton> tokens;
sf::RenderWindow window(sf::VideoMode(320, 384), "Filler Level Creator");
string getexepath()
{
    char szFilePath[MAX_PATH + 1] = { 0 };
    GetModuleFileNameA(NULL, szFilePath, MAX_PATH);
    /*
    strrchr: function function: find the position of the last occurrence of a character c in another string str (that is, find the position of the first occurrence of the character c from the right side of str),
     And return the address of this location. If the specified character cannot be found, the function returns NULL.
     Use this address to return the string from the last character c to the end of str.
    */
    (strrchr(szFilePath, '\\'))[0] = 0; // Delete the file name, only get the path string //
    string path = szFilePath;
    return path;
}
void loadlevel(std::string filename) {
    string json;
    string line;
    ifstream f(filename);
    if (f.is_open())
    {
        while (getline(f, line))
        {
            json = json + line;
        }
        f.close();
    }
    Document level;
    level.Parse(json.c_str());
    for (int i = 0; i < tokens.size(); i++) {
        tokens[i].state = false;
        tokens[i].arrowstate = false;
        tokens[i].checkpointstate = false;
        tiles[i].state = false;
        tokens[i].startPos = false;
        startX = level["StartingX"].GetInt();
        startY = level["StartingY"].GetInt();
        if (tokens[i].x == level["StartingX"].GetInt() && tokens[i].y == level["StartingY"].GetInt()) {
            tokens[i].startPos = true;
        }
    }
    if (level.HasMember("Tokens")) {
        for (size_t i = 0; i < level["Tokens"].Size(); i++) {
            const Value& token1 = level["Tokens"][i];
            Token token;
            token.x = token1["x"].GetInt();
            token.y = token1["y"].GetInt();
            for (int i = 0; i < tokens.size(); i++) {
                if (token.x == tokens[i].x && token.y == tokens[i].y) {
                    tokens[i].state = true;
                    tokens[i].arrowstate = false;
                    tokens[i].checkpointstate = false;
                    //tiles[i].state = false;
                    std::cout << "Token: " << i << std::endl;
                }
                else {
                    tiles[i].state = true;
                    //tokens[i].state = false;
                }
            }
        }
    }
    if (level.HasMember("ArrowShooters")) {
        for (size_t i = 0; i < level["ArrowShooters"].Size(); i++) {
            const Value& Arrowshooter1 = level["ArrowShooters"][i];
            for (int i = 0; i < tokens.size(); i++) {
                if (Arrowshooter1["x"].GetInt() == tokens[i].x && Arrowshooter1["y"].GetInt() == tokens[i].y) {
                    tokens[i].arrowstate = true;
                    tokens[i].checkpointstate = false;
                    tokens[i].state = false;
                    tokens[i].aType = Arrowshooter1["type"].GetInt();
                    std::cout << "ArrowShooter: " << i << std::endl;
                    //tiles[i].state = false;
                }
                else {
                    tiles[i].state = true;
                    //tokens[i].state = false;
                }
            }
        }
    }
    if (level.HasMember("Checkpoints")) {
        for (size_t i = 0; i < level["Checkpoints"].Size(); i++) {
            const Value& Checkpoint1 = level["Checkpoints"][i];
            for (int i = 0; i < tokens.size(); i++) {
                if (Checkpoint1["x"].GetInt() == tokens[i].x && Checkpoint1["y"].GetInt() == tokens[i].y) {
                    tokens[i].arrowstate = false;
                    tokens[i].checkpointstate = true;
                    tokens[i].state = true;
                    //tokens[i].state = false;
                    std::cout << "Checkpoint: " << i << std::endl;
                    //tiles[i].state = false;
                }
                else {
                    tiles[i].state = true;
                    //tokens[i].state = false;
                }
            }
        }
    }
}
int main()
{
    window.setFramerateLimit(60);
    window.setKeyRepeatEnabled(false);
    FillerUI::TextButton startB;
    startB.text = "Set StartPos";
    startB.sfText.setLetterSpacing(0.8);
    startB.rect.setPosition(320 - 16 - startB.sizeX, 320);
    FillerUI::TextButton saveB;
    saveB.text = "Save";
    saveB.rect.setPosition(16, 320);
    FillerUI::TextButton loadB;
    loadB.text = "Load JSON";
    float Width1 = 320;
    float Width2 = loadB.rect.getGlobalBounds().width;
    float X2 = loadB.rect.getPosition().x;
    loadB.rect.setPosition(((Width1 - Width2) / 2), 320);
    FillerUI::TextButton arrowB;
    arrowB.sizeX = 110;
    arrowB.text = "Set ArrowShooter";
    Width1 = 320;
    Width2 = arrowB.sizeX;
    X2 = arrowB.rect.getPosition().x;
    arrowB.rect.setPosition(((Width1 - Width2) / 2), 352);
    arrowB.sfText.setLetterSpacing(0.6);
    for (int x = 0; x < 20; x++) {
        for (int y = 0; y < 20; y++) {
            FillerUI::RectButton button;
            button.x = x;
            button.y = y;
            tiles.push_back(button);
            FillerUI::CircleButton cbutton;
            cbutton.x = x;
            cbutton.y = y;
            tokens.push_back(cbutton);
        }
    }
    while (window.isOpen())
    {
        sf::Event event;
        bool debounce = false;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::MouseButtonPressed) {
                if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
                    FillerUI::SetClicking(true);
                if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
                    FillerUI::SetRightClicking(true);
            }
        }
        window.clear(sf::Color::White);
        for (int i = 0; i < tiles.size(); i++) {\
            if (tokens[i].state) {
                tiles[i].state = false;
            }
            else
                tiles[i].state = true;
            if(tiles[i].state)
                tiles[i].draw(&window);
            if (tiles[i].clicked(&window) && tiles[i].state) {
                tiles[i].state = false;
                tokens[i].state = true;
                tokens[i].checkpointstate = false;
                debounce = true;
            }
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) {
            FillerUI::globalAtype = 1;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) {
            FillerUI::globalAtype = 0;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) {
            FillerUI::globalAtype = 3;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
            FillerUI::globalAtype = 2;
        }
        if (!debounce) {
            for (int i = 0; i < tokens.size(); i++) {
                if (tokens[i].state || tokens[i].arrowstate || tokens[i].checkpointstate)
                    tokens[i].draw(&window);
                if (tokens[i].clicked(&window) && !tiles[i].state && !FillerUI::StartPos && !FillerUI::ArrowShooter) {
                    tokens[i].state = false;
                    tokens[i].startPos = false;
                    tokens[i].arrowstate = false;
                    tokens[i].checkpointstate = false;
                    tiles[i].state = true;
                }
                if (tokens[i].clicked(&window) && tokens[i].state && FillerUI::StartPos && !FillerUI::ArrowShooter) {
                    tokens[i].startPos = true;
                    startY = tokens[i].y;
                    startX = tokens[i].x;
                    FillerUI::StartPos = false;
                }
                if (tokens[i].rclicked(&window) && !FillerUI::StartPos && !FillerUI::ArrowShooter) {
                    tokens[i].arrowstate = false;
                    tokens[i].checkpointstate = true;
                }
                if (tokens[i].clicked(&window) && tokens[i].state && !FillerUI::StartPos && FillerUI::ArrowShooter) {
                    tokens[i].state = false;
                    tokens[i].checkpointstate = false;
                    tokens[i].arrowstate = true;
                    tokens[i].aType = FillerUI::globalAtype;
                    FillerUI::ArrowShooter = false;
                }
                
            }
        }
        if (startB.clicked(&window)) {
            FillerUI::StartPos = true;
        }
        if (arrowB.clicked(&window)) {
            FillerUI::ArrowShooter = true;
        }
        if (saveB.clicked(&window)) {
            char filename[MAX_PATH];
            OPENFILENAME ofn;
            ZeroMemory(&filename, sizeof(filename));
            ZeroMemory(&ofn, sizeof(ofn));
            ofn.lStructSize = sizeof(ofn);
            ofn.hwndOwner = window.getSystemHandle();
            ofn.lpstrFilter = "JSON Files\0*.json\0Any File\0*.*\0";
            ofn.lpstrFile = filename;
            ofn.nMaxFile = MAX_PATH;
            ofn.lpstrTitle = "Save Level";
            ofn.Flags = OFN_DONTADDTORECENT;

            if (GetOpenFileNameA(&ofn))
            {
                bool firstTime = true;
                std::string output;
                for (int i = 0; i < tokens.size(); i++) {
                    if (tokens[i].state && !tokens[i].arrowstate && !tokens[i].checkpointstate) {
                        if (firstTime) {
                            output = "{\"StartingY\": " + std::to_string((int)startY) + ", \"StartingX\": " + std::to_string((int)startX) + R"(, "Tokens": [)";
                            firstTime = false;
                        }
                        std::cout << "Token: " << i << std::endl;
                        output = output + R"({"x": )" + std::to_string((int)tokens[i].x) + R"(, "y": )" + std::to_string((int)tokens[i].y) + "},";
                    }
                }
                output.erase(output.size());
                char* outputchar = (char*)output.c_str();
                (strrchr(outputchar, ','))[0] = 0;
                output = outputchar;
                output = output + "],";
                output = output + "\"ArrowShooters\": [";
                for (int i = 0; i < tokens.size(); i++) {
                    if (tokens[i].arrowstate) {
                        std::cout << "ArrowShooter: " << i << std::endl;
                        output = output + R"({"x": )" + std::to_string((int)tokens[i].x) + R"(, "y": )" + std::to_string((int)tokens[i].y) + R"(, "type": )" + std::to_string((int)tokens[i].aType) + "},";
                    }
                }
                output.erase(output.size());
                outputchar = (char*)output.c_str();
                if (output.back() == ',')
                    (strrchr(outputchar, ','))[0] = 0;
                output = outputchar;
                output = output + "],";
                output = output + "\"Checkpoints\": [";
                for (int i = 0; i < tokens.size(); i++) {
                    if (tokens[i].checkpointstate) {
                        std::cout << "Checkpoint: " << i << std::endl;
                        output = output + R"({"x": )" + std::to_string((int)tokens[i].x) + R"(, "y": )" + std::to_string((int)tokens[i].y) + "},";
                    }
                }
                output.erase(output.size());
                outputchar = (char*)output.c_str();
                if(output.back() == ',')
                    (strrchr(outputchar, ','))[0] = 0;
                output = outputchar;
                output = output + "]}";
                ofstream lvl;
                lvl.open(filename);
                lvl << output;
                lvl.close();
            }
            else
            {
                switch (CommDlgExtendedError())
                {
                case CDERR_DIALOGFAILURE: std::cout << "CDERR_DIALOGFAILURE\n";   break;
                case CDERR_FINDRESFAILURE: std::cout << "CDERR_FINDRESFAILURE\n";  break;
                case CDERR_INITIALIZATION: std::cout << "CDERR_INITIALIZATION\n";  break;
                case CDERR_LOADRESFAILURE: std::cout << "CDERR_LOADRESFAILURE\n";  break;
                case CDERR_LOADSTRFAILURE: std::cout << "CDERR_LOADSTRFAILURE\n";  break;
                case CDERR_LOCKRESFAILURE: std::cout << "CDERR_LOCKRESFAILURE\n";  break;
                case CDERR_MEMALLOCFAILURE: std::cout << "CDERR_MEMALLOCFAILURE\n"; break;
                case CDERR_MEMLOCKFAILURE: std::cout << "CDERR_MEMLOCKFAILURE\n";  break;
                case CDERR_NOHINSTANCE: std::cout << "CDERR_NOHINSTANCE\n";     break;
                case CDERR_NOHOOK: std::cout << "CDERR_NOHOOK\n";          break;
                case CDERR_NOTEMPLATE: std::cout << "CDERR_NOTEMPLATE\n";      break;
                case CDERR_STRUCTSIZE: std::cout << "CDERR_STRUCTSIZE\n";      break;
                case FNERR_BUFFERTOOSMALL: std::cout << "FNERR_BUFFERTOOSMALL\n";  break;
                case FNERR_INVALIDFILENAME: std::cout << "FNERR_INVALIDFILENAME\n"; break;
                case FNERR_SUBCLASSFAILURE: std::cout << "FNERR_SUBCLASSFAILURE\n"; break;
                default: std::cout << "You cancelled.\n";
                }
            }
        }
        if (loadB.clicked(&window)) {
            char filename[MAX_PATH];
            OPENFILENAME ofn;
            ZeroMemory(&filename, sizeof(filename));
            ZeroMemory(&ofn, sizeof(ofn));
            ofn.lStructSize = sizeof(ofn);
            ofn.hwndOwner = window.getSystemHandle();
            ofn.lpstrFilter = "JSON Files\0*.json\0Any File\0*.*\0";
            ofn.lpstrFile = filename;
            ofn.nMaxFile = MAX_PATH;
            ofn.lpstrTitle = "Save Level";
            ofn.Flags = OFN_DONTADDTORECENT;

            if (GetOpenFileNameA(&ofn))
            {
                loadlevel(filename);
            }
            else
            {
                switch (CommDlgExtendedError())
                {
                case CDERR_DIALOGFAILURE: std::cout << "CDERR_DIALOGFAILURE\n";   break;
                case CDERR_FINDRESFAILURE: std::cout << "CDERR_FINDRESFAILURE\n";  break;
                case CDERR_INITIALIZATION: std::cout << "CDERR_INITIALIZATION\n";  break;
                case CDERR_LOADRESFAILURE: std::cout << "CDERR_LOADRESFAILURE\n";  break;
                case CDERR_LOADSTRFAILURE: std::cout << "CDERR_LOADSTRFAILURE\n";  break;
                case CDERR_LOCKRESFAILURE: std::cout << "CDERR_LOCKRESFAILURE\n";  break;
                case CDERR_MEMALLOCFAILURE: std::cout << "CDERR_MEMALLOCFAILURE\n"; break;
                case CDERR_MEMLOCKFAILURE: std::cout << "CDERR_MEMLOCKFAILURE\n";  break;
                case CDERR_NOHINSTANCE: std::cout << "CDERR_NOHINSTANCE\n";     break;
                case CDERR_NOHOOK: std::cout << "CDERR_NOHOOK\n";          break;
                case CDERR_NOTEMPLATE: std::cout << "CDERR_NOTEMPLATE\n";      break;
                case CDERR_STRUCTSIZE: std::cout << "CDERR_STRUCTSIZE\n";      break;
                case FNERR_BUFFERTOOSMALL: std::cout << "FNERR_BUFFERTOOSMALL\n";  break;
                case FNERR_INVALIDFILENAME: std::cout << "FNERR_INVALIDFILENAME\n"; break;
                case FNERR_SUBCLASSFAILURE: std::cout << "FNERR_SUBCLASSFAILURE\n"; break;
                default: std::cout << "You cancelled.\n";
                }
            }
        }
        if (!SetCurrentDirectoryA(getexepath().c_str())) { printf("SetCurrentDirectory failed (%d)\n", GetLastError()); }
        saveB.draw(&window);
        loadB.draw(&window);
        startB.draw(&window);
        arrowB.draw(&window);
        FillerUI::SetClicking(false);
        FillerUI::SetRightClicking(false);
        window.display();
    }
    return 0;
}