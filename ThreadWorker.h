#pragma once

#include <QObject>
#include "System.h"
#include<QDebug>
#include <QMetaType>
 
using namespace boost::asio;
class ThreadWorker : public QObject
{
	Q_OBJECT

private:
	std::shared_ptr<System> sys_object;
	io_context io;
	//executor_work_guard<io_context::executor_type> guard;
	ip::tcp::endpoint ep;
public:
	
	threadsafe_queue<std::string>& input_que;
	ThreadWorker(QObject* parent, threadsafe_queue<std::string>& _input_que);
	void set_sys_object(std::shared_ptr<System>ptr);
	std::shared_ptr<System> get_sys()
	{
		return sys_object;
	};
	//~ThreadWorker();

public slots:
	void doRun();
	void login(QString);
	void send_success();
	void send_err();

	void joinRoom(int);
	void makeRoom(QString,int);

	void get_start_game();
	void get_set_ready();

signals:
	void get_success();
	void get_err();
	void send_room(Room_prop,int);

	void get_join_state(int);

	void set_room_info(Room_info);
	void set_room_info(std::string, int, int, int);//+
	void send_user_info(User_info, int);

	void send_game_info_quick(basic_game_info);
	void send_game_info_slow(basic_game_info);

	void send_treasure(int, int, int);
	void send_bid_info(std::string, std::string, std::string);
	void send_message(std::string);
};
