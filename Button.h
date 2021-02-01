#pragma once

#include <QToolButton>

class CButton : public QToolButton // QToolButton -> QAbstractButton -> QWidget
{
    Q_OBJECT

public:
    explicit CButton(const QString& Text, QWidget* Parent = nullptr);
};
