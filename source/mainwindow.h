#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QKeyEvent>
#include <QTimer>
#include <QMediaPlayer>
#include <QAudioOutput>
#include "game.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void initLabels();
    void keyPressEvent(QKeyEvent*) override;
    void on_durationChanged(qint64 position);
    void on_positionChanged(qint64 position);

private slots:
    void update_score();

    void on_VolumeSlider_sliderMoved(int position);

    void on_pushButton_released();

private:
    Ui::MainWindow *ui;
    QLabel *score_title, *score;
    QLabel *win_label, *lose_label;
    QTimer *score_timer;
    Game *game;
    QMediaPlayer* player = nullptr;
    QAudioOutput* audioOutput = nullptr;
};
#endif // MAINWINDOW_H
