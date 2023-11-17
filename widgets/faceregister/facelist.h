

#include <QWidget>
#include "facelistitem.h"
#include "facelistheader.h"
#include "models/models.h"

namespace Ui {
    class FaceList;
}

class FaceList : public QWidget {
Q_OBJECT
private:
    void deleteLayoutWidget();

public:
    explicit FaceList(QWidget *parent = nullptr);

    void addItemWidgets(const QVector<Staff> &staffs);

    std::vector<int64_t> getCheckedIndexIds();

    void selectAll();

    void unSelectAll();

    void reverseSelectAll();

    ~FaceList() override;


public slots:

    void on_getCheckBoxStatusChange(bool, const QString &);

private:

    std::vector<int64_t> checkedIndexIds;

    Ui::FaceList *ui;
};

