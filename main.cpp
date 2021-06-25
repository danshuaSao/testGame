#include "testGame.h"
#include"Login.h"
#include <QtWidgets/QApplication>
#include"Controller.h"
#include "System.h"
#include "hall_system.h"
#include"room_system.h"

#include <QMetaType>
//#pragma comment( linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"" )
using namespace boost::asio;

int main(int argc, char* argv[])
{
	//隐藏控制台
	//HWND hwndDOS = GetForegroundWindow();
	//ShowWindow(hwndDOS, SW_HIDE);

	QApplication a(argc, argv);
	Login login_dialog;
	testGame win;
	hall_system hall;
	room_system room;
	Controller c;
	//注册Room_prop类型参数
	qRegisterMetaType<Room_prop>("Room_prop");
	qRegisterMetaType<Room_info>("Room_info");

	//监听来自ui的信号
	//QObject::connect(&login_dialog, SIGNAL(send_name(QString)), &c, SIGNAL(getLogin(QString)));
	QObject::connect(&login_dialog, SIGNAL(send_name(QString)), &c, SLOT(getLogin(QString)));
	QObject::connect(&hall, SIGNAL(send_make(QString, int)), &c, SLOT(getMake(QString, int)));
	QObject::connect(&hall, SIGNAL(send_join(int)), &c, SLOT(getJoin(int)));
	QObject::connect(&room, SIGNAL(start_game()), &c, SLOT(get_start()));
	QObject::connect(&room, SIGNAL(set_ready()), &c, SLOT(get_ready()));
	QObject::connect(&room, SIGNAL(exit()), &c, SLOT(get_exit()));
	QObject::connect(&win, SIGNAL(send_input(QString)), &c, SLOT(get_input(QString)));



	//监听来自控制器的信号
	QObject::connect(&c, SIGNAL(Login_suc()), &login_dialog, SLOT(login_accept()));
	QObject::connect(&c, SIGNAL(Login_err()), &login_dialog, SLOT(login_again()));
	QObject::connect(&c, SIGNAL(send_room(Room_prop, int)), &hall, SLOT(insert_room(Room_prop, int)));
	QObject::connect(&c, SIGNAL(get_join_state(int)), &hall, SLOT(get_join_state(int)));
	QObject::connect(&c, SIGNAL(set_room_info(Room_info)), &room, SLOT(get_room_info(Room_info)));
	QObject::connect(&c, SIGNAL(start_result(bool)), &room, SLOT(get_start_result(bool)));
	QObject::connect(&c, SIGNAL(send_message(std::string)), &room, SLOT(get_message(std::string)));

	//QObject::connect(&c, SIGNAL(set_room_info(std::string, int, int, int)), &room, SLOT(get_room_info(std::string, int, int, int)));
	QObject::connect(&c, SIGNAL(send_user_info(User_info, int)), &room, SLOT(insert_user(User_info, int)));
	QObject::connect(&c, SIGNAL(send_game_info_quick(basic_game_info)), &win, SLOT(get_game_info_quick(basic_game_info)));
	QObject::connect(&c, SIGNAL(send_game_info_slow(basic_game_info)), &win, SLOT(get_game_info_slow(basic_game_info)));
	QObject::connect(&c, SIGNAL(send_treasure(int, int, int)), &win, SLOT(get_treasure(int, int, int)));
	QObject::connect(&c, SIGNAL(send_bid_info(std::string, std::string, std::string)), &win, SLOT(get_bid_info(std::string, std::string, std::string)));
	QObject::connect(&c, SIGNAL(send_message(std::string)), &win, SLOT(get_message(std::string)));



	if (login_dialog.exec() == QDialog::Accepted)
	{
		while (1)
		{
			int result = hall.exec();
			if (result == QDialog::Accepted)
			{
				if (room.exec() == QDialog::Accepted)
				{
					win.show();
					break;
				}
			}
			else if (result == QDialog::Rejected)
				break;
		}
	}
	return a.exec();

}
