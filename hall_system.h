#pragma once

#include <QDialog>
#include<Qstandarditemmodel>
#include<QDebug>
#include<QMessagebox>

#include"room_system.h"
#include "ui_hall_system.h"
#include "Room_info.h"
#include<iostream>
class hall_system : public QDialog
{
	Q_OBJECT

public:
	hall_system(QWidget *parent = Q_NULLPTR);
	~hall_system();

private:
	Ui::hall_system ui;
	QStandardItemModel* model;
	QItemSelectionModel* md;
signals:
	//void clickJoin();
	//void clickMake();

	void send_make(QString, int);
	void send_join(int);

public slots:
	void click_make();
	void click_join();

	void get_join_state(int);//1-success,2-err

	void insert_room(Room_prop,int);
	void view_select_check(QItemSelection, QItemSelection);

};
