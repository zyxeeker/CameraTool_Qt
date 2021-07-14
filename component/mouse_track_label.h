//
// Created by zyx on 2021/7/14.
//

#ifndef TEST_MOUSE_TRACK_LABEL_H
#define TEST_MOUSE_TRACK_LABEL_H

#include <QLabel>
#include <QObject>

class MouseTrackLabel : public QLabel {
Q_OBJECT
public:
    explicit MouseTrackLabel(QWidget *parent = nullptr);

protected:
    void mouseReleaseEvent(QMouseEvent *ev);  //抬起

//    void mousePressEvent(QMouseEvent *ev);    //按下
signals:

    void clicked(QMouseEvent *ev);

//    void pressed(QMouseEvent *ev);
};


#endif //TEST_MOUSE_TRACK_LABEL_H
