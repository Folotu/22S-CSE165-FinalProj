#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "game.h"
#include <iostream>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle(tr("pac-man"));

    /* Initialize graphicsview and game scene */
    ui->graphicsView->setStyleSheet("QGraphicsView {border: none;}");
    ui->graphicsView->setBackgroundBrush(Qt::black);
    ui->graphicsView->setFocusPolicy(Qt::NoFocus);

    int map_height = 20, map_width = 29;            // 20x29 game map
    int x = 50, y = 50;                             // x y in mainwindow
    int w = (map_width * GameObject::Width);
    int h = (map_height * GameObject::Width);

    ui->graphicsView->setGeometry(x, y, w, h);
    game = new Game(x, y, map_width, map_height, ":/game_objects/map_objects/map.txt");
    ui->graphicsView->setScene(game);
    initLabels();


//   player = new QMediaPlayer(this);
//   audioOutput = new QAudioOutput(this);
//   player->setAudioOutput(audioOutput);

//   //player->setSource(QUrl::fromLocalFile("C:/Users/olotu/Downloads/Fucked Up.mp3"));
//   player->setSource(QUrl("qrc:/audio/sheriff_beginning.wav"));
//   qDebug() << player->source();
//   player->play();
//   player->setLoops(4);
game->start();

}


/* Initialize UI */
void MainWindow::initLabels()
{
    score_title = new QLabel(this);
    score_title->setText("score");
    score_title->setStyleSheet("QLabel {font-family: Fixedsys;color: white;font-size: 16px;}");
    score_title->setGeometry(50, 12, 60, 26);

    score = new QLabel(this);
    score->setIndent(-80);
    score->setText("0");
    score->setStyleSheet("QLabel {font-family: Fixedsys;color: white;font-size: 16px;}");
    score->setGeometry(110, 12, 150, 26);

    win_label = new QLabel(this);
    win_label->hide();
    win_label->setText("You win!");
    win_label->setStyleSheet("QLabel {font-family: Fixedsys;color: yellow;font-size: 16px;}");
    win_label->setGeometry(310, 12, 150, 26);

    lose_label = new QLabel(this);
    lose_label->hide();
    lose_label->setText("You lose!");
    lose_label->setStyleSheet("QLabel {font-family: Fixedsys;color: red;font-size: 16px;}");
    lose_label->setGeometry(310, 12, 150, 26);

    score_timer = new QTimer(this);
    score_timer->start(25);
    connect(score_timer, SIGNAL(timeout()), this , SLOT(update_score()));
}


/* Update score UI */
void MainWindow::update_score()
{
    score->setText(QString::number(game->get_score()));
    if (game->stat == Game::Win) {
        win_label->show();
        score_timer->stop();
    } else if (game->stat == Game::Lose) {
        lose_label->show();
        score_timer->stop();
    }
}


void MainWindow::keyPressEvent(QKeyEvent *e) {
    switch (e->key()) {
    case Qt::Key_W:
        game->sheriff_next_direction(GameObject::Up);
        break;
    case Qt::Key_A:
        game->sheriff_next_direction(GameObject::Left);
        break;
    case Qt::Key_S:
        game->sheriff_next_direction(GameObject::Down);
        break;
    case Qt::Key_D:
        game->sheriff_next_direction(GameObject::Right);
        break;
    }
}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_VolumeSlider_sliderMoved(int position)
{
    std::cout << position << std::endl;
    double temp = position / 100.0;
     std::cout << temp << std::endl;
     Game::audioOutput23->volumeChanged(temp);

}


void MainWindow::on_pushButton_released()
{

//Game::Game(1, 1, 6, 7, "map_src");
    //Game(50, 50, 29, 20, derr);

}

