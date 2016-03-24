#ifndef MAPSCENE_H
#define MAPSCENE_H

#include <QGraphicsScene>

class QUndoStack;

class MapScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit MapScene(QObject *parent = 0);

    QUndoStack *undoStack() const;
    void setUndoStack(QUndoStack *undoStack);

signals:

public slots:

protected:
    QUndoStack *undoStack_ = nullptr;
};

#endif // MAPSCENE_H
