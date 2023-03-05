#include "TableClass.h"
#include <QDebug>
#include <QRandomGenerator>

static bool randomFlipper();

QmlTableModel::QmlTableModel(QObject *parent) : QAbstractTableModel(parent)
{
    m_currpos = QPoint(0,0);
    beginResetModel();
    m_currentState.fill(0.0);
    endResetModel();
}

void QmlTableModel::startSim()
{
    timerSequence = new QTimer();
    connect(timerSequence, SIGNAL(timeout()), this, SLOT(processTheMove()));
    timerSequence->start(2);
}

void QmlTableModel::mover(QmlTableModel::Direction dir)
{
    switch(dir)
    {
    case UP:
        moveY(m_currpos.y() - 1);
        break;
    case DOWN:
        moveY(m_currpos.y() + 1);
        break;
    case RIGHT:
        moveX(m_currpos.x() + 1);
        break;
    case LEFT:
        moveX(m_currpos.x() - 1);
        break;
    case UPRIGHT:
        moveX(m_currpos.x() + 1);
        moveX(m_currpos.y() - 1);
        break;
    case UPLEFT:
        moveX(m_currpos.x() - 1);
        moveX(m_currpos.y() - 1);
        break;
    case DOWNRIGHT:
        moveX(m_currpos.x() + 1);
        moveX(m_currpos.y() + 1);
        break;
    case DOWNLEFT:
        moveX(m_currpos.x() - 1);
        moveX(m_currpos.y() + 1);
        break;
    }
            setIndicator(m_currpos);
}

//! [modelsize]
int QmlTableModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return height;
}

int QmlTableModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return width;
}
//! [modelsize]

//! [read]
QVariant QmlTableModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || role != CellRole)
        return QVariant();

    return QVariant(m_currentState[cellIndex({index.column(), index.row()})]);
}
//! [read]

//! [write]
bool QmlTableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (role != CellRole || data(index, role) == value)
        return false;

    m_currentState[cellIndex({index.column(), index.row()})] = value.toInt();
    emit dataChanged(index, index, {role});

    return true;
}

void QmlTableModel::setIndicator(const QPoint &pt)
{
    QPoint ptnext;
    beginResetModel();
        m_currentState.fill(0.0);
    m_currentState[cellIndex({pt.x(), pt.y()})] = .5;
    for (int i = 1; i < 3; i++)
    {
        //top left -> right
        for (int j = 0; j < i*2+1; j++)
        {
            ptnext = QPoint{pt.x()-i+j, pt.y()-i};
            if( cellIndexAgainstWallCheck(ptnext) )
           {
            m_currentState[cellIndex(ptnext)] = (.5 - (i * .1));
           }
        }
        //top right -> down
        for (int j = 1; j < i*2+1; j++)
        {
            ptnext = QPoint{pt.x()+i, pt.y()-i+j};
            if( cellIndexAgainstWallCheck(ptnext) )
           {
            m_currentState[cellIndex(ptnext)] = (.5 - (i * .1));
           }
        }
        //bottom right -> left
        for (int j = 1; j < i*2+1; j++)
        {
            ptnext = QPoint{pt.x()+i-j, pt.y()+i};
            if( cellIndexAgainstWallCheck(ptnext) )
           {
            m_currentState[cellIndex(ptnext)] = (.5 - (i * .1));
           }
        }
        //bottom left -> up
        for (int j = 1; j < i*2+1; j++)
        {
            ptnext = QPoint{pt.x()-i, pt.y()+i-j};
            if( cellIndexAgainstWallCheck(ptnext) )
           {
            m_currentState[cellIndex(ptnext)] = (.5 - (i * .1));
           }
        }
    }
    endResetModel();
}
//! [write]

Qt::ItemFlags QmlTableModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return Qt::ItemIsEditable;
}

std::size_t QmlTableModel::cellIndex(const QPoint &coordinates)
{
    return std::size_t(coordinates.y() * width + coordinates.x());
}

bool QmlTableModel::cellIndexCheck(const QPoint &coordinates)
{
    return (coordinates.y() < height) && (coordinates.x() < width);
}

bool QmlTableModel::cellIndexAgainstWallCheck(const QPoint &newcoordinates)
{
    return (newcoordinates.y() < height) && (newcoordinates.x() < width) && (newcoordinates.x() >= 0) && (newcoordinates.y() >= 0);
}

void QmlTableModel::moveX(int coordx)
{
    if (coordx < 0)
    {
        coordx = width - 1;
    }
    else if(coordx >= width)
    {
        coordx = 0;
    }
    m_currpos.setX(coordx);
}

void QmlTableModel::moveY(int coordy)
{
    if (coordy < 0)
    {
        coordy = height - 1;
    }
    else if(coordy >= height)
    {
        coordy = 0;
    }
    m_currpos.setY(coordy);
}

void QmlTableModel::processTheMove()
{
    quint32 motion = 0;
    if(randomFlipper())
    {
    motion = QRandomGenerator::global()->bounded( MAXMOTION-1 );
    }
    else
    {
motion = QRandomGenerator::global()->bounded( RIGHT );
    }
mover(static_cast<Direction>(motion));
setIndicator(m_currpos);
}

static bool randomFlipper()
{
    quint32 coinflip = QRandomGenerator::global()->bounded( 8 );
    if (coinflip % 2 == 0)
        return true;
    else
        return false;
}

#if 0
void QmlTableModel::setColumns(const QStringList &columns)
{
    beginResetModel();
    m_columns = columns;
    endResetModel();
}

void QmlTableModel::clear()
{
    beginResetModel();
    m_data.clear();
    m_columns.clear();
    endResetModel();
}

void QmlTableModel::appendRow(const QStringList &row)
{
    beginInsertRows(QModelIndex(), m_data.size(), m_data.size());
    m_data.append(row);
    endInsertRows();
}

void QmlTableModel::appendColumn(const QString &column)
{
    beginInsertColumns(QModelIndex(), m_columns.size(), m_columns.size());
    m_columns.append(column);
    endInsertColumns();
}

void QmlTableModel::removeRow(int row)
{
    beginRemoveRows(QModelIndex(), row, row);
    m_data.removeAt(row);
    endRemoveRows();
}

void QmlTableModel::removeColumn(int column)
{
    beginRemoveColumns(QModelIndex(), column, column);
    m_columns.removeAt(column);
    endRemoveColumns();
}
#endif
