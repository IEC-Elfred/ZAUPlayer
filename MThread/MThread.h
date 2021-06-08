
class MThread
{
private:
    /* data */
public:
    MThread(/* args */);
    virtual void run() = 0;
    int Start();
    ~MThread();
};

inline MThread::MThread(/* args */)
{
}

inline MThread::~MThread()
{
}
