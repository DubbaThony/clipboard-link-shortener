#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QNetworkReply>
#include <QSystemTrayIcon>

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

private slots:
	void on_Toggle_clicked();

	void on_responseFromServer(QNetworkReply *reply);

	void on_hide_btn_clicked();

	void toggle();

	void showUp();

private:
	Ui::MainWindow *ui;
	void showTrayIcon();
	QString currentURI = "";
	QString lastConsidered = "";
	QSystemTrayIcon * trayIcon = 0;

	bool enabled = false;
	bool processing = false;
	bool processingDone = false;
};


#endif // MAINWINDOW_H
