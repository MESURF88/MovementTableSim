#include <QAbstractTableModel>
#include <QPoint>
#include <QTimer>
#include <array>

class QmlTableModel: public QAbstractTableModel
{
    Q_OBJECT
    Q_ENUMS(Roles)
    Q_ENUMS(Direction)
public:
    explicit QmlTableModel(QObject *parent = 0);

    enum Roles {
        CellRole
    };

    enum Direction
    {
        UP = 0,
        DOWN,
        LEFT,
        RIGHT,
        UPRIGHT,
        UPLEFT,
        DOWNRIGHT,
        DOWNLEFT,
        MAXMOTION
    };


    Q_INVOKABLE void startSim();
    Q_INVOKABLE void mover(QmlTableModel::Direction dir);

    QHash<int, QByteArray> roleNames() const override {
        return {
            { CellRole, "value" }
        };
    }
int rowCount(const QModelIndex &parent) const override;

int columnCount(const QModelIndex &parent) const override;

QVariant data(const QModelIndex &index, int role) const override;

Qt::ItemFlags flags(const QModelIndex &index) const override;

bool setData(const QModelIndex &index, const QVariant &value, int role) override;

void setIndicator(const QPoint &pt);

bool cellIndexCheck(const QPoint &coordinates);

bool cellIndexAgainstWallCheck(const QPoint &newcoordinates);

void moveX(int coordx);

void moveY(int coordy);

private slots:
void processTheMove();
private:
QTimer *timerSequence = nullptr;
static constexpr int ringnum = 5;
static constexpr int width = 30;
static constexpr int height = 30;
static constexpr int size = width * height;

using StateContainer = std::array<double, size>;
StateContainer m_currentState;

static std::size_t cellIndex(const QPoint &coordinates);
QPoint m_currpos;
};


