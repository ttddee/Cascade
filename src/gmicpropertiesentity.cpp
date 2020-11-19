#include "gmicpropertiesentity.h"
#include "ui_gmicpropertiesentity.h"

#include <iostream>

#include <QTextStream>

#include "nodeproperties.h"
#include "propertiesheading.h"
#include "cssliderbox.h"
#include "colorbuttonentity.h"
#include "textbrowserentity.h"

GmicPropertiesEntity::GmicPropertiesEntity(
        UIElementType et,
        const QString& gType,
        const QString& gHash,
        QWidget *parent) :
    UiEntity(et, parent),
    ui(new Ui::GmicPropertiesEntity)
{
    ui->setupUi(this);

    gmicNodeType = gType;
    gmicHash = gHash;

    gmicHelper = &GmicHelper::getInstance();

    auto heading = new PropertiesHeading("G'MIC " + gType);
    ui->verticalLayout->addWidget(heading);

    std::cout << gmicHelper->getFiltersModel().getFilterFromHash(gHash).parameters().toStdString() << std::endl;

    auto parameters = gmicHelper->getFiltersModel().getFilterFromHash(gHash).parameters();
    QString line;
    QTextStream stream(&parameters);

    while (stream.readLineInto(&line))
    {
        // TEMP
        if (line.contains("(%)"))
        {
            line.remove(QRegExp("(%)"));
        }

        if (line.contains("=int"))
        {
            auto title = line.split("=")[0];
            auto values = line.split(QRegExp("[()]"))[1].split(",");
            auto value = values[0];
            auto min = values[1];
            auto max = values[2];

            CsSliderBox* item =
                    new CsSliderBox(
                        UI_ELEMENT_TYPE_SLIDER_BOX_INT,
                        this);
            item->setName(title);
            item->setMinMaxStepValue(
                        min.toInt(),
                        max.toInt(),
                        1,
                        value.toInt());
            item->selfConnectToValueChanged(static_cast<NodeProperties*>(parent));
            ui->verticalLayout->addWidget(item);

            connect(item, &CsSliderBox::valueChangedInt,
                    this, &GmicPropertiesEntity::valueChanged);

            propElements.push_back(item);
        }
        else if (line.contains("=float"))
        {
            auto title = line.split("=")[0];
            auto values = line.split(QRegExp("[()]"))[1].split(",");
            auto value = values[0];
            auto min = values[1];
            auto max = values[2];

            CsSliderBox* item =
                    new CsSliderBox(
                        UI_ELEMENT_TYPE_SLIDER_BOX_DOUBLE,
                        this);
            item->setName(title);
            item->setMinMaxStepValue(
                        min.toDouble(),
                        max.toDouble(),
                        0.01,
                        value.toDouble());
            item->selfConnectToValueChanged(static_cast<NodeProperties*>(parent));
            ui->verticalLayout->addWidget(item);

            connect(item, &CsSliderBox::valueChangedDouble,
                    this, &GmicPropertiesEntity::valueChanged);

            propElements.push_back(item);
        }
        else if (line.contains("=color"))
        {
            auto title = line.split("=")[0];
            auto value = line.split(QRegExp("[()]"))[1];

            ColorButtonEntity* item = new ColorButtonEntity(
                        UI_ELEMENT_TYPE_COLOR_BUTTON,
                        this);
            item->setName(title);
            item->setColor(QColor(value));
            item->selfConnectToValueChanged(static_cast<NodeProperties*>(parent));
            ui->verticalLayout->addWidget(item);

            connect(item, &ColorButtonEntity::valueChanged,
                    this, &GmicPropertiesEntity::valueChanged);

            propElements.push_back(item);
        }
        else if (line.contains("=note"))
        {
            auto text = line.split("\"")[1];

            TextBrowserEntity* item = new TextBrowserEntity(
                        UI_ELEMENT_TYPE_TEXTBROWSER,
                        this);
            item->setText(text);
            ui->verticalLayout->addWidget(item);

            propElements.push_back(item);
        }
    }
}

void GmicPropertiesEntity::selfConnectToValueChanged(NodeProperties* p)
{
    connect(this, &GmicPropertiesEntity::valueChanged,
            p, [p]{p->handleSomeValueChanged();});
}

QString GmicPropertiesEntity::getValuesAsString()
{
    QString output;

    output.append(gmicHelper->getFiltersModel().getFilterFromHash(gmicHash).command());
    output.append("_preview");
    output.append(" ");

    foreach(auto& item, propElements)
    {
        output.append(item->getValuesAsString());
        output.append(",");
    }
    output.chop(1);
    output.append("2,0,50,50");
    std::cout << output.toStdString() << std::endl;
    return output;
}


GmicPropertiesEntity::~GmicPropertiesEntity()
{
    delete ui;
}
