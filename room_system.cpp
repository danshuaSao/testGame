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
	model_1->setHeaderData(0, Qt::Horizontal, QString::fromLocal8Bit("�����"));
	model_1->setHeaderData(1, Qt::Horizontal, QString::fromLocal8Bit("������"));
	model_1->setHeaderData(2, Qt::Horizontal, QString::fromLocal8Bit("��������"));

	ui.room_info_table->setModel(model_1);

	// �����п��ڿ��ӽ�������Ӧ��� 
	ui.room_info_table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	// ����ɫ������ʾ 
	ui.room_info_table->setAlternatingRowColors(true);
	// ��������ͼ�ν����޸����� 
	ui.room_info_table->setEditTriggers(QAbstractItemView::NoEditTriggers);
	//����ѡ�еķ�ʽ
	ui.room_info_table->setSelectionBehavior(QAbstractItemView::SelectRows);
	//���ò���ѡ��
	ui.room_info_table->setSelectionMode(QAbstractItemView::NoSelection);

	model_2 = new QStandardItemModel();
	model_2->setColumnCount(2);
	model_2->setHeaderData(0, Qt::Horizontal, QString::fromLocal8Bit("�û���"));
	model_2->setHeaderData(1, Qt::Horizontal, QString::fromLocal8Bit("׼��״̬"));

	ui.user_table->setModel(model_2);
	// �����п��ڿ��ӽ�������Ӧ��� 
	ui.user_table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	// ����ɫ������ʾ 
	ui.user_table->setAlternatingRowColors(true);
	// ��������ͼ�ν����޸����� 
	ui.user_table->setEditTriggers(QAbstractItemView::NoEditTriggers);
	//����ѡ�еķ�ʽ
	ui.user_table->setSelectionBehavior(QAbstractItemView::SelectRows);
	//���ò���ѡ��
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
		ready = QStringLiteral ("��׼��");
	}
	else ready = QStringLiteral("δ׼��");

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
	if (str == "���û��ȫ��׼��"||str== "����û����Ա"||str== "�㲻�Ƿ���")
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


