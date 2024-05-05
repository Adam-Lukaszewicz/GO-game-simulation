#pragma once
#include <SFML/Graphics.hpp>
#include "functions.h"
#define mousePosition sf::Vector2f(event.mouseMove.x, event.mouseMove.y)
#define mouseClick sf::Vector2f(event.mouseButton.x, event.mouseButton.y)

enum JunctionType {
    Middle,
    Side,
    Corner
};

enum Ownership {
    Unclaimed,
    White,
    Black
};

class Junction {
private:
    JunctionType type;
    Ownership state;
    sf::CircleShape shape;
    std::vector<std::shared_ptr<Junction>> neighbours;
    bool inChain = false;
    //InGroupWith - jakis wektor pointerow na to z czym jest w grupie, zeby umozliwic jednoczesna aktualizacje?-
    int liberties;
    void updateLook() {
        sf::FloatRect old = this->shape.getGlobalBounds();
        switch (this->state) {
        case Ownership::Unclaimed:
            shape.setRadius(5.f);
            shape.setFillColor(sf::Color::Black);
            break;
        case Ownership::White:
            shape.setRadius(15.f);
            shape.setFillColor(sf::Color::White);
           
            break;
        case Ownership::Black:
            shape.setRadius(15.f);
            shape.setFillColor(sf::Color::Black);
            break;
        }
        sf::FloatRect updated = this->shape.getGlobalBounds();
        if(old != updated)
        shape.move((old.width - updated.width) / 2.0, (old.height - updated.height) / 2.0);
    }
public:
    Junction(){        
        shape.setPointCount(20);       
        this->state = Ownership::Unclaimed;
        updateLook();
    }
    sf::CircleShape getShape() {
        return this->shape;
    }
    sf::Vector2f getPosition() {
        return this->shape.getPosition();
    }
    sf::FloatRect getGlobalBounds() {
        return this->shape.getGlobalBounds();
    }
    Ownership getState() {
        return this->state;
    }
    void setOrigin(sf::RectangleShape& board) {
        this->shape.setOrigin(board.getPosition());
    }
    void setPosition(const sf::Vector2f& n_position) {
        this->shape.setPosition(n_position);
    }
    void setType(JunctionType n_junction) {
        switch (n_junction) {
        case JunctionType::Corner:
            this->liberties = 2;
            break;
        case JunctionType::Side:
            this->liberties = 3;
            break;
        case JunctionType::Middle:
            this->liberties = 4;
            break;
        }
    }
    void changeState(Ownership n_state) {
        this->state = n_state;
        updateLook();
    }
    void updateLiberties() {
        int n_liberties = 0;
    }
    void addNeighbour(std::shared_ptr<Junction> neighbour){
        this->neighbours.push_back(neighbour);
    }
};

class Chain {
public:
    std::vector<std::shared_ptr<Junction>> links;
    Ownership color;
    Chain(std::shared_ptr<Junction> first){
        links.push_back(first);
        this->color = first->getState();
    }
    void addToChain(std::shared_ptr<Junction> fresh){
        this->links.push_back(fresh);
    }
    void update() {
        int liberties{ 0 };
        std::vector<Junction> checked;
        for (auto it = links.begin(); it != links.end(); it++) {

        }
    }
};

class sceneCoordinator;

class GameEngine {
public:
    GameEngine() {
        this->turn = Ownership::Black;
        this->window.create(sf::VideoMode(1000, 1000), "GO Game");
        this->font.loadFromFile("Fonts/arial.ttf");
        this->arrowCursor.loadFromSystem(sf::Cursor::Arrow);
        this->handCursor.loadFromSystem(sf::Cursor::Hand);
        //this->mainMenuInst.show();
    }
    //void chooseSceneCoordinator()

    friend class mainMenu;
    friend class helpScene;
    friend class gameScene;
    friend class sceneCoordinator;

private:
    sf::RenderWindow window;
    sf::Font font;
    sf::Cursor arrowCursor, handCursor;
    Ownership turn;
    void changeTurn() {
        switch (this->turn) {
        case Ownership::Black:
            this->turn = Ownership::White;
            break;
        case Ownership::White:
            this->turn = Ownership::Black;
            break;
        }
    }

    //helpScene helpSceneInst;
    //gameScene gameSceneInst;
    //mainMenu mainMenuInst;
};

class mainMenu{
private:
    std::shared_ptr<GameEngine> engine;
    sf::RectangleShape quickPlay;
    sf::RectangleShape help;
    sf::RectangleShape quit;
    sf::Text quickPlayText;
    sf::Text helpText;
    sf::Text quitText;
public:
    friend class sceneCoordinator;
    mainMenu(std::shared_ptr<GameEngine> n_engine) {
        this->engine = n_engine;
        this->quickPlay.setSize(sf::Vector2f(100.f, 50.f));
        this->quickPlay.setFillColor(sf::Color::Green);
        quickPlay.setPosition(centerPosition(engine->window, quickPlay));
        this->quickPlayText.setFont(engine->font);
        this->quickPlayText.setCharacterSize(18);
        this->quickPlayText.setString("Szybka gra");
        this->quickPlayText.setPosition(centerPosition(quickPlay, quickPlayText));

        this->help.setSize(sf::Vector2f(100.f, 50.f));
        this->help.setFillColor(sf::Color::Green);
        help.setPosition(centerPosition(engine->window, help).x, centerPosition(engine->window, help).y + 80);
        this->helpText.setFont(engine->font);
        this->helpText.setCharacterSize(18);
        this->helpText.setString("Pomoc");
        this->helpText.setPosition(centerPosition(help, helpText));

        this->quit.setSize(sf::Vector2f(100.f, 50.f));
        this->quit.setFillColor(sf::Color::Green);
        quit.setPosition(centerPosition(engine->window, quit).x, centerPosition(engine->window, quit).y + 160);
        this->quitText.setFont(engine->font);
        this->quitText.setCharacterSize(18);
        this->quitText.setString("Wyjdz");
        this->quitText.setPosition(centerPosition(quit, quitText));
    }


};
class gameScene{
private:
    std::shared_ptr<GameEngine> engine;
    sf::RectangleShape board;
    std::vector<std::vector<std::shared_ptr<Junction>>> gameBoard;
    std::vector<sf::RectangleShape> lines;
    std::vector<sf::FloatRect> postionCache;
public:
    friend class sceneCoordinator;
    gameScene(std::shared_ptr<GameEngine> n_engine) {
        this->engine = n_engine;
        //this->board.setSize(sf::Vector2f(500.f, 500.f));
        this->board.setFillColor(sf::Color::Color(199, 167, 78));
        
    }
    void setGameBoard(int boardSize) {
        this->board.setSize(sf::Vector2f(40 + ((boardSize - 1) * 60), 40 + ((boardSize - 1) * 60)));
        this->board.setPosition(this->engine->window.getSize().x * 0.1, centerPosition(this->engine->window, this->board).y);
        this->gameBoard.clear();
        for (int i = 0; i < boardSize; i++) {
            std::vector<std::shared_ptr<Junction>> temp;
            sf::RectangleShape horizontal(sf::Vector2f(60 * (boardSize - 1), 2));
            horizontal.setFillColor(sf::Color::Black);
            for (int j = 0; j < boardSize; j++) {
                std::shared_ptr<Junction> newJ = std::make_shared<Junction>();
                sf::RectangleShape vertical(sf::Vector2f(2, 60 * (boardSize - 1)));
                
                vertical.setFillColor(sf::Color::Black);
                newJ->setPosition(sf::Vector2f(this->board.getPosition().x + 20 + (60 * j) - newJ->getShape().getGlobalBounds().width * 0.5, this->board.getPosition().y + 20 + (60 * i) - newJ->getShape().getGlobalBounds().height * 0.5));
                postionCache.push_back(newJ->getGlobalBounds());
                vertical.setPosition(newJ->getPosition().x + newJ->getShape().getGlobalBounds().width * 0.5, newJ->getPosition().y + newJ->getShape().getGlobalBounds().height * 0.5);
                if(j == 0)
                horizontal.setPosition(newJ->getPosition().x + newJ->getShape().getGlobalBounds().width * 0.5, newJ->getPosition().y + newJ->getShape().getGlobalBounds().height * 0.5);
                if (i == 0 or i == boardSize - 1) {
                    if (j == i or j == boardSize - 1) {
                        newJ->setType(JunctionType::Corner);
                    }
                    else {
                        newJ->setType(JunctionType::Side);
                    }
                }
                else {
                    if (j == i or j == boardSize - 1) {
                        newJ->setType(JunctionType::Side);
                    }
                    else {
                        newJ->setType(JunctionType::Middle);
                    }
                }
                temp.push_back(newJ);
                
                if(i == 0)
                lines.push_back(vertical);
            }
            lines.push_back(horizontal);
            this->gameBoard.push_back(temp);
        }
        for (int i = 0; i < boardSize; i++) {
            for (int j = 0; j < boardSize; j++) {
                if (i != 0)
                    this->gameBoard[i][j]->addNeighbour(this->gameBoard[i - 1][j]);
                else
                    this->gameBoard[i][j]->addNeighbour(nullptr);
                if(j != boardSize-1)
                    this->gameBoard[i][j]->addNeighbour(this->gameBoard[i][j+1]);
                else
                    this->gameBoard[i][j]->addNeighbour(nullptr);
                if (i != boardSize - 1)
                    this->gameBoard[i][j]->addNeighbour(this->gameBoard[i + 1][j]);
                else
                    this->gameBoard[i][j]->addNeighbour(nullptr);
                if (j != 0)
                    this->gameBoard[i][j]->addNeighbour(this->gameBoard[i][j - 1]);
                else
                    this->gameBoard[i][j]->addNeighbour(nullptr);

            }
        }
    }
};
class helpScene{
private:
    std::shared_ptr<GameEngine> engine;
    sf::Text placeholder;
public:
    friend class sceneCoordinator;
    helpScene(std::shared_ptr<GameEngine> n_engine) {
        this->engine = n_engine;
        this->placeholder.setFont(engine->font);
        this->placeholder.setCharacterSize(18);
        this->placeholder.setString("WIP");
    }
};

//sf::RenderWindow GameEngine::window;
//sf::Font GameEngine::font;
//sf::Cursor GameEngine::arrowCursor;
//sf::Cursor GameEngine::handCursor;


class sceneCoordinator {
private:
    std::shared_ptr<GameEngine> engine;
    std::shared_ptr<mainMenu> mainMenuInst;
    std::shared_ptr<helpScene> helpSceneInst;
    std::shared_ptr<gameScene> gameSceneInst;
public:
    sceneCoordinator(std::shared_ptr<GameEngine> n_engine, std::shared_ptr<mainMenu> n_mainMenuInst, std::shared_ptr<helpScene> n_helpSceneInst, std::shared_ptr<gameScene> n_gameSceneInst) {
        this->engine = n_engine;
        this->mainMenuInst = n_mainMenuInst;
        this->helpSceneInst = n_helpSceneInst;
        this->gameSceneInst = n_gameSceneInst;
        showMainMenu();
    }
    void showGameScene() {
        bool isActive = true;
        while (isActive) {
            sf::Event event;
            this->engine->window.clear();
            this->engine->window.draw(this->gameSceneInst->board);
            for (int i = 0; i < this->gameSceneInst->lines.size(); i++) {
                this->engine->window.draw(this->gameSceneInst->lines[i]);
            }
            for (int i = 0; i < this->gameSceneInst->gameBoard.size(); i++) {
                for (int j = 0; j < this->gameSceneInst->gameBoard.size(); j++) {
                    this->engine->window.draw(this->gameSceneInst->gameBoard[i][j]->getShape());
                }
            }
            while (this->engine->window.pollEvent(event)) {
                if (event.type == sf::Event::MouseMoved) {
                    for (int i = 0; i < this->gameSceneInst->postionCache.size(); i++) {
                        if (this->gameSceneInst->postionCache[i].contains(mousePosition)) {
                            engine->window.setMouseCursor(engine->handCursor);
                            i = this->gameSceneInst->postionCache.size();
                        }
                        else {
                                engine->window.setMouseCursor(engine->arrowCursor);
                        }
                        
                    }
                }
                if (event.type == sf::Event::MouseButtonPressed) {
                    for (int i = 0; i < this->gameSceneInst->gameBoard.size(); i++)
                        for (int j = 0; j < this->gameSceneInst->gameBoard.size(); j++) {
                            if (this->gameSceneInst->gameBoard[i][j]->getShape().getGlobalBounds().contains(mouseClick)) {
                                if (this->gameSceneInst->gameBoard[i][j]->getState() == Ownership::Unclaimed) {
                                    this->gameSceneInst->gameBoard[i][j]->changeState(this->engine->turn);
                                    this->engine->changeTurn();
                                    
                                }
                                i = this->gameSceneInst->postionCache.size();
                                j = this->gameSceneInst->postionCache.size();
                                std::cout << "In\n";
                            }
                        }
                }
                if (event.type == sf::Event::Closed) {
                    engine->window.close();
                    isActive = false;
                }
            }
            this->engine->window.display();
        }
    }
    void showHelpScene() {
        bool isActive = true;
        while (isActive) {
            sf::Event event;
            engine->window.clear();
            engine->window.draw(helpSceneInst->placeholder);
            while (engine->window.pollEvent(event)) {
                if (event.type == sf::Event::MouseMoved) {
                    if (helpSceneInst->placeholder.getGlobalBounds().contains(mousePosition))
                        engine->window.setMouseCursor(engine->handCursor);
                    else
                        engine->window.setMouseCursor(engine->arrowCursor);
                }
                if (event.type == sf::Event::MouseButtonPressed) {
                    if (helpSceneInst->placeholder.getGlobalBounds().contains(mouseClick)) {
                        isActive = false;
                        showMainMenu();
                    }
                }
                if (event.type == sf::Event::Closed) {
                    engine->window.close();
                    isActive = false;
                }
            }
            engine->window.display();
        }
    }
    void showMainMenu() {
        bool isActive = true;
        while (isActive)
        {
            sf::Event event;
            engine->window.clear();
            engine->window.draw(mainMenuInst->quickPlay);
            engine->window.draw(mainMenuInst->help);
            engine->window.draw(mainMenuInst->quit);
            engine->window.draw(mainMenuInst->quickPlayText);
            engine->window.draw(mainMenuInst->helpText);
            engine->window.draw(mainMenuInst->quitText);
            engine->window.display();
            while (engine->window.pollEvent(event))
            {
                if (event.type == sf::Event::MouseMoved) {
                    if (mainMenuInst->quickPlay.getGlobalBounds().contains(mousePosition) || mainMenuInst->help.getGlobalBounds().contains(mousePosition) || mainMenuInst->quit.getGlobalBounds().contains(mousePosition))
                        engine->window.setMouseCursor(engine->handCursor);
                    else
                        engine->window.setMouseCursor(engine->arrowCursor);
                }
                if (event.type == sf::Event::MouseButtonPressed) {
                    if (mainMenuInst->quickPlay.getGlobalBounds().contains(mouseClick)) {
                        isActive = false;
                        this->gameSceneInst->setGameBoard(8);
                        showGameScene();
                    }
                    if (mainMenuInst->help.getGlobalBounds().contains(mouseClick)) {
                        std::cout << "scene change";
                        isActive = false;
                        showHelpScene();                       
                    }
                }
                if (event.type == sf::Event::Closed) {
                    engine->window.close();
                    isActive = false;
                }
            }
        }
    }
};