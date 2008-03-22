/**********************************************************************************
 ***         �������������� ������� ������������� ��� ��������/����������       ***
 *** ������ (����� � �������!). ������������ ������� �������� �������� Camellia ***
 ***        ��������� ��������, ����������� ����������, ����� out there...      ***
 **********************************************************************************/
/* code: ���������� �����, zorg1331@gmail.com */

// Time-stamp: <xcrypt.cpp - 16:08:54 07.05.2006>

#include <camellia.h>
#include <xcrypt.h>

// ���������� � ����� ��������!!!
// ���������� �������������� ����� ������������ �����!

// ������� ���������� ���������� Camellia, �� ����� ������ ���� QString
// � ��� ����� ���� ������� � ������������ ���������
// �� ������: QByteArray, �.�. � �������� ����������� ������ ��� �� ���������
QByteArray encrypt(QString sourceString)
{
  // ������ ����, ����������� 16 ��������, ���� ��� 128 ;)
  KEY_TABLE_TYPE keyTable;
  const int keyBitLength = 128;
  const unsigned char rawKey[16] = {'c','d','e','V','F','R','W','S','X','a',
				    '.',';','[',',','l','p'};
  QByteArray result;
  // �������� �������, ������� ��������� ����� ������� �� 16 ��������
  int j = 0;
  int size = 16;
  while (j < sourceString.length()) {
    // ��� ����� �������� � ������������� ������� ����������
    unsigned char plainText[size];
    int i = 0;
    while ((i < size) && (j < sourceString.length())) {
      // !!!�������� �������� �� ������ ����������� � unsigned char!!!
      plainText[i] = sourceString.toLocal8Bit().at(j);
      i++;
      j++;
    }
    // ����������� �����
    unsigned char ct_buf[size];
    // �������������� ��� �����
    Camellia_Ekeygen(keyBitLength, rawKey, keyTable);
    // �������
    Camellia_EncryptBlock(keyBitLength, plainText, keyTable, ct_buf);
    for (int k = 0; k < size; k++) {
      result.append(ct_buf[k]);
    }
  }
  return result;
}

// ������� ���������� ���������� Camellia, �� ����� - ����� ���� QByteArray
// �� ������ - �������� QString. ���� ������� ������� � ������������ ���������,
// ���������� ���������� ������ �������� ����� translate(), ��. translate.h
QString decrypt(QByteArray sourceString)
{
  // ������ ����, ����������� 16 ��������, ���� ��� 128 ;)
  KEY_TABLE_TYPE keyTable;
  const int keyBitLength = 128;
  const unsigned char rawKey[16] = {'c','d','e','V','F','R','W','S','X','a',
				    '.',';','[',',','l','p'};
  QString result;
  // �������� �������, ������� ����������� ����� ������� �� 16 ��������
  int j = 0;
  int size = 16;
  while (j < sourceString.length()) {
    // ��� ���������� � ������������� ������� ����������
    unsigned char cryptedText[size];
    int i = 0;
    while ((i < size) && (j < sourceString.length())) {
      cryptedText[i] = sourceString[j];
      i++;
      j++;
    }
    // �� ������ - �������������� �����
    unsigned char pt_buf[size];
    // �������������� ��� �����
    Camellia_Ekeygen(keyBitLength, rawKey, keyTable);
    // ���������
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
