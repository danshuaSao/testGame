#include "room_system.h"

room_system::room_system(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);

	connect(ui.beginBtn, SIGNAL(clicked()), this, SLOT(get_start()));
	connect(ui.readyBtn, SIGNAL(clicked()), this, SLOT(get_set_ready()));
	connect(ui.exitBtn, SIGNAL(clicked()), this, SLOT(get_exit()));


	model_1 = new QStandardItemModel();
	model_1->setColumnCount(3);
	model_1->setHeaderData(0, Qt::Horizontal, QString::fromLocal8Bit("房间号"));
	model_1->setHeaderData(1, Qt::Horizontal, QString::fromLocal8Bit("房间名"));
	model_1->setHeaderData(2, Qt::Horizontal, QString::fromLocal8Bit("房间人数"));

	ui.room_info_table->setModel(model_1);

	// 设置列宽在可视界面自适应宽度 
	ui.room_info_table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	// 行颜色交替显示 
	ui.room_info_table->setAlternatingRowColors(true);
	// 不允许在图形界面修改内容 
	ui.room_info_table->setEditTriggers(QAbstractItemView::NoEditTriggers);
	//整行选中的方式
	ui.room_info_table->setSelectionBehavior(QAbstractItemView::SelectRows);
	//设置不可选择
	ui.room_info_table->setSelectionMode(QAbstractItemView::NoSelection);

	model_2 = new QStandardItemModel();
	model_2->setColumnCount(2);
	model_2->setHeaderData(0, Qt::Horizontal, QString::fromLocal8Bit("用户名"));
	model_2->setHeaderData(1, Qt::Horizontal, QString::fromLocal8Bit("准备状态"));

	ui.user_table->setModel(model_2);
	// 设置列宽在可视界面自适应宽度 
	ui.user_table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	// 行颜色交替显示 
	ui.user_table->setAlternatingRowColors(true);
	// 不允许在图形界面修改内容 
	ui.user_table->setEditTriggers(QAbstractItemView::NoEditTriggers);
	//整行选中的方式
	ui.user_table->setSelectionBehavior(QAbstractItemView::SelectRows);
	//设置不可选择
	ui.user_table->setSelectionMode(QAbstractItemView::NoSelection);


}

room_system::~room_system()
{
}


void room_system::insert_user(User_info usr,int i)
{
	QString name, ready;
	name = QString::fromLocal8Bit(usr.name.data());
	if (usr.ready)
	{
		ready = QStringLiteral ("已准备");
	}
	else ready = QStringLiteral("未准备");

	QStandardItem* item = 0;
	item = new QStandardItem(name);
	model_2->setItem(i, 0, item);
	model_2->item(i, 0)->setTextAlignment(Qt::AlignCenter);
	item = new QStandardItem(ready);
	model_2->setItem(i, 1, item);
	model_2->item(i, 1)->setTextAlignment(Qt::AlignCenter);

	ui.user_table->setModel(model_2);
}

void room_system::get_room_info(Room_info room)
{


	QString id, name, size, capacity, user;
	id = QString::number(room.room_id);
	name = QString::fromStdString(room.name);
	size = QString::number(room.user.size());
	capacity = QString::number(room.capacity);
	user = size + "/" + capacity;

	QStandardItem* item = 0;
	item = new QStandardItem(id);
	model_1->setItem(0, 0, item);
	model_1->item(0, 0)->setTextAlignment(Qt::AlignCenter);
	item = new QStandardItem(name);
	model_1->setItem(0, 1, item);
	model_1->item(0, 1)->setTextAlignment(Qt::AlignCenter);
	item = new QStandardItem(user);
	model_1->setItem(0, 2, item);
	model_1->item(0, 2)->setTextAlignment(Qt::AlignCenter);

	ui.room_info_table->setModel(model_1);
}

void room_system::get_room_info(std::string name, int id, int size, int capacity)
{

	QString qid, qname, qsize, qcapacity, quser;
	qid = QString::number(id);
	qname = QString::fromStdString(name);
	qsize = QString::number(size);
	qcapacity = QString::number(capacity);
	quser = qsize + "/" + qcapacity;

	QStandardItem* item = 0;
	item = new QStandardItem(qid);
	model_1->setItem(0, 0, item);
	model_1->item(0, 0)->setTextAlignment(Qt::AlignCenter);
	item = new QStandardItem(qname);
	model_1->setItem(0, 1, item);
	model_1->item(0, 1)->setTextAlignment(Qt::AlignCenter);
	item = new QStandardItem(quser);
	model_1->setItem(0, 2, item);
	model_1->item(0, 2)->setTextAlignment(Qt::AlignCenter);

	ui.room_info_table->setModel(model_1);
}

void room_system::get_start_result(bool result)
{
	if (result)
	{
		accept();
	}
}

void room_system::get_message(std::string str)
{
	if (str == "玩家没有全部准备"||str== "房间没有满员"||str== "你不是房主")
	{
		QMessageBox::warning(this, QStringLiteral("Waring"),
			QString::fromLocal8Bit(str.data()),
			QMessageBox::Yes);
	}
}

void room_system::get_set_ready()
{
	qDebug() << "press ready";
	emit set_ready();
}

void room_system::get_start()
{
	qDebug() << "press start";
	emit start_game();
}

void room_system::get_exit()
{
	emit exit();
	close();
}


