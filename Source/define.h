#ifndef DEFINE
#define DEFINE

#include <QVector>

#define screen_res 800.0
#define screen_size 1020.0
#define TOPBAR_OFFSET 30

typedef struct sparameter_data_t {
    QVector<float> S11;
    QVector<float> S12;
    QVector<float> S21;
    QVector<float> S22;
    QVector<int> freq;
    int f_start;
    int f_end;
    int point_count;
    float step;
} sparameter_data;

enum plotID {
    S11_PLOT,
    S12_PLOT,
    S21_PLOT,
    S22_PLOT,
    NO_PLOT
};

void *serial_main(void *arg);

#endif // DEFINE

