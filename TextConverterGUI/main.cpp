#include <QtGui>
#include "TextConverterForm.h"

int main(int argc, char **argv)
{
    
    QApplication app(argc, argv);

    TextUI::TextConverterForm form{};
    form.show();

    return app.exec();
}
