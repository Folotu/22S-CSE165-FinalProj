#ifndef GAME_H
#define GAME_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QTimer>
#include <QPair>
#include <QMediaPlayer>
#include <QAudioOutput>
#include "gameobject.h"


/* Changeable game options */
#define BALL_SCORE      10          // score of balls
#define POWERBALL_SCORE 30          // score of powerballs
#define AMONGCHAR_SCORE     50          // score of amongUsCharacters
#define INTERVAL        10          // move interval of sheriff
#define NORMAL_INTERVAL 10          // move interval of normal amongUsCharacters
#define PANNIC_INTERVAL 15          // move interval of pannic amongUsCharacters
#define RUNNING_INTERVAL 5          // move interval of running amongUsCharacters
#define PANNIC_TIME     1000        // interval number of pannic amongUsCharacters
#define FLASH_INTERVAL 200          // flash interval of powerballs

/* Game control class */
class Game : public QGraphicsScene
{
    Q_OBJECT

public:
    enum GameStatus {Playing, Win, Lose, Pause};
    Game(int, int, int, int, QString);
    ~Game();
    void start();
    void stop();
    void sheriff_next_direction(GameObject::Dir);
    int get_score();

    GameObject ***map;              // the map of pac-man game
    GameObject *gate;               // pointer of the gate of cage of amongUsCharacters
    Sheriff *sheriff;                 // pointer of Sheriff
    AmongChar *amongChar[AmongChar::amongCharNum];  // pointers of amongUsCharacters
    QVector<GameObject*> powerball; // pointers of powerball
    GameStatus stat;

    friend class Sheriff;
    friend class AmongChar;
    static inline QAudioOutput* audioOutput23 = nullptr;
    //static QString derr;

private slots:
    void sheriff_handler();
    void powerball_flash();
    void amongChar_handler(int);


private:
    int map_height, map_width;      // game map (20 x 29 in this app)
    int map_size;                   // map_width * map_height
    int ball_num, eat_num, score;
    int geo_x, geo_y;               // geometric coordinate

    QTimer *amongChar_timer[AmongChar::amongCharNum];
    QTimer *sheriff_timer;
    QTimer *powerball_flash_timer;
    bool flash_tick;
    QMediaPlayer* player = nullptr;


};

#endif // GAME_H
