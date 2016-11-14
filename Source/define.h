#ifndef DEFINE
#define DEFINE

#include <QVector>

#define screen_res 800.0
#define screen_size 1020.0
#define TOPBAR_OFFSET 30

typedef struct sparameter_data_t {
    QVector<int> S11;
    QVector<int> S12;
    QVector<int> S21;
    QVector<int> S22;
} sparameter_data;

void *serial_main(void *arg);

#endif // DEFINE

