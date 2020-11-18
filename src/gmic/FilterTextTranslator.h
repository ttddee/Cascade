/** -*- mode: c++ ; c-basic-offset: 2 -*-
 * @file   FilterTextTranslator.h
 * @author Sebastien Fourey
 * @date   Sep 2020
 *
 * @brief  Declaration of the class FilterTextTranslator
 *
 * @copyright
 */
#ifndef GMIC_QT_FILTERTEXTTRANSLATOR_H
#define GMIC_QT_FILTERTEXTTRANSLATOR_H

#include <QObject>
#include <QString>

/**
 *  The FilterTextTranslator class.
 */
class FilterTextTranslator : public QObject {
  Q_OBJECT
public:
  FilterTextTranslator() = delete;

  inline static QString translate(const QString & str);

protected:
private:
};

QString FilterTextTranslator::translate(const QString & str)
{
  return tr(str.toUtf8());
}

#endif // GMIC_QT_FILTERTEXTTRANSLATOR_H
