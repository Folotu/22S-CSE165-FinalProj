#include "game.h"


QPair<int, int> strategy1(AmongChar* amongChar) {
    Sheriff *sheriff = amongChar->game->sheriff;
    int dist_x = sheriff->get_x() - amongChar->get_x();
    int dist_y = sheriff->get_y() - amongChar->get_y();
    return QPair<int, int>(dist_x, dist_y);
}

QPair<int, int> strategy2(AmongChar* amongChar) {
    Sheriff *sheriff = amongChar->game->sheriff;
    int dist_x = sheriff->get_x() - amongChar->get_x();
    int dist_y = sheriff->get_y() - amongChar->get_y();
    switch(sheriff->get_dir()) {
    case GameObject::Up:
        dist_y-=4;
        break;
    case GameObject::Down:
        dist_y+=4;
        break;
    case GameObject::Left:
        dist_x-=4;
        break;
    case GameObject::Right:
        dist_x+=4;
        break;
    default:
        break;
    }
    return QPair<int, int>(dist_x, dist_y);
}

QPair<int, int> strategy3(AmongChar* amongChar) {
    Sheriff *sheriff = amongChar->game->sheriff;
    AmongChar *red_amongChar = amongChar->game->amongChar[AmongChar::Red];
    int dist_x = (sheriff->get_x() - red_amongChar->get_x()) * 2 - amongChar->get_x();
    int dist_y = (sheriff->get_y() - red_amongChar->get_y()) * 2 - amongChar->get_y();
    return QPair<int, int>(dist_x, dist_y);
}

QPair<int, int> strategy4(AmongChar* AmongChar) {
    Sheriff *sheriff = AmongChar->game->sheriff;
    int dist_x = sheriff->get_x() - AmongChar->get_x();
    int dist_y = sheriff->get_y() - AmongChar->get_y();
    if (dist_x * dist_x + dist_y * dist_y < 64) {
        dist_x = 2 - AmongChar->get_x();
        dist_y = 2 - AmongChar->get_y();
    }
    return QPair<int, int>(dist_x, dist_y);
}
