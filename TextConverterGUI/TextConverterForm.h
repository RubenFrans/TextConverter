#pragma once
#include <QtGui>
#include <QWidget>
#include <qmainwindow.h>

#ifndef TEXTCONVERTERFORM_H
#define TEXTCONVERTERFORM_H
#include "ui_TextConverter.h"

namespace TextUI {
	class TextConverter : public QMainWindow
	{
		Q_OBJECT
	public:
		explicit TextConverter();

	private slots:


	private:
		Ui::MainWindow ui;
	};
}


#endif // !