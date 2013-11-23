 #include "DigiFo.h"


DigiFo::DigiFo(QObject *parent) :
    QThread(parent)
{
    _reset();
    moveToThread(this);
    thr_random.start();
    is_running = false;
    trans_depth = MIN_DEPTH;
    randomness = DEFAULT_RANDOMNESS;
    hash = new QCryptographicHash(QCryptographicHash::Md5);
    hash_len = 16;
    hash_id = 0;
    stats_num = MAX_LEN;
}

void DigiFo::set_stats_num(unsigned int num)
{
    if(is_running)
    {
        emit message(QString("Previous process running, setting ignored."), MESSAGE_ERROR);
        return;
    }
    if(num < MAX_LEN)
    {
        stats_num = num;
    }
    else
    {
        stats_num = MAX_LEN;
    }
}

void DigiFo::gen_random_file(QString file, unsigned long long file_size)
{
    QFile  f;
    char buff[1024];
    int finished = 0;
    unsigned long long total = file_size;

    if(is_running)
    {
        emit message(QString("Previous process running. Please stop first"), MESSAGE_ERROR);
        return;
    }

    emit message(QString("Generate random file...\n Please wait"), MESSAGE_INFO);

    f.setFileName(file);
    if(f.open(QIODevice::WriteOnly))
    {
        is_running = true;

        while(file_size >= 1024 && is_running)
        {
            thr_random.gen_random_array(buff, 1024);
            f.write(buff, 1024);
            file_size-=1024;
            if((100*(total - file_size)/total) > finished)
            {
                finished = (100*(total - file_size)/total);
                emit finished_num(finished);
            }
        }
        if(file_size && is_running)
        {
            thr_random.gen_random_array(buff, file_size);
            f.write(buff,file_size);
            emit message(QString("Random file ok!"), MESSAGE_INFO);
        }
        f.close();
        is_running = false;
        emit finished_num(100);
    }
    else
    {
        emit message(QString("Cannot open file!"), MESSAGE_ERROR);
    }
}

void DigiFo::set_hash_function(unsigned char id)
{
    if(is_running)
    {
        emit message(QString("Previous process running, setting ignored."), MESSAGE_ERROR);
        return;
    }
    if(id == hash_id)
    {
        return;
    }
    if(hash)
    {
        delete hash;
    }
    hash_id = id;
    switch(hash_id)
    {
    case 0:
        hash = new QCryptographicHash(QCryptographicHash::Md5);
        hash_len = 16;
        break;
    case 1:
        hash = new QCryptographicHash(QCryptographicHash::Sha1);
        hash_len = 20;
        break;
    case 2:
        hash = new QCryptographicHash(QCryptographicHash::Sha224);
        hash_len = 26;
        break;
    case 3:
        hash = new QCryptographicHash(QCryptographicHash::Sha256);
        hash_len = 32;
        break;
    case 4:
        hash = new QCryptographicHash(QCryptographicHash::Sha384);
        hash_len = 48;
        break;
    case 5:
        hash = new QCryptographicHash(QCryptographicHash::Sha512);
        hash_len = 64;
        break;
    case 6:
        hash = new QCryptographicHash(QCryptographicHash::Sha3_224);
        hash_len = 26;
        break;
    case 7:
        hash = new QCryptographicHash(QCryptographicHash::Sha3_256);
        hash_len = 32;
        break;
    case 8:
        hash = new QCryptographicHash(QCryptographicHash::Sha3_384);
        hash_len = 48;
        break;
    case 9:
        hash = new QCryptographicHash(QCryptographicHash::Sha3_512);
        hash_len = 64;
        break;
    default:
        emit message(QString("Unknown type, using MD5."), MESSAGE_ERROR);
        hash = new QCryptographicHash(QCryptographicHash::Md5);
        hash_len = 16;
        break;
    }
}

void DigiFo::terminate()
{
    is_running = false;
}

DigiFo::~DigiFo()
{
    is_running = false;
    wait(1000);
    this->quit();
}

void DigiFo::run()
{
    this->exec();
}

void DigiFo::set_randomness(unsigned int num)
{
    if(is_running)
    {
        emit message(QString("Previous process running, setting ignored."), MESSAGE_ERROR);
        return;
    }
    if(num > hash_len - 2) {
        this->randomness = hash_len - 2;
    } else {
        this->randomness = num;
    }
}

void DigiFo::set_trans_depth(int d)
{
    if(is_running)
    {
        emit message(QString("Previous process running, setting ignored."), MESSAGE_ERROR);
        return;
    }
    if(is_running)
    {
        emit message(QString("Previous process running, setting ignored."), 0);
        return;
    }
    if(d>= MIN_DEPTH && d <= MAX_DEPTH)
        this->trans_depth = d;
}

void DigiFo::setInFile(QString file)
{
    if(is_running)
    {
        emit message(QString("Previous process running, setting ignored."), MESSAGE_ERROR);
        return;
    }
    this->InFile.setFileName(file);
}

void DigiFo::setOutFile(QString file)
{
    if(is_running)
    {
        emit message(QString("Previous process running, setting ignored."), MESSAGE_ERROR);
        return;
    }
    this->OutFile.setFileName(file);
}

void DigiFo::set_rFile(QString file)
{
    if(is_running)
    {
        emit message(QString("Previous process running, setting ignored."), MESSAGE_ERROR);
        return;
    }
    r_file_hashed = false;
    this->rFile.setFileName(file);

}

void DigiFo::set_password(char *password, int len)
{
    QByteArray hvalue;

    if(is_running)
    {
        emit message(QString("Previous process running, setting ignored."), MESSAGE_ERROR);
        return;
    }
    hash->reset();
    hash->addData(password, len);
    hvalue = hash->result();
    memcpy(this->passwd, hvalue.data(), hash_len);
}

void DigiFo::buff_hash(const char *buff, int len, char *result)
{
    QByteArray hvalue;

    hash->reset();
    hash->addData(this->passwd, hash_len);
    hash->addData(buff,len);
    if(r_file_hashed)
    {
        hash->addData(this->r_file_hash, hash_len);
    }
    hvalue = hash->result();
    memcpy(result, hvalue.data(), hash_len);
    return;
}

void DigiFo::get_random_array(char *array, int len)
{
    thr_random.gen_random_array(array,len);
}

void DigiFo::setRFF(QString file)
{
    if(file != NULL) {
        thr_random.randomness_from_file(true, file);
    } else {
        thr_random.randomness_from_file(false, NULL);
    }
}

int DigiFo::get_hash_len()
{
    return hash_len;
}

void DigiFo::print_s(const char *s)
{
    QByteArray a = QByteArray(s, hash_len);
    printf("%s\n", a.toBase64().data());
    //emit message(QString(a.toBase64()), MESSAGE_DEBUG);
}

void DigiFo::hash_from_rFile( const char *trans, char *result)
{
    char temp[MAX_LEN];
    int len;
    int i = 2048;

    memcpy(result, trans, hash_len);
    transform(result, this->passwd, trans_depth, 0);
    transform(result, r_file_hash, trans_depth, 1);

    rFile.seek(0);
    while(i && hash_len == (len = rFile.read(temp, hash_len)))
    {
        transform(temp, trans, 1, 0);
        transform(result, temp, 1, 1);
        i--;
    }
    if(hash_len != len && len)
    {
        buff_hash(temp, len, temp);
        transform(temp, trans, 1, 0);
        transform(result, temp, 1, 1);
    }
    return;
}

bool DigiFo::init_stats(bool encode)
{
    char rand_data[MAX_LEN][MAX_LEN];
    char temp[MAX_LEN];
    int i;
    QByteArray qb;

    emit message(QString("init stats... "), MESSAGE_INFO);
    //init r_key
    if(!r_file_hashed)
    {
        qb = rFile.readAll();
        hash->reset();
        hash->addData(this->passwd, hash_len);
        hash->addData(qb.data(),qb.size());
        qb = hash->result();
        memcpy(this->r_file_hash, qb.data(), qb.length());
        r_file_hashed = true;
    }
    memcpy(this->r_key, this->r_file_hash, hash_len);

    if(encode)
    {
        for(i = 0; i < stats_num; i++)
        {
            this->thr_random.gen_random_array(rand_data[i], hash_len);
        }

        transform(this->r_key, this->passwd, trans_depth, 1);

        //wirte rand_data to outFile
        for(i = 0; i < stats_num; i++)
        {
            memcpy(temp, rand_data[i], hash_len);
            transform(temp,this->r_key, trans_depth, 0);

            OutFile.write(temp, hash_len);
            transform(this->r_key, rand_data[i], trans_depth, 0);
        }
    }
    else
    {
        if(InFile.size() <= hash_len * stats_num)
        {
            return false;
        }

        transform(this->r_key, this->passwd, trans_depth, 1);
        for(i = 0; i < stats_num; i++)
        {
            this->InFile.read(rand_data[i], hash_len);
            transform(rand_data[i], this->r_key, trans_depth, 1);
            transform(this->r_key, rand_data[i], trans_depth, 0);
        }
    }

    //init r_stats
    for(i = 0; i < stats_num; i++)
    {
        hash_from_rFile(rand_data[i], this->r_stat[i]);
    }

    //init d_stat
    memset(this->d_stat, 0, hash_len);
    for(i = 0; i < stats_num; i++)
    {
        transform(this->d_stat, rand_data[i],trans_depth, 0);
    }
    emit finished_num(1);
    emit message(QString("init stats... Done!"), MESSAGE_INFO);
    return true;
}

void DigiFo::encode()
{
#ifdef DEBUG_TRANS
    char trans[MAX_LEN] = "c3594d58a7c0a03bdf5608d2195d38cb";
    buff_hash(trans, hash_len, trans);
    memset(s_data, 0, hash_len);
    printf("encode:\n");
    printf("Data:");
    print_s(s_data);
    transform(s_data, trans,15,0);
    printf("\n****:");
    print_s(s_data);
    printf("\n");
    emit finished_num(100);
#else
    int i, j, value, r_state_index = 0;
    char data_nums[MAX_LEN];
    bool tag;
    unsigned long long file_size, size_count = 0;
    int finished = 0;
    QTime curr_time = QTime::currentTime();

    if(is_running)
    {
        emit message(QString("Previous process running. Please stop first"), MESSAGE_ERROR);
        return;
    }

    if(!InFile.open(QIODevice::ReadOnly) ||
            !OutFile.open(QIODevice::WriteOnly) ||
            !rFile.open(QIODevice::ReadOnly)) {
        emit message(QString("Cannot open file!"), MESSAGE_ERROR);
        emit finished_num(100);
        is_running = false;
        return;
    }
    file_size = InFile.size();
    emit finished_num(0);
    emit message("Starting at: "+curr_time.toString("hh:mm:ss:zzz"), MESSAGE_INFO);
    is_running = true;
    if(false == this->init_stats(true))
    {
        InFile.close();
        OutFile.close();
        rFile.close();
        is_running = false;
        emit finished_num(100);
    }

    tag = true;
    while(tag&&is_running)
    {
        r_state_index++;
        r_state_index %= stats_num;
        //update r_key, r_stat
        transform(r_key,r_stat[r_state_index],trans_depth,0);
        transform(r_stat[r_state_index],d_stat,trans_depth,1);
        value = thr_random.gen_random_num();

        data_nums[0] = value%256;
        if(data_nums[0])
        {
            //adding randomness
            for(j = 1; j < this->randomness; j++)
            {
                data_nums[j] = thr_random.gen_random_num();
            }

            i = InFile.read(data_nums+j, hash_len - j);
            size_count += i;
            if(i < hash_len - j) {
                data_nums[hash_len - 1] = i+j;
                tag = false;
                i = i+j;
                while(hash_len-1 > i)
                {
                    *(data_nums+i++) = 0;
                }
            }
        }
        else
        {
            thr_random.gen_random_array((data_nums+1), hash_len - 1);
        }

        transform(data_nums,d_stat, trans_depth, 0);
        //update d_stat
        memcpy(d_stat,data_nums, hash_len);

        transform(data_nums,this->r_key,trans_depth,0);

        OutFile.write(data_nums, hash_len);

        if(size_count%(1024) == 0)
        {
            if((100*size_count)/file_size > finished)
            {
                finished = (100*size_count)/file_size;
                emit finished_num(finished);
                emit message("time used: " + QString::number(curr_time.elapsed(),10), MESSAGE_INFO);
            }
        }
    }
    InFile.close();
    OutFile.close();
    rFile.close();
    is_running = false;
    emit finished_num(100);
    emit message("finished: " + QString::number(curr_time.elapsed(),10), MESSAGE_INFO);
#endif
}

void DigiFo::decode()
{
#ifdef DEBUG_TRANS
    char trans[MAX_LEN] = "c3594d58a7c0a03bdf5608d2195d38cb";
    buff_hash(trans, hash_len, trans);
    printf("decode:\n");
    transform(s_data, trans,15,1);
    printf("Data:");
    print_s(s_data);
    emit finished_num(100);
#else
    int i, r_state_index = 0;
    char rand_nums[MAX_LEN];
    char data_nums[MAX_LEN];
    bool tag;
    QTime curr_time = QTime::currentTime();

    unsigned long long file_size, size_count = 0;
    int finished = 0;

    if(is_running)
    {
        emit message(QString("Previous process running. Please stop first"), MESSAGE_ERROR);
        return;
    }

    if(!InFile.open(QIODevice::ReadOnly) ||
            !OutFile.open(QIODevice::WriteOnly) ||
            !rFile.open(QIODevice::ReadOnly)) {
        emit message(QString("Cannot opne file!"), MESSAGE_ERROR);
        emit finished_num(100);
        is_running = false;
        return;
    }
    file_size = InFile.size();
    emit finished_num(finished);

    emit message("Starting at: "+curr_time.toString("hh:mm:ss:zzz"), MESSAGE_INFO);

    is_running = true;
    if(false == this->init_stats(false))
    {
        InFile.close();
        OutFile.close();
        rFile.close();
        is_running = false;
        emit finished_num(100);
    }
    tag = true;
    while(tag&&is_running)
    {
        r_state_index++;
        r_state_index %= stats_num;
        //update r_key, r_stat
        transform(r_key,r_stat[r_state_index],trans_depth,0);
        transform(r_stat[r_state_index],d_stat,trans_depth,1);

        i = InFile.read(data_nums, hash_len);
        size_count += i;
        if(i != hash_len)
        {
            //this should not happen for real decode processing.
            emit finished_num(100);
            is_running = false;
            InFile.close();
            OutFile.close();
            rFile.close();
        }

        transform(data_nums,this->r_key,trans_depth,1);
        memcpy(rand_nums, data_nums, hash_len);
        transform(data_nums,d_stat, trans_depth ,1);
        //update d_stat
        memcpy(d_stat,rand_nums, hash_len);

        if(data_nums[0])
        {
            if(!InFile.atEnd())
            {
                //remove randomness numbers, and write data to file.
                OutFile.write(data_nums+randomness, hash_len - randomness);
            }
            else
            {
                if((uchar)(data_nums[hash_len - 1]) < hash_len &&
                        (uchar)(data_nums[hash_len - 1]) > 1)
                    OutFile.write(data_nums+randomness, data_nums[hash_len-1]-randomness);

                tag = false;
            }
        }
        if(size_count%(1024) == 0)
        {
            if((100*size_count)/file_size > finished)
            {
                finished = ((100*size_count)/file_size);
                emit message("time used: " + QString::number(curr_time.elapsed(),10), MESSAGE_INFO);
                emit finished_num(finished);
            }
        }
    }
    InFile.close();
    OutFile.close();
    rFile.close();
    is_running = false;
    emit finished_num(100);
    emit message("finished: "+ QString::number(curr_time.elapsed(),10), MESSAGE_INFO);

#endif
}

void DigiFo::swape_trans(uchar target[], const uchar trans[],int inverse)
{
    int i, k;
    if(inverse)
    {
        for(i=hash_len -1; i>=0; i--)
        {
            k = (trans[i]^passwd[hash_len-1 -i])%hash_len;
            if(i == k) {
                target[i] ^= r_file_hash[k];
            } else {
                //swap(a, b) (a) ^= (b), (b) ^= (a), (a) ^= (b)
                target[i] ^= target[k];
                target[k] ^=  target[i];
                target[i] ^= target[k];
            }
        }
    }
    else
    {
        for(i=0; i< hash_len; i++)
        {
            k = (trans[i]^passwd[hash_len-1 -i])%hash_len;
            if(i == k) {
                target[i] ^= r_file_hash[k];
            } else {
                //swap(a, b) (a) ^= (b), (b) ^= (a), (a) ^= (b)
                target[i] ^= target[k];
                target[k] ^=  target[i];
                target[i] ^= target[k];
            }
        }
    }
}

void DigiFo::self_swape_trans(uchar *target, const uchar *trans, int inverse)
{
    int i, m, n;

#ifdef DEBUG_TRANS
    print_s((const char *)trans);
    print_s((const char*)target);
#endif

    if(inverse)
    {
        for(i = hash_len*8-2; i >= 1; i--)
        {
            m = target[i/8] & (1<<(i%8));
            n = target[(i+1)/8] &(1<<((i+1)%8));

            if(trans[i/8] & (1<<(i%8)))
            {
                //swape two bit.
                if(n)
                {
                    target[i/8]  |= 1<<(i%8);
                }
                else
                {
                    target[i/8] &= (~(1<<(i%8)));
                }

                if(m)
                {
                    target[(i+1)/8] |= 1<<((i+1)%8);
                }
                else
                {
                    target[(i+1)/8] &= (~(1<<((i+1)%8)));
                }
            }
        }
    }
    else
    {
        for(i = 1; i <= hash_len*8-2; i++)
        {
            m = target[i/8] & (1<<(i%8));
            n = target[(i+1)/8] & (1<<((i+1)%8));

            if(trans[i/8] & (1<<(i%8)))
            {
                //swape two bit.
                if(n)
                {
                    target[i/8]  |= 1<<(i%8);
                }
                else
                {
                    target[i/8] &= (~(1<<(i%8)));
                }

                if(m)
                {
                    target[(i+1)/8] |= 1<<((i+1)%8);
                }
                else
                {
                    target[(i+1)/8] &= (~(1<<((i+1)%8)));
                }
            }
        }
    }
}

void DigiFo::XOR_trans(uchar target[], const uchar trans[], int inverse)
{
    int i,k;

#ifdef DEBUG_TRANS
    print_s((const char*)trans);
    print_s((const char*)target);
#endif
    if(inverse)
    {
        for(i=hash_len-1; i>=0; i--)
        {
            k = (trans[i]^passwd[i])%hash_len;
            if(i == k) {
                target[i] = ~target[i];
            } else {
                target[i] ^=target[k];
            }
        }
    }
    else
    {
        for(i=0; i< hash_len; i++)
        {
            k = (trans[i]^passwd[i])%hash_len;
            if(i == k) {
                target[i] = ~target[i];
            } else {
                target[i] ^=target[k];
            }
        }
    }

}

void DigiFo::self_circular_shift(uchar *target, const uchar *trans, int inverse)
{
    int i,k;
#ifdef DEBUG_TRANS
    print_s((const char*)trans);
    print_s((const char*)target);
#endif
    if(inverse)
    {
        for(i=0; i<hash_len; i++)
        {
            k = trans[i]%8;
            target[i] = target[i] >>k | target[i] <<(8-k);
        }
    }
    else
    {
        for(i=0; i<hash_len; i++)
        {
            k = trans[i]%8;
            target[i] = target[i] << k | target[i] >>(8-k);
        }
    }

}

void DigiFo::circular_shift(uchar target[], const uchar trans[], int inverse)
{
    int i, k;
    uchar t;

#ifdef DEBUG_TRANS
    print_s((const char*)trans);
    print_s((const char*)target);
#endif
    if(inverse)
    {
        for(i=hash_len -1; i>=0; i--)
        {
            k = (trans[i]^r_file_hash[hash_len-1 - i])%hash_len;
            if(i == k) {
                target[i] = target[i] << (i%8) | target[i] >>(8-(i%8));
            } else {
                t = target[i];
                target[i] = (target[i] >> (trans[i]%8)) | (target[k] << (8 - trans[i]%8)) ;
                target[k] = (target[k] >> (trans[i]%8)) | (t << (8 - trans[i]%8))  ;
            }
        }
    }
    else
    {
        for(i=0; i<hash_len; i++)
        {
            k = (trans[i]^r_file_hash[hash_len-1 -i])%hash_len;
            if(i == k) {
                target[i] = target[i] >> (i%8) | target[i] <<(8-(i%8));
            } else {
                t = target[i];
                target[i] = (target[i] << (trans[i]%8)) | (target[k] >> (8 - trans[i]%8)) ;
                target[k] = (target[k] << (trans[i]%8)) | (t >> (8 - trans[i]%8))  ;
            }
        }
    }
}

void DigiFo::__transform_inverse(char target[], const char trans[], int depth)
{
    char tmd5[MAX_LEN];

    if(depth == 0)
    {
        buff_hash(trans, hash_len, tmd5);
        circular_shift ((uchar *)target, (uchar *)tmd5, 1);
        return;
    }
    else
    {
        buff_hash(trans, hash_len, tmd5);

        __transform_inverse(target, tmd5, --depth);

        swape_trans    ((uchar *)target, (uchar *)tmd5, 1);
        self_swape_trans((uchar *)target, (uchar *)tmd5, 1);
        circular_shift ((uchar *)target, (uchar *)tmd5, 1);
        self_circular_shift((uchar *)target, (uchar *)tmd5, 1);
        XOR_trans      ((uchar *)target, (uchar *)tmd5, 1);
        return;
    }
}

void DigiFo::transform(char target[], const char trans[],int depth, int inverse)
{
    int i;
    char tmd5[MAX_LEN];

    if(inverse)//inverse transform
    {
        buff_hash(trans, hash_len, tmd5);

        __transform_inverse(target,tmd5,depth);

        for(i = 0; i<hash_len; i++)
        {
            target[i] = tmd5[i]^target[i];
        }
        return;
    }
    else
    {
        buff_hash(trans, hash_len, tmd5);
        for(i = 0; i<hash_len; i++)
        {
            target[i] = tmd5[i]^target[i];
        }

        while(depth--)
        {
            buff_hash(tmd5, hash_len, tmd5);

            XOR_trans((uchar *)target,(uchar *)tmd5, 0);
            self_circular_shift((uchar *)target, (uchar *)tmd5, 0);
            circular_shift((uchar *)target,(uchar *)tmd5, 0);
            self_swape_trans((uchar *)target, (uchar *)tmd5, 0);
            swape_trans((uchar *)target,(uchar *)tmd5, 0);
        }
        buff_hash(tmd5, hash_len, tmd5);
        circular_shift((uchar *)target,(uchar *)tmd5, 0);
        return;
    }
}
