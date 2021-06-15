#pragma once

#include <QDialog>
#include<Qstandarditemmodel>
#include<QDebug>
#include<QMessagebox>

#include "ui_room_system.h"
#include"User_info.h"
#include"Room_info.h"
#include"hall_system.h"

#include<iostream>

class room_system : public QDialog
{
	Q_OBJECT

public:
	room_system(QWidget *parent = Q_NULLPTR);
	~room_system();

private:
	Ui::room_system ui;
	QStandardItemModel* model_1;
	QStandardItemModel* model_2;

public slots:
	void insert_user(User_info,int);
	void get_room_info(Room_info);
	void get_room_info(std::string, int, int, int);

	void get_set_ready();
	void get_start();
	void get_exit();
signals:
	void set_ready();
	void start_game();
	void exit();
};
