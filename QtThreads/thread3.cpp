#include "thread3.h"

Producer3::Producer3(char id, int size, QMutex* pMutex, QObject *parent) :
    QThread(parent),
    m_id(id),
    m_size(size),
    m_index(0),
    m_mutex(pMutex)
{
    m_buffer = new char[m_size];
    m_finishedCon = new QWaitCondition();

}

Producer3::~Producer3()
{
    delete []m_buffer;
    delete m_finishedCon;
}

void Producer3::run()
{
    qDebug() << "producing" << m_id;

    for(int i = 0; i < m_size; i++)
    {
        for(int x = 0; x < 10000; x++)
            for(int y = 0; y < 10000; y++)
                int z = x-y;

        m_buffer[m_index] = m_id;
        m_index++;
    }

    m_mutex->lock();
    m_finishedCon->wakeAll();
    m_mutex->unlock();

    qDebug() << "producing done" << m_id;

}

Consumer3::Consumer3(char id, QObject *parent) :
    QThread(parent),
    m_id(id)
{
}

void Consumer3::run()
{
    qDebug() << "Consumer started...";

    QMutex m;

    Producer3 p1('A',10, &m);
    p1.start();

    Producer3 p2('B',10, &m);
    p2.start();

    Producer3 p3('C',10, &m);
    p3.start();

    qDebug() << "Before p1.wait";
    p1.finishedCondition()->wait(&m);

    qDebug() << "Before p2.wait";
    p2.finishedCondition()->wait(&m);

    qDebug() << "Before p3.wait";
    p3.finishedCondition()->wait(&m);

    qDebug() << "All Done:";
    qDebug() << p1.buffer();
    qDebug() << p2.buffer();
    qDebug() << p3.buffer();


    p1.wait();
    p2.wait();
    p3.wait();
    qDebug() << "Consumer finished!";

}
