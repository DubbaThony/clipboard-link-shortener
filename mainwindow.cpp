#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QClipboard>
#include <QDebug>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QAction>
#include <QThread>

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	ui->statusBar->showMessage("Initializing");

	this->setWindowIcon(QIcon(":/ico/url_off.ico"));

	QAction *showAction = new QAction(tr("&Show/Hide"), this);
	connect(showAction, &QAction::triggered, this, &MainWindow::showUp);

	QAction *exitAction = new QAction(tr("&Exit"), this);
	connect(exitAction, &QAction::triggered, this, &MainWindow::on_actionExit_triggered);

	QMenu * trayMenu = new QMenu; /*its in constructor so I dont need pointer for that later down the linea*/
	trayMenu->addAction(showAction);
	trayMenu->addAction(exitAction);

	trayIcon = new QSystemTrayIcon;
	trayIcon->setContextMenu(trayMenu);
	trayIcon->setIcon(QIcon(":/ico/url_off.ico"));


	connect(trayIcon, &QSystemTrayIcon::activated, this, &MainWindow::on_sys_tray_click);

	trayIcon->show();

	this->networkManager = new QNetworkAccessManager();

	connect(this->networkManager, SIGNAL(finished(QNetworkReply*)), this,
					 SLOT(on_responseFromServer(QNetworkReply*)));


	connect(QApplication::clipboard(), SIGNAL(dataChanged()), this, SLOT(on_clipboard_change()));
	ui->statusBar->showMessage("Disabled");
}

MainWindow::~MainWindow()
{
	delete trayIcon;
	delete ui;
}

void MainWindow::on_clipboard_change(){

	QThread::msleep(1); //workaround to let the OS finish handling the clipboard

	QString text = QGuiApplication::clipboard()->text();

	if(!enabled) return;

	if(processingDone){
		processing = false;
		processingDone = false;
		return;
	}
	if(processing) return;

	processing = true;

	if(text.mid(0,7) != "http://" && text.mid(0,8) != "https://" && text != this->lastConsidered){
		processing = false;
		return;
	}

	trayIcon->showMessage("The link is beeing processed",
						  "Your link will be shortened in a second...",
						  QSystemTrayIcon::Information,
						  2500);

	QNetworkRequest request;
	QString fullUrl;

	if(ui->base64->isChecked())
		fullUrl = this->ui->call_URI->text() + "=" + text.toUtf8().toBase64();
	else
		fullUrl = this->ui->call_URI->text() + "=" + text;

	request.setUrl(QUrl(fullUrl));


	this->networkManager->get(request);


}

void MainWindow::on_Toggle_clicked()
{
	if(enabled){
		enabled = false;
		ui->statusBar->showMessage("Disabled");
		trayIcon->setIcon(QIcon(":/ico/url_off.ico")); //mem leak?
		this->setWindowIcon(QIcon(":/ico/url_off.ico"));
	}
	else{
		enabled = true;
		ui->statusBar->showMessage("Enabled");
		trayIcon->setIcon(QIcon(":/ico/url_on.ico")); //mem leak?
		this->setWindowIcon(QIcon(":/ico/url_on.ico"));
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
	delete reply;
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
	if(this->isHidden())
		this->show();
	else
		this->hide();
}

bool MainWindow::on_sys_tray_click(enum QSystemTrayIcon::ActivationReason reason){
	if(reason == QSystemTrayIcon::Trigger){
		this->on_Toggle_clicked();
		return false;
	} else if(reason == QSystemTrayIcon::DoubleClick){
		this->showUp();
		return false;
	} else {
		return true;
	}
}

void MainWindow::on_actionExit_triggered(){
	qApp->exit();
}
