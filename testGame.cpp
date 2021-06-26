#include "testGame.h"

testGame::testGame(QWidget* parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	connect(ui.mvBtn, SIGNAL(clicked()), this, SLOT(click_mv()));
	connect(ui.mineBtn, SIGNAL(clicked()), this, SLOT(click_mine()));
	connect(ui.atkBtn, SIGNAL(clicked()), this, SLOT(click_atk()));
	connect(ui.healBtn, SIGNAL(clicked()), this, SLOT(click_heal()));
	connect(ui.expBtn, SIGNAL(clicked()), this, SLOT(click_exp()));
	connect(ui.bidBtn, SIGNAL(clicked()), this, SLOT(click_bid()));



/**********************************************************************************************************************************/
//�����Ʒ��Ϣ��
	model_player_info = new QStandardItemModel();
	model_player_info->setColumnCount(6);
	model_player_info->setRowCount(1);
	model_player_info->setHeaderData(0, Qt::Horizontal, QString::fromLocal8Bit("����"));
	model_player_info->setHeaderData(1, Qt::Horizontal, QString::fromLocal8Bit("�ص�"));
	model_player_info->setHeaderData(2, Qt::Horizontal, QString::fromLocal8Bit("��ҩ"));
	model_player_info->setHeaderData(3, Qt::Horizontal, QString::fromLocal8Bit("���"));
	model_player_info->setHeaderData(4, Qt::Horizontal, QString::fromLocal8Bit("����"));
	model_player_info->setHeaderData(5, Qt::Horizontal, QString::fromLocal8Bit("����"));
	ui.playerInfo->setModel(model_player_info);
	ui.playerInfo->verticalHeader()->setVisible(false);//�����к�
	ui.playerInfo->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);// �������п��ڿ��ӽ�������Ӧ��� 
	ui.playerInfo->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	ui.playerInfo->setEditTriggers(QAbstractItemView::NoEditTriggers);// ��������ͼ�ν����޸����� 
	ui.playerInfo->setSelectionMode(QAbstractItemView::NoSelection);//����ѡ��
	
																	
/**********************************************************************************************************************************/
//���������״̬�б�
	model_player_list = new QStandardItemModel();
	model_player_list->setColumnCount(4);
	model_player_list->setHeaderData(0, Qt::Horizontal, QString::fromLocal8Bit("id"));
	model_player_list->setHeaderData(1, Qt::Horizontal, QString::fromLocal8Bit("�ǳ�"));
	model_player_list->setHeaderData(2, Qt::Horizontal, QString::fromLocal8Bit("�ص�"));
	model_player_list->setHeaderData(3, Qt::Horizontal, QString::fromLocal8Bit("����"));

	ui.playerList->setModel(model_player_list);
	ui.playerList->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);	// �����п��ڿ��ӽ�������Ӧ��� 
	//ui.playerList->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	ui.playerList->setAlternatingRowColors(true);	// ����ɫ������ʾ 
	ui.playerList->setEditTriggers(QAbstractItemView::NoEditTriggers);	// ��������ͼ�ν����޸�����
	ui.playerList->setSelectionMode(QAbstractItemView::SingleSelection);//����ѡ��
	ui.playerList->setSelectionBehavior(QAbstractItemView::SelectRows);

/**********************************************************************************************************************************/
//��ұ�����Ϣ��
	model_my_info = new  QStandardItemModel();
	model_my_info->setColumnCount(1);
	model_my_info->setRowCount(4);
	model_my_info->setHeaderData(0, Qt::Vertical, QString::fromLocal8Bit("�ҵ����"));
	model_my_info->setHeaderData(1, Qt::Vertical, QString::fromLocal8Bit("��ǰʱ��"));
	model_my_info->setHeaderData(2, Qt::Vertical, QString::fromLocal8Bit("�ж��׶�"));
	model_my_info->setHeaderData(3, Qt::Vertical, QString::fromLocal8Bit("��ǰ����"));
	
	ui.myInfo->setModel(model_my_info);
	ui.myInfo->horizontalHeader()->setVisible(false);//�����к�
	ui.myInfo->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);// �������п��ڿ��ӽ�������Ӧ��� 
	ui.myInfo->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	ui.myInfo->setAlternatingRowColors(true);// ����ɫ������ʾ 
	ui.myInfo->setEditTriggers(QAbstractItemView::NoEditTriggers);// ��������ͼ�ν����޸����� 
	ui.myInfo->setSelectionMode(QAbstractItemView::NoSelection);//����ѡ��

/**********************************************************************************************************************************/
//������
	model_bid = new QStandardItemModel();
	model_bid->setColumnCount(3);
	model_bid->setHeaderData(0, Qt::Horizontal, QString::fromLocal8Bit("��Ʒ��"));
	model_bid->setHeaderData(1, Qt::Horizontal, QString::fromLocal8Bit("�۸�"));
	model_bid->setHeaderData(2, Qt::Horizontal, QString::fromLocal8Bit("��ǰ������"));

	ui.bidTable->setModel(model_bid);
	ui.bidTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);	// �����п��ڿ��ӽ�������Ӧ��� 
	ui.bidTable->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	ui.bidTable->setEditTriggers(QAbstractItemView::NoEditTriggers);// ��������ͼ�ν����޸����� 
	ui.bidTable->setSelectionMode(QAbstractItemView::NoSelection);//����ѡ��

/**********************************************************************************************************************************/
//̽����
	model_exp = new QStandardItemModel();
	model_exp->setColumnCount(2);
	model_exp->setHeaderData(0, Qt::Horizontal, QString::fromLocal8Bit("�ص�"));
	model_exp->setHeaderData(1, Qt::Horizontal, QString::fromLocal8Bit("���ر��"));
	ui.expTable->setModel(model_exp);
	ui.expTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);	// �����п��ڿ��ӽ�������Ӧ��� 

	

}


void testGame::get_game_info_quick(basic_game_info info)
{
/**********************************************************************************************************************************/
//��ұ�����Ϣ��
	//std::string character_name;
	QString character;
	if (info.character_id == 1)
	{
		QString character_name = QString::fromLocal8Bit("̽�ռ�");
		character = character_name;
	}
	if (info.character_id == 2)
	{
		QString character_name = QString::fromLocal8Bit("����");
		character = character_name;
	}

	QStandardItem* item = new QStandardItem(character);
	model_my_info->setItem(0, 0, item);
	model_my_info->item(0, 0)->setTextAlignment(Qt::AlignCenter);

	QString time = QString::number(info.now_time - info.today_time);
	item = new QStandardItem(time);
	model_my_info->setItem(1, 0, item);
	model_my_info->item(1, 0)->setTextAlignment(Qt::AlignCenter);

	QString stage = get_stage(info.stage_out,info.turn);
	item = new QStandardItem(stage);
	model_my_info->setItem(2, 0, item);
	model_my_info->item(2, 0)->setTextAlignment(Qt::AlignCenter);
	
	QString day = QString::number(info.day+1);
	item = new QStandardItem(day);
	model_my_info->setItem(3, 0, item);
	model_my_info->item(3, 0)->setTextAlignment(Qt::AlignCenter);
	ui.myInfo->setModel(model_my_info);
/**********************************************************************************************************************************/
//�����Ʒ��Ϣ��
	std::vector<QString>player_info_vec;
	player_info_vec.emplace_back(QString::number(info.HP));
	player_info_vec.emplace_back(QString::number(info.location));
	player_info_vec.emplace_back(QString::number(info.res.armo));
	player_info_vec.emplace_back(QString::number(info.res.coin));
	player_info_vec.emplace_back(QString::number(info.res.bandage));
	player_info_vec.emplace_back(QString::number(info.res.hint));
	for (int i = 0; i < player_info_vec.size(); i++)
	{
		item = new QStandardItem(player_info_vec[i]);
		model_player_info->setItem(0, i, item);
		model_player_info->item(0, i)->setTextAlignment(Qt::AlignCenter);
	}
	ui.playerInfo->setModel(model_player_info);

/**********************************************************************************************************************************/
	//���������
	if (info.stage_out==2)
	{
		item = new QStandardItem("");
		model_bid->setItem(0, 0, item);
		item = new QStandardItem("");
		model_bid->setItem(0, 1, item);
		item = new QStandardItem("");
		model_bid->setItem(0, 2, item);
		ui.bidTable->setModel(model_bid);
	}

/**********************************************************************************************************************************/
	
	//���̽����
	if (info.stage_out==2)
	{
		for (int i = 0; i < model_exp->rowCount(); i++)
		{
			QStandardItem* item = new QStandardItem("");
			model_exp->setItem(i, 0, item);
			item = new QStandardItem("");
			model_exp->setItem(i, 1, item);
		}
		ui.expTable->setModel(model_exp);
	}

	set_action(info.action);//���ÿ��ð���
}

QString testGame::get_stage(int stage,int turn)
{
	QString str;
	if (stage == 0)
		str = QString::fromLocal8Bit("�ж��׶�1");
	if (stage == 1)
		str = QString::fromLocal8Bit("�ж��׶�2");
	if (stage == 2)
		str = QString::fromLocal8Bit("����") + QString::number(turn+1);
	if (stage == 3)
		str = QString::fromLocal8Bit("ҹ��һ�׶�");
	if (stage == 4)
		str = QString::fromLocal8Bit("ҹ����׶�");
	if (stage == 5)
		str = QString::fromLocal8Bit("׼���׶�");

	return str;
}

void testGame::set_action(bool a[])
{
	ui.mvBtn->setDisabled(!a[0]);
	ui.mvBtn->setEnabled(a[0]);

	ui.mineBtn->setDisabled(!a[1]);
	ui.mineBtn->setEnabled(a[1]);

	ui.atkBtn->setDisabled(!a[2]);
	ui.atkBtn->setEnabled(a[2]);

	ui.healBtn->setDisabled(!a[3]);
	ui.healBtn->setEnabled(a[3]);

	ui.expBtn->setDisabled(!a[4]);
	ui.expBtn->setEnabled(a[4]);
}

void testGame::get_game_info_slow(basic_game_info info)
{
	QStandardItem* item = nullptr;
	/**********************************************************************************************************************************/
//���������״̬�б�
	for (int i = 0; i < info.player.size(); i++)
	{
		QString unknown = QString::fromLocal8Bit("δ֪");
		item = new QStandardItem(QString::number(i));
		model_player_list->setItem(i, 0, item);
		item = new QStandardItem(QString::fromLocal8Bit(info.player[i].name.data()));
		model_player_list->setItem(i, 1, item);
		item = new QStandardItem(info.player[i].location == -1 ? unknown : QString::fromStdString(std::to_string(info.player[i].location)));
		model_player_list->setItem(i, 2, item);
		item = new QStandardItem(QString::number(info.player[i].HP));
		model_player_list->setItem(i, 3, item);
	}
	ui.playerList->setModel(model_player_list);
}

void testGame::get_treasure(int tre1, int tre2, int i)
{
	QStandardItem* item = new QStandardItem(QString::number(tre1));
	model_exp->setItem(i, 0, item);
	model_exp->item(i, 0)->setTextAlignment(Qt::AlignCenter);

	item = new QStandardItem(QString::number(tre2));
	model_exp->setItem(i, 1, item);
	model_exp->item(i, 1)->setTextAlignment(Qt::AlignCenter);

	ui.expTable->setModel(model_exp);
}

void testGame::get_bid_info(std::string _name, std::string _price, std::string _bidder)
{
	QString name = QString::fromLocal8Bit(_name.data());
	QString price = QString::fromStdString(_price);
	QString bidder = QString::fromLocal8Bit(_bidder.data());
	
	QStandardItem* item = new QStandardItem(name);
	model_bid->setItem(0, 0, item);
	item = new QStandardItem(price);
	model_bid->setItem(0, 1, item);
	item = new QStandardItem(bidder);
	model_bid->setItem(0, 2, item);

	ui.bidTable->setModel(model_bid);
}

void testGame::get_message(std::string message)
{
	QString print = QString::fromLocal8Bit(message.data());
	ui.tips->setText("");
	ui.tips->setText(print);

}

void testGame::click_mv()
{
	QString mv;
	if (ui.mv1Radio->isChecked())
		mv = "1";
	else if (ui.mv2Radio->isChecked())
		mv = "2";

	mv = "mv" + mv;
	emit send_input(mv);
}

void testGame::click_mine()
{
	emit send_input("mine");
}

void testGame::click_atk()
{
	int row = ui.playerList->currentIndex().row();
	QAbstractItemModel* model = ui.playerList->model();
	QModelIndex index = model->index(row, 0);
	QVariant id = model->data(index);
	QString atk ="atk"+id.toString();

	emit send_input(atk);
}

void testGame::click_heal()
{
	int row = ui.playerList->currentIndex().row();
	QAbstractItemModel* model = ui.playerList->model();
	QModelIndex index = model->index(row, 0);
	QVariant id = model->data(index);
	QString heal = "heal" + id.toString();

	emit send_input(heal);
}

void testGame::click_exp()
{
	emit send_input("exp");
}

void testGame::click_bid()
{
	QString coin = ui.bidEdit->text();
	QString bid = "b" + coin;
	emit send_input(bid);
}
