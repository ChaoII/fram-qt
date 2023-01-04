#ifndef STANDBYPAGE_H
#define STANDBYPAGE_H

#include <QWidget>

namespace Ui {
class StandbyPage;
}

class StandbyPage : public QWidget
{
    Q_OBJECT

public:
    explicit StandbyPage(QWidget *parent = nullptr);
    ~StandbyPage();

private:
    Ui::StandbyPage *ui;
};

#endif // STANDBYPAGE_H
