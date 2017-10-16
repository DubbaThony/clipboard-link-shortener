#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QClipboard>
#include <QDebug>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QSystemTrayIcon>

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	trayIcon = new QSystemTrayIcon;

	trayIcon->setIcon(QIcon(":/ico/url_off.ico"));
	//trayIcon->
	trayIcon->show();

	connect(QApplication::clipboard(), SIGNAL(dataChanged()), this, SLOT(on_clipboard_change()));
	ui->statusBar->showMessage("Disabled");
}

MainWindow::~MainWindow()
{
	delete trayIcon;
	delete ui;
}

void MainWindow::on_clipboard_change(){

	if(!enabled) return;

	if(processingDone){
		processing = false;
		processingDone = false;
		return;
	}
	if(processing) return;

	processing = true;

	QClipboard *clipboard = QGuiApplication::clipboard(); //memory leak?

	if(clipboard->text().mid(0,7) != "http://" && clipboard->text().mid(0,8) != "https://" && clipboard->text() != this->lastConsidered){
		processing = false;
		return;
	}

	QNetworkAccessManager *manager = new QNetworkAccessManager();
	QNetworkRequest request;
	QString fullUrl;

	if(ui->base64->isChecked())
		fullUrl = this->ui->call_URI->text() + "=" + clipboard->text().toUtf8().toBase64();
	else
		fullUrl = this->ui->call_URI->text() + "=" + clipboard->text();

	request.setUrl(QUrl(fullUrl));


	manager->get(request);

	connect(manager, SIGNAL(finished(QNetworkReply*)), this,
					 SLOT(on_responseFromServer(QNetworkReply*)));

}

void MainWindow::on_Toggle_clicked()
{
	if(enabled){
		enabled = false;
		ui->statusBar->showMessage("Disabled");
		trayIcon->setIcon(QIcon(":/ico/url_off.ico")); //mem leak?
	}
	else{
		enabled = true;
		ui->statusBar->showMessage("Enabled");
		trayIcon->setIcon(QIcon(":/ico/url_on.ico")); //mem leak?
	}
}

void MainWindow::on_responseFromServer(QNetworkReply *reply){
	QClipboard *clipboard = QGuiApplication::clipboard();
	lastConsidered = reply->readAll();
	qDebug()<<lastConsidered;
	clipboard->setText(lastConsidered);
	trayIcon->showMessage("The link has been shortened",
						  "Your link has been shortened and conviniently pasted into your clipboard. New URI is "+lastConsidered,
						  QSystemTrayIcon::Information,
						  2500);
	processingDone = true;
}


void MainWindow::showTrayIcon(){
	trayIcon->show();
}

void MainWindow::on_hide_btn_clicked()
{
	this->hide();
}

void MainWindow::toggle(){
	this->on_Toggle_clicked();
}

void MainWindow::showUp(){
	this->show();
}
