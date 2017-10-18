#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QNetworkReply>
#include <QSystemTrayIcon>
#include <QEvent>
#include <QSystemTrayIcon>
#include <QNetworkAccessManager>

namespace Ui {
	class MainWindow;
	}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();
public slots:
	void on_clipboard_change();

	void showUp();

private slots:
	void on_Toggle_clicked();

	void on_responseFromServer(QNetworkReply *reply);

	void on_hide_btn_clicked();

	void toggle();

	bool on_sys_tray_click(enum QSystemTrayIcon::ActivationReason reason);

	void on_actionExit_triggered();

private:
	Ui::MainWindow *ui;
	void showTrayIcon();
	QString currentURI = "";
	QString lastConsidered = "";
	QSystemTrayIcon * trayIcon = 0;
	QNetworkAccessManager *networkManager;

	bool enabled = false;
	bool processing = false;
	bool processingDone = false;
};


#endif // MAINWINDOW_H
