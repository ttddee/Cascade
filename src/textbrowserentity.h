#ifndef TEXTBROWSERENTITY_H
#define TEXTBROWSERENTITY_H

#include <QWidget>

#include "uientity.h"

namespace Ui {
class TextBrowserEntity;
}

class TextBrowserEntity : public UiEntity
{
    Q_OBJECT

public:
    explicit TextBrowserEntity(UIElementType et, QWidget *parent = nullptr);

    void setText(const QString& s);

    QString getValuesAsString() override;

    ~TextBrowserEntity();

private:
    Ui::TextBrowserEntity *ui;
};

#endif // TEXTBROWSERENTITY_H
