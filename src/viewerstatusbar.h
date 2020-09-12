#ifndef VIEWERSTATUSBAR_H
#define VIEWERSTATUSBAR_H

#include <QWidget>

namespace Ui {
class ViewerStatusBar;
}

class ViewerStatusBar : public QWidget
{
    Q_OBJECT

public:
    explicit ViewerStatusBar(QWidget *parent = nullptr);
    ~ViewerStatusBar();

private:
    Ui::ViewerStatusBar *ui;
};

#endif // VIEWERSTATUSBAR_H
