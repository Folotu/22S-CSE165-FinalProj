#include "game.h"
#include <QString>
#include <QFile>
#include <QRandomGenerator>
#include <iostream>
#include <string>
#define W (GameObject::Width)


// interval number before amongChars going out the cage
int amongChar_RELEASE_TIME[] = {0, 200, 400, 600};

Game::Game(int x, int y, int map_w, int map_h, QString map_src)
    : QGraphicsScene(x, y, W * map_w, W * map_h)
{   
     //derr = map_src;

    std::cout << x << " "<< y << " "<< map_w << " " << map_h << " " << std::endl;
    geo_x = x;
    geo_y = y;
    stat = Playing;
    player = new QMediaPlayer(this);
    audioOutput23 = new QAudioOutput(this);
    player->setAudioOutput(audioOutput23);
//    connect(player, &QMediaPlayer::positionChanged, this, &MainWindow::on_positionChanged):
//    connect(player, &QMediaPlayer::durationChanged, this, 6Dialog::on durationChanged);

//    qDebug() << player->source();


    /* Initialize map pointers */
    map_size = map_w * map_h;
    map_width = map_w;
    map_height = map_h;
    map = new GameObject**[map_height];
    for (int i = 0; i < map_height; i++) {
        map[i] = new GameObject*[map_width];
        for (int j = 0; j < map_width; j++)
            map[i][j] = nullptr;
    }

    /* Initialize map graphics */
    ball_num = eat_num = score = 0;
    int amongChar_i = 0;
    QPixmap wallpix(":/game_objects/map_objects/wall.png");
    QPixmap ballpix(":/game_objects/map_objects/dot.png");
    QPixmap powerballpix(":/game_objects/map_objects/power_ball.png");
    QPixmap gatepix(":/game_objects/map_objects/gate.png");
    QPixmap blankpix;
    QFile mapfile(map_src);
    mapfile.open(QIODevice::ReadOnly|QIODevice::Text);

    sheriff = new Sheriff();

    for (int i = 0; i < map_h; i++) {
        QByteArray line = mapfile.readLine();
        for (int j = 0; j < map_w; j++) {
            int tmp_x = x + (j * W);
            int tmp_y = y + (i * W);
            switch (line[j]) {
            case '1':
                map[i][j] = new GameObject(GameObject::Wall, wallpix);
                map[i][j]->setPos(tmp_x, tmp_y);
                addItem(map[i][j]);
                break;
            case '0':
                map[i][j] = new GameObject(GameObject::Ball, ballpix);
                map[i][j]->set_score(BALL_SCORE);
                map[i][j]->setPos(tmp_x, tmp_y);
                addItem(map[i][j]);
                ball_num++;
                break;
            case '4':
                map[i][j] = new GameObject(GameObject::PowerBall, powerballpix);
                map[i][j]->set_score(POWERBALL_SCORE);
                map[i][j]->setPos(tmp_x, tmp_y);
                addItem(map[i][j]);
                powerball.push_back(map[i][j]);
                ball_num++;
                break;
            case '3':
                map[i][j] = new GameObject(GameObject::Blank, blankpix);
                break;
            case '2':
                gate = new GameObject(GameObject::Gate, gatepix);
                gate->_x = j;
                gate->_y = i;
                gate->setPos(tmp_x, tmp_y);
                addItem(gate);
                map[i][j] = gate;
                break;
            case 'p':
                sheriff = new Sheriff();
                sheriff->game = this;
                sheriff->setZValue(2);
                sheriff->setPos(tmp_x, tmp_y);
                addItem(sheriff);
                map[i][j] = sheriff;
                break;
            case 'g':
                map[i][j] = new GameObject(GameObject::Blank, blankpix);
                amongChar[amongChar_i] = new AmongChar(amongChar_i);
                amongChar[amongChar_i]->game = this;
                amongChar[amongChar_i]->setZValue(2);
                amongChar[amongChar_i]->release_time = amongChar_RELEASE_TIME[amongChar_i];
                amongChar[amongChar_i]->_x = j;
                amongChar[amongChar_i]->_y = i;
                amongChar[amongChar_i]->set_score(AMONGCHAR_SCORE);
                amongChar[amongChar_i]->setPos(tmp_x, tmp_y);
                addItem(amongChar[amongChar_i]);
                amongChar_i++;
                break;
            }
            if (map[i][j]) {
                map[i][j]->_x = j;
                map[i][j]->_y = i;
            }
        }
    }

    amongChar[AmongChar::Red]->chase_strategy = &strategy1;
    amongChar[AmongChar::Pink]->chase_strategy = &strategy2;
    amongChar[AmongChar::Green]->chase_strategy = &strategy3;
    amongChar[AmongChar::Yellow]->chase_strategy = &strategy4;
}

void Game::start()
{
    player->setSource(QUrl("qrc:/audio/AmongUStrapTheme.wav"));

    player->setLoops(-1);
    player->play();

    powerball_flash_timer = new QTimer(this);
    connect(powerball_flash_timer, SIGNAL(timeout()), this , SLOT(powerball_flash()));
    powerball_flash_timer->start(FLASH_INTERVAL);

    sheriff_timer = new QTimer(this);
    connect(sheriff_timer, SIGNAL(timeout()), this , SLOT(sheriff_handler()));
    sheriff_timer->start(INTERVAL);

    for (int i = 0; i < AmongChar::amongCharNum; i++) {
        amongChar_timer[i] = new QTimer(this);
        // Managed to pass amongChar id to amongChar_handler.
        connect(amongChar_timer[i], &QTimer::timeout, [=](){amongChar_handler(i);} );
        amongChar_timer[i]->start(NORMAL_INTERVAL);
    }
}


void Game::stop()
{
    player->stop();
    sheriff_timer->stop();
    powerball_flash_timer->stop();
    for (int i = 0; i < AmongChar::amongCharNum; i++) {
        amongChar_timer[i]->stop();
    }

}


void Game::powerball_flash()
{
    if (powerball.empty()) {
        powerball_flash_timer->stop();
        return;
    }

    if (flash_tick) {
        for (int i = 0; i < powerball.size(); i++) {
            powerball.at(i)->hide();
        }
        flash_tick = 0;
    } else {
        for (int i = 0; i < powerball.size(); i++) {
            powerball.at(i)->show();
        }
        flash_tick = 1;
    }
}

void Game::sheriff_handler()
{
    sheriff->move();
    if (stat == Win) {
        stop();
    }

}


void Game::amongChar_handler(int amongChar_id)
{
    amongChar[amongChar_id]->move();
    if (stat == Lose) {
        stop();
    }
}


void Game::sheriff_next_direction(GameObject::Dir d)
{
    sheriff->set_next_dir(d);


}

int Game::get_score()
{
    return score;
}

Game::~Game()
{
    for (int i = 0; i < map_height; i++) {
        for (int j = 0; j < map_width; j++) {
            if (map[i][j] != nullptr)
                delete map[i][j];
        }
        delete[] map[i];
    }
    delete[] map;
    delete sheriff_timer;
    delete powerball_flash_timer;
    for (int i = 0; i < AmongChar::amongCharNum; i++) {
        delete amongChar_timer[i];
    }
}
