#ifndef THREAD_RANDOM_H
#define THREAD_RANDOM_H

#include <QThread>
#include <QObject>
#include <QFile>

typedef unsigned long int      uint32_t;
typedef unsigned long long int uint64_t;

#define PHI 0x9e3779b9

class thread_random : public QThread
{
    Q_OBJECT

public:
    explicit thread_random(QObject *parent = 0);
    ~thread_random();

    void run();

    unsigned int gen_random_num();
    bool _gen_random_num(unsigned int *value);
    int gen_random_array(char *array, int len); 

    void stop_processing();

signals:

public slots:
    void terminate();
    void randomness_from_file(bool set, QString file);

private:

    volatile bool is_running;
    volatile bool req_stat[2];
    volatile unsigned int random_num;
    QFile randomness_file;
    bool using_randomness_file;

    uint32_t Q[4096], c,c1;
    uint32_t rand_cmwc(void)
    {
        uint64_t t, a = 18782LL;
        static uint32_t i = 4095;
        uint32_t x,x1, r = 0xfffffffe;
        i = (i + 1) & 4095;
        t = a * Q[i] + c;
        c = (t >> 32);
        x = t + c;
        x1=x;
        if (x < c) {
            x++;
            c++;
        }
        else
        {
            x1++;
            c1++;
        }
        c1=x1;
        return (Q[i] = r - x);
    }
    void init_rand(uint32_t x)
    {
        int i;

        Q[0] = x;
        Q[1] = x + PHI;
        Q[2] = x + PHI + PHI;

        for (i = 3; i < 4096; i++)
            Q[i] = Q[i - 3] ^ Q[i - 2] ^ PHI ^ i;
    }
};

#endif // THREAD_RANDOM_H
