/**********************************************************************************
 ***         Представленные функции предназначены для шифровки/дешифровки       ***
 *** текста (катит и русский!). Используется блочный ОпенСорс алгоритм Camellia ***
 ***        исходники которого, практически неизменные, лежат out there...      ***
 **********************************************************************************/
/* code: Наконечный Павел, zorg1331@gmail.com */

// Time-stamp: <xcrypt.cpp - 16:08:54 07.05.2006>

#include <camellia.h>
#include <xcrypt.h>

// Относиться к обоим функциям!!!
// необходимо контролировать длину возвращаемых строк!

// функция шифрования алгоритмом Camellia, на входе строка типа QString
// в ней могут быть символы в национальной кодировке
// на выходе: QByteArray, т.к. в КуСтринг шифрованную строку уже не запихаешь
QByteArray encrypt(QString sourceString)
{
  // задаем ключ, обязательно 16 символов, если бит 128 ;)
  KEY_TABLE_TYPE keyTable;
  const int keyBitLength = 128;
  const unsigned char rawKey[16] = {'c','d','e','V','F','R','W','S','X','a',
				    '.',';','[',',','l','p'};
  QByteArray result;
  // алгоритм блочный, поэтому шифровать будем кусками по 16 символов
  int j = 0;
  int size = 16;
  while (j < sourceString.length()) {
    // сие будет передано в Камеллиевскую функцию шифрования
    unsigned char plainText[size];
    int i = 0;
    while ((i < size) && (j < sourceString.length())) {
      // !!!обратить внимание на способ конвертации в unsigned char!!!
      plainText[i] = sourceString.toLocal8Bit().at(j);
      i++;
      j++;
    }
    // шифрованная ботва
    unsigned char ct_buf[size];
    // инициализируем хеш ключа
    Camellia_Ekeygen(keyBitLength, rawKey, keyTable);
    // шифруем
    Camellia_EncryptBlock(keyBitLength, plainText, keyTable, ct_buf);
    for (int k = 0; k < size; k++) {
      result.append(ct_buf[k]);
    }
  }
  return result;
}

// функция дешифровки алгоритмом Camellia, на входе - ботва типа QByteArray
// на выходе - вменяемы QString. Если имеются символы в национальной кодировке,
// необходимо полученную строку прогнать через translate(), см. translate.h
QString decrypt(QByteArray sourceString)
{
  // задаем ключ, обязательно 16 символов, если бит 128 ;)
  KEY_TABLE_TYPE keyTable;
  const int keyBitLength = 128;
  const unsigned char rawKey[16] = {'c','d','e','V','F','R','W','S','X','a',
				    '.',';','[',',','l','p'};
  QString result;
  // алгоритм блочный, поэтому дешифровать будем кусками по 16 символов
  int j = 0;
  int size = 16;
  while (j < sourceString.length()) {
    // это передается в Камеллиевскую функцию дешифровки
    unsigned char cryptedText[size];
    int i = 0;
    while ((i < size) && (j < sourceString.length())) {
      cryptedText[i] = sourceString[j];
      i++;
      j++;
    }
    // на выходе - расшифрованный текст
    unsigned char pt_buf[size];
    // инициализируем хеш ключа
    Camellia_Ekeygen(keyBitLength, rawKey, keyTable);
    // дешифруем
    Camellia_DecryptBlock(keyBitLength, cryptedText, keyTable, pt_buf);
    QByteArray bytes;
    bytes.resize(size);
    for (int i = 0; i < bytes.size(); i++) {
      bytes[i] = pt_buf[i];
    }
    result.append(bytes);
  }
  return result;
}
