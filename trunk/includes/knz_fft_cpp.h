#ifndef _KNZ_FFT_CPP_H_
#define _KNZ_FFT_CPP_H_

struct KNZComplex {
  float re,im;
};

//������� �������������� �����
//p - ������� ������, �.� ��� 1024-��������� ������������� ����� p=10 (2^p = N)
//in - ������� ������ �������� ������� ����������� 2^p
//out - ��������� �� �������� ������ ������������ �������� ����������� 2^p
void fft(int p, double *in, KNZComplex **out);


#endif
