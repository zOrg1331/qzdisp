/****************************************************************
 ***    ������ ������� ������������� ��� ������� �������� �   ***
 ***     �����������, ��������������, ������������ �� �����   ***
 ***    ���� QString. ���������� ���������� ��������� xor��.  ***
 *** ��������: ���������������, ��� ����� � ��������� koi8-r. ***
 ****************************************************************/

// Time-stamp: <xcrypt.h - 20:52:04 23.04.2006>

#ifndef XCRYPT_H
#define XCRYPT_H

#include <QTextCodec>

QByteArray encrypt(QString sourceString);

QString decrypt(QByteArray cryptedString);

#endif
