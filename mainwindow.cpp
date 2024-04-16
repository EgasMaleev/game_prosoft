#include "mainwindow.h"
#include <QTimer>
#include <QKeyEvent>
#include <QPushButton>
#include <QRandomGenerator>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    constexpr int SizeOfButton = 20; //константы вычисляются в compile time, они могут быть заданы выражением
    constexpr int min_y = 100;

    resize(350, 600);
    setWindowTitle("Widget Movement");
    QTimer* glob_timer = new QTimer(this); //глобальный таймер, обновляющий положение всех кнопок
    glob_timer->setInterval(80);
    QTimer* button_maker = new QTimer(this); // создаёт кнопки
    QRandomGenerator* rand = QRandomGenerator::global();
    button_maker->setInterval(rand->bounded(100,1000));
    connect(button_maker, &QTimer::timeout, [this, glob_timer, button_maker, rand]
        {
        QPushButton* newBtn = new QPushButton(this);
        newBtn->setGeometry(rand->bounded(0, width()-SizeOfButton), min_y, SizeOfButton, SizeOfButton);
        int  moveSpeed = rand->bounded(1, 3);
        // "вложенный" таймер, чтобы можно было следить за данной кнопкой
        connect(glob_timer, &QTimer::timeout, newBtn, [this, newBtn, moveSpeed]
            {
            newBtn->move(newBtn->geometry().x(), newBtn->geometry().y() + moveSpeed*(newBtn->underMouse()?2:1));
            if (newBtn->geometry().x() > width())  //чтобы работало в полноэкранном режиме
                newBtn->deleteLater();

            if (newBtn->geometry().y() + SizeOfButton > height())  //проигрыш
            {
                setWindowTitle("YOU LOOSE");
                setStyleSheet("background-color: red");
            }
        });

        connect(newBtn, &QPushButton::pressed, newBtn, &QPushButton::deleteLater);  //уничтожаем по нажатию
        newBtn->show();
        button_maker->setInterval(rand->bounded(100, 1000));
    });
    button_maker->start();
    glob_timer->start();
}

MainWindow::~MainWindow()
{
    delete ui;
}

