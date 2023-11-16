

#include <QWidget>
#include "facelistitem.h"
#include "facelistheader.h"

namespace Ui {
    class FaceList;
}

class FaceList : public QWidget {
Q_OBJECT
private:
    void deleteLayoutWidget();

public:
    explicit FaceList(QWidget *parent = nullptr);

    void addItemWidgets(const QVector<FaceListItem *> &faceListItems);

    ~FaceList();

private:
    Ui::FaceList *ui;
};

