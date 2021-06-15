#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_testGame.h"
#include <QString>
#include<Qstandarditemmodel>
#include<QDebug>
#include"Game_info.h"
#include<memory>

class testGame : public QMainWindow
{
    Q_OBJECT

public:
    testGame(QWidget *parent = Q_NULLPTR);

private:
    Ui::testGameClass ui;
    QStandardItemModel* model_player_info;
    QStandardItemModel* model_player_list;
    QStandardItemModel* model_bid;
    QStandardItemModel* model_exp;
    QStandardItemModel* model_my_info;

    QString get_stage(int,int);
    void set_action(bool a[]);

public slots:
    void get_game_info_quick(basic_game_info);
    void get_game_info_slow(basic_game_info);
    void get_treasure(int, int, int);
    void get_bid_info(std::string, std::string, std::string);
    void get_message(std::string);

    //°´¼üÊÂ¼þ
    void click_mv();
    void click_mine();
    void click_atk();
    void click_heal();
    void click_exp();
    void click_bid();

signals:
    void send_input(QString);
};
