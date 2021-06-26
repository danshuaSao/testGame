#include "System.h"
#include "tools.h"
#include <array>
#include "Resource.h"

void System::run()
{
	sock->connect(ep);
	conn = std::make_shared<Tcp_connection>(io, sock, msg_que, session_id);
	io.post(bind(&System::route, shared_from_this()));
	io.post(bind(&System::message_route, shared_from_this()));
	conn->run();
	state = STATE::LOGIN;
	std::thread input_t(bind(&System::read_input, shared_from_this()));
	input_t.detach();
	std::cout << "run before\n";
	io.run();
	std::cout << "run after\n";
}

void System::receive_session_id(std::shared_ptr<Proto_msg> msg)
{
	deserialize_obj(msg->body, session_id);
	std::cout << "�����˻Ự��" << session_id << std::endl;
	std::cout << "������login+username���Ե�¼\n";
}

void System::login(std::string_view username)
{
	auto msg = std::make_shared<Proto_msg>(1, 1);
	std::string username_str(username.begin(), username.end());
	serialize_obj(msg->body, session_id, username_str);
	conn->push_event(msg);
	state = STATE::WAIT_LOGIN;
}
//+++++++++++++++++++++++++++++++++++++
void System::login()
{
	std::cout << "login";
	auto msg = std::make_shared<Proto_msg>(1, 1);
	serialize_obj(msg->body, session_id, username);
	conn->push_event(msg);
	state = STATE::WAIT_LOGIN;
}

void System::receive_login_result(std::shared_ptr<Proto_msg>msg)
{
	state_code sc;
	deserialize_obj(msg->body, sc);
	if (sc == CODE::LOGIN_SUCCESS)
	{
		emit login_success();
		std::cout << "��¼�ɹ�\n";
		hall_system_run();
	}
	if (sc == CODE::LOGIN_REPEATED)
	{
		emit login_err();
		std::cout << "���Ѿ��������ص��¼\n";
		std::cout << "������login+username���Ե�¼\n";
		state = STATE::LOGIN;
	}
}

void System::request_room_prop(bool no_cycle)
{
	using namespace std::chrono;
	static seconds last_request_time = 0s;
	if (no_cycle)
	{
		auto msg = std::make_shared<Proto_msg>(1, 2);
		serialize_obj(msg->body, session_id);
		conn->push_event(msg);
		return;
	}

	seconds dura = duration_cast<seconds>(steady_clock::now() - start_time);
	if (dura >= last_request_time + CONSTV::update_room_list_duration)
	{
		last_request_time = dura;
		auto msg = std::make_shared<Proto_msg>(1, 2);
		serialize_obj(msg->body, session_id);
		conn->push_event(msg);
	}
	io.post(bind(&System::request_room_prop, shared_from_this(), false));
}

void System::receive_room_prop(std::shared_ptr<Proto_msg>msg)
{
	if (state != STATE::HALL)
		return;
	std::vector<Room_prop>info_vec;
	state_code sc;
	deserialize_obj(msg->body, sc, info_vec);
	Otp_table room_info(3);
	room_info.insert({ "������","�����","��������" });
	int i = 0;//+
	for (const auto& info : info_vec)
	{
		room_info.insert({ info.name,std::to_string(info.id),std::to_string(info.size) + "/" + std::to_string(info.capacity) });
		emit send_roominfo(info, i);//+
		i++;//+
	}		
	std::cout << room_info;
}

void System::join_room(int room_id)
{
	auto msg = std::make_shared<Proto_msg>(1, 5);
	serialize_obj(msg->body, session_id, room_id);
	conn->push_event(msg);
	state = STATE::WAIT_JOIN;
}

void System::receive_join_result(std::shared_ptr<Proto_msg>msg)
{
	int room_id;
	state_code sc;
	deserialize_obj(msg->body, sc, room_id);
	if (sc != CODE::NONE)
	{
		std::cout << sc.message();
		state = STATE::HALL;
		emit join_result(0);
		return;
	}
	emit join_result(1);
	std::cout << "�ɹ����뷿��\n";
	this->room_id = room_id;
	room_system_run();
}

void System::make_room(std::string_view user_input)
{
	const std::string please_input_std_format = "�밴��ȷ��ʽ����,�磺\n my room name+6\n";
	int offset = user_input.find('+', 0);
	if (offset == std::string::npos)
	{
		std::cout << "��ʽ����\n" << please_input_std_format;
		return;
	}
	if (offset == user_input.size() - 1)
	{
		std::cout << "��ʽ����\n" << please_input_std_format;
		return;
	}
	int capacity = 0;
	for (int i = offset + 1; i < user_input.size(); i++)
	{
		if (!isdigit(user_input[i]))
		{
			std::cout << "��ʽ����\n" << please_input_std_format;
			return;
		}
		capacity = capacity * 10 + user_input[i] - '0';
		constexpr int MAX_CAPACITY = 20;
		if (capacity > MAX_CAPACITY)
		{
			std::cout << "��������\n" << please_input_std_format;
			return;
		}
	}
	auto msg = std::make_shared<Proto_msg>(1, 3);
	std::string name = std::string(user_input.substr(0, offset).begin(), user_input.substr(0, offset).end());
	serialize_obj(msg->body, session_id, name, capacity);
	conn->push_event(msg);
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void System::make_room(std::string room_name,int capacity)
{
	
	auto msg = std::make_shared<Proto_msg>(1, 3);
	//std::string name = std::string(user_input.substr(0, offset).begin(), user_input.substr(0, offset).end());
	serialize_obj(msg->body, session_id, room_name, capacity);
	conn->push_event(msg);
}

void System::receive_make_result(std::shared_ptr<Proto_msg>msg)
{
	Room_prop prop;
	deserialize_obj(msg->body, prop);
	room_id = prop.id;
	Otp_table room_info(3);
	std::cout << "�����ѽ���";
	room_info.insert({ "������", "�����", "��������" });
	room_info.insert({ prop.name,std::to_string(room_id),"1/" + std::to_string(prop.capacity) });
	std::cout << room_info;
	room_system_run();
}

void System::exit_room()
{
	auto req_msg = std::make_shared<Proto_msg>(1, 4);
	serialize_obj(req_msg->body, session_id, room_id);
	conn->push_event(req_msg);
	hall_system_run();
	request_room_prop(true);
}

ASYNC_RET System::route()
{
	if (msg_que.empty())
	{
		io.post(bind(&System::route, shared_from_this()));
		return;
	}
	std::shared_ptr<Proto_msg> msg = msg_que.front();
	msg_que.pop();
	switch (msg->head.service)
	{
	case 0:
		receive_session_id(msg);
		break;
	case 1:
		receive_login_result(msg);
		break;
	case 2:
		receive_room_prop(msg);
		break;
	case 3:
		receive_make_result(msg);
		break;
	case 5:
		receive_join_result(msg);
		break;
	case 6:
		update_room_info(msg);
		break;
	case 49:
		sync_time(msg);
		break;
	case 51:
		create_game_info(msg);
		break;
	case 52:
		receive_state_code_result(msg);
		break;
	case 53:
		receive_location_info(msg);
		break;
	case 54:
		receive_hp_info(msg);
		break;
	case 55:
		receive_res_info(msg);
		break;
	case 56:
		receive_bid_info(msg);
		break;
	case 57:
		receive_buyer_info(msg);
		break;
	case 58:
		receive_stage_change(msg);
		break;
	case 59:
		receive_treasure_result(msg);
		break;
	case 60:
		receive_treasure_info(msg);
		break;
	case 61:
		game_finish(msg);
		break;
	case 62:
		receive_ghost_sight(msg);
		break;

	default:
		std::cerr << "unknown package" << std::endl;
	}
	io.post(bind(&System::route, shared_from_this()));
}

ASYNC_RET System::message_route()
{
	auto input_ptr = input_que.try_pop();
	if (input_ptr == nullptr)
	{
		io.post(bind(&System::message_route, shared_from_this()));
		return;
	}
	std::string_view input = *input_ptr;

	if (state == STATE::LOGIN)
	{
		if (input.size() > 6 && input.substr(0, 5) == "login")
		{
			login(input.substr(6, input.size() - 6));
			io.post(bind(&System::message_route, shared_from_this()));
			return;
		}
	}

	if (state == STATE::HALL)
	{
		if (input.size() > 3 && input.substr(0, 2) == "mk")
		{
			make_room(input.substr(3, input.size() - 3));
			io.post(bind(&System::message_route, shared_from_this()));
			return;
		}
		if (input.size() > 3 && input.substr(0, 2) == "cd")
		{
			int room_id = 0;
			for (int i = 3; i < input.size(); i++)
			{
				if (!isdigit(input[i]))
				{
					io.post(bind(&System::message_route, shared_from_this()));
					return;
				}
				room_id = room_id * 10 + input[i] - '0';
			}
			join_room(room_id);
			io.post(bind(&System::message_route, shared_from_this()));
			return;
		}
	}

	if (state == STATE::ROOM)
	{
		if (input == "q")
			exit_room();
		if (input == "sr")
			set_ready();
		if (input == "sg")
			start_game();
	}

	if (state == STATE::GAME)
	{
		if (input.size() >= 3 && input.substr(0, 2) == "mv")
		{
			int val = input[2] - '0';
			change_location(val);
		}
		if (input.size() >= 4 && input.substr(0, 3) == "atk")
		{
			int game_id = input[3] - '0';
			attack(game_id);
		}
		if (input.size() >= 5 && input.substr(0, 4) == "heal")
		{
			int game_id = input[4] - '0';
			heal(game_id);
		}
		if (input.size() == 4 && input.substr(0, 4) == "mine")
		{
			mine();
		}

		if (input.size() >= 1 && input.size() <= 7 && input[0] == 'b')
		{
			for (int i = 1; i < input.size(); i++)
			{
				if (!isdigit(input[i]) && input[i] != ' ')
				{
					io.post(bind(&System::message_route, shared_from_this()));
					return;
				}
			}
			int val = 0;
			for (int i = 1; i < input.size(); i++)
				val = val + input[i] - '0';
			bid(val);
		}
		if (input.size() == 3 && input.substr(0, 3) == "exp")
		{
			explore();
		}
	}
	if (input == "?")
	{
		if (state == STATE::HALL)
			otp_hall_operation();
		if (state == STATE::ROOM)
			otp_room_operation();
		if (state == STATE::GAME)
			otp_game_operation();
	}
	io.post(bind(&System::message_route, shared_from_this()));
}

void System::otp_hall_operation()
{
	Otp_table table(2);
	table.insert({ "���뷿��","join 'id'" });
	table.insert({ "��������","mk name" });
	std::cout << table;
}

void System::otp_room_operation()
{
	Otp_table tbl(2);
	tbl.insert({ "�˳�����","q" });
	tbl.insert({ "׼��/ȡ��׼��","sr" });
	tbl.insert({ "��ʼ��Ϸ","sg" });
	std::cout << tbl;
}

void System::otp_game_operation()
{

}

void System::hall_system_run()
{
	state = STATE::HALL;
	request_room_prop();
}

void System::room_system_run()
{
	state = STATE::ROOM;
}

void System::sync_time(std::shared_ptr<Proto_msg>msg)
{
	deserialize_obj(msg->body, game_info->now_time);
	emit send_game_info_quick(*game_info.get());
}

void System::game_system_run()
{
	state = STATE::GAME;
	emit start_result(true);
}

void System::update_room_info(std::shared_ptr<Proto_msg>msg)
{
	deserialize_obj(msg->body, room_info);
	Otp_table prop_table(3);
	prop_table.insert({ "����","�����","����" });
	prop_table.insert({ room_info.name,std::to_string(room_info.room_id),std::to_string(room_info.user.size()) + "/" + std::to_string(room_info.capacity) });
	emit set_room_info(room_info);//+
	
	room_info.user;
	Otp_table user_table(2);
	user_table.insert({ "�û���","׼��״̬" });
	int i = 0;//+
	for (const auto& n : room_info.user)
	{
		user_table.insert({ n.name, (n.ready ? "��׼��" : "δ׼��") });
		emit send_user_info(n, i);//+
		i++;
	}
	std::cout << prop_table << user_table;
}

void System::read_input()
{
	std::string msg;
	getline(std::cin, msg);
	input_que.push(std::move(msg));
	//io.post(bind(&System::read_input, shared_from_this()));
	read_input();
}

void System::set_ready()
{
	auto req_msg = std::make_shared<Proto_msg>(1, 7);
	serialize_obj(req_msg->body, session_id, room_id);
	conn->push_event(req_msg);
}

void System::start_game()
{
	auto req_msg = std::make_shared<Proto_msg>(1, 8);
	serialize_obj(req_msg->body, session_id, room_id);
	conn->push_event(req_msg);
}

void System::create_game_info(std::shared_ptr<Proto_msg>msg)
{
	game_system_run();
	int character_id, hp;
	Resource res;
	deserialize_obj(msg->body, character_id, hp, res);
	std::cerr << character_id << "---------\n";
	game_info = std::make_shared<basic_game_info>();
	game_info->player.resize(room_info.user.size());
	int p = 0;
	for (auto ite = room_info.user.begin(); ite != room_info.user.end(); ite++, p++)
	{
		game_info->player[p].name = ite->name;
		game_info->player[p].session_id = ite->session_id;
		if (ite->session_id == session_id)
			game_info->game_id = p;

	}
	game_info->character_id = character_id;
	game_info->HP = hp;
	game_info->res = res;
	std::cout << *game_info;

	std::cout << "******************************************************************************************\n";
	std::cout << "******************************************************************************************\n";
}

bool System::daytime_action_check()
{
	if (!game_info->action_point)
	{
		std::cout << "û���㹻������\n";
		emit send_message("û���㹻������");
		return true;
	}
	if (game_info->stage != STAGE::DAYTIME)
	{
		std::cout << "���ڲ��ǰ���\n";
		emit send_message("���ڲ��ǰ���");
		return true;
	}
	return false;
}

void System::change_location(int location)
{
	if (!game_info->action_point)
	{
		std::cout << "û���㹻������\n";
		emit send_message("û���㹻������");
		return;
	}
	if (game_info->location == location)
	{
		std::cout << "�����ƶ�����ͬ�ĵص�\n";
		emit send_message("�����ƶ�����ͬ�ĵص�");
		return;
	}
	auto msg = std::make_shared<Proto_msg>(1, 52);
	serialize_obj(msg->body, session_id, location, room_id);
	conn->push_event(msg);
}

void System::attack(int game_id)
{
	if (daytime_action_check())
		return;
	if (game_info->res.armo == 0)
	{
		std::cout << "û�е�ҩ\n";
		emit send_message("û�е�ҩ");
		return;
	}
	int des = game_info->player[game_id].session_id;
	auto msg = std::make_shared<Proto_msg>(1, 53);
	serialize_obj(msg->body, session_id, des, room_id);
	conn->push_event(msg);
}

void System::heal(int game_id)
{
	if (daytime_action_check())
		return;
	if (game_info->res.bandage == 0)
	{
		std::cout << "û�б���\n";
		emit send_message("û�б���");
		return;
	}
	int des = game_info->player[game_id].session_id;
	auto msg = std::make_shared<Proto_msg>(1, 54);
	serialize_obj(msg->body, session_id, des, room_id);
	conn->push_event(msg);
}

void System::mine()
{
	if (daytime_action_check())
		return;
	auto msg = std::make_shared<Proto_msg>(1, 55);
	serialize_obj(msg->body, session_id, room_id);
	conn->push_event(msg);
}

void System::explore()
{
	if (daytime_action_check())
		return;
	if (game_info->character_id != 1)
		return;
	auto msg = std::make_shared<Proto_msg>(1, 57);
	serialize_obj(msg->body, session_id, room_id);
	conn->push_event(msg);
}

void System::bid(int price)
{
	if (game_info->res.coin < price)
	{
		std::cout << "û���㹻�Ľ�Ǯ";
		emit send_message("û���㹻�Ľ�Ǯ");

	}
	auto msg = std::make_shared<Proto_msg>(1, 56);
	serialize_obj(msg->body, session_id, room_id, price);
	conn->push_event(msg);
}

void System::receive_state_code_result(std::shared_ptr<Proto_msg> msg)
{
	state_code sc;
	deserialize_obj(msg->body, sc);
	switch (sc)
	{
	case CODE::START_GAME:
		break;
	case CODE::MOVE_SUCCESS:
		game_info->action_point = false;
		break;
	case CODE::ATTACK_SUCCESS:
		game_info->action_point = false;
		game_info->res.armo--;
		break;
	case CODE::HEAL_SUCCESS:
		game_info->action_point = false;
		game_info->res.bandage--;
		break;
	case CODE::MINE_SUCCESS:
		game_info->action_point = false;
		game_info->res.coin += CONSTV::MINE_COIN;
		break;
	case CODE::EXPLORE_RECEIVE:
		game_info->action_point = false;
		break;
	default:
		break;
	}
	std::cout << sc.message() << "\n";
	emit send_message(sc.message());
}

void System::receive_hp_info(std::shared_ptr<Proto_msg> msg)
{
	std::cout << msg->body << "\n";
	typedef std::pair<int, int>life_info;
	std::vector<life_info>hp_set;
	deserialize_obj(msg->body, hp_set);
	for (const auto& p : hp_set)
	{
		if (game_info->game_id == p.first)
			game_info->HP = p.second;
		game_info->player[p.first].HP = p.second;
	}
}

void System::receive_location_info(std::shared_ptr<Proto_msg>msg)
{
	std::cout << "call receive_location_info\n";
	std::vector<int>location_set;
	deserialize_obj(msg->body, location_set);
	for (auto& n : game_info->player)
		n.location = -1;
	for (int i = 1; i < location_set.size(); i++)
	{
		if (location_set[i] == game_info->game_id)
			game_info->location = location_set[0];
		game_info->player[location_set[i]].location = location_set[0];
	}
}

void System::receive_ghost_sight(std::shared_ptr<Proto_msg> msg)
{
	std::vector<int>location_set;
	deserialize_obj(msg->body, location_set);
	for (int i = 0; i < game_info->player.size(); i++)
	{
		if (i == game_info->game_id)
			game_info->location = location_set[i];
		game_info->player[i].location = location_set[i];
	}
}

void System::receive_res_info(std::shared_ptr<Proto_msg>msg)
{
	deserialize_obj(msg->body, game_info->res);
}

void System::receive_treasure_result(std::shared_ptr<Proto_msg> msg)
{
	state_code sc;
	deserialize_obj(msg->body, sc);
	if (sc == CODE::EXPLORE_SUCCESS)
	{
		std::cout << "�ɹ���ȡ����\n";
		emit send_message("�ɹ���ȡ����");
		game_info->res.hint++;
	}
	else
	{
		std::cout << "δ�ܻ�ȡ����\n";
		emit send_message("δ�ܻ�ȡ����");
	}
}

void System::receive_treasure_info(std::shared_ptr<Proto_msg> msg)
{
	typedef std::pair<int, int>P;
	std::vector<P>treasure_vec;
	deserialize_obj(msg->body, treasure_vec);
	Otp_table table(2);
	table.insert({ "�ص�","���ر��" });
	int i = 0;//+
	for (const auto& p : treasure_vec)
	{
		table.insert({ std::to_string(p.first),std::to_string(p.second) });
		emit send_treasure(p.first, p.second, i);//+
		i++;//+
	}
	std::cout << table;
}

void System::receive_bid_info(std::shared_ptr<Proto_msg> msg)
{
	Auction_item item;
	deserialize_obj(msg->body, item);
	Otp_table table(3);
	table.insert({ "��Ʒ��", "�۸�", "��ǰ������" });

	//+++++++++++++++++++
	std::string name = Auction_item::item_name[item.item_id];
	std::string price = std::to_string(item.price);
	std::string bidder;
	if (item.bidder != -1)
	{	
		bidder = game_info->player[item.bidder].name;
		//table.insert({ Auction_item::item_name[item.item_id],std::to_string(item.price),game_info->player[item.bidder].name });
		table.insert({ name,price,bidder });
		emit send_bid_info(name, price, bidder);//+
	}
	else
	{
		table.insert({ name,price,"���˾���" });
		emit send_bid_info(name, price, "���˾���");//+
	}
	std::cout << table;
}

void System::receive_buyer_info(std::shared_ptr<Proto_msg>msg)
{
	int game_id, price;
	deserialize_obj(msg->body, game_id, price);
	std::string message= game_info->player[game_id].name + "���� " +  std::to_string(price) + " �ļ۸����˱�����Ʒ";
	std::cout << message << "\n";
	emit send_message(message);
	std::cout << "*********************************************************************************\n";
}

void System::receive_stage_change(std::shared_ptr<Proto_msg>msg)
{
	//std::cout << "receive stage change\n";
	emit send_game_info_slow(*game_info.get());
	game_info->next_stage();
}

void System::game_finish(std::shared_ptr<Proto_msg> msg)
{
	std::cout << "��Ϸ����\n";
	emit send_message("��Ϸ����");
}

System::System(io_context& _io, ip::tcp::endpoint& _ep, threadsafe_queue<std::string>& _input_que)
	:io(_io), ep(_ep), input_que(_input_que)

{
	sock = std::make_shared<ip::tcp::socket>(io);

	qRegisterMetaType<Room_prop>("Room_prop");
	qRegisterMetaType<std::string>("std::string");
	qRegisterMetaType<Room_info>("Room_info");
	qRegisterMetaType<User_info>("User_info");
	qRegisterMetaType<basic_game_info>("basic_game_info");

}