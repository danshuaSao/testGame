#include "Login.h"

Login::Login(QDialog *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	connect(ui.loginBtn, SIGNAL(clicked()), this, SLOT(clicklogin()));
}

Login::~Login()
{
}

void Login::clicklogin()
{
	QString name = ui.usrEdit->text();
	emit send_name(name);
}

void Login::login_accept()
{
	accept();
}

void Login::login_again()
{
	QMessageBox::warning(this, QStringLiteral("Waring"),
		QStringLiteral("登录失败，请尝试更改用户名"),
		QMessageBox::Yes);
}