#pragma once
#pragma once
#pragma once
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/serialization/serialization.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/utility.hpp>
#include <memory>
#include <list>
#include "Game_proto.h"
#include "state_code.h"
#include "User_info.h"
#include "Auction_list.h"
#include "Otp_table.h"
#include "Tcp_connection.h"
#include "Room_info.h"
#include "threadsafe_queue.h"
#include "Game_info.h"
#include <queue>

#include <QObject>
#include<QString>
#include <QMetaType>
using namespace boost::asio;

#define ASYNC_RET void
class System :public QObject,public std::enable_shared_from_this<System>
{
	Q_OBJECT
public:
	void run();
	System(io_context& _io, ip::tcp::endpoint& _ep, threadsafe_queue<std::string>& _input_que);

private:
	std::string username;//+
	io_context& io;
	ip::tcp::endpoint& ep;
	std::shared_ptr<ip::tcp::socket>sock;
	std::shared_ptr<Tcp_connection>conn;
	std::queue<std::shared_ptr<Proto_msg>>msg_que;
	int session_id;
	int room_id;
	Room_info room_info;
	std::chrono::time_point<std::chrono::steady_clock>start_time;

	void receive_session_id(std::shared_ptr<Proto_msg>msg);
	void login(std::string_view username);
	void login();//+
	void receive_login_result(std::shared_ptr<Proto_msg>msg);
	void request_room_prop(bool no_cycle = false);
	void receive_room_prop(std::shared_ptr<Proto_msg>msg);
	void join_room(int room_id);
	void receive_join_result(std::shared_ptr<Proto_msg>msg);
	void make_room(std::string_view user_input);
	void make_room(std::string room_name,int capacity);//+
	void receive_make_result(std::shared_ptr<Proto_msg>msg);
	void exit_room();


	enum class STATE { LOGIN, WAIT_LOGIN, WAIT_JOIN, HALL, ROOM, GAME };
	STATE state;
	threadsafe_queue<std::string>&input_que;
	void otp_hall_operation();
	void otp_room_operation();
	void otp_game_operation();
	void hall_system_run();
	void room_system_run();

	void sync_time(std::shared_ptr<Proto_msg>msg);
	void game_system_run();
	void update_room_info(std::shared_ptr<Proto_msg>msg);

	void read_input();

	void set_ready();
	void start_game();

	ASYNC_RET route();
	ASYNC_RET message_route();

	std::shared_ptr<basic_game_info>game_info;
	void create_game_info(std::shared_ptr<Proto_msg>msg);
	bool daytime_action_check();
	void change_location(int location);
	void attack(int game_id);
	void heal(int game_id);
	void mine();
	void explore();
	void bid(int price);
	void receive_state_code_result(std::shared_ptr<Proto_msg>msg);
	void receive_location_info(std::shared_ptr<Proto_msg>msg);
	void receive_ghost_sight(std::shared_ptr<Proto_msg>msg);
	void receive_hp_info(std::shared_ptr<Proto_msg>msg);
	void receive_res_info(std::shared_ptr<Proto_msg>msg);
	void receive_treasure_result(std::shared_ptr<Proto_msg>msg);
	void receive_treasure_info(std::shared_ptr<Proto_msg>msg);
	void receive_bid_info(std::shared_ptr<Proto_msg>msg);
	void receive_buyer_info(std::shared_ptr<Proto_msg>msg);
	void receive_stage_change(std::shared_ptr<Proto_msg>msg);
	void game_finish(std::shared_ptr<Proto_msg>msg);
	void next_day();

	friend class ThreadWorker;

signals://+
	//登录
	void login_success();//+
	void login_err();//+
	//大厅
	void send_roominfo(Room_prop, int);//+
	void join_result(int);//+
	//房间
	void set_room_info(Room_info);//+
	void send_user_info(User_info, int);	//+
	void set_room_info(std::string, int, int, int);//+
	void start_result(bool);
	//游戏中
	void send_game_info_quick(basic_game_info);
	void send_game_info_slow(basic_game_info);
	void send_treasure(int, int, int);
	void send_bid_info(std::string, std::string, std::string);
	void send_message(std::string);
};