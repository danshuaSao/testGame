#include "hall_system.h"

hall_system::hall_system(QWidget* parent)
	: QDialog(parent)
{
	ui.setupUi(this);

	ui.enterBtn->setDisabled(true);
	connect(ui.enterBtn, SIGNAL(clicked()), this, SLOT(click_join()));
	connect(ui.creatBtn, SIGNAL(clicked()), this, SLOT(click_make()));


	model = new QStandardItemModel();
	model->setColumnCount(3);
	model->setHeaderData(0, Qt::Horizontal, QString::fromLocal8Bit("�����"));
	model->setHeaderData(1, Qt::Horizontal, QString::fromLocal8Bit("������"));
	model->setHeaderData(2, Qt::Horizontal, QString::fromLocal8Bit("��������"));

	ui.roomTable->setModel(model);
	// �����п��ڿ��ӽ�������Ӧ��� 
	ui.roomTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	// ����ɫ������ʾ 
	ui.roomTable->setAlternatingRowColors(true);
	// ��������ͼ�ν����޸����� 
	ui.roomTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
	//����ѡ�еķ�ʽ
	ui.roomTable->setSelectionBehavior(QAbstractItemView::SelectRows);
	//���õ���ѡ��
	ui.roomTable->setSelectionMode(QAbstractItemView::SingleSelection);

	ui.roomTable->setUpdatesEnabled(true);
	//��׼ѡ��״̬�Ƿ�ı�
	md = ui.roomTable->selectionModel();
	connect(md, SIGNAL(selectionChanged(QItemSelection, QItemSelection)), this, SLOT(view_select_check(QItemSelection, QItemSelection)));


	ui.capacity->clear();
	for (int i = 1; i <=20; i++)
	{
		ui.capacity->addItem(QString::asprintf("%d", i));
	}


}

hall_system::~hall_system()
{
}

void hall_system::click_join()
{
	int room_id;
	int row = ui.roomTable->currentIndex().row();
	QAbstractItemModel* model = ui.roomTable->model();
	QModelIndex index = model->index(row, 0);
	QVariant id = model->data(index);
	room_id = id.toInt();
	emit send_join(room_id);
}

void hall_system::click_make()
{
	QString name = ui.roomnameEdit->text();
	int capacity = ui.capacity->currentIndex() + 1;
	emit send_make(name, capacity);
	accept();
}

void hall_system::insert_room(Room_prop room,int i)
{
	QString id, name,size, capacity,user;
	id=QString::number(room.id);
	name = QString::fromStdString(room.name);
	size = QString::number(room.size);
	capacity = QString::number(room.capacity);
	user = size + "/" + capacity;
	//qDebug() << "\n\n\n" << i << "    " << name;

	QStandardItem* item = 0;
	item = new QStandardItem(id);
	model->setItem(i, 0, item);
	model->item(i, 0)->setTextAlignment(Qt::AlignCenter);
	item = new QStandardItem(name);
	model->setItem(i, 1, item);
	model->item(i, 1)->setTextAlignment(Qt::AlignCenter);
	item = new QStandardItem(user);
	model->setItem(i, 2, item);
	model->item(i, 2)->setTextAlignment(Qt::AlignCenter);

	ui.roomTable->setModel(model);

}

void hall_system::get_join_state(int state)
{
	if (state == 1)
	{
		accept();
	}
	else if(state==0)
	{
		QMessageBox::warning(this, QStringLiteral("Waring"),
			QStringLiteral("�÷���������"),
			QMessageBox::Yes);
	}
	//else if (state == -1)
	//{
	//	QMessageBox::warning(this, QStringLiteral("Waring"),
	//		QStringLiteral("��ѡ�񷿼�"),
	//		QMessageBox::Yes);
	//}
}
void hall_system :: view_select_check(QItemSelection, QItemSelection)
{
	if (md->hasSelection())
		ui.enterBtn->setEnabled(true);
	else
		ui.enterBtn->setDisabled(true);
}