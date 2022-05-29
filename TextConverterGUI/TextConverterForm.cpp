#include "TextConverterForm.h"
#include <sstream>
#include <qmessagebox.h>
#include <qfiledialog.h>
#include <qstandardpaths.h>
#include <iostream>

TextUI::TextConverterForm::TextConverterForm()
{
	ui.setupUi(this);
}

void TextUI::TextConverterForm::on_btnLoadInput_clicked() {
	std::cout << "Clicked load button" << std::endl;

	std::stringstream ss{};

	ss << "Open ";

	std::string fileformat{};

	if (m_CodePageToUtf8) {
		fileformat = "Code page 347";
	}
	else {
		fileformat = "Utf-8";
	}

	ss << fileformat;
	ss << " file";

	QString file = QFileDialog::getOpenFileName(this,
		 tr(ss.str().c_str()),
		 QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation),
		 tr("txt Files (*.txt)"));

	ui.inputFileLabel->setText(file);
	m_InputFileString = file.toStdString();
	ui.btnConvert->setEnabled(!file.isEmpty());
}

void TextUI::TextConverterForm::on_itxtOutputPath_textEdited(QString text) {

	m_OutputFileString = text.toStdString();
}

void TextUI::TextConverterForm::on_rbToUtf_toggled(bool value) {

	m_CodePageToUtf8 = value;
}


void TextUI::TextConverterForm::on_rbToCodePage_toggled(bool value) {
	m_Utf8ToCodePage = value;
}

void TextUI::TextConverterForm::on_btnConvert_clicked() {

	if (m_Utf8ToCodePage) {

		if (m_OutputFileString.empty()) {

			m_TextConverter.ConvertUTF8FileToCodePage(m_InputFileString);
		}
		else
		{
			m_TextConverter.ConvertUTF8FileToCodePage(m_InputFileString, m_OutputFileString);
		}

	} else if (m_CodePageToUtf8) {

		if (m_OutputFileString.empty())
			m_TextConverter.ConvertCodePageToUTF8(m_InputFileString);
		else
			m_TextConverter.ConvertCodePageToUTF8(m_InputFileString, m_OutputFileString);
	}

	ui.tableWidget->insertRow(ui.tableWidget->rowCount());
	ui.tableWidget->setItem(ui.tableWidget->rowCount() - 1,
		 0,
		 new QTableWidgetItem(QString(m_InputFileString.c_str())));
	ui.tableWidget->setItem(ui.tableWidget->rowCount() - 1,
		1,
		new QTableWidgetItem(QString(m_OutputFileString.c_str())));

	ui.tableWidget->setItem(ui.tableWidget->rowCount() - 1,
		2,
		new QTableWidgetItem(QString("Converted")));

}