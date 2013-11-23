#ifndef DIGIFO_H
#define DIGIFO_H

#include <QObject>
#include <QFile>
#include "thread_random.h"
#include <QApplication>
#include <QThread>
#include <QCryptographicHash>
#include <QTime>

#define  MAX_LEN           100
#define MIN_DEPTH          1
#define MAX_DEPTH          32
#define DEFAULT_RANDOMNESS 1
//#define DEBUG_TRANS

#define MESSAGE_ERROR            0
#define MESSAGE_WARNNING         1
#define MESSAGE_INFO             2
#define MESSAGE_DEBUG            3

class DigiFo : public QThread
{
    Q_OBJECT
public:
    explicit DigiFo(QObject *parent = 0);

    ~DigiFo();
    void get_random_array(char *array, int len);
    void run();
    void set_trans_depth(int d);
    int get_hash_len();
    int get_max_randomness()
    {
        return get_hash_len() - 2;
    }
    unsigned char get_hash_function()
    {
        return hash_id;
    }
    unsigned int get_stats_num()
    {
        return stats_num;
    }
    void set_stats_num(unsigned int num);

signals:
    void finished_num(int i);
    void message(QString message, int level);

public slots:
    void setInFile(QString file);
    void setOutFile(QString file);
    void set_rFile(QString file);
    void setRFF(QString file); // randomness from file.

    void set_password(char *password, int len);
    void set_randomness(unsigned int num);
    void set_hash_function(unsigned char id);

    void reset(){_reset();}
    void encode();
    void decode();
    void terminate();

    void gen_random_file(QString file, unsigned long long file_size);

private:
    QFile InFile;
    QFile OutFile;
    QFile rFile;


    bool r_file_hashed;

    unsigned int hash_len;
    QCryptographicHash *hash;
    unsigned char hash_id;
    thread_random thr_random;
    int trans_depth;
    bool is_running;
    unsigned int randomness;
    unsigned int stats_num;

    char r_key [MAX_LEN];
    char d_stat[MAX_LEN];
    char r_stat[MAX_LEN][MAX_LEN];

    //static stats.
    char r_file_hash[MAX_LEN];
    char passwd[MAX_LEN];
#ifdef  DEBUG_TRANS
    char s_data[MAX_LEN];
#endif
    void print_s(const char *s);

    bool init_stats(bool encode);
    void hash_from_rFile(const char *trans, char *result);


    void _reset()
    {
        InFile.setFileName(QString("DigiFo.in"));
        OutFile.setFileName(QString("DigiFo.out"));
        memset(r_key,0,sizeof(r_key));
        memset(d_stat,0,sizeof(d_stat));
        memset(r_stat,0,sizeof(r_stat));
        memset(passwd, 0, sizeof(passwd));
    }
    void buff_hash(const char *buff, int len, char *result);


    void transform(char *target, const char *trans,int depth,int inverse);
    void __transform_inverse(char *target, const char *trans, int depth);


    void swape_trans(uchar *target,const uchar *trans, int inverse);
    void self_swape_trans(uchar *target, const uchar *trans, int inverse);

    void circular_shift(uchar *target, const uchar *trans, int inverse);
    void self_circular_shift(uchar *target, const uchar *trans, int inverse);

    void XOR_trans(uchar *target, const uchar *trans, int inverse);
};

#endif // DIGIFO_H
