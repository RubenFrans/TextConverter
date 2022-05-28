#pragma once
#include <QtGui>
#include <QWidget>
#include <qmainwindow.h>
#include "TextConverter.h"

#ifndef TEXTCONVERTERFORM_H
#define TEXTCONVERTERFORM_H
#include "ui_TextConverterForm.h"

namespace TextUI {
	class TextConverterForm : public QMainWindow
	{
		Q_OBJECT
	public:
		explicit TextConverterForm();

	private slots:
		void on_btnLoadInput_clicked();
		void on_rbToUtf_toggled(bool value);
		void on_rbToCodePage_toggled(bool value);
		void on_itxtOutputPath_textEdited(QString text);

	private:
		Ui::MainWindow ui;
		bool m_Utf8ToCodePage{ true };
		bool m_CodePageToUtf8{ false };

		TextConverter m_TextConverter;
		std::string m_InputFileString{};
		std::string m_OutputFileString{};

	};
}


#endif // !