#ifndef MESSAGEBOX_H
#define MESSAGEBOX_H

#include <QObject>
#include <QMessageBox>

class CsMessageBox : public QMessageBox
{
    Q_OBJECT

public:
    explicit CsMessageBox(QWidget* parent = nullptr);

    virtual ~CsMessageBox();
};

#endif // MESSAGEBOX_H
