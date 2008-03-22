// Time-stamp: <translate.h - 19:05:17 29.01.2006>
/************************************************************************
 ***      Это заголовок функции переводит великий могучий в ЮТФ       ***
 ************************************************************************/

/* code: Наконечный Павел <zorg1331@mail.ru> */

#ifndef TRANSLATE_H
#define TRANSLATE_H

#include <QTextCodec>

// Переводим надписи в кодировке koi8-r в UTF-8
inline QString translate(QString encodedString)
{
  QTextCodec *codec = QTextCodec::codecForName("koi8-r");
  QByteArray newString("");
  newString.append(encodedString);
  QString string = codec->toUnicode(newString);
  return string;
}

#endif
