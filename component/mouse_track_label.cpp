//
// Created by zyx on 2021/7/14.
//

#include "mouse_track_label.h"

MouseTrackLabel::MouseTrackLabel(QWidget *parent) {

}

void MouseTrackLabel::mouseReleaseEvent(QMouseEvent *ev) {
    emit clicked(ev);
}

//void MouseTrackLabel::mousePressEvent(QMouseEvent *ev) {
//    emit pressed(ev);
//}