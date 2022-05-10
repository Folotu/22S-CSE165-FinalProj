#include "game.h"
#include <QMediaPlayer>
#include <QAudioOutput>

#define W (GameObject::Width)

Sheriff::Sheriff() : GameObject(
    GameObject::Sheriff, QPixmap(":/game_objects/sheriff/sheriff2.png"))
{
    dir = Stop;
    next_dir = Stop;
    anim_index = 2;
    anim[Right].push_back(QPixmap(":/game_objects/sheriff/sheriff2.png"));
    anim[Right].push_back(QPixmap(":/game_objects/sheriff/right1.png"));
    anim[Right].push_back(QPixmap(":/game_objects/sheriff/right2.png"));
    anim[Right].push_back(QPixmap(":/game_objects/sheriff/right1.png"));
    anim[Right].push_back(QPixmap(":/game_objects/sheriff/right3.png"));


    anim[Up].push_back(QPixmap(":/game_objects/sheriff/sheriff2.png"));
    anim[Up].push_back(QPixmap(":/game_objects/sheriff/up1.png"));
    anim[Up].push_back(QPixmap(":/game_objects/sheriff/up2.png"));
    anim[Up].push_back(QPixmap(":/game_objects/sheriff/up1.png"));
    anim[Up].push_back(QPixmap(":/game_objects/sheriff/up3.png"));



    anim[Left].push_back(QPixmap(":/game_objects/sheriff/sheriff2.png"));
    anim[Left].push_back(QPixmap(":/game_objects/sheriff/left1.png"));
    anim[Left].push_back(QPixmap(":/game_objects/sheriff/left2.png"));
    anim[Left].push_back(QPixmap(":/game_objects/sheriff/left1.png"));
    anim[Left].push_back(QPixmap(":/game_objects/sheriff/left3.png"));



    anim[Down].push_back(QPixmap(":/game_objects/sheriff/sheriff2.png"));
    anim[Down].push_back(QPixmap(":/game_objects/sheriff/down1.png"));
    anim[Down].push_back(QPixmap(":/game_objects/sheriff/down2.png"));
    anim[Down].push_back(QPixmap(":/game_objects/sheriff/down1.png"));
    anim[Down].push_back(QPixmap(":/game_objects/sheriff/down3.png"));

    player = new QMediaPlayer();
    audioOutput = new QAudioOutput();
    player->setAudioOutput(audioOutput);

}

void Sheriff::moveup()
{
    anim_index++;
    if (anim_index == anim[Up].size()) {
        anim_index = 0;
    }
    setPixmap(anim[Up][anim_index]);
    setY(static_cast<int>(y()) - 1);
}

void Sheriff::moveleft()
{
    anim_index++;
    if (anim_index == anim[Left].size()) {
        anim_index = 0;
    }
    setPixmap(anim[Left][anim_index]);
    setX(static_cast<int>(x()) - 1);
}

void Sheriff::movedown()
{
    anim_index++;
    if (anim_index == anim[Down].size()) {
        anim_index = 0;
    }
    setPixmap(anim[Down][anim_index]);
    setY(static_cast<int>(y()) + 1);
}

void Sheriff::moveright()
{
    anim_index++;
    if (anim_index == anim[Right].size()) {
        anim_index = 0;
    }
    setPixmap(anim[Right][anim_index]);
    setX(static_cast<int>(x()) + 1);
}


void Sheriff::eat_ball(int __y, int __x)
{
    GameObject *obj = game->map[__y][__x];
    switch (obj->get_type()) {
    case Ball:
        game->score += obj->get_score();
        game->ball_num--;
        player->setSource(QUrl("qrc:/audio/AmongUsTask_complete.wav"));
        player->play();
        break;
    case PowerBall:
        game->score += obj->get_score();
        game->ball_num--;
        player->setSource(QUrl("qrc:/audio/AmongUSPowerUp.wav"));
        player->play();
        for (int i = 0; i < game->powerball.size(); i++) {
            if (game->powerball.at(i) == obj) {
                game->powerball.remove(i);
                break;
            }
        }

        for (int i = 0; i < AmongChar::amongCharNum; i++) {
            if (game->amongChar[i]->status != AmongChar::Running) {
                game->amongChar[i]->status = AmongChar::Panic;
                game->amongChar[i]->panic_time = PANNIC_TIME;
                game->amongChar_timer[i]->setInterval(PANNIC_INTERVAL);
            }
        }
        break;
    default:
        return;
    }

    /* sheriff eat a ball, and
     * fill the previous block with blank. */
    QPixmap blankpix;
    game->map[_y][_x] = new GameObject(GameObject::Blank, blankpix);
    game->map[__y][__x] = this;
    delete obj;
}


bool Sheriff::overlapable(int i, int j)
{
    if (i < 0 || j < 0) {
        return false;
    }

    if (i >= game->map_height || j >= game->map_width) {
        return false;
    }

    switch (game->map[i][j]->get_type()) {
    case Wall:
    case Gate:
        return false;
    default:
        return true;
    }
}


void Sheriff::move()
{
    int pac_x = static_cast<int>(x());
    int pac_y = static_cast<int>(y());
    int __x = (pac_x - game->geo_x) / W;            // x coordinate in map
    int __y = (pac_y - game->geo_y) / W;            // y coordinate in map
    int x_remainder = (pac_x - game->geo_x) % W;    // remainder x pixel to fit a block
    int y_remainder = (pac_y - game->geo_y) % W;    // remainder y pixel to fit a block
    Dir next_dir = get_next_dir();

    /* When Sheriff completely fits a block,
     * decide whether to change direction. */
    if (x_remainder == 0) {
        if (y_remainder == 0) {
            // When Sheriff completely fits a block,
            // test if this block is a ball to eat.
            // If all balls are eaten, win.
            eat_ball(__y, __x);

            // update Sheriff's coordinate in map
            _x = __x;
            _y = __y;

            if (game->ball_num == 0) {
                game->stat = Game::Win;
                return;
            }
        }

        switch (next_dir) {
        case Stop:
            dir = next_dir;
            break;
        case Up:        // test if it can turn upward
            if (overlapable(_y - 1, _x)) {
                dir = next_dir;
            }
            break;
        case Down:      // test if it can turn downward
            if (overlapable(_y + 1, _x)) {
                dir = next_dir;
            }
            break;
        case Left:      // test if it can turn left
            if (y_remainder == 0 && overlapable(_y, _x - 1)) {
                dir = next_dir;
            }
            break;
        case Right:      // test if it can turn right
            if (y_remainder == 0 && overlapable(_y, _x + 1)) {
                dir = next_dir;
            }
            break;
        }
    } else if (y_remainder == 0) {
        switch (next_dir) {
        case Stop:
            dir = next_dir;
            break;
        case Left:      // test if it can turn left
            if (overlapable(_y, _x - 1)) {
                dir = next_dir;
            }
            break;
        case Right:     // test if it can turn right
            if (overlapable(_y, _x + 1)) {
                dir = next_dir;
            }
            break;
        default:                    // decide when x_remainder = 0
            break;
        }
    }

    /* Sheriff keeps moving or stops when hitting wall */
    switch (dir) {
    case Stop:
        break;
    case Up:
        if (y_remainder == 0 && !overlapable(_y - 1, _x)) {
            dir = Stop;
            next_dir = Stop;
        } else {
            moveup();
        }
        break;
    case Down:
        if (y_remainder == 0 && !overlapable(_y + 1, _x)) {
            dir = Stop;
            next_dir = Stop;
        } else {
            movedown();
        }
        break;
    case Left:
        if (x_remainder == 0 && !overlapable(_y, _x - 1)) {
            dir = Stop;
            next_dir = Stop;
        } else {
            moveleft();
        }
        break;
    case Right:
        if (x_remainder == 0 && !overlapable(_y, _x + 1)) {
            dir = Stop;
            next_dir = Stop;
        } else {
            moveright();
        }
        break;
    }
}
