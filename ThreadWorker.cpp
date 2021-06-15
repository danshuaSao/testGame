#include "ThreadWorker.h"


void ThreadWorker::set_sys_object(std::shared_ptr<System> ptr)
{
	sys_object = ptr;
}

void ThreadWorker::doRun()
{
	qDebug() << "doRun recieve";
	std::cout << sys_object.get()<<std::endl;
	sys_object->run();

}

ThreadWorker::ThreadWorker(QObject* parent, threadsafe_queue<std::string>& _input_que):
	io(), ep(ip::address::from_string("127.0.0.1"), atoi("12345")),input_que(_input_que)
{
	auto sys = std::make_shared<System>(io, ep, input_que);
	sys_object = sys;

	//注册Room_prop类型参数
	qRegisterMetaType<Room_prop>("Room_prop");
	qRegisterMetaType<Room_info>("Room_info");
	QObject::connect(sys_object.get(), SIGNAL(send_roominfo(Room_prop, int)), this, SIGNAL(send_room(Room_prop, int)));
	QObject::connect(sys_object.get(), SIGNAL(set_room_info(Room_info)), this, SIGNAL(set_room_info(Room_info)));
	//QObject::connect(sys_object.get(), SIGNAL(set_room_info(std::string, int, int, int)), this, SIGNAL(set_room_info(std::string, int, int, int)));
	QObject::connect(sys_object.get(), SIGNAL(send_user_info(User_info, int)), this, SIGNAL(send_user_info(User_info, int)));
	QObject::connect(sys_object.get(), SIGNAL(login_success()), this, SLOT(send_success()));
	QObject::connect(sys_object.get(), SIGNAL(login_err()), this, SLOT(send_err()));
	QObject::connect(sys_object.get(), SIGNAL(join_result(int)), this, SIGNAL(get_join_state(int)));
	QObject::connect(sys_object.get(), SIGNAL(send_game_info_quick(basic_game_info)), this, SIGNAL(send_game_info_quick(basic_game_info)));
	QObject::connect(sys_object.get(), SIGNAL(send_game_info_slow(basic_game_info)), this, SIGNAL(send_game_info_slow(basic_game_info)));
	QObject::connect(sys_object.get(), SIGNAL(send_treasure(int, int, int)), this, SIGNAL(send_treasure(int, int, int)));
	QObject::connect(sys_object.get(), SIGNAL(send_bid_info(std::string, std::string, std::string)), this, SIGNAL(send_bid_info(std::string, std::string, std::string)));
	QObject::connect(sys_object.get(), SIGNAL(send_message(std::string)), this, SIGNAL(send_message(std::string)));


}

void ThreadWorker::login(QString str)
{
	qDebug() << "worker get login";
	sys_object->username = str.toStdString();
	std::string login = "login " + str.toStdString();
	input_que.push(login);
}

void ThreadWorker::send_success()
{
	qDebug() << "login_success() recieve";
	emit get_success();
}

void ThreadWorker::send_err()
{
	qDebug() << "login_err() recieve";
	emit get_err();
	sys_object.reset();
}

void ThreadWorker::joinRoom(int id)
{
	//if (id = -1)emit get_join_state(-1);
	qDebug() << "joinRoom() recieve";
	sys_object->join_room(id);
}

void ThreadWorker::makeRoom(QString str,int capacity)
{
	qDebug() << "makeRoom() recieve";
	qDebug() << "capacity:" << capacity;
	std::string room_name = str.toStdString();
	sys_object->make_room(room_name,capacity);
}

void ThreadWorker::get_start_game()
{
	qDebug() << "threadwork get start_game\n";
	sys_object->start_game();
}

void ThreadWorker::get_set_ready()
{
	qDebug() << "threadwork get set_ready\n"; 
	sys_object->set_ready();
}
