#ifndef FILEBOXENTITY_H
#define FILEBOXENTITY_H

#include <QWidget>
#include <QStringListModel>

namespace Ui {
class FileBoxEntity;
}

class FileBoxEntity : public QWidget
{
    Q_OBJECT

public:
    explicit FileBoxEntity(QWidget *parent = nullptr);
    ~FileBoxEntity();

private:
    void setCurrentState(const QString&);

    Ui::FileBoxEntity *ui;
    QStringListModel* m_fileListModel;

signals:
    void valueChanged();

private slots:
    void handleLoadButtonClicked();
    void handleDeleteButtonClicked();
};

#endif // FILEBOXENTITY_H
