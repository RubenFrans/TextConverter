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