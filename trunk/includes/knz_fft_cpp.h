#ifndef _KNZ_FFT_CPP_H_
#define _KNZ_FFT_CPP_H_

struct KNZComplex {
  float re,im;
};

//быстрое преобразование Фурье
//p - порядок задачи, т.е для 1024-точечного пребразования Фурье p=10 (2^p = N)
//in - входной массив отсчетов сигнала размерности 2^p
//out - указатель на выходной массив спектральных отсчетов размерности 2^p
void fft(int p, double *in, KNZComplex **out);


#endif
