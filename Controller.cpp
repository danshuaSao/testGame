#include "Controller.h"

Controller::Controller(QObject* parent)
	: QObject(parent)
{

	worker = new ThreadWorker(nullptr,input_que);
	worker->moveToThread(&workerThread);

	//注册Room_prop类型参数
	qRegisterMetaType<Room_prop>("Room_prop");
	qRegisterMetaType<Room_info>("Room_info");

	//监听来自ui的信号
	connect(this, SIGNAL(run()), worker, SLOT(doRun()));
	//connect(this, SIGNAL(getLogin(QString)), worker, SLOT(login(QString)));
	//connect(this, SIGNAL(getJoin(int)), worker, SLOT(joinRoom(int)));
	//connect(this, SIGNAL(getMake(QString,int)), worker, SLOT(makeRoom(QString,int)));
	//connect(this, SIGNAL(set_ready()), worker, SLOT(get_set_ready()));
	//connect(this, SIGNAL(start_game()), worker, SLOT(get_start_game()));



	//监听来自线程的信号
	connect(worker, SIGNAL(get_success()), this, SIGNAL(Login_suc()));
	connect(worker, SIGNAL(get_err()), this, SIGNAL(Login_err()));
	connect(worker, SIGNAL(send_room(Room_prop,int)), this, SIGNAL(send_room(Room_prop,int)));
	connect(worker, SIGNAL(get_join_state(int)), this, SIGNAL(get_join_state(int)));
	connect(worker, SIGNAL(set_room_info(Room_info)), this, SIGNAL(set_room_info(Room_info)));
	//connect(worker, SIGNAL(set_room_info(std::string, int, int, int)), this, SIGNAL(set_room_info(std::string, int, int, int)));
	connect(worker, SIGNAL(send_user_info(User_info, int)), this, SIGNAL(send_user_info(User_info, int)));
	connect(worker, SIGNAL(send_game_info_slow(basic_game_info)), this, SIGNAL(send_game_info_slow(basic_game_info)));
	connect(worker, SIGNAL(send_game_info_quick(basic_game_info)), this, SIGNAL(send_game_info_quick(basic_game_info)));
	connect(worker, SIGNAL(send_treasure(int, int, int)), this, SIGNAL(send_treasure(int, int, int)));
	connect(worker, SIGNAL(send_bid_info(std::string, std::string, std::string)), this, SIGNAL(send_bid_info(std::string, std::string, std::string)));
	connect(worker, SIGNAL(send_message(std::string)), this, SIGNAL(send_message(std::string)));
	
	connect(worker->get_sys().get(), SIGNAL(start_result(bool)), this, SIGNAL(start_result(bool)));


	workerThread.start();
	emit run();
}

Controller::~Controller()
{
	workerThread.quit();
	workerThread.wait();
}

void Controller::get_ready()
{
	std::string ready = "sr";
	input_que.push(ready);
}

void Controller::get_start()
{
	std::string start = "sg";
	input_que.push(start);
}

void Controller::getLogin(QString str) 
{
	qDebug() << "controller get login";
	//sys_object->username = str.toStdString();
	std::string login = "login " + str.toStdString();
	input_que.push(login);
}

void Controller::getMake(QString name, int capacity)
{
	std::string capa= std::to_string(capacity);
	std::string makeroom = "mk " + name.toStdString() + "+" + capa;
	input_que.push(makeroom);
}

void Controller::getJoin(int id)
{
	std::string join = "cd " + std::to_string(id);
	input_que.push(join);
}

void Controller::get_exit()
{
	std::string exit = "q";
	input_que.push(exit);
}

void Controller::get_input(QString str)
{
	std::string game_input = str.toStdString();
	input_que.push(game_input);
}
