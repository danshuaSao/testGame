#pragma once

#include<QDialog>
#include "ui_Login.h"
#include<QDebug>
#include<QMessagebox>

class Login : public QDialog
{
	Q_OBJECT

public:
	Login(QDialog *parent = Q_NULLPTR);
	~Login();
	
private:
	Ui::Login ui;
signals:
	void send_name(QString);
public slots:
	void clicklogin();
	void login_accept();
	void login_again();
};
