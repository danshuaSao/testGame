#pragma once
#include"Login.h"
#include <QObject>
#include<QThread>
#include"ThreadWorker.h"
#include <QMetaType>

#include"threadsafe_queue.h"

class Controller : public QObject
{
	Q_OBJECT

public:
	ThreadWorker* worker;
	QThread workerThread;
	QObject* Login_object;
		
	threadsafe_queue<std::string>input_que;
	
	Controller(QObject* parent = nullptr);
	~Controller();
public slots:
	//登录页面
	void getLogin(QString);
	//大厅页面
	void getJoin(int);
	void getMake(QString, int);
	//房间页面
	void get_start();
	void get_ready();
	void get_exit();
	//游戏内
	void get_input(QString);
signals:
	void run();

	//void getLogin(QString);
	void Login_suc();
	void Login_err();
	
	//void getJoin(int);
	//void getMake(QString,int);
	void send_room(Room_prop,int);
	void get_join_state(int);
	void set_room_info(Room_info);
	void set_room_info(std::string, int, int, int);//+
	void send_user_info(User_info, int);

	void set_ready();
	void start_game();

	void send_game_info_quick(basic_game_info);
	void send_game_info_slow(basic_game_info);
	void send_treasure(int, int, int);
	void send_bid_info(std::string, std::string, std::string);
	void send_message(std::string);

};
