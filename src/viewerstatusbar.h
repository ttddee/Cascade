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

    void setZoomText(const QString& s);
    void setWidthText(const QString& s);
    void setHeightText(const QString& s);
    void setViewerModeText(const QString& s);

    ~ViewerStatusBar();

private:
    Ui::ViewerStatusBar *ui;

signals:
    void requestZoomReset();
};

#endif // VIEWERSTATUSBAR_H
